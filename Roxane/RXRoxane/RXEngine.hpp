/*
 *  RXEngine.hpp
 *  BitBoard
 *
 *  Created by Bruno Causse on 10/08/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXENGINE_HPP
#define RXENGINE_HPP

#include <string>
#include <cstddef> // define NULL
#include <cstdlib> // abs()
#include <vector>
#include <array>
#include <fstream>
#include <locale>
#include <assert.h>
#include <atomic>

#include "RXBBPatterns.hpp"
#include "RXBitBoard.hpp"
#include "RXMove.hpp"
#include "RXHashTable.hpp"
#include "RXHashShallow.hpp"
#include "RXEvaluation.hpp"
#include "RXSearch.hpp"

#include "RXHelper.hpp"

#include "RXTools.hpp"

/*
 Matchs avec / sans alpha_cut
 91/157/49 [297]
 #define USE_PROBCUT_ALPHA
*/

//for record fforum-40-59 setting
//Matchs avec /sans :
//69/319/58 [446] s8r14 2:00
//15,5%/71,5%/13%
#define USE_SPLIT_AT_ROOT


extern "C"
void* init_threadHelper(void* pt);

extern "C"
void* init_pthreadMain(void* pt);

class RXRoxane;


class My_punct:public std::numpunct<char> {
public:
	
	typedef char char_type;
	typedef std::string	string_type;
	
	
	explicit My_punct(size_t r=0):std::numpunct<char>(r) {}
	
protected:
    //bug ??? Parse ggs entre '.' & ','
	char do_decimal_point() const {return ',';}
	char do_thousands_sep() const {return ' ';}
	std::string do_grouping() const {return "\003";}
};


class RXSplitPoint {
	
public:
	
	enum t_callBackSearch {MID_ROOT, MID_PVS, MID_XPROBCUT, END_ROOT, END_PVS, END_XPROBCUT, END_ETC_MOBILITY};
		
	RXSplitPoint* parent;
	
	RXBBPatterns* sBoard;
    //non copiableAssignable, mais il n'y a pas de redimensionnenent (semble fonctionner)
    //soluce : remplacer le vector par un tableau static a taille fixe :-(
	std::vector<RXBBPatterns> sBoardStack;
	
	RXMove* list;
	
	t_callBackSearch CBSearch;
	
	bool pv;
	int pvDev;
	int depth;
	int selectivity;
    volatile bool selective_cutoff;
    volatile int alpha, beta, bestscore, bestmove;

	
	mutable pthread_mutex_t lock;
	

    unsigned int master;
    volatile unsigned int n_Slaves;
 
	std::vector<bool>  slaves;
	
    volatile bool explored;		// splitPoint resolved
    //std::atomic_bool explored;
	
	RXSplitPoint(int maxThreads) : parent(NULL), sBoard(NULL), sBoardStack(maxThreads), slaves(maxThreads),
	list(NULL) {
        explored = false;
		pthread_mutex_init(&lock, NULL);
	}
	
	~RXSplitPoint() {
		pthread_mutex_destroy(&lock);
	}
    
	
};


class RXThread {
	
public:

	enum thread_state {
        UNINITIALISED,
		INITIALIZING,
		SEARCHING,
		AVAILABLE,
		RESERVED,
		WORKISWAITING,
		TERMINATED
	};
	
	
	RXSplitPoint* splitPoint;

    volatile uint activeSplitPoints;
    //std::atomic<uint> activeSplitPoints;

    //non copiableAssignable, mais il n'y a pas de redimensionnenent (semble fonctionner)
    //soluce : remplacer le vector par un tableau static a taille fixe :-(
    std::vector<RXSplitPoint> splitPointStack;
    
	RXMove _move[61][34];
	

	pthread_mutex_t lock;
	pthread_cond_t  cond;


    volatile thread_state state = UNINITIALISED;
    //std::atomic<thread_state> state{UNINITIALISED};
	
	//le parametre maxThread est utile pour splitPointStack
	RXThread(int maxThreads, int maxActiveSplitPoint = 8) : splitPoint(NULL), activeSplitPoints(0),
	splitPointStack(maxActiveSplitPoint, RXSplitPoint(maxThreads)) {
	
        pthread_mutex_init(&lock, NULL);
		pthread_cond_init(&cond, NULL);
		
	}
	
	~RXThread() {
		pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&lock);

	}
    	
	
};


class RXEngine: public Runnable, public RXHelper {

