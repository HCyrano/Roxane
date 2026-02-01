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
#include <cmath>

#include "RXBBPatterns.hpp"
#include "RXBitBoard.hpp"
#include "RXMove.hpp"
#include "RXHashTable.hpp"
#include "RXHashShallow.hpp"
#include "RXEvaluation.hpp"
#include "RXSearch.hpp"

#include "RXHelper.hpp"

#include "RXTools.hpp"

#include <ostream>

struct NullBuffer : public std::streambuf {
    int overflow(int c) override { return c; }   // ignore tout
};

struct NullStream : public std::ostream {
    NullStream() : std::ostream(&nb) {}
private:
    NullBuffer nb;
};



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
    
    static const int DEPTH_4;
    
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
    unsigned long long hash_code_search;
    
    RXBestMove best_answer;
    
    RXSearch::t_client search_client;
    
    int search_alpha;
    int search_beta;
    int search_depth;
    int search_selectivity;
    
    RXRoxane* manager;
    
    pthread_t pthreadMain[1];
    pthread_mutex_t mutex;
    
    //volatile bool resume_flag;
    std::atomic_bool resume_flag;
    
    bool use_pv_ext;
    int depth_pv_extension;
    
    std::ostream* log;
    std::ofstream* logfile;
    NullStream nullStream;
    
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
    std::string showBestmove(const int depth, const int selectivity, const int alpha, const int beta, const int score, const unsigned int bestmove) const;
    std::string display(RXBitBoard& board, const int type, const int allowed_display = 0, int score = 0, const int time = 0, const int time_level = 0);
    
    
    inline double sigma(const int n_empty, const int depth, const int depth_probcut) const;
    int probcut_bounds(const RXBitBoard& board, const int selectivity, const int depth, const int depth_probcut, const int pvDev, const int alpha, const int beta, int& lower_bound, int& upper_bound) const;
    
    void sort_moves(const unsigned int threadID, const bool endgame, RXBBPatterns& sBoard, const int depth, const int selectivity, const int alpha, const int beta, RXMove* list);
    
    int probcut(const unsigned int threadID, RXBBPatterns& sBoard, const int selectivity, const int alpha, const int depth, const int depth_probcut, const int lower_probcut, const int upper_probcut, RXMove* list, const bool hashMove);
    int PVS_last_ply(const unsigned int threadID, RXBBPatterns& sBoard, const int depth, int alpha, const int beta, const bool passed);
    int alphabeta_last_three_ply(const unsigned int threadID, RXBBPatterns& sBoard, int alpha, const int beta, const bool passed);
    int alphabeta_last_two_ply(const unsigned int threadID, RXBBPatterns& sBoard, int alpha, const int beta, const bool passed);
    
    /*--------------------------------------------     MidGame part (RXMidGame.cpp)    --------------------------------------------*/ 
    
        
    static const int MG_SELECT;
    
    static const int MG_DEEP_TO_SHALLOW;
    static const int MG_MOVING_WINDOW;
    
    static const bool USE_PV_EXTENSION;
    static const int PV_EXTENSION_DEPTH;
    static const int MIN_DEPTH_USE_PV_EXTENSION;
    
    
    
    void iterative_deepening(RXBBPatterns& sBoard, RXMove* list, int selectivity, int depth, const int max_depth);
    void aspiration_search(RXBBPatterns& sBoard, const int selectivity, const int depth, RXMove* list);
    
    void MG_PVS_root(RXBBPatterns& sBoard, const int selectivity, const int depth,  const int alpha, const int beta, RXMove* list);
    void MG_SP_search_root(RXSplitPoint* sp, const unsigned int threadID);
    
    int MG_PVS_deep(const unsigned int threadID, RXBBPatterns& sBoard, const bool pv, const int selectivity, const int depth, int alpha, const int beta, const bool passed);
    void MG_SP_search_deep(RXSplitPoint* sp, const unsigned int threadID);
    
    int MG_PVS_shallow(const unsigned int threadID, RXBBPatterns& sBoard, const bool pv, const int depth, int alpha, const int beta, const bool passed);
    
    int MG_NWS_XProbCut(const unsigned int threadID, RXBBPatterns& sBoard, const int pvDev, const int selectivity, const int depth, const int alpha, const bool passed);
    void MG_SP_search_XProbcut(RXSplitPoint* sp, const unsigned int threadID);
    
    
    /*--------------------------------------------     EndGame part (RXEndGame.cpp)    --------------------------------------------*/ 
    
    static const int stability_threshold[];
    
    static const int EG_MEDIUM_TO_SHALLOW;
    static const int EG_MEDIUM_HI_TO_LOW ;
    static const int EG_DEEP_TO_MEDIUM;
    
    static const int MIN_DEPTH_USE_ENDCUT;
    
    static const int EG_HIGH_SELECT;
    
    
    void EG_check_PV(RXBBPatterns& sBoard, const int score);
    bool EG_check_PV(std::vector<unsigned char>& pv, RXBBPatterns& sBoard, const int score);

    
    void EG_driver(RXBBPatterns& board, int selectivity, int end_selectivity, RXMove* list);
    
    void EG_PVS_root(RXBBPatterns& board, const int selectivity, int alpha, const int beta, RXMove* list);
    void EG_SP_search_root(RXSplitPoint* sp, const unsigned int threadID);
    
    int	EG_PVS_deep(const unsigned int threadID, RXBBPatterns& sBoard, const bool pv, const int selectivity, int alpha, const int beta, const bool passed);
    void EG_SP_search_DEEP(RXSplitPoint* sp, const unsigned int threadID);
    
    int EG_PVS_ETC_mobility(const unsigned int threadID, RXBBPatterns& sBoard, const bool pv, int alpha, const int beta, const bool passed);
    void EG_SP_search_ETC_Mobility(RXSplitPoint* sp, const unsigned int threadID);
    
    int EG_PVS_hash_mobility(const unsigned int threadID, RXBitBoard& board, const bool pv, int alpha, const int beta, const bool passed);
    int EG_alphabeta_hash_mobility(const unsigned int threadID, RXBitBoard& board, const bool pv, int alpha, const int beta, const bool passed);
    int EG_alphabeta_hash_parity(const unsigned int threadID, RXBitBoard& board, const bool pv, int alpha, const int beta, const bool passed);
    int EG_alphabeta_parity(const unsigned int threadID, RXBitBoard& board, int alpha, int beta, const bool passed);
    
    int EG_NWS_XEndCut(const unsigned int threadID, RXBBPatterns& sBoard, const int pvDev, const int selectivity, const int alpha, const bool passed);
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
    
    std::atomic_bool abort;
    
    void init_threads();
    void stop_threads();
    void* idle_loop(const unsigned int threadID, RXSplitPoint* waitSp);
    void wake_sleeping_threads();
    //	void wake_sleeping_thread(unsigned int threadID);
    bool idle_thread_exists(unsigned int master);
    bool thread_is_available(unsigned int slave, unsigned int master);
    bool thread_should_stop(unsigned int threadID);
    
    bool split(	RXBBPatterns& sBoard, bool pv, int pvDev, 
               int depth, int selectivity, int alpha, int beta, int& bestscore, unsigned int& bestmove,
               RXMove* list, unsigned int master, RXSplitPoint::t_callBackSearch callback);
    
    
