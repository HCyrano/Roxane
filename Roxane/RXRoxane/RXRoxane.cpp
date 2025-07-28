/*
 *  RXRoxane.cpp
 *  BitBoard
 *
 *  Created by Bruno Causse on 01/09/05.
 *  Copyleft 2005-2025 personnel.
 *
 */
 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>

#include "RXRoxane.hpp"
#include "RXEvaluation.hpp"
#include "RXBitBoard.hpp"


extern "C"
void* init_process(void* pt)  {
	
	RXRoxane* roxane = (RXRoxane*)(pt);
	
	roxane->run();
	
	return NULL;
	
}


/*________________________________________________________________________________________________________*/


RXRoxane::RXRoxane(int size_hashtable, int max_thread): GGSClient(NULL) {

	pthread_mutex_init(&mutex, NULL);
    
    max_threads = max_thread;
	
	/*initialisations*/
	engine[BLACK] = new RXEngine(this, "../build/log/engine_0.txt", max_thread);
    engine[WHITE] = new RXEngine(this, "../build/log/engine_1.txt", max_thread);

	hTable = new RXHashTable(size_hashtable);
	main_PV = new RXHashTable(std::min(size_hashtable, 19));
	expected_PV = new RXHashTable(std::min(size_hashtable, 19));
	
    RXBitBoard::static_init();
 	RXEvaluation::load();
	RXEngine::probcut_coefficients();
	
}

RXRoxane::~RXRoxane() {
	
	delete expected_PV;
	delete main_PV;
	delete hTable;

	delete engine[WHITE];
	delete engine[BLACK];
	
	pthread_mutex_destroy(&mutex);
}

void RXRoxane::connectGGS(CODKStream* client) {
	GGSClient = client;
}

void RXRoxane::stop_engine(const string& _idg, COsGame* g) {
    
    int player = g->pos.board.fBlackMove? BLACK : WHITE;    
    engine[player]->resume();
    
}


/* unSynchronized method */
void RXRoxane::resume() {
		
	resume_flag.store(true);

	engine[BLACK]->resume();
	engine[WHITE]->resume();
	
	game[BLACK].Clear();
	game[WHITE].Clear();
    	
	hTable->reset();
	main_PV->reset();
	expected_PV->reset();
    
		
}

RXEngine* RXRoxane::getEngine(const int color) const {
    if (color == BLACK || color == WHITE)
        return engine[color];
    return NULL;
}

