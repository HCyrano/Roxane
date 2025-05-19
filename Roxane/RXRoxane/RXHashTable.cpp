/*
 *  RXHashTable.cpp
 *  Roxane
 *
 *  Created by Bruno Causse on 18/05/05.
 *  Copyleft 2005-2025 personnel.
 *
 */
#include <iostream>
#include <new>
#include <sstream>
#include <algorithm>

#include <cassert>

#include "RXHashTable.hpp"

RXHashValue::RXHashValue(unsigned long long packed) {
	
	lower = static_cast<short>(packed & 0x000000000000FFFFULL);
	packed >>= 16;
	upper = static_cast<short>(packed & 0x000000000000FFFFULL);
	packed >>= 16;
	move = static_cast<unsigned char>(packed & 0x00000000000000FFULL);
	packed >>= 8;	
	selectivity = static_cast<unsigned char>(packed & 0x00000000000000FFULL);
	packed >>= 8;	
	depth = static_cast<unsigned char>(packed & 0x00000000000000FFULL);
	packed >>= 8;	
	date = static_cast<unsigned char>(packed & 0x00000000000000FFULL);
	
}


RXHashTable::RXHashTable(unsigned int nBT) : table(0), nBitsTable(nBT),  _shared(true) {

	_offsetTable[HASH_SHARED] = 0;
	_offsetTable[HASH_BLACK] = 0;
	_offsetTable[HASH_WHITE] = 1<<(nBitsTable-1);
	
	_maskTable[HASH_SHARED] = (1<<nBitsTable)-1;
	_maskTable[HASH_BLACK] = (1<<(nBitsTable-1))-1;
	_maskTable[HASH_WHITE] = (1<<(nBitsTable-1))-1;
	
	table.resize(1UL<<nBitsTable);
		 
	date[0] = date[1] = 0;
    
    
}


void RXHashTable::shared(const bool flag) {
	_shared = flag;
	
	if (date[1]>date[0])
		date[0] = date[1];
	else
		date[1] = date[0];
		
}

bool RXHashTable::is_shared() const {
	return _shared;
}


void RXHashTable::reset() {
    

	_shared = true;
	
	const unsigned long capacity = 1UL<<nBitsTable;
	
	for(unsigned long i = 0; i<capacity; i++) {
		table[i].deepest.packed = 0ULL;
		table[i].newest.packed = 0ULL;
	}
	
	date[0] = date[1] = 0;
}