public:
    
    static const int NO_SELECT;
    static unsigned int confidence_to_selectivity(int c);
    static unsigned int selectivity_to_confidence(int s);

    RXEngine(RXRoxane* _manager, std::string _id, int maxThread = 1);
    ~RXEngine();	
    
    RXBBPatterns& get_board();
    
    void writeLog(std::string s); //log
    
    void get_move(RXSearch& s);
    //    void verif_sBoard(RXSearch& s);
    
    virtual void* run();
    
    void stop(std::string msg);
    void resume();
    
    //MidGame part (RXMidGame.cpp)
    
    
    //EndGame part (RXEndGame.cpp)
    
    
    //Multithread part
    
    virtual void* idle_loop();
    int get_THREAD_MAX();

#ifdef TUNE_PROBCUT_MID

    void probcut_mid_data(RXHashTable* HT, RXHashTable* PV);

#endif
    
#ifdef TUNE_PROBCUT_END

    void probcut_end_data(RXHashTable* HT, RXHashTable* PV);

#endif

#ifdef TUNE_PROBCUT_END2

    void probcut_end2_data(const std::string& file_name, RXHashTable* HT, RXHashTable* PV);

#endif

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


#ifdef SIGMA_2ZONES

// --- Modèle principal : probabilité (sigma) ---
inline double RXEngine::sigma(const int n_empty, const int depth, const int depth_probcut) const
{
    
    constexpr int mid = 0;
    constexpr int end = 1;

    
    // --- Coefficients appris (issus du script Python) ---
    // version 6: 119 779 393
    // const float RXEngine::PERCENTILE[] = {1.00f, 1.10f, 1.35f, 1.70f, 2.20f, 2.80f, 3.60f};
    // s8r14 2:00 Edmond vs edax
    // w88 d121 l68
    constexpr double probcut_a[] = {-0.00906229, 0.00483133};
    constexpr double probcut_b[] = {0.12115910, 0.07975382};
    constexpr double probcut_c[] = {-0.12071974, -0.01763140};
    constexpr double probcut_d[] = {-0.47638522, -3.58586370};
    constexpr double probcut_e[] = {-2.93341676, 9.99659776};
    constexpr double probcut_f[] = {-5.44314615, -9.52512272};
    constexpr double probcut_g[] = {0.01672187, 6.76910942};

    // Transitions progressives entre zones
    double s1 = sigmoid(n_empty, 40.0, 0.3);

    double w_mid   = s1;
    double w_end   = 1.0 - s1;

    // Fonction polynomiale par zone
    auto sigma = [&](int i) {
        double r = probcut_a[i] * n_empty + probcut_b[i] * depth_probcut + probcut_c[i] * depth;
        return probcut_d[i] * r * r * r +
               probcut_e[i] * r * r +
               probcut_f[i] * r +
               probcut_g[i];
    };

    // Combinaison douce
    double sig_mid = sigma(mid);
    double sig_end = sigma(end);

    double res = w_mid * sig_mid + w_end * sig_end;

    return res;
}