///* synchronized method */
//void RXRoxane::check_allWTHOR() {
//	
//	//	../../WTH_7708/WTH_1977.wtb
//	
//	for(int annee = 1977; annee < 2009; annee++) {
//		
//		std::ostringstream file_name;
//		file_name << "../../WTH_7708/WTH_" << annee << ".wtb";
//		
//		std::ostringstream file_error;
//		file_error << "../../WTH_7708/WTH_" << annee << ".err";
//		
//		std::cout << file_name.str() << std::endl;
//		
//		check_WTHOR(file_name.str(), file_error.str());
//		
//	}
//}
//
//
///* synchronized method */
//void RXRoxane::check_WTHOR(const std::string& file_name, const std::string& file_error) {
//	
//	static const char wthor2BitBoard[] ={
//		0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
//		0, A1, B1, C1, D1, E1, F1, G1, H1, 0,
//		0, A2, B2, C2, D2, E2, F2, G2, H2, 0,
//		0, A3, B3, C3, D3, E3, F3, G3, H3, 0,
//		0, A4, B4, C4, D4, E4, F4, G4, H4, 0,
//		0, A5, B5, C5, D5, E5, F5, G5, H5, 0,
//		0, A6, B6, C6, D6, E6, F6, G6, H6, 0,
//		0, A7, B7, C7, D7, E7, F7, G7, H7, 0,
//	0, A8, B8, C8, D8, E8, F8, G8, H8, 0	};
//	
//	
//	mutex.lock();
//	
//	clientMode = RXSearch::kPrivate;
//
//	hTable->shared(true);
//	
//	int n_threads = engine[SHARED]->get_THREAD_MAX();
//	
//	parameter.idEngine = SHARED;
//	parameter.nThreads = std::max(1, n_threads);
//	parameter.clientMode = clientMode; //kPrivate
//	parameter.dependent_time = false;	
//	parameter.search_on_opponent_time = false;
//	
//	
//	std::ofstream to(file_error.c_str());
//	if(to) {
//		
//		
//		std::ifstream from(file_name.c_str(), std::ios::binary);
//		if(from) {
//			
//			//read header
//			char  c[] = {0};	// 1 octet		
//			short s[] = {0};	// 2 octets
//			int   i[] = {0};	// 4 octets
//			
//			//read date
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //siecle de creation
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //annee de creation
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //mois de creation
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //jour de creation
//			
//			//nombres d'enregistrements
//			from.read(reinterpret_cast<char*>(i), sizeof(int)); //nombre d'enregistrement 1
//			int nRecords = i[0];
//			from.read(reinterpret_cast<char*>(s), sizeof(short)); //nombre d'enregistrement 2
//			
//			//annee des parties
//			from.read(reinterpret_cast<char*>(s), sizeof(short));
//			
//			//parametres
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //P1
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //P2
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //P3
//			int solved = c[0];
//			
//			//reserved
//			from.read(reinterpret_cast<char*>(c), sizeof(char)); //
//			
//			for(int iRecord = 0; iRecord < nRecords; iRecord++) {
//				
//				from.read(reinterpret_cast<char*>(s), sizeof(short)); //N du tournoi
//				from.read(reinterpret_cast<char*>(s), sizeof(short)); //N joueur noir
//				from.read(reinterpret_cast<char*>(s), sizeof(short)); //N joueur blanc
//				
//				from.read(reinterpret_cast<char*>(c), sizeof(char)); //score reel
//				from.read(reinterpret_cast<char*>(c), sizeof(char)); //score theorique
//				int score_theorique = c[0];
//				
//				RXBBPatterns sBoard;
//				RXBitBoard& board = sBoard.board;
//				RXMove move;
//				
//				char moves[60];
//				from.read(reinterpret_cast<char*>(moves), 60*sizeof(char)); //move
//				int iMove = 0;
//				
//				bool error = false;
//				
//				do {
//					
//					int position = wthor2BitBoard[moves[iMove++]];
//					
//					if(board.squareIsEmpty(position) && ((board).*(board.generate_flips[position]))(move)) {
//						((sBoard).*(sBoard.update_patterns[position][board.player]))(move);
//						
//						board.do_move(move);
//						*(sBoard.pattern) = *(move.pattern); //copy
//					} else {
//						error = true;
//						to << "***** RED ALERT *****" << std::endl;
//						
//						to << "record : " << iRecord << std::endl;
//						
//						to << "BAD GAME" << std::endl;
//						
//						for(int i = 0; i<60; i++)
//							to << RXMove::index_to_coord(wthor2BitBoard[moves[i]]) << " ";
//						
//						to << "\n" << std::endl;
//						
//					}
//					
//					if(board.isPassed())
//						board.do_pass();
//					
//				} while (!error && !board.isEndGame() && board.n_empties>solved);
//				
//				
//				if (!error && !board.isEndGame()) {
//					
//					std::cout << "record : " << iRecord << std::endl;
//					
//					RXBestPosition bPosition;
//					
//					engine[SHARED]->get_move(sBoard, hTable, bPosition, parameter);
//					
//					score_theorique = 2*score_theorique - 64;
//					
//					bPosition.score /= VALUE_DISC;
//					
//					if ((bPosition.score) != (board.player == BLACK? score_theorique:-score_theorique)) {
//						to << "***** RED ALERT *****" << std::endl;
//						
//						to << "record : " << iRecord << std::endl;
//						
//						to << "BAD RESULT" << std::endl;
//						
//						to << board << std::endl;
//						to << "score theorique : " << (board.player == BLACK? score_theorique:-score_theorique) << std::endl;
//						to << "score Reel      : " << bPosition.score << std::endl;
//						
//						to << std::endl;
//					}
//				}
//			}
//			
//			from.close();
//			
//		}
//		
//		to.close();
//	}
//	
//	mutex.unlock();
//	
//}