void RXHashTable::update(const unsigned long long hash_code, const bool pv, const t_hash type_hashtable, const unsigned char selectivity, const unsigned char depth, const int alpha, const int beta, const int score, const char move) {
	
//	assert(alpha >= -MAX_SCORE && alpha <= MAX_SCORE);
//	assert(beta >= -MAX_SCORE && beta <= MAX_SCORE);
//
//	assert(score != -MAX_SCORE || score != MAX_SCORE);
//	assert(score != -INTERRUPT_SEARCH || score != INTERRUPT_SEARCH);
//	assert(score != -UNDEF_SCORE || score != UNDEF_SCORE);
	
//    //track Bug hash 24/01/2025
//    if (std::abs(score) == MAX_SCORE) {
//        std::cout << "RED ALERT : abs(score) == MAX_SCORE" << std::endl;
//        std::cout << "score = " << score << std::endl;
//        std::cout << "alpha = " << alpha << std::endl;
//        std::cout << "beta  = " << beta << std::endl;
//    }

	RXHashEntry& entry = table[_offsetTable[type_hashtable] | (static_cast<unsigned int>(hash_code>>32) & _maskTable[type_hashtable])];
	
	RXHashRecord& deepest = entry.deepest;
	
	const unsigned long long deepest_lock     = deepest.lock;
	const unsigned long long deepest_packed	  = deepest.packed;
	const unsigned long long deepest_hashcode = deepest_lock ^ deepest_packed;
	
	RXHashValue deepest_value(deepest_packed);
	
	int _date = date[type_hashtable	== HASH_WHITE? WHITE:BLACK];
	if(pv)
		_date++; //bonus for pv
	
	
	/* try to update deepest entry */
	if (hash_code == deepest_hashcode && selectivity == deepest_value.selectivity  && depth == deepest_value.depth && alpha < score) {
        
		
		if (score < beta && score < deepest_value.upper) 
			deepest_value.upper = static_cast<short>(score);
		if (/*score > alpha && */score > deepest_value.lower)
			deepest_value.lower = static_cast<short>(score);

        /* control if lower>upper : Instability */
		if(deepest_value.lower>deepest_value.upper) {

 			if(score<beta)
				deepest_value.upper = static_cast<short>(score);
			else
				deepest_value.upper = MAX_SCORE;
			
			/*if(score>alpha)*/
				deepest_value.lower = static_cast<short>(score);
			/*else
				deepest_value.lower = -MAX_SCORE;*/
            
		}
		
		deepest_value.date = _date;
		deepest_value.move = move;
        		
		deepest.packed = deepest_value.wide_2_compact();
		deepest.lock   = hash_code ^ deepest.packed;
		
	} else {
		
		RXHashRecord& newest = entry.newest;
		
		const unsigned long long newest_packed   = newest.packed;
		const unsigned long long newest_hashcode = newest.lock ^ newest_packed;
		RXHashValue newest_value(newest_packed);
		
		
		/* else try to update newest entry */
		if (hash_code == newest_hashcode && selectivity == newest_value.selectivity  && depth == newest_value.depth && alpha < score) {
			
			if (score < beta && score < newest_value.upper)
				newest_value.upper = static_cast<short>(score);
			if (/*score > alpha && */score > newest_value.lower)
				newest_value.lower =  static_cast<short>(score);

            /* control if lower>upper : Instability */
			if(newest_value.lower>newest_value.upper) {

				if(score<beta)
					newest_value.upper = static_cast<short>(score);
				else
					newest_value.upper = MAX_SCORE;
				
				/*if(score>alpha)*/
					newest_value.lower = static_cast<short>(score);
				/*else
					newest_value.lower = -MAX_SCORE;*/
			}
			
			newest_value.date = _date;
			newest_value.move =  move;
            
			newest.packed = newest_value.wide_2_compact();
			newest.lock   = hash_code ^ newest.packed;
			
			
			/* else try to add to deepest entry */
		} else if (deepest_hashcode == hash_code ||  deepest_value.date <  _date ||
													(deepest_value.date == _date && ( deepest_value.depth <  depth ||
													(deepest_value.depth == depth && deepest_value.selectivity < selectivity)))) { // priority
			
			if(deepest_hashcode != hash_code &&	(newest_hashcode == hash_code ||  newest_value.date <  deepest_value.date ||
																			  	 (newest_value.date == deepest_value.date && (newest_value.depth <  deepest_value.depth ||
																			  	 (newest_value.depth == deepest_value.depth && newest_value.selectivity <= deepest_value.selectivity))))) {
					
				//copy
				newest.lock   = deepest_lock;
				newest.packed = deepest_packed;
			}
			
			deepest_value.depth = depth;
			deepest_value.date = _date;
			deepest_value.selectivity = selectivity;
			deepest_value.lower = -MAX_SCORE;
			deepest_value.upper = +MAX_SCORE;
			if (score < beta) deepest_value.upper = static_cast<short>(score);
			if (score > alpha) deepest_value.lower = static_cast<short>(score);
			deepest_value.move =  move;

			deepest.packed = deepest_value.wide_2_compact();
			deepest.lock   = hash_code ^ deepest.packed;
			
			
			/* else add to newest entry */
		} else {
			
			newest_value.depth = depth;
			newest_value.date = _date;
			newest_value.selectivity = selectivity;
			newest_value.lower = -MAX_SCORE;
			newest_value.upper = +MAX_SCORE;
			if (score < beta) newest_value.upper = static_cast<short>(score);
			if (score > alpha) newest_value.lower = static_cast<short>(score);
			newest_value.move =  move;

			newest.packed = newest_value.wide_2_compact();
			newest.lock   = hash_code ^ newest.packed;
			
			
		}
	}
	
}