#else

inline double RXEngine::sigma(const int n_empty, const int depth, const int depth_probcut) const {
    
    double sigma;
    
#ifdef PROBCUT_x2
    //polynome 2d
    
    //edax coefficients
    constexpr double probcut_a = -0.10026799;
    constexpr double probcut_b = 0.31027733;
    constexpr double probcut_c = -0.57772603;
    constexpr double probcut_d = 0.07585621;
    constexpr double probcut_e = 1.16492647;
    constexpr double probcut_f = 5.9171698;
    
     
    sigma= probcut_a * n_empty + probcut_b * depth_probcut + probcut_c * depth;
    sigma = probcut_d * sigma * sigma + probcut_e * sigma + probcut_f;
    
#else
    //polynome 3d

    /*
    // version 115 694 019
    //const float RXEngine::PERCENTILE[] = {1.00f, 1.10f, 1.35f, 1.70f, 2.20f, 2.80f, 3.60f};
    // s8r14 3:00 Edmond vs Rostand
    // w27 d66 l37
    // s8r14 3:00 Edmond vs edax
    // w42 d52 l31
    // version tournament november 2025
    constexpr double probcut_a = -0.0010379521160583595;
    constexpr double probcut_b = 0.03864157929092302;
    constexpr double probcut_c = -0.012813733660428612;
    constexpr double probcut_d = -13.88994753874693;
    constexpr double probcut_e = 23.77203236686711;
    constexpr double probcut_f = -12.021232747834466;
    constexpr double probcut_g = 4.377396934700955;
    */
    
    
    //version 3 (113 590 954)
    //const float RXEngine::PERCENTILE[] = {1.00f, 1.10f, 1.35f, 1.70f, 2.20f, 2.80f, 3.60f};
    //s8r14 2:00 Edmond vs edax
    //w75 d118 l62
    constexpr double probcut_a = -0.0017319169860170334;
    constexpr double probcut_b = 0.046028020390195414;
    constexpr double probcut_c = -0.02121182792010099;
    constexpr double probcut_d = -5.248427428319801;
    constexpr double probcut_e = 10.287391794385476;
    constexpr double probcut_f = -5.173813504774759;
    constexpr double probcut_g = 3.3344983118071387;
    
    
    
    sigma = probcut_a * n_empty + probcut_b * depth_probcut + probcut_c * depth;
    sigma =   probcut_d * sigma * sigma * sigma
            + probcut_e * sigma * sigma
            + probcut_f * sigma
            + probcut_g;

    
#endif
    
    //sigma with lower bound at 2,5
    return sigma;
    
}

#endif

inline int RXEngine::probcut_bounds(const RXBitBoard& board, const int selectivity, const int depth, const int depth_probcut,  const int pvDev, const int alpha, const int beta, int& lower_bound, int& upper_bound) const {
    
    double coeff_pv = std::max(0.90, (115-3*pvDev)/100.0);
        
    //error evaluation with lower bound at 3
    int eval_error = std::round(sigma(board.n_empty, depth, depth_probcut) * coeff_pv * PERCENTILE[selectivity]);
    eval_error = std::max(3, eval_error);
    
    lower_bound = std::max(-MAX_SCORE, alpha - eval_error);
    upper_bound = std::min(+MAX_SCORE, beta  + eval_error);
    
    return eval_error;

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
    
    RXSplitPoint* sp = threads[threadID].splitPoint;
    
    //emptie loop
    while(sp != NULL && sp->explored == false)
        sp = sp->parent;
    
    return sp != NULL;
    
}


#endif