/* synchronized method */
void RXRoxane::get_move(const string& _idg, COsGame* g) {
	
	pthread_mutex_lock(&mutex);

	resume_flag.store(false);
    
	idg = _idg;

	RXBBPatterns& sBoard = search.sBoard;
	sBoard.build(g->pos.board.fromGGS());

	RXBitBoard& board = sBoard.board;

	
	//True if it is black's move
	int player = g->pos.board.fBlackMove? BLACK : WHITE;
	
	int id_engine = SHARED;
	
	int n_threads = 1;
	
	//verification que les deux jeux sont identiques
	//sauf 1er coup
	if(g->pos.board == g->posStart.board) {
		
		hTable->shared(true);
		main_PV->shared(true);
		expected_PV->shared(true);
		
		n_threads = engine[SHARED]->get_THREAD_MAX();
		
	} else {
		
		//same game -> engine[Shared]
		if(game[player^1].board.bt.n == 0 || game[player^1].board == g->pos.board) {
			
			if(!hTable->is_shared()) {
				engine[BLACK]->stop("engine BLACK stop : become SHARED");
				engine[WHITE]->stop("engine WHITE stop : become SHARED");
								
				hTable->mergePV(board);
				main_PV->mergePV(board);
				expected_PV->mergePV(board);

				hTable->shared(true);
				main_PV->shared(true);
				expected_PV->shared(true);

			}
			
			id_engine = SHARED;
			n_threads = engine[SHARED]->get_THREAD_MAX();
			
		} else {
			
			if(hTable->is_shared()) {
				
				engine[SHARED]->stop("engine SHARED stop");

				hTable->copyPV(expected_PV, RXHashTable::HASH_SHARED, board, board.player==BLACK? RXHashTable::HASH_BLACK:RXHashTable::HASH_WHITE);
				hTable->copyPV(main_PV, RXHashTable::HASH_SHARED, board, board.player==BLACK? RXHashTable::HASH_BLACK:RXHashTable::HASH_WHITE);

				hTable->copyPV_shared_to_color(   (engine[SHARED]->get_board()).board, player^1);
				main_PV->copyPV_shared_to_color((engine[SHARED]->get_board()).board, player^1);
				expected_PV->copyPV_shared_to_color((engine[SHARED]->get_board()).board, player^1);

				
				hTable->shared(false);
				main_PV->shared(false);
				expected_PV->shared(false);
								
                n_threads = engine[SHARED]->get_THREAD_MAX();
				
            } else {
                
                n_threads = std::max(1, (engine[player]->get_THREAD_MAX())/2);
                
            }
			
			id_engine = player;
		}
	}
	
	
	//affectation
	game[player] = g->pos;

	search.clientMode              = RXSearch::kGGSMode;
	search.idEngine                = id_engine;
	search.nThreads                = std::max(1, n_threads);
	search.htable                  = hTable;
	search.main_PV                 = main_PV;
	search.expected_PV             = expected_PV;
	search.search_on_opponent_time = (max_threads == 1? false:true);
	
	search.dependent_time = true;	
    
	search.tMatch         = static_cast<int>( g->posStart.cks[player^1].tCurrent*1000);
	search.tRemaining     = static_cast<int>(game[player].cks[player^1].tCurrent*1000);
    
    //debug
    //std::cout << "time match : " << search.tMatch << std::endl;
    //std::cout << "time remaining : " << search.tRemaining << std::endl;

    

	search.sBoard.build(g->pos.board.fromGGS());
	
	search.depth       = search.sBoard.board.n_empties;
	search.alpha       = -MAX_SCORE;
	search.beta        =  MAX_SCORE;
	search.selectivity = RXEngine::NO_SELECT;

	search.bestMove.position    = NOMOVE;
	search.bestMove.score       = UNDEF_SCORE;
	search.bestMove.selectivity = 0;
	search.bestMove.tElapsed    = 0.0;
	search.bestMove.nodes	    = 0;
	
	if(GGSClient->IsConnected() && board.n_empties>19) {
		sendMsg("--");
		sendMsg("game " + idg);

		std::ostringstream buffer, buffer1;
		buffer	<< "HashTable " << (hTable->is_shared()? "SHARED" : (player == BLACK? "BLACK" : "WHITE")) << std::endl;
		sendMsg(buffer.str());
		buffer1 << "Active thread" << (n_threads >1? "s : ":" : ") <<  n_threads  << std::endl;
		sendMsg(buffer1.str());
		
	}
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	
	if(!resume_flag.load() && pthread_create(process, &attr, init_process, (void*)(this)) != 0)
		std::cout << "Echec: Thread main Roxane" << std::endl;
	
	
	pthread_attr_destroy(&attr);
	
	pthread_mutex_unlock(&mutex);
}