void RXHashTable::update(const unsigned long long hash_code, const t_hash type_hashtable, const unsigned char selectivity, const unsigned char depth, const int alpha, const int score, const char move) {

//	assert(alpha >= -MAX_SCORE && alpha <= MAX_SCORE-1);
//	
//	assert(score != -MAX_SCORE || score != MAX_SCORE);
//	assert(score != -INTERRUPT_SEARCH || score != INTERRUPT_SEARCH);
//	assert(score != -UNDEF_SCORE || score != UNDEF_SCORE);
    
    
//    //track Bug hash 24/01/2025
//    if (std::abs(score) == MAX_SCORE) {
//        std::cout << "RED ALERT : abs(score) == MAX_SCORE" << std::endl;
//        std::cout << "score = " << score << std::endl;
//        std::cout << "alpha = " << alpha << std::endl;
//    }
        
	RXHashEntry& entry = table[_offsetTable[type_hashtable] | (static_cast<unsigned int>(hash_code>>32) & _maskTable[type_hashtable])];
	
	RXHashRecord& deepest = entry.deepest;
	
	const unsigned long long deepest_lock     = deepest.lock;
	const unsigned long long deepest_packed   = deepest.packed;
	const unsigned long long deepest_hashcode = deepest_lock ^ deepest_packed;
	
	RXHashValue deepest_value(deepest_packed);
	
	int _date = date[type_hashtable	== HASH_WHITE? WHITE:BLACK];

	
	/* try to update deepest entry */
	if (hash_code == deepest_hashcode && selectivity == deepest_value.selectivity  && depth == deepest_value.depth) {
		
		if (score > alpha) {
			if(score > deepest_value.lower)
				deepest_value.lower =  static_cast<short>(score);
		} else { //score <= alpha equivalent score < alpha + VALUE_DISC
            if(score < deepest_value.upper) {
                deepest_value.upper =  static_cast<short>(score);
            }
                
		}
		
		/* control if lower>upper : Instability */
		if(deepest_value.lower>deepest_value.upper) {

			if (score > alpha) {
				deepest_value.upper = MAX_SCORE;
				deepest_value.lower = static_cast<short>(score);
			} else {
				deepest_value.upper = static_cast<short>(score);
				deepest_value.lower = -MAX_SCORE;
			}
		}
		
		deepest_value.date = _date;
		deepest_value.move = move;
		
		deepest.packed = deepest_value.wide_2_compact();
		deepest.lock   = hash_code ^ deepest.packed;
		
	} else {
		
		RXHashRecord& newest = entry.newest;
		
		const unsigned long long newest_packed   = newest.packed;
		const unsigned long long newest_hashcode = newest.lock ^ newest_packed;
		RXHashValue newest_value(newest_packed);
		
		
		/* else try to update newest entry */
		if (hash_code == newest_hashcode && selectivity == newest_value.selectivity  && depth == newest_value.depth) {
			
			if (score > alpha) {
				if(score > newest_value.lower)
					newest_value.lower = static_cast<short>(score);
            } else {
				if(score < newest_value.upper)
					newest_value.upper = static_cast<short>(score);
			}
			
            /* control if lower>upper : Instability */
			if(newest_value.lower>newest_value.upper) {

				if (score > alpha) {
					newest_value.upper = MAX_SCORE;
					newest_value.lower = static_cast<short>(score);
				} else {
					newest_value.upper = static_cast<short>(score);
					newest_value.lower = -MAX_SCORE;
				}
			}
			
			newest_value.date = _date;
			newest_value.move =  move;
			
			newest.packed = newest_value.wide_2_compact();
			newest.lock   = hash_code ^ newest.packed;
			
			
			/* else try to add to deepest entry */
		} else if (deepest_hashcode == hash_code ||  deepest_value.date <  _date ||
													(deepest_value.date == _date && (deepest_value.depth <  depth ||
													(deepest_value.depth == depth && deepest_value.selectivity < selectivity)))) { // priority
        
    
            
            
			if(deepest_hashcode != hash_code &&	(newest_hashcode == hash_code ||  newest_value.date <  deepest_value.date ||
																			  	 (newest_value.date == deepest_value.date && (newest_value.depth <  deepest_value.depth ||
																			  	 (newest_value.depth == deepest_value.depth && newest_value.selectivity <= deepest_value.selectivity))))) {
				
				//copy
				newest.lock   = deepest_lock;
				newest.packed = deepest_packed;
			}
			
			deepest_value.depth = depth;
			deepest_value.date = _date;
			deepest_value.selectivity = selectivity;
			if (score > alpha) {
				deepest_value.upper = MAX_SCORE;
				deepest_value.lower = static_cast<short>(score);
			} else {
				deepest_value.upper = static_cast<short>(score);
				deepest_value.lower = -MAX_SCORE;
			}
			deepest_value.move =  move;
			
			deepest.packed = deepest_value.wide_2_compact();
			deepest.lock   = hash_code ^ deepest.packed;
			
			
			/* else add to newest entry */
		} else {
			
			newest_value.depth = depth;
			newest_value.date = _date;
			newest_value.selectivity = selectivity;
			if (score > alpha) {
				newest_value.upper = MAX_SCORE;
				newest_value.lower = static_cast<short>(score);
			} else {
				newest_value.upper = static_cast<short>(score);
				newest_value.lower = -MAX_SCORE;
			}
			newest_value.move =  move;
			
			newest.packed = newest_value.wide_2_compact();
			newest.lock   = hash_code ^ newest.packed;
			
			
		}
	}	
	
}