	/*--------------------------------------------     shared part (RXEngine.cpp)    --------------------------------------------*/ 
	
	enum t_search {BOOK, MIDGAME, ENDGAME};
    enum probcut_cut {NO_CUT, ALPHA_CUT, BETA_CUT};

	static const int CHECK_TO_LAST_THREE;
	

	static const int HASHTABLE;
	static const int INFERIOR;
	static const int EXACT;
	static const int SUPERIOR;
	static const int INTERRUPT;
	static const int GGS_MSG;

	static const int CONFIDENCE[];
	static const float PERCENTILE[];
	static const int DEPTH_BOOSTER;
		
	//time manager part
    volatile int time_match;
    volatile int time_remaining;
	
    volatile int time_start;
    volatile int dependentTime_start;
    volatile int time_search;
	
    volatile int time_startLevel;
    volatile int time_nextLevel;

    volatile int time_move;
    volatile int extratime_move;
    //volatile int extra_time;
    std::atomic<int> extra_time;

    //volatile bool first_move;
    std::atomic_bool first_move;
    
    volatile bool dependent_time;
	
	t_search type_search;
	int select_search;
	
	bool new_search;
	
	RXHashTable* hTable;
	RXHashTable* main_PV;
	RXHashTable* expected_PV;
	RXHashTable* hTable_PV;

	RXHashTable::t_hash type_hashtable;

	RXHashShallow* hTable_shallow;

			
	RXBBPatterns search_sBoard;
	int root_player;
	unsigned long long hash_code_search;
	
	RXBestMove best_answer;
	
	RXSearch::t_client search_client;
	
	int search_alpha;
	int search_beta;
	int search_depth;
	int search_selectivity;

	static std::vector< std::vector<int> > probcut_data;
	
	RXRoxane* manager;
	
	pthread_t pthreadMain[1];
	pthread_mutex_t mutex;

    //volatile bool resume_flag;
    std::atomic_bool resume_flag;
    
    bool use_pv_ext;
    int depth_pv_extension;
    
	std::ofstream* log;
    
    //track BUG final
    int final_score = UNDEF_SCORE;
    int final_player = UNDEF_COLOR;

	

	bool probable_timeout(double probable_time_next_level) const;
	int pTime_next_level(RXBitBoard& board, int time_level, int depth = 1, int next_depth = 3) const;
	void determine_move_time(RXBitBoard& board);
    
    int get_current_time() const;
    int get_current_dependentTime() const;
    int time_limit() const;
	
	void set_type_search(t_search ts);
	t_search get_type_search() const;

	void set_select_search(int selectivity);
	int get_select_search() const;
	
	
	std::string variationPrincipal(RXBitBoard& sBoard, int depth) const;
	std::string showPV(RXBitBoard& board, int depthLine) const;
	std::string showHashmove(const RXBitBoard& board, RXHashValue& entry) const;
	std::string showBestmove(const int depth, const int selectivity, const int alpha, const int beta, const int score, const int bestmove) const;
	std::string display(RXBitBoard& board, const int type, const int allowed_display = 0, int score = 0, const int time = 0, const int time_level = 0);
	


	void probcut_bounds(const RXBitBoard& board, const int selectivity, const int depth, const int pvDev, const int pivot, int& lower_bound, int& upper_bound) const;

	void sort_moves(int threadID, const bool endgame, RXBBPatterns& sBoard, const int depth, const int selectivity, const int alpha, const int beta, RXMove* list);
		
	int probcut(int threadID, const bool endgame, RXBBPatterns& sBoard, const int selectivity, const int depth, const int lower_probcut, const int upper_probcut, RXMove* list, const bool hashMove);
	int PVS_last_ply(int threadID, RXBBPatterns& sBoard, const int depth, int alpha, int beta, bool passed);
	int alphabeta_last_three_ply(int threadID, RXBBPatterns& sBoard, int alpha, int beta, bool passed);
	int alphabeta_last_two_ply(int threadID, RXBBPatterns& sBoard, int alpha, int beta, bool passed);
	
	/*--------------------------------------------     MidGame part (RXMidGame.cpp)    --------------------------------------------*/ 


	static const int MIN_DEPTH_USE_PROBCUT;
	
	static const int MG_SELECT;

	static const int MG_DEEP_TO_SHALLOW;
	static const int MG_MOVING_WINDOW;
    
    static const bool USE_PV_EXTENSION;
    static const int PV_EXTENSION_DEPTH;
    static const int MIN_DEPTH_USE_PV_EXTENSION;

	