/* synchronized method */
// solved fforum test
void RXRoxane::get_move(const std::string& file_name) {
    
	
	pthread_mutex_lock(&mutex);
	
//	while(true) {

		resume_flag.store(false);
		
		double T = 0;
		unsigned long long nodes = 0;
		
		hTable->shared(true);
		
		int n_threads = engine[SHARED]->get_THREAD_MAX();

		search.clientMode = RXSearch::kPrivate;
		search.idEngine = SHARED;
		search.nThreads = std::max(1, n_threads);
		search.htable = hTable;
		search.main_PV = main_PV;
		search.expected_PV = expected_PV;
		search.search_on_opponent_time = false;	

		search.dependent_time = false;	


		std::ifstream in(file_name.c_str());
		if(in) {
            
            
			std::string line;
			
			while(!resume_flag.load() && std::getline(in, line)) {
							                
				search.htable->reset();
				search.main_PV->reset();
				search.expected_PV->reset();
                engine[search.idEngine]->resume(); //hTable_shallow->reset()

                
				search.sBoard.build(line);
				search.depth       = search.sBoard.board.n_empties;
				search.alpha       = -MAX_SCORE;
				search.beta        = +MAX_SCORE;
				search.selectivity = RXEngine::NO_SELECT;
				
				search.bestMove.position    = NOMOVE;
				search.bestMove.score       = UNDEF_SCORE;
				search.bestMove.selectivity = 0;
				search.bestMove.tElapsed    = 0.0;
				search.bestMove.nodes	    = 0;
                
                engine[search.idEngine]->get_move(search);
                
                //engine[search.idEngine]->verif_sBoard(search);
                
				T += search.bestMove.tElapsed;
				nodes += search.bestMove.nodes;
				
			}
					
			engine[SHARED]->writeLog("---------------------------------------------------------------------------------------------");
			engine[SHARED]->writeLog("Total time  :     " + toHMS(T));
			
			
			std::ostringstream buffer;
            
            std::locale loc(std::locale(),new My_punct);
            buffer.imbue(loc);

            
			buffer << "Total nodes : " << std::setw(15) <<  nodes << "\n";
            
            int speed = 0;
            if(T !=0)
            speed = static_cast<int>((nodes/1000)/T);
            
            buffer << "Speed       : " << std::setw(15);
			if (nodes > 300000) {
				buffer  << speed << " kN/s";
            } else {
                buffer << "N/A";
            }
			engine[SHARED]->writeLog(buffer.str());
			
			engine[SHARED]->writeLog("");
			
			in.close();
            
            cout.imbue(loc);

			std::cout << "Total time  : " << toHMS(T) << std::endl;
			std::cout << "Total nodes : " << nodes << std::endl;
			if (T>0) {
				std::cout << "N/s         : " << static_cast<int>(nodes/T) << std::endl;
			}
			
		}
			
//	}
	
	pthread_mutex_unlock(&mutex);
	
}



void RXRoxane::run() {


	RXSearch s(search); //copy
	RXBitBoard& board = s.sBoard.board;
	
	if(board.hashcode() == hash_opening[60-board.n_empties])
		s.bestMove.position = move_opening[60-board.n_empties];
	else
		engine[s.idEngine]->get_move(s);
	
	
	if(!resume_flag.load() && s.clientMode == RXSearch::kGGSMode) {
		
		COsMoveListItem mli;
		s.bestMove.to_COsMoveListItem(mli);
		
		game[board.player].Update(mli);
		
		if(!resume_flag.load() && GGSClient->IsConnected())
			GGSClient->SendMove(idg, mli);
		
		
	}
	
	
	
}


void RXRoxane::imposed_opening(const std::string& line) {
	
	RXBitBoard board;
	RXMove move;
	
	for(unsigned int index = 0; index<line.length()-1; index++) {
		char abscise = std::toupper(line[index]);
		if(	abscise == 'A' ||
		   abscise == 'B' ||	
		   abscise == 'C' ||	
		   abscise == 'D' ||	
		   abscise == 'E' ||	
		   abscise == 'F' ||	
		   abscise == 'G' ||	
		   abscise == 'H') {
			
			char ordonne = std::toupper(line[index+1]);
			if(	ordonne == '1' ||
			   ordonne == '2' ||	
			   ordonne == '3' ||	
			   ordonne == '4' ||	
			   ordonne == '5' ||	
			   ordonne == '6' ||	
			   ordonne == '7' ||	
			   ordonne == '8') {
				
				int square = (('8' - ordonne)*8 + ('H' - abscise));
				
                ((board).*(board.generate_flips[square]))(move);
                
				if(move.flipped) {
					hash_opening[60-board.n_empties] = board.hashcode();
					move_opening[60-board.n_empties] = square;
					
					board.do_move(move);
					if(board.isPassed())
						board.do_pass();
					
				} else {
					break;
				}
				
				++index;
				
			}
		}
	}
	
	std::cout << "imposed line" << std::endl;
	
}

void RXRoxane::sendMsg(std::string msg) {
	

	if(GGSClient != NULL && search.clientMode == RXSearch::kGGSMode && GGSClient->IsConnected())
		GGSClient->SendMsg(msg);
	else
		std::cout << msg << std::endl;
	
}