std::string RXHashTable::line2String(RXBitBoard& board, int depth, const t_hash type_hashtable) const {

	std::ostringstream buffer;
	mainVariation(buffer, board, type_hashtable, depth, true);
	return buffer.str();
	
}

void RXHashTable::mainVariation(std::ostringstream& buffer, RXBitBoard& board, const t_hash type_hashtable, int depth, const bool myTurn) const {
	
	
	if(depth>0) {
		RXHashValue entry;
		if(get(board, type_hashtable, entry) && entry.move != NOMOVE) {	
			
//			if(entry.lower == entry.upper)
//				buffer << "=";
//			else if (entry.lower == -MAX_SCORE) {
//				buffer << (myTurn? "<<":"<");
//			} else if (entry.upper == MAX_SCORE)
//				buffer << ">";
//			else
//				buffer << "#";

			std::string data = RXMove::index_to_coord(entry.move);
			if(!myTurn)
				std::transform(data.begin(), data.end(), data.begin(), ::tolower);
			
			buffer << data << ' ';
			if(entry.move == PASS) {
				board.do_pass();
				mainVariation(buffer, board, type_hashtable, depth-1, !myTurn);
				board.do_pass();
			} else {
				RXMove& move = _move[board.n_empties][type_hashtable==HASH_WHITE? WHITE:BLACK]; //multithread, for shared use BLACK
				((board).*(board.generate_flips[entry.move]))(move);
				board.do_move(move);
				mainVariation(buffer, board, type_hashtable, depth-1, !myTurn);
				board.undo_move(move);
			}
		} else {
			buffer << "-- ";
			mainVariation(buffer, board, type_hashtable, depth-1, !myTurn);
		}
	}
}

void RXHashTable::mainline(std::ostringstream& buffer, RXBitBoard& board, const int pos, const t_hash type_hashtable) const {

	RXMove& move = _move[board.n_empties][type_hashtable==HASH_WHITE? WHITE:BLACK]; //multithread, for shared use BLACK
	((board).*(board.generate_flips[pos]))(move);
	board.do_move(move);
	mainline(buffer, board, type_hashtable);
	board.undo_move(move);	

}

int RXHashTable::bestmove(RXBitBoard& board, const t_hash type_hashtable) const {
	
	
	RXHashValue entry;
	if(get(board, type_hashtable, entry) && entry.move != NOMOVE)
		return entry.move;
	
	return NOMOVE;
	
}

void RXHashTable::mainline(std::ostringstream& buffer, RXBitBoard& board, const t_hash type_hashtable) const {
	
	
	RXHashValue entry;
	if(get(board, type_hashtable, entry) && entry.move != NOMOVE) {	
					
		buffer << RXMove::index_to_coord(entry.move);
		if(entry.move == PASS) {
			board.do_pass();
			mainline(buffer, board, type_hashtable);
			board.do_pass();
		} else {
			RXMove& move = _move[board.n_empties][type_hashtable==HASH_WHITE? WHITE:BLACK]; //multithread, for shared use BLACK
			((board).*(board.generate_flips[entry.move]))(move);
			board.do_move(move);
			mainline(buffer, board, type_hashtable);
			board.undo_move(move);
		}
	}
	
}