	void iterative_deepening(RXBBPatterns& sBoard, RXMove* list, int depth, const int max_depth);
	void aspiration_search(RXBBPatterns& sBoard, const int depth, RXMove* list);
	
	void MG_PVS_root(RXBBPatterns& sBoard, const int depth,  int alpha, int beta, RXMove* list);
    void MG_SP_search_root(RXSplitPoint* sp, const unsigned int threadID);
	
	int MG_PVS_deep(int threadID, RXBBPatterns& sBoard, const bool pv, const int selectivity, const int depth, bool& selective_cutoff, int alpha, int beta, const bool passed);
	void MG_SP_search_deep(RXSplitPoint* sp, const unsigned int threadID);
	
	int MG_PVS_shallow(int threadID, RXBBPatterns& sBoard, const bool pv, const int depth, int alpha, int beta, const bool passed);
	
	int MG_NWS_XProbCut(int threadID, RXBBPatterns& sBoard, const int pvDev, const int selectivity, const int depth, bool& selective_cutoff, int alpha, const bool passed);
	void MG_SP_search_XProbcut(RXSplitPoint* sp, const unsigned int threadID);
	

	/*--------------------------------------------     EndGame part (RXEndGame.cpp)    --------------------------------------------*/ 

    static const bool USE_ETC;
	static const bool USE_STABILITY;
    static const bool USE_ENHANCED_STABLILITY;

	static const int stability_threshold[];

	static const int EG_MEDIUM_TO_SHALLOW;
	static const int EG_MEDIUM_HI_TO_LOW ;
	static const int EG_DEEP_TO_MEDIUM;
    
	static const int MIN_DEPTH_USE_ENDCUT;

	static const int EG_HIGH_SELECT;
	

	void check_PV(RXBBPatterns& sBoard, const int score, const int alpha, const int beta);
		
	void EG_driver(RXBBPatterns& board, int selectivity, int end_selectivity, RXMove* list);
	
	void EG_PVS_root(RXBBPatterns& board, const int selectivity, int alpha, int beta, RXMove* list);
	void EG_SP_search_root(RXSplitPoint* sp, const unsigned int threadID);
	
	int	EG_PVS_deep(int threadID, RXBBPatterns& sBoard, const bool pv, const int selectivity, bool& selective_cutoff, int alpha, int beta, bool passed);
	void EG_SP_search_DEEP(RXSplitPoint* sp, const unsigned int threadID);

	int EG_PVS_ETC_mobility(int threadID, RXBBPatterns& sBoard, const bool pv, int alpha, int beta, bool passed);
    void EG_SP_search_ETC_Mobility(RXSplitPoint* sp, const unsigned int threadID);
    
	int EG_PVS_hash_mobility(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed);
    int EG_alphabeta_hash_mobility(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed);
    int EG_alphabeta_hash_parity(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed);
	int EG_alphabeta_parity(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed);
	
	int EG_NWS_XEndCut(int threadID, RXBBPatterns& sBoard, const int pvDev, const int selectivity, bool& selective_cutoff, int alpha, const bool passed);
	void EG_SP_search_XEndcut(RXSplitPoint* sp, const unsigned int threadID);
	
	/*--------------------------------------------     Multithreads part (RXEngine.cpp)    --------------------------------------------*/ 
	
	const unsigned int THREAD_MAX;
	static const unsigned int ACTIVE_SPLITPOINT_MAX = 12;
    static const unsigned int THREAD_PER_SPLITPOINT_MAX = 4; //4

	
	//parameter for launch thread
    volatile uint idThread;
	

	static const int MIN_DEPTH_SPLITPOINT;

	std::vector<RXThread> threads;
    volatile bool allThreadsShouldExit, allThreadsShouldSleep;
	
	
	unsigned int activeThreads;

	pthread_mutex_t MP_sync;

	//volatile bool abort;
    std::atomic_bool abort;
	
	void init_threads();
	void stop_threads();
	void idle_loop(const unsigned int threadID, RXSplitPoint* waitSp);
	void wake_sleeping_threads();
//	void wake_sleeping_thread(unsigned int threadID);
	bool idle_thread_exists(unsigned int master);
	bool thread_is_available(unsigned int slave, unsigned int master);
	bool thread_should_stop(unsigned int threadID);
	