void RXHashTable::printVariation(RXBitBoard& board, const t_hash type_hashtable, const bool myTurn) const {
	
	
		RXHashValue entry;
		if(get(board, type_hashtable, entry) && entry.move != NOMOVE) {	
			
			
			std::string data = RXMove::index_to_coord(entry.move);
			if(!myTurn)
				std::transform(data.begin(), data.end(), data.begin(), ::tolower);
			
			int depth = entry.depth;
			int selectivity = entry.selectivity;
			
			std::cout << data  << " " << depth << "@" << selectivity << "[" << entry.lower << ";" << entry.upper << "] " ;
			if(entry.move == PASS) {
				board.do_pass();
				printVariation(board, type_hashtable, !myTurn);
				board.do_pass();
			} else {
				RXMove& move = _move[board.n_empties][type_hashtable==HASH_WHITE? WHITE:BLACK]; //multithread, for shared use BLACK
				((board).*(board.generate_flips[entry.move]))(move);
				board.do_move(move);
				printVariation(board, type_hashtable, !myTurn);
				board.undo_move(move);
			}
		} else {
			std::cout << std::endl;
		}

	
}


void RXHashTable::copyPV(RXHashTable* from_hash, const t_hash from_type_hash, RXBitBoard& board, const t_hash to_type_hash) {
	
	const RXHashRecord* from_Record = from_hash->get_record(board, from_type_hash);						// source
	if(from_Record != NULL) {
		
		int square = from_Record->get_move();
		
		if(square != NOMOVE) {
			
			RXHashRecord& to_Record = table[_offsetTable[to_type_hash] |(static_cast<unsigned int>(board.hashcode()>>32) & _maskTable[to_type_hash])].deepest;	//dest
			
			if( to_Record.lock  != from_Record->lock) {
				to_Record.lock   = from_Record->lock;
				to_Record.packed = from_Record->packed;
				
			}
			
			if(square == PASS) {
				board.do_pass();
				copyPV(from_hash, from_type_hash, board, to_type_hash);
				board.do_pass();
			} else {
				RXMove& move = _move[board.n_empties][to_type_hash]; //SHARED 3/02/2025
				((board).*(board.generate_flips[square]))(move);
				board.do_move(move);
				copyPV(from_hash, from_type_hash, board, to_type_hash);
				board.undo_move(move);
			}
		}
	}
	
}




void RXHashTable::copyPV_shared_to_color(RXBitBoard& board, const int color) {

//	std::cout << "copyPV_shared_to_color" << std::endl;
//	std::cout << "source : " << line2String(board, 12, HASH_SHARED) << std::endl;

	if(color == BLACK) {
		copyPV(board, HASH_SHARED, HASH_BLACK);
	} else { //player == WHITE
		copyPV(board, HASH_SHARED, HASH_WHITE);
	}

//	std::cout << "dest   : " << line2String(board, 12, (color==BLACK? HASH_BLACK:HASH_WHITE)) << std::endl;

}

void RXHashTable::copyPV_color_to_shared (RXBitBoard& board, const int color) {

//	std::cout << "copyPV_color_to_shared" << std::endl;
//	std::cout << "source : " << line2String(board, 12, (color==BLACK? HASH_BLACK:HASH_WHITE)) << std::endl;
	
	if(color == BLACK) {
		copyPV(board, HASH_BLACK, HASH_SHARED);
	} else { //player == WHITE
		copyPV(board, HASH_WHITE, HASH_SHARED);
	}
	
//	std::cout << "dest   : " << line2String(board, 12, HASH_SHARED) << std::endl;

}

/* *********** Attention ***********/
/*         Aucun moteur actif      */
/* *********** Attention ***********/

void RXHashTable::copyPV(RXBitBoard& board, const t_hash from_hashtable, const t_hash to_hashtable) {
	
	
	const RXHashRecord* from_Record = get_record(board, from_hashtable);						// source
	if(from_Record != NULL) {
		
		int square = from_Record->get_move();
		
		if(square != NOMOVE) {
			
			RXHashRecord& to_Record = table[_offsetTable[to_hashtable] |(static_cast<unsigned int>(board.hashcode()>>32) & _maskTable[to_hashtable])].deepest;	//dest
			
			if( to_Record.lock  != from_Record->lock) {
				to_Record.lock   = from_Record->lock;
				to_Record.packed = from_Record->packed;
				
			}
			
			if(square == PASS) {
				board.do_pass();
				copyPV(board, from_hashtable, to_hashtable);
				board.do_pass();
			} else {
				RXMove& move = _move[board.n_empties][to_hashtable]; //SHARED 03/02/2025
				((board).*(board.generate_flips[square]))(move);
				board.do_move(move);
				copyPV(board, from_hashtable, to_hashtable);
				board.undo_move(move);
			}
		}
	}
}

// *********** Attention ***********
//         Aucun moteur actif
// *********** Attention **********/

void RXHashTable::mergePV(RXBitBoard& board) {
	
//	std::cout << "merge PV" << std::endl;
	
	const RXHashRecord* from_RecordBlack = get_record(board, HASH_BLACK);		// source black
	const RXHashRecord* from_RecordWhite = get_record(board, HASH_WHITE); 		// source white
	
	//entry choice
	const RXHashRecord* from_Record = NULL;
	if(from_RecordBlack != NULL && from_RecordBlack->get_move() != NOMOVE) {
		from_Record = from_RecordBlack;
		if(from_RecordWhite!=NULL && from_RecordWhite->get_move() != NOMOVE) {
			
			if(from_RecordWhite->get_depth() > from_RecordBlack->get_depth())
				from_Record = from_RecordWhite;
			else if((from_RecordWhite->get_depth() == from_RecordBlack->get_depth()) && from_RecordWhite->get_selectivity() > from_RecordBlack->get_selectivity())
				from_Record = from_RecordWhite;
			
		}
		
	} else if(from_RecordWhite != NULL && from_RecordWhite->get_move() != NOMOVE) {
		from_Record = from_RecordWhite;
	}
	
	
	if(from_Record != NULL) {
		
		RXHashRecord& to_Record = table[(static_cast<unsigned int>(board.hashcode()>>32) & _maskTable[HASH_SHARED])].deepest; 		//dest
		
		if( to_Record.lock  != from_Record->lock) {
			to_Record.lock   = from_Record->lock;
			to_Record.packed = from_Record->packed;
		}
		
		int square = from_Record->get_move();
		
		if( square == PASS) {
			board.do_pass();
			mergePV(board);
			board.do_pass();
		} else {
			RXMove& move = _move[board.n_empties][SHARED];						//shared
			((board).*(board.generate_flips[square]))(move);
			board.do_move(move);
			mergePV(board);
			board.undo_move(move);
		}
	}
}

void RXHashTable::protectPV(RXBitBoard& board) {

	t_hash type_hashtable = HASH_SHARED;
		
	if(!_shared) {
		if (board.player == BLACK)
			type_hashtable = HASH_BLACK;
		else
			type_hashtable = HASH_WHITE;
			
	}
	
//	std::cout << "protectPV" << std::endl;
//	std::cout << "source : " << line2String(board, 12, type_hashtable) << std::endl;
	
	protectPV(board, type_hashtable);
	
//	std::cout << "dest   : " << line2String(board, 12, type_hashtable) << std::endl;

}


void RXHashTable::protectPV(RXBitBoard& board, const t_hash	type_hashtable, const bool passed) {
	
    const unsigned long long hash_code = board.hashcode();
	
	RXHashEntry& entry = table[_offsetTable[type_hashtable] | (static_cast<unsigned int>(hash_code>>32) & _maskTable[type_hashtable])];
	RXHashRecord& deepest = entry.deepest;
	RXHashRecord& newest  = entry.newest;
	
	
	if(hash_code == (newest.lock ^ newest.packed)) { //si newest
		//Swap deepest/newest
		const unsigned long long lock   = deepest.lock;
		const unsigned long long packed = deepest.packed;
		
		deepest.lock   = newest.lock;
		deepest.packed = newest.packed;
		
		newest.lock   = lock;
		newest.packed = packed;
		
	}
	
	if(hash_code == (deepest.lock ^ deepest.packed)) {
		
		int square = deepest.get_move();
		
		if(square != NOMOVE) {
			
			//update date
			deepest.packed = ((static_cast<unsigned long long>(date[type_hashtable == HASH_WHITE? WHITE:BLACK]) + 1)<<56) | (deepest.packed & 0x00FFFFFFFFFFFFFFULL);
			deepest.lock   = hash_code ^ deepest.packed;
			
			if(square == PASS) {
				if(!passed) {
					board.do_pass();
					protectPV(board, type_hashtable, true);
					board.do_pass();	
				}
			} else {
				RXMove& move = _move[board.n_empties][type_hashtable == HASH_WHITE? WHITE:BLACK]; //multithread BLACK or SHARED
				((board).*(board.generate_flips[square]))(move);
				board.do_move(move);
				protectPV(board, type_hashtable, false);
				board.undo_move(move);
			}
		}
	}
}