	bool split(	RXBBPatterns& sBoard, bool pv, int pvDev, 
			   int depth, int selectivity, bool& selective_cutoff,
			   int alpha, int beta, int& bestscore, int& bestmove,
			   RXMove* list, unsigned int master, RXSplitPoint::t_callBackSearch callback);
	
	
public:

	static const int NO_SELECT;
	static unsigned int confidence_to_selectivity(int c);
	static unsigned int selectivity_to_confidence(int s);

	static void probcut_coefficients();

	RXEngine(RXRoxane* _manager, std::string _id, int maxThread = 1);	
	~RXEngine();	

	RXBBPatterns& get_board();
		
	void writeLog(std::string s); //log
		
	void get_move(RXSearch& s);
//    void verif_sBoard(RXSearch& s);

	virtual void run();

	void stop(std::string msg);
	void resume();
		
	//MidGame part (RXMidGame.cpp)
		
		
	//EndGame part (RXEndGame.cpp)
	
	
	//Multithread part
	
	virtual void idle_loop();
	int get_THREAD_MAX();
	
	
};

inline int RXEngine::get_THREAD_MAX() {
	return THREAD_MAX;
}


inline int RXEngine::get_current_time() const {
	return get_system_time() - time_start;
}

inline int RXEngine::get_current_dependentTime() const {
	return get_system_time() - dependentTime_start;
}

inline int RXEngine::time_limit() const {
    
    int time = time_move;
    
    if(first_move.load())
        time = (66*time)/100;
        
    if(extra_time)
        time += extratime_move;
    
    return time;
}


inline void RXEngine::probcut_bounds(const RXBitBoard& board, const int selectivity, const int depth, const int pvDev, const int pivot, int& lower_bound, int& upper_bound) const {
	
    double coeff_score = 1.0f+(std::abs(pivot)/(128.0f * VALUE_DISC));
    double coeff_pv = std::max(0.80f, (109-3*pvDev)/100.0f);
    
#ifndef __ARM_NEON
    
    //version CyranoF more aggressive
	int sigma = static_cast<int> (probcut_data[board.n_empties][depth] * PERCENTILE[selectivity]* coeff_score * coeff_pv);
    sigma = RXBBPatterns::QUANTA * ((sigma + RXBBPatterns::QUANTA/2)/RXBBPatterns::QUANTA);

    lower_bound = std::max(-MAX_SCORE, pivot - sigma);    //(bug limit 23/10/2008)
    upper_bound = std::min(+MAX_SCORE, pivot + sigma);    //(bug limit 23/10/2008)
    
#else

    double sigma = probcut_data[board.n_empties][depth] * PERCENTILE[selectivity]* coeff_score * coeff_pv;

    /* validé le 1/01/2025 avec/sans 26w/57d/13l*/
	int error_alpha = static_cast<int> (sigma);
	int error_beta  = static_cast<int> ((board.player == root_player? 1.1f:1.0f) * sigma); //1.1f:1.0f

	//always positif
	error_alpha = RXBBPatterns::QUANTA * ((error_alpha + RXBBPatterns::QUANTA/2)/RXBBPatterns::QUANTA);
	error_beta  = RXBBPatterns::QUANTA * ((error_beta  + RXBBPatterns::QUANTA/2)/RXBBPatterns::QUANTA);
	
	
	lower_bound = std::max(-MAX_SCORE, pivot - error_alpha);	//(bug limit 23/10/2008)
	upper_bound = std::min(+MAX_SCORE, pivot + error_beta );	//(bug limit 23/10/2008)
    
#endif
	
}


inline void RXEngine::set_type_search(t_search ts) {
	type_search = ts;
}	

inline RXEngine::t_search RXEngine::get_type_search() const {	
	return type_search;
}	

inline void RXEngine::set_select_search(int selectivity) {
	select_search = selectivity;
}

inline int RXEngine::get_select_search() const {	
	return select_search;
}	

inline RXBBPatterns& RXEngine::get_board() {
	
	return search_sBoard;
}	

// thread_should_stop() checks whether the thread with a given threadID has
// been asked to stop, directly or indirectly.  This can happen if a beta
// cutoff has occured in thre thread's currently active split point, or in
// some ancestor of the current split point.

inline bool RXEngine::thread_should_stop(unsigned int threadID) {
	
    //assert(threadID >= 0 && threadID < activeThreads);
	
	
	RXSplitPoint* sp = threads[threadID].splitPoint;
	
	//emptie loop
	while(sp != NULL && sp->explored == false)
		sp = sp->parent;
	
	return sp != NULL;
	
	
}


#endif
