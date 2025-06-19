/*
 *  RXEndGame.cpp
 *  Roxane
 *
 *  Created by Bruno Causse on 11/06/05.
 *  Copyleft 2005-2025 personnel.
 *
 */
#include <iostream>
#include <iomanip>
#include <cmath>


#include "RXEngine.hpp"
#include "RXRoxane.hpp"
#include "RXBitBoard.hpp"

//#define USE_SPLIT_AT_ETC_MOBILITY
//for record fforum-40-59 setting
//#define USE_SPLIT_AT_ROOT


const bool RXEngine::USE_ETC = true;
const bool RXEngine::USE_STABILITY = true;
const bool RXEngine::USE_ENHANCED_STABLILITY = true;

//const int RXEngine::stability_threshold[] =
//{   6500, 6500, 6500, 6500,  800, 1000, 1400, 1600,
//    1800, 2200, 2400, 2600, 2800, 3000, 3200, 3400,
//    3400, 3600, 3800, 4000, 4000, 4200, 4200, 4400,
//    4400, 4600, 4600, 4800, 4800, 5000, 5200, 5200,
//    5400, 5400, 5600, 5600, 5800, 5800, 6000, 6000,
//    6200, 6200, 6500, 6500, 6500, 6500, 6500, 6500,
//    6500, 6500, 6500, 6500, 6500, 6500, 6500, 6500,
//    6500, 6500, 6500, 6500, 6500, 6500, 6500, 6500};

const int RXEngine::stability_threshold[] =
{   6500, 6500, 6500, 6500,  600,  800, 1000, 1400,
    1600, 1800, 2200, 2400, 2600, 2800, 3000, 3200,
    3400, 3600, 3800, 4000, 4000, 4200, 4200, 4400,
    4400, 4600, 4600, 4800, 4800, 5000, 5200, 5200,
    5400, 5400, 5600, 5600, 5800, 5800, 6000, 6000,
    6200, 6200, 6500, 6500, 6500, 6500, 6500, 6500,
    6500, 6500, 6500, 6500, 6500, 6500, 6500, 6500,
    6500, 6500, 6500, 6500, 6500, 6500, 6500, 6500};


/* standart setting */
const int RXEngine::EG_DEEP_TO_MEDIUM = 17;
const int RXEngine::EG_MEDIUM_HI_TO_LOW = 14;
const int RXEngine::EG_MEDIUM_TO_SHALLOW = 8;
const int RXEngine::MIN_DEPTH_USE_ENDCUT = 16;


/* for record fforum-40-59 setting*/
//const int RXEngine::EG_DEEP_TO_MEDIUM = 16;
//const int RXEngine::EG_MEDIUM_HI_TO_LOW = 13;
//const int RXEngine::EG_MEDIUM_TO_SHALLOW = 7;
//const int RXEngine::MIN_DEPTH_USE_ENDCUT = 16;


/*!
 * \brief  Evaluate a position using a shallow Alphabeta.
 *
 * This function is used when there are 5 or 6 empty squares on the board. Here,
 * optimizations are in favour of speed instead of efficiency. A simple
 * alphabeta is used because of the low branching factor that makes PVS less
 * efficient.
 * \param board   board.
 * \param alpha   lower bound.
 * \param beta    upper bound.
 * \param passed  a flag indicating if previous move was a pass.
 * \return        the final score, as a disc difference.
 */

int RXEngine::EG_alphabeta_parity(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed) {
    
    int score, bestscore = UNDEF_SCORE;
    
    if(USE_STABILITY) {
        if ( beta >= stability_threshold[board.n_empties]) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= alpha )
                return stability_bound;
            if ( stability_bound < beta )
                beta = stability_bound;
        }
    }
    
    
    const unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
    
    if(legal_movesBB) {
        
        RXMove& move = threads[threadID]._move[board.n_empties][1];
        
        if(board.n_empties == 5) {
            
            unsigned long long parity_movesBB = RXBitBoard::QUADRANT_MASK[board.parity];
            
            for (int parity = 1; parity >= 0; parity--) {
                unsigned long long movesBB = legal_movesBB & parity_movesBB;
                
                for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next) {
                    if (movesBB & 0x1ULL<<empties->position) {
                        ((board).*(board.generate_flips[empties->position]))(move);
                        
                        board.do_move(move);
                        score = -board.final_score_4(pv, -beta, -alpha, false);
                        board.undo_move(move);
                        
                        if (score >= beta)
                            return score;
                        
                        if (score > bestscore) {
                            bestscore = score;
                            if (bestscore > alpha) {
                                alpha = bestscore;
                            }
                        }
                    }
                }
                
                parity_movesBB = ~parity_movesBB;
            }
            
            
        } else {
            
            if(board.parity == 0 || board.parity == 0xF) {
                
                for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next) {
                    if (legal_movesBB & 0x1ULL<<empties->position) {
                        ((board).*(board.generate_flips[empties->position]))(move);
                        
                        board.do_move(move);
                        score = -EG_alphabeta_parity(threadID, board, pv, -beta, -alpha, false);
                        board.undo_move(move);
                        
                        if (score >= beta)
                            return score;
                        
                        if (score > bestscore) {
                            bestscore = score;
                            if (bestscore > alpha) {
                                alpha = bestscore;
                            }
                        }
                    }
                }
                
            } else {
                
                unsigned long long parity_movesBB = RXBitBoard::QUADRANT_MASK[board.parity];
                
                for (int parity = 1; parity >= 0; parity--) {
                    unsigned long long movesBB = legal_movesBB & parity_movesBB;
                    
                    for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next) {
                        if (movesBB & 0x1ULL<<empties->position) {
                            ((board).*(board.generate_flips[empties->position]))(move);
                            
                            board.do_move(move);
                            score = -EG_alphabeta_parity(threadID, board, pv, -beta, -alpha, false);
                            board.undo_move(move);
                            
                            if (score >= beta)
                                return score;
                            
                            if (score > bestscore) {
                                bestscore = score;
                                if (bestscore > alpha) {
                                    alpha = bestscore;
                                }
                            }
                        }
                    }
                    
                    parity_movesBB = ~parity_movesBB;
                }
            }
        }
        
    } else {
        
        if(passed) {
            board.n_nodes--;
            bestscore = board.final_score();
        } else {
            //update/restore player
            board.player ^= 1;
            board.n_nodes++;
            bestscore = -EG_alphabeta_parity(threadID, board, pv, -beta, -alpha, true);
            //update/restore player
            board.player ^= 1;
        }
    }
    
    return bestscore;
}


//unused [if hashmove at 7 empties]
int RXEngine::EG_alphabeta_hash_parity(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed) {
    
    
    int score, bestscore = UNDEF_SCORE;
    int lower = alpha;
    int upper = beta;
    
    int bestmove = NOMOVE;
    
    const unsigned long long  hash_code = board.hashcode();
    
    
    hTable->entry_prefetch(hash_code, type_hashtable);
    
    if(USE_STABILITY) {
        
        if ( upper >= stability_threshold[board.n_empties] ) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= lower )
                return stability_bound;
            
            if ( stability_bound < upper )
                upper = stability_bound;
            
        }
    }
    
    //synchronized acces
    RXHashValue entry;
    if(!pv && hTable->get(hash_code, type_hashtable, entry)) {
        
        if(entry.selectivity == NO_SELECT && entry.depth >= board.n_empties) { //
            
            if (upper > entry.upper) {
                upper = entry.upper;
                if (upper <= lower) {
                    return upper;
                }
            }
            if (lower < entry.lower) {
                lower = entry.lower;
                if (lower >= upper) {
                    return lower;
                }
            }
            
            //04/02/2025
            bestmove = entry.move;
            
        }
    }
    
    
    if(bestmove != PASS) {
        
        RXMove move = threads[threadID]._move[board.n_empties][1];
        
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(move);
            
            // first move
            board.do_move(move);
            bestscore = -EG_alphabeta_parity(threadID, board, pv, -upper, -lower, false);
            board.undo_move(move);
            
            if (bestscore > lower)
                lower = bestscore;
            
        }
        
        if(lower < upper) {
            
            unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
            if(bestmove !=NOMOVE)
                legal_movesBB ^= 0x1ULL<<bestmove;
            
            if(board.parity == 0 || board.parity == 0xF) {
                
                for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next) {
                    if (legal_movesBB & 0x1ULL<<empties->position) {
                        ((board).*(board.generate_flips[empties->position]))(move);
                        
                        board.do_move(move);
                        score = -EG_alphabeta_parity(threadID, board, pv, -upper, -lower, false);
                        board.undo_move(move);
                        
                        if (score >= upper) {
                            hTable->update(hash_code, pv, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, score, move.position);
                            return score;
                        }
                        
                        if (score > bestscore) {
                            bestmove = empties->position;
                            bestscore = score;
                            if (bestscore > lower) {
                                lower = bestscore;
                            }
                        }
                    }
                }
                
            } else {
                
                unsigned long long parity_movesBB = RXBitBoard::QUADRANT_MASK[board.parity];
                
                for (int parity = 1; parity >= 0; parity--) {
                    const unsigned long long movesBB = legal_movesBB & parity_movesBB;
                    
                    for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next) {
                        if ((movesBB & 0x1ULL<<empties->position) ) {
                            ((board).*(board.generate_flips[empties->position]))(move);
                            
                            board.do_move(move);
                            score = -EG_alphabeta_parity(threadID, board, pv, -upper, -lower, false);
                            board.undo_move(move);
                            
                            if (score >= upper) {
                                hTable->update(hash_code, pv, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, score, move.position);
                                return score;
                            }
                            
                            if (score > bestscore) {
                                bestmove = empties->position;
                                bestscore = score;
                                if (bestscore > lower) {
                                    lower = bestscore;
                                }
                            }
                        }
                    }
                    
                    parity_movesBB = ~parity_movesBB;
                }
            }
        }
    }
    
    //if PASS
    if (bestscore == UNDEF_SCORE) {
        if (passed) {
            board.n_nodes--;
            bestscore = board.final_score();
            alpha = -(upper = +MAX_SCORE);
            bestmove = NOMOVE;
        } else {
            board.do_pass();
            board.n_nodes++;
            bestscore = -EG_alphabeta_hash_parity(threadID, board, pv, -upper, -lower, true);
            board.do_pass();
            bestmove = PASS;
        }
    }
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(hash_code, pv, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
    
    return bestscore;
}

//7 empty
int RXEngine::EG_alphabeta_hash_mobility(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed) {
    
    int bestscore = UNDEF_SCORE;
    int lower = alpha;
    int upper = beta;
    
    int bestmove = NOMOVE;
    
    const unsigned long long  hash_code = board.hashcode();
    
    
    hTable->entry_prefetch(hash_code, type_hashtable);
    
    if(USE_STABILITY) {
        
        if ( upper >= stability_threshold[board.n_empties] ) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= lower )
                return stability_bound;
            
            if ( stability_bound < upper )
                upper = stability_bound;
            
        }
    }
    
    
    RXHashValue entry;
    if(!pv && hTable->get(hash_code, type_hashtable, entry)) {
        
        if(entry.selectivity == NO_SELECT && entry.depth >= board.n_empties) { //
            
            if (upper > entry.upper) {
                upper = entry.upper;
                if (upper <= lower) {
                    return upper;
                }
            }
            if (lower < entry.lower) {
                lower = entry.lower;
                if (lower >= upper) {
                    return lower;
                }
            }
            
            //04/02/2025
            bestmove = entry.move;
            
        }
    }
    
    
    if(bestmove != PASS) {
        
        RXMove* list = threads[threadID]._move[board.n_empties];
        RXMove* move = list + 1;
        
        
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            
            // first move
            board.do_move(*move);
            //            if (pv)
            //                bestscore = -EG_alphabeta_hash_parity(threadID, board, pv, -upper, -lower, false);
            //            else
            bestscore = -EG_alphabeta_parity(threadID, board, pv, -upper, -lower, false);
            board.undo_move(*move);
            
            if (bestscore > lower)
                lower = bestscore;
            
        }
        
        if(lower < upper) {
            
            //for all empty square
            unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
            if(bestmove !=NOMOVE)
                legal_movesBB ^=  0x1ULL<<bestmove;
            
            if(legal_movesBB) {
                
                RXMove* previous = list;
                
                for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
                    if(legal_movesBB & 0x1ULL<<empties->position) {
                        
                        ((board).*(board.generate_flips[empties->position]))(*move);
                        
                        previous = previous->next = move++;
                    }
                
                previous->next = NULL;
                
                if((list->next)->next != NULL) { //nb moves > 1
                    
                    const int p = board.player;
                    const int o = p^1;
                    
                    //sort list by mobility
                    for(RXMove* iter = list->next; iter != NULL; iter = iter->next) {
                        
                        board.n_nodes++;
                        
                        const unsigned long long p_discs = board.discs[p] | (iter->flipped | iter->square);
                        const unsigned long long o_discs = board.discs[o] ^ iter->flipped;
                        
                        iter->score = (RXBitBoard::count_potential_moves(o_discs, p_discs)<<4)
                        - (RXBitBoard::get_corner_stability(p_discs)<<2)
                        - (((board.parity & RXBitBoard::QUADRANT_ID[iter->position])>>RXBitBoard::QUADRANT_SHITF[iter->position])<<4);
                        
                    }
                    
                    
                }
                
                
                int score;
                do {
                    
                    RXMove* previous_move = list;
                    RXMove* move = previous_move->next;
                    
                    if(move->next != NULL) {
                        
                        RXMove* previous_iter = move;
                        for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                            if(iter->score < move->score) {
                                move = iter;
                                previous_move = previous_iter;
                            }
                        }
                        
                        if(previous_move != list) {
                            //move to front
                            previous_move->next = move->next;
                            move->next = list->next;
                            list->next = move;
                        }
                    }
                    
                    
                    board.do_move(*move);
                    score = -EG_alphabeta_parity(threadID, board, pv, -upper, -lower, false);
                    board.undo_move(*move);
                    
                    if (score > bestscore) {
                        bestscore = score;
                        bestmove  = move->position;
                        if (bestscore > lower)
                            lower = bestscore;
                    }
                    
                    list = list->next;
                    
                } while(lower < upper && list->next != NULL);
            }
        }
    }
    
    
    //if PASS
    if (bestscore == UNDEF_SCORE) {
        if (passed) {
            board.n_nodes--;
            bestscore = board.final_score();
            alpha = -(upper = +MAX_SCORE);
            bestmove = NOMOVE;
        } else {
            board.do_pass();
            board.n_nodes++;
            bestscore = -EG_alphabeta_hash_mobility(threadID, board, pv, -upper, -lower, true);
            board.do_pass();
            bestmove = PASS;
        }
    }
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(hash_code, pv, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
    
    return bestscore;
}

/*!
 * \brief Evaluate a position with a deep Principal Variation Search algorithm.
 *
 * This function is used when there are still many empty squares on the board. Move
 * ordering, hash table cutoff, etc. are used in
 * order to diminish the size of the tree to analyse, but at the expense of a
 * slower speed.
 *
 * \param board      board.
 * \param alpha      lower bound.
 * \param beta       upper bound.
 * \param passed     a flag indicating if previous move was a pass.
 * \return the final score, as a disc difference.
 */
int RXEngine::EG_PVS_hash_mobility(int threadID, RXBitBoard& board, const bool pv, int alpha, int beta, bool passed)
{
    
    int bestmove = NOMOVE;
    int lower = alpha;
    int upper = beta;
    
    const unsigned long long  hash_code = board.hashcode();
    
    
    hTable->entry_prefetch(hash_code, type_hashtable);
    
    if(USE_STABILITY) {
        
        /*
         calculated stability is less than or equal to the real stability
         stability_bound is overestimated.
         score_max <= Stability_bound <= alpha ==> cutoff
         &
         score_max<=stability_bound < beta  ==> adjustment search window
         */
        
        
        if ( upper >= stability_threshold[board.n_empties] ) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= lower )
                return stability_bound;
            
            if ( stability_bound < upper )
                upper = stability_bound;
            
        }
        
    }
    
    
    RXHashValue entry;
    if(!pv && hTable->get(hash_code, type_hashtable, entry)) {
        
        if(entry.selectivity == NO_SELECT && entry.depth >= board.n_empties) { //
            
            
            if (upper > entry.upper) {
                upper = entry.upper;
                if (upper <= lower) {
                    return upper;
                }
            }
            if (lower < entry.lower) {
                lower = entry.lower;
                if (lower >= upper) {
                    return lower;
                }
            }
            
            //04/02/2025
            bestmove = entry.move;
            
        }
    }
    
    int bestscore = UNDEF_SCORE;
    
    if(bestmove != PASS) {
        
        RXMove* list = threads[threadID]._move[board.n_empties];
        RXMove* move = list + 1;
        
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            
            /* first move */
            board.do_move(*move);
            if (board.n_empties < EG_MEDIUM_TO_SHALLOW) {
                bestscore = -EG_alphabeta_hash_mobility(threadID, board, pv, -upper, -lower, false);
            } else {
                bestscore = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -lower, false);
            }
            board.undo_move(*move);
            
            
            //bestmove = move->position;
            if (bestscore > lower)
                lower = bestscore;
            
        }
        
        if(lower < upper) {
            
            //for all empty square
            unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
            if(bestmove !=NOMOVE)
                legal_movesBB ^= 0x1ULL<<bestmove;
            
            if(legal_movesBB) {
                
                RXMove* previous = list;
                
                
                for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
                    if(legal_movesBB & 0x1ULL<<empties->position) {
                        
                        ((board).*(board.generate_flips[empties->position]))(*move);
                        previous = previous->next = move++;
                    }
                
                previous->next = NULL;
                
                
                if((list->next)->next != NULL) { //nb moves > 1
                    
                    const int p = board.player;
                    const int o = p^1;
                    
                    //sort list by mobility
                    for(RXMove* iter = list->next; iter != NULL; iter = iter->next) {
                        
                        board.n_nodes++;
                        
                        const unsigned long long p_discs = board.discs[p] | (iter->flipped | iter->square);
                        const unsigned long long o_discs = board.discs[o] ^ iter->flipped;
                        
                        iter->score = (RXBitBoard::get_mobility(o_discs, p_discs)<<5)
                        - (RXBitBoard::get_corner_stability(p_discs)<<2)
                        + RXBitBoard::count_potential_moves(o_discs, p_discs)
                        - ((board.parity & RXBitBoard::QUADRANT_ID[iter->position])>>RXBitBoard::QUADRANT_SHITF[iter->position]);
                        
                    }
                    
                    
                }
                
                
                if(bestmove == NOMOVE) {
                    
                    RXMove* previous_move = list;
                    RXMove* move = previous_move->next;
                    
                    RXMove* previous_iter = move;
                    for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                        if(iter->score < move->score) {
                            move = iter;
                            previous_move = previous_iter;
                        }
                    }
                    
                    if(previous_move != list) {
                        //move to front
                        previous_move->next = move->next;
                        move->next = list->next;
                        list->next = move;
                    }
                    
                    board.do_move(*move);
                    if (board.n_empties < EG_MEDIUM_TO_SHALLOW) {
                        bestscore = -EG_alphabeta_hash_mobility(threadID, board, pv, -upper, -lower, false);
                    } else {
                        bestscore = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -lower, false);
                    }
                    board.undo_move(*move);
                    
                    bestmove = move->position;
                    if (bestscore > lower)
                        lower = bestscore;
                    
                    // next move
                    list = list->next;
                    
                }
                
                
                // other moves : try to refute the first/best one
                int score;
                for(; lower < upper && list->next != NULL; list = list->next) {
                    
                    RXMove* previous_move = list;
                    RXMove* move = previous_move->next;
                    
                    if(move->next != NULL) {
                        RXMove* previous_iter = move;
                        for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                            if(iter->score < move->score) {
                                move = iter;
                                previous_move = previous_iter;
                            }
                        }
                        
                        if(previous_move != list) {
                            //move to front
                            previous_move->next = move->next;
                            move->next = list->next;
                            list->next = move;
                        }
                    }
                    
                    
                    board.do_move(*move);
                    
                    if (board.n_empties < EG_MEDIUM_TO_SHALLOW) {
                        score = -EG_alphabeta_hash_mobility(threadID, board, pv, -upper, -lower, false);
                    } else {
                        score = -EG_PVS_hash_mobility(threadID, board, false, -lower - VALUE_DISC, -lower, false);
                        if (lower < score && score < upper)
                            score = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -score, false);
                    }
                    
                    board.undo_move(*move);
                    
                    if (score > bestscore) {
                        bestscore = score;
                        bestmove = move->position;
                        if (bestscore > lower)
                            lower = bestscore;
                    }
                }
            }
        }
    }
    
    
    if (bestscore == UNDEF_SCORE) {
        if (passed) {
            board.n_nodes--;
            bestscore = board.final_score();
            alpha = -(upper = +MAX_SCORE);
            bestmove = NOMOVE;
        } else {
            board.do_pass();
            board.n_nodes++;
            bestscore = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -lower, true);
            board.do_pass();
            bestmove = PASS;
        }
    }
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(hash_code, pv, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
    
    return bestscore;
}

/*!
 * \brief Evaluate a position with a deep Principal Variation Search algorithm.
 *
 * This function is used when there are still many empty squares on the board. Move
 * ordering, hash table cutoff, enhanced transposition cutoff, etc. are used in
 * order to diminish the size of the tree to analyse, but at the expense of a
 * slower speed.
 *
 * \param sBoard      sBoard : just for conformity with split method.
 * \param alpha      lower bound.
 * \param beta       upper bound.
 * \param passed     a flag indicating if previous move was a pass.
 * \return the final score, as a disc difference.
 */
int RXEngine::EG_PVS_ETC_mobility(int threadID, RXBBPatterns& sBoard, const bool pv, int alpha, int beta, bool passed)
{
    
    //	assert(alpha>=-64*VALUE_DISC && beta<=64*VALUE_DISC);
    
    if(abort.load() || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    RXBitBoard& board = sBoard.board;
    
    int bestmove = NOMOVE;
    int lower = alpha;
    int upper = beta;
    
    const unsigned long long  hash_code = board.hashcode();
    //hTable->entry_prefetch(hash_code, type_hashtable);
    
    RXHashValue entry;
    if(hTable->get(hash_code, type_hashtable, entry)) {
        
        if(!pv &&  entry.selectivity == NO_SELECT && entry.depth >= board.n_empties) {
            
            if (upper > entry.upper) {
                upper = entry.upper;
                if (upper <= lower) {
                    return upper;
                }
            }
            if (lower < entry.lower) {
                lower = entry.lower;
                if (lower >= upper) {
                    return lower;
                }
            }
            
        }
        
        bestmove = entry.move;
        
    }
    
    if(USE_STABILITY && bestmove == NOMOVE) {
        
        /*
         calculated stability is less than or equal to the real stability
         stability_bound is overestimated.
         score_max <= Stability_bound <= alpha ==> cutoff
         &
         score_max<=stability_bound < beta  ==> adjustment search window
         */
        if (  upper >= stability_threshold[board.n_empties] ) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= lower )
                return stability_bound;
            
            if ( stability_bound < upper )
                upper = stability_bound;
            
        }
        
    }
    
    int bestscore = UNDEF_SCORE;
    
    RXMove* list = threads[threadID]._move[board.n_empties];
    list->next = NULL;
    
    if(bestmove != PASS) {
        
        RXMove* move = list + 1;
        RXMove* previous = list;
        
        unsigned long long  hashcode_after_move;
        
        //ENHANCED TRANSPOSITION CUTOFF
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            board.n_nodes++;
            
            if(USE_ETC) {
                hashcode_after_move = board.hashcode_after_move(move);
                hTable->entry_prefetch(hashcode_after_move, type_hashtable);
            }
            
            
            if ( USE_ENHANCED_STABLILITY && lower <= -stability_threshold[board.n_empties-1]  ) {
                const unsigned long long d_player = board.discs[board.player] | (move->flipped | move->square);
                int stability_bound = 2 * board.get_stability(d_player, board.discs[board.player^1] ^ move->flipped) - 64*VALUE_DISC;
                if ( stability_bound >= upper ) {
                    return stability_bound;
                }
            }
            
            //synchronized acces
            if(USE_ETC && !pv && hTable->get(hashcode_after_move, type_hashtable, entry) && entry.selectivity == NO_SELECT && entry.depth>=(board.n_empties-1)) {
                
                if(-entry.upper >= upper) {
                    return -entry.upper ;
                }
                
            }
            
            
            previous = previous->next = move++;
        }
        
        //for all empty square
        unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
        if(bestmove !=NOMOVE)
            legal_movesBB ^= 0x1ULL<<bestmove;
        
        for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
            if(legal_movesBB & 0x1ULL<<empties->position){
                
                ((board).*(board.generate_flips[empties->position]))(*move);
                board.n_nodes++;
                
                if(USE_ETC) {
                    hashcode_after_move = board.hashcode_after_move(move);
                    hTable->entry_prefetch(hashcode_after_move, type_hashtable);
                }
                
                if ( USE_ENHANCED_STABLILITY && lower <= -stability_threshold[board.n_empties-1]  ) {
                    const unsigned long long d_player = board.discs[board.player] | (move->flipped | move->square);
                    int stability_bound = 2 * board.get_stability(d_player, board.discs[board.player^1] ^ move->flipped) - 64*VALUE_DISC;
                    if ( stability_bound >= upper ) {
                        return stability_bound;
                    }
                }
                move->score = 0; //not in Hash
                
                //synchronized acces
                if(USE_ETC && hTable->get(hashcode_after_move, type_hashtable, entry) && entry.depth>=(board.n_empties-1)) {
                    
                    if (!pv && entry.selectivity == NO_SELECT && -entry.upper >= upper )
                        return -entry.upper ;
                    
                    move->score = -2*VALUE_DISC; //in hash
                    
                }
                
                
                previous = previous->next = move++;
                
            }
        
        previous->next = NULL;
        
    }
    
    if (list->next == NULL) {
        if (passed) {
            board.n_nodes--;
            bestscore = board.final_score();
            alpha = -(upper = +MAX_SCORE);
            bestmove = NOMOVE;
        } else {
            board.do_pass();
            board.n_nodes++;
            bestscore = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -lower, true);
            board.do_pass();
            bestmove = PASS;
        }
    } else {
        
        
        if(bestmove != NOMOVE) {
            /* first move */
            list = list->next;
            
            board.do_move(*list);
            if (board.n_empties < EG_MEDIUM_HI_TO_LOW) {
                bestscore = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -lower, false);
            } else {
                bestscore = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -lower, false);
            }
            board.undo_move(*list);
            
            //interrupt search
            if(abort.load()  || thread_should_stop(threadID))
                return INTERRUPT_SEARCH;
            
            //bestmove = list->position;
            if (bestscore > lower)
                lower = bestscore;
            
        }
        
        
        if(lower < upper && list->next != NULL) {
            
            if((list->next)->next != NULL) {
                
                const int p = board.player;
                const int o = p^1;
                
                //calc answer move->score
                for(RXMove* iter = list->next; iter != NULL; iter = iter->next) {
                    
                    board.n_nodes++;
                    
                    const unsigned long long p_discs = board.discs[p] | (iter->flipped | iter->square);
                    const unsigned long long o_discs = board.discs[o] ^ iter->flipped;
                    
                    //score for try : mobility * VALUE_DISC - corner_stability * 8
                    iter->score += (2*RXBitBoard::get_mobility(o_discs, p_discs) - RXBitBoard::get_corner_stability(p_discs))*VALUE_DISC;
                    /* + (RXBitBoard::count_potential_moves(o_discs, p_discs)*VALUE_DISC/64)*/
                    /*- ((board.parity & RXBitBoard::QUADRANT_ID[iter->position])>>RXBitBoard::QUADRANT_SHITF[iter->position])*VALUE_DISC/64;*/
                    
                    
                    
                }
                
            }
            
            if(bestmove == NOMOVE) {
                
                //find moves with worst answer
                
                RXMove* previous_move = list;
                RXMove* move = previous_move->next;
                
                
                RXMove* previous_iter = move;
                for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                    if(iter->score < move->score) {
                        move = iter;
                        previous_move = previous_iter;
                    }
                }
                
                if(previous_move != list) {
                    //move to front
                    previous_move->next = move->next;
                    move->next = list->next;
                    list->next = move;
                }
                
                board.do_move(*move);
                if (board.n_empties < EG_MEDIUM_HI_TO_LOW) {
                    bestscore = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -lower, false);
                } else {
                    bestscore = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -lower, false);
                }
                board.undo_move(*move);
                
                //interrupt search
                if(abort.load()  || thread_should_stop(threadID))
                    return INTERRUPT_SEARCH;
                
                bestmove = move->position;
                if (bestscore > lower)
                    lower = bestscore;
                
                // next move
                list = list->next;
                
            }
            
#ifdef USE_SPLIT_AT_ETC_MOBILITY
            
            //unused: only for conformity with split method
            bool selective_cutoff= false;
            bool child_selective_cutoff = false;
            
#endif
            
            // other moves : try to refute the first/best one
            int score;
            for(;lower < upper && list->next != NULL; list = list->next) {
                
                //find moves with worst answer
                
                RXMove* move = list->next;
                if(move->next != NULL) {
                    
#ifdef USE_SPLIT_AT_ETC_MOBILITY
                    
                    // Split? not efficient : unused
                    if(activeThreads > 1  && !abort.load() && board.n_empties>=EG_MEDIUM_HI_TO_LOW
                       && !thread_should_stop(threadID) && idle_thread_exists(threadID)
                       && split(sBoard, pv, 0, board.n_empties, NO_SELECT, selective_cutoff, child_selective_cutoff,
                                lower, upper, bestscore, bestmove, list, threadID, RXSplitPoint::END_ETC_MOBILITY))
                        
                        break;
                    
#endif
                    
                    RXMove* previous_move = list;
                    RXMove* previous_iter = move;
                    
                    for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                        if(iter->score < move->score) {
                            move = iter;
                            previous_move = previous_iter;
                        }
                    }
                    
                    if(previous_move != list) {
                        //move to front
                        previous_move->next = move->next;
                        move->next = list->next;
                        list->next = move;
                    }
                }
                
                board.do_move(*move);
                if (board.n_empties < EG_MEDIUM_HI_TO_LOW) {
                    score = -EG_PVS_hash_mobility(threadID, board, false, -lower - VALUE_DISC, -lower, false);
                    if (lower < score && score < upper)
                        score = -EG_PVS_hash_mobility(threadID, board, pv, -upper, -score, false);
                } else {
                    score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -lower - VALUE_DISC, -lower, false);
                    if (lower < score && score < upper)
                        score = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -score, false);
                }
                board.undo_move(*move);
                
                //interrupt search
                if(abort.load()  || thread_should_stop(threadID))
                    return INTERRUPT_SEARCH;
                
                if (score > bestscore) {
                    bestscore = score;
                    bestmove = move->position;
                    if (bestscore > lower)
                        lower = bestscore;
                }
            }
        }
    }
    
    //interrupt search
    if(abort.load()  || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(hash_code, pv, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
    
    
    return bestscore;
}

// EG_SP_search_ETC_Mobility() is used to search from a PV split point.  This function
// is called by each thread working at the split point.  It is similar to
// the normal EG_PVS_ETC_Mobility() function, but simpler.  Because we have already
// probed the hash table and searched the first move before splitting, we
// don't have to repeat all this work in EG_SP_search_ETC_Mobility().  We also don't
// need to store anything to the hash table here:  This is taken care of
// after we return from the split point.

void RXEngine::EG_SP_search_ETC_Mobility(RXSplitPoint* sp, const unsigned int threadID) {
    
    
    RXBBPatterns& sBoard = sp->sBoardStack[threadID];
    sBoard = *(sp->sBoard); //operator=
    RXBitBoard& board = sBoard.board;
    board.n_nodes = 0;
    
    
    //here sp->beta is const
    while(sp->alpha < sp->beta && !abort.load()  && !thread_should_stop(threadID)) {
        
        pthread_mutex_lock(&(sp->lock));
        
        if(sp->list->next == NULL) {
            pthread_mutex_unlock(&(sp->lock));
            break;
        }
        
        RXMove* move = sp->list->next;
        sp->list = move;
        
        pthread_mutex_unlock(&(sp->lock));
        
        board.do_move(*move);
        
        int alpha = sp->alpha; //local copy
        /*
         int score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -alpha - VALUE_DISC, -alpha, false);
         
         if (alpha < score && score < sp->beta)
         score = -EG_PVS_ETC_mobility(threadID, sBoard, sp->pv, -sp->beta, -score, false);
         */
        int score;
        if (board.n_empties < EG_MEDIUM_HI_TO_LOW) {
            score = -EG_PVS_hash_mobility(threadID, board, false, -alpha - VALUE_DISC, -alpha, false);
            if (alpha < score && score < sp->beta)
                score = -EG_PVS_hash_mobility(threadID, board, sp->pv, -sp->beta, -score, false);
        } else {
            score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -alpha - VALUE_DISC, -alpha, false);
            if (alpha < score && score < sp->beta)
                score = -EG_PVS_ETC_mobility(threadID, sBoard, sp->pv, -sp->beta, -score, false);
        }
        
        
        board.undo_move(*move);
        
        if(abort.load()  || thread_should_stop(threadID))
            break;
        
        
        //first without mutex
        if(score > sp->bestscore) {
            
            //update
            pthread_mutex_lock(&(sp->lock));
            
            // New best move?
            if(score > sp->bestscore) {
                sp->bestscore = score;
                sp->bestmove = move->position;
                if(score > sp->alpha) {
                    
                    if(score >= sp->beta) {
                        sp->explored =true;
                    } else {
                        sp->alpha = score;
                    }
                }
            }
            
            pthread_mutex_unlock(&(sp->lock));
        }
        
    }
    
    pthread_mutex_lock(&(sp->lock));
    
    sp->sBoard->board.n_nodes += board.n_nodes;
    
    sp->slaves[threadID] = false;
    sp->n_Slaves--;
    
    pthread_mutex_unlock(&(sp->lock));
}




int RXEngine::EG_PVS_deep(int threadID, RXBBPatterns& sBoard, const bool pv, const int selectivity, bool& selective_cutoff, int alpha, int beta, bool passed) {
    
    
    if(abort.load() || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    
    //time gestion
    if(dependent_time && get_current_dependentTime() > time_limit()) {
        abort.store(true);
        return INTERRUPT_SEARCH;
    }
    
    int bestmove = NOMOVE;
    int lower = alpha;
    int upper = beta;
    
    
    RXBitBoard& board = sBoard.board;
    selective_cutoff = false;
    
    
    //synchronized acces
    RXHashValue entry;
    const unsigned long long hash_code = board.hashcode();
    if(hTable->get(hash_code, type_hashtable, entry)) {
        
        
        if(!pv &&  entry.selectivity >= selectivity && entry.depth >= board.n_empties) {
            
            if (upper > entry.upper) {
                
                upper = entry.upper;
                
                if (upper <= lower) {
                    if(entry.selectivity != NO_SELECT)
                        selective_cutoff = true;
                    
                    return upper;
                }
            }
            
            if (lower < entry.lower) {
                
                lower = entry.lower;
                
                if (lower >= upper) {
                    if(entry.selectivity != NO_SELECT)
                        selective_cutoff = true;
                    
                    return lower;
                }
            }
            
        }
        
        bestmove = entry.move;
        
    }
    
    if(USE_STABILITY && bestmove == NOMOVE) {
        
        /*
         calculated stability is less than or equal to the real stability
         stability_bound is overestimated.
         score_max <= Stability_bound <= alpha ==> cutoff
         &
         score_max<=stability_bound < beta  ==> adjustment search window
         */
        if (upper >= stability_threshold[board.n_empties] ) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= lower )
                return stability_bound;
            
            if ( stability_bound < upper )
                upper = stability_bound;
            
        }
        
    }
    
    //    IID & IIS
    if(pv && bestmove == NOMOVE && board.n_empties >= EG_DEEP_TO_MEDIUM) {
        bool child_selective_cutoff = false;
        
        if(selectivity > EG_HIGH_SELECT)
            EG_PVS_deep(threadID, sBoard, pv, selectivity-1, child_selective_cutoff, lower, upper, passed);
        else
            MG_PVS_deep(threadID, sBoard, pv, MG_SELECT, board.n_empties-(USE_PV_EXTENSION? 10 : 8), child_selective_cutoff, lower, upper, passed);
        
        if(abort.load() || thread_should_stop(threadID))
            return INTERRUPT_SEARCH;
        
        if(hTable->get(hash_code, type_hashtable, entry))
            bestmove = entry.move;
    }
    
    
    int score, bestscore = UNDEF_SCORE;
    
    RXMove* list = threads[threadID]._move[board.n_empties];
    list->next = NULL;
    
    if(bestmove != PASS) {
        
        
        RXMove* move = list + 1;
        RXMove* previous = list;
        
        //ENHANCED TRANSPOSITION CUTOFF
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            board.n_nodes++;
            
            if (USE_ENHANCED_STABLILITY && lower <= -stability_threshold[board.n_empties-1]  ) {
                const unsigned long long d_player = board.discs[board.player] | (move->flipped | move->square);
                int stability_bound = 2 * board.get_stability(d_player, board.discs[board.player^1] ^ move->flipped) - 64*VALUE_DISC;
                if ( stability_bound >= upper ) {
                    return stability_bound;
                }
            }
            //synchronized acces
            if(USE_ETC && !pv && hTable->get(board.hashcode_after_move(move), type_hashtable, entry) && entry.selectivity >= selectivity && entry.depth>=(board.n_empties-1)) {
                
                if(-entry.upper >= upper) {
                    if(entry.selectivity != NO_SELECT)
                        selective_cutoff = true;
                    
                    return -entry.upper ;
                }
            }
            
            
            
            previous = previous->next = move++;
            
        }
        
        
        //for other move
        unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
        if(bestmove !=NOMOVE)
            legal_movesBB ^= 0x1ULL<<bestmove;
        
        for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
            if(legal_movesBB & 0x1ULL<<empties->position){
                
                ((board).*(board.generate_flips[empties->position]))(*move);
                board.n_nodes++;
                
                if (USE_ENHANCED_STABLILITY && lower <= -stability_threshold[board.n_empties-1]  ) {
                    const unsigned long long d_player = board.discs[board.player] | (move->flipped | move->square);
                    int stability_bound = 2 * board.get_stability(d_player, board.discs[board.player^1] ^ move->flipped) - 64*VALUE_DISC;
                    if ( stability_bound >= upper ) {
                        return stability_bound;
                    }
                }
                
                move->score = 0;    //not in hash
                
                //synchronized acces
                if(USE_ETC && hTable->get(board.hashcode_after_move(move), type_hashtable, entry) && entry.depth>=(board.n_empties-1)) {
                    
                    
                    if(!pv && entry.selectivity >= selectivity && -entry.upper >= upper) {
                        
                        if(entry.selectivity != NO_SELECT)
                            selective_cutoff = true;
                        
                        return -entry.upper ;
                    }
                    
                    move->score = -3*VALUE_DISC;    //in hash
                    
                }
                
                previous = previous->next = move++;
                
            }
        
        
        
        previous->next = NULL;
        
        
    }
    
    bool child_selective_cutoff = false;
    
    if (list->next == NULL) {
        if (passed) {
            board.n_nodes--;
            bestscore = board.final_score();
            alpha = -(upper = +MAX_SCORE);
            bestmove = NOMOVE;
        } else {
            board.do_pass();
            board.n_nodes++;
            bestscore = -EG_PVS_deep(threadID, sBoard, pv, selectivity, child_selective_cutoff, -upper, -lower, true);
            board.do_pass();
            bestmove = PASS;
            
            if(child_selective_cutoff)
                selective_cutoff = true;
            
        }
    } else {
        
        
        if(bestmove != NOMOVE) {
            
            /* first move */
            list = list->next;
            
            
            if (board.n_empties <= EG_DEEP_TO_MEDIUM) {
                board.do_move(*list);
                bestscore = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -lower, false);
                board.undo_move(*list);
            } else {
                ((sBoard).*(sBoard.update_patterns[list->position][board.player]))(*list);
                
                sBoard.do_move(*list);
                bestscore = -EG_PVS_deep(threadID, sBoard, pv, selectivity , child_selective_cutoff, -upper, -lower, false);
                sBoard.undo_move(*list);
                
            }
            
            
            //interrupt search
            if(abort.load() || thread_should_stop(threadID))
                return INTERRUPT_SEARCH;
            
            
            
            //bestmove = list->position;
            if (bestscore > lower)
                lower = bestscore;
            
            if (child_selective_cutoff)
                selective_cutoff = true;
            
        }
        
        if(lower < upper && list->next != NULL) {
            
            if((list->next)->next != NULL) {
                
                const int p = board.player;
                const int o = p^1;
                
                if(board.n_empties > EG_DEEP_TO_MEDIUM) {
                    
                    int lower_probcut = -MAX_SCORE;
                    int upper_probcut =  MAX_SCORE;
                    probcut_bounds(board, std::max(EG_HIGH_SELECT, (selectivity == NO_SELECT ? selectivity-2:selectivity-1)), ((board.n_empties & 0x1UL) == 0 ? 9:8), 0, (lower+upper)/2, lower_probcut, upper_probcut);
                    
                    for(RXMove* iter = list->next; iter!=NULL; iter = iter->next) {
                        
                        ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                        
                        sBoard.do_move(*iter);
                        
                        int eval_move;
                        if((board.n_empties & 0x1UL) == 0) {
                            if(board.n_empties >= 28) {
                                eval_move = -PVS_last_ply(threadID, sBoard, 6, -MAX_SCORE, -lower_probcut, false);
                            } else if(board.n_empties >= 24) {
                                eval_move = -PVS_last_ply(threadID, sBoard, 4, -MAX_SCORE, -lower_probcut, false);
                            } else {
                                eval_move = -alphabeta_last_two_ply(threadID, sBoard, -MAX_SCORE, -lower_probcut, false);
                            }
                        } else {
                            if(board.n_empties >= 27) {
                                eval_move = -PVS_last_ply(threadID, sBoard, 5, -MAX_SCORE, -lower_probcut, false);
                            } else if(board.n_empties >= 23) {
                                eval_move = -alphabeta_last_three_ply(threadID, sBoard, -MAX_SCORE, -lower_probcut, false);
                            } else {
                                
                                RXMove& lastMove = threads[threadID]._move[board.n_empties][1];
                                
                                int bestscore = UNDEF_SCORE; //masquage
                                const unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
                                for(RXSquareList* empties = board.empties_list->next; bestscore < -lower_probcut && empties->position != NOMOVE; empties = empties->next) {
                                    if ((legal_movesBB & 0x1ULL<<empties->position)){
                                        
                                        ((board).*(board.generate_flips[empties->position]))(lastMove);
                                        ((sBoard).*(sBoard.update_patterns[empties->position][board.player]))(lastMove);
                                        board.n_nodes++;
                                        
                                        int score = -sBoard.get_score(lastMove);
                                        if (score>bestscore)
                                            bestscore = score;
                                        
                                        
                                    }
                                }
                                
                                
                                if(bestscore == UNDEF_SCORE) {
                                    //PASS
                                    sBoard.board.do_pass();
                                    bestscore = -sBoard.get_score();
                                    sBoard.board.do_pass();
                                    
                                }
                                
                                eval_move = -bestscore;
                                
                            }
                            
                        }
                        
                        
                        
                        if(eval_move > upper_probcut) {
                            
                            eval_move /= 2;
                            eval_move += 12*VALUE_DISC;
                            
                        } else if(lower_probcut > eval_move ) {
                            
                            eval_move /= 2;
                            eval_move -= 12*VALUE_DISC;
                            
                        }
                        
                        iter->score += RXBitBoard::get_mobility(board.discs[o], board.discs[p])*VALUE_DISC - eval_move; // - (board.get_edge_stability(board.player)*VALUE_DISC)/16;
                        
                        sBoard.undo_move(*iter);
                        
                    }
                    
                } else {
                    
                    //empties<18;
                    
                    for(RXMove* iter = list->next; iter!=NULL; iter = iter->next) {
                        ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                        
                        board.n_nodes++;
                        const unsigned long long p_discs = board.discs[p] | (iter->flipped | iter->square);
                        
                        //test 1 : score + 2*mobility_adv + corner_stability/4
                        iter->score += sBoard.get_score(*iter) + 2*(RXBitBoard::get_mobility(board.discs[o] ^ iter->flipped, p_discs)*VALUE_DISC) - (RXBitBoard::get_corner_stability(p_discs)*VALUE_DISC)/4;
                    }
                    
                }
                
                
            } else {
                ((sBoard).*(sBoard.update_patterns[list->next->position][board.player]))(*(list->next));
            }
            
            
            
            if(bestmove == NOMOVE) {
                
                RXMove* previous_move = list;
                RXMove* move = previous_move->next;
                
                RXMove* previous_iter = move;
                for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                    if(iter->score < move->score) {
                        move = iter;
                        previous_move = previous_iter;
                    }
                }
                
                if(previous_move != list) {
                    //move to front
                    previous_move->next = move->next;
                    move->next = list->next;
                    list->next = move;
                }
                
                sBoard.do_move(*move);
                if (board.n_empties < EG_DEEP_TO_MEDIUM) {
                    bestscore = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -lower, false);
                } else {
                    bestscore = -EG_PVS_deep(threadID, sBoard, pv, selectivity, child_selective_cutoff, -upper, -lower, false);
                }
                sBoard.undo_move(*move);
                
                
                //interrupt search
                if(abort.load()  || thread_should_stop(threadID))
                    return INTERRUPT_SEARCH;
                
                
                
                bestmove = move->position;
                if (bestscore > lower)
                    lower = bestscore;
                
                if ( child_selective_cutoff )
                    selective_cutoff = true;
                
                // next move
                list = list->next;
                
            }
            
            
            //************************************************************************************************
            //dans la methode search sequentielle dans la boucle des autres moves
            //************************************************************************************************
            
            
            /* other moves : try to refute the first/best one */
            for(;!abort.load()  && lower < upper && list->next != NULL; list = list->next) {
                
                RXMove* move = list->next;
                if (move->next != NULL) {
                    
                    // Split?
                    if(activeThreads > 1  && !abort.load() && !thread_should_stop(threadID) && idle_thread_exists(threadID)
                       && split(sBoard, pv, 0, board.n_empties, selectivity, selective_cutoff, child_selective_cutoff,
                                lower, upper, bestscore, bestmove, list, threadID, RXSplitPoint::END_PVS))
                        
                        break;
                    
                    RXMove* previous_move = list;
                    
                    RXMove* previous_iter = move;
                    for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
                        if(iter->score < move->score) {
                            move = iter;
                            previous_move = previous_iter;
                        }
                    }
                    
                    if(previous_move != list) {
                        //move to front
                        previous_move->next = move->next;
                        move->next = list->next;
                        list->next = move;
                    }
                }
                
                
                sBoard.do_move(*move);
                if (board.n_empties < EG_DEEP_TO_MEDIUM) {
                    
                    score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -lower - VALUE_DISC, -lower, false);
                    if (lower < score && score < upper)
                        score = -EG_PVS_ETC_mobility(threadID, sBoard, pv, -upper, -score, false);
                    
                } else if(selectivity != NO_SELECT) {
                    
                    score = -EG_NWS_XEndCut(threadID, sBoard, 0, selectivity, child_selective_cutoff, -lower - VALUE_DISC, false);
                    if (lower < score && score < upper)
                        score = -EG_PVS_deep(threadID, sBoard, pv, selectivity, child_selective_cutoff, -upper, -lower, false); //-score
                    
                } else {
                    
                    score = -EG_PVS_deep(threadID, sBoard, false, NO_SELECT, child_selective_cutoff, -lower - VALUE_DISC, -lower, false);
                    if (lower < score && score < upper)
                        score = -EG_PVS_deep(threadID, sBoard, pv, NO_SELECT, child_selective_cutoff, -upper, -score, false);
                    
                }
                sBoard.undo_move(*move);
                
                
                //interrupt search
                if(abort.load()  || thread_should_stop(threadID))
                    return INTERRUPT_SEARCH;
                
                
                
                if (score > bestscore) {
                    bestscore = score;
                    bestmove = move->position;
                    if (bestscore > lower)
                        lower = bestscore;
                }
                
                if ( child_selective_cutoff )
                    selective_cutoff = true;
            }
            
        }
    }
    
    //interrupt search
    if(abort  || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    
    
    if(bestscore>=upper)
        selective_cutoff = child_selective_cutoff;
    
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(   hash_code, pv, type_hashtable, selective_cutoff? selectivity : NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
    /*if(pv)*/
    hTable_PV->update(hash_code, pv, type_hashtable, selective_cutoff? selectivity : NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
    
    return bestscore;
}


// EG_SP_search_DEEP() is used to search from a PV split point.  This function
// is called by each thread working at the split point.  It is similar to
// the normal EG_PVS_deep() function, but simpler.  Because we have already
// probed the hash table and searched the first move before splitting, we
// don't have to repeat all this work in EG_SP_search_DEEP().  We also don't
// need to store anything to the hash table here:  This is taken care of
// after we return from the split point.

void RXEngine::EG_SP_search_DEEP(RXSplitPoint* sp, const unsigned int threadID) {
    
    
    RXBBPatterns& sBoard = sp->sBoardStack[threadID];
    sBoard = *(sp->sBoard); //operator=
    RXBitBoard& board = sBoard.board;
    board.n_nodes = 0;
    
    
    //here sp->beta is const
    while(sp->alpha < sp->beta && !abort.load() && !thread_should_stop(threadID)) {
        
        pthread_mutex_lock(&(sp->lock));
        
        if(sp->list->next == NULL) {
            pthread_mutex_unlock(&(sp->lock));
            break;
        }
        
        RXMove* move = sp->list->next;
        sp->list = move;
        
        
        //        RXMove* move;
        //        if(sp->list != NULL && sp->list->next != NULL) {
        //
        //            RXMove* previous_move = sp->list;
        //            move = previous_move->next;
        //
        //            RXMove* previous_iter = move;
        //            for(RXMove* iter = previous_iter->next ; iter != NULL; iter = (previous_iter = iter)->next) {
        //                if(iter->score < move->score) {
        //                    move = iter;
        //                    previous_move = previous_iter;
        //                }
        //            }
        //
        //            if(previous_move != sp->list) {
        //                //move to front
        //                previous_move->next = move->next;
        //                move->next = sp->list->next;
        //                sp->list->next = move;
        //            }
        //
        //            sp->list = sp->list->next;
        //
        //
        //        } else {
        //            pthread_mutex_unlock(&(sp->lock));
        //            break;
        //        }
        
        
        bool child_selective_cutoff = sp->child_selective_cutoff;
        
        pthread_mutex_unlock(&(sp->lock));
        
        
        
        
        int score;
        
        int alpha = sp->alpha; //local copy
        
        sBoard.do_move(*move);
        
        if (board.n_empties < EG_DEEP_TO_MEDIUM) {
            
            score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -alpha - VALUE_DISC, -alpha, false);
            
            if (alpha < score && score < sp->beta)
                score = -EG_PVS_ETC_mobility(threadID, sBoard, sp->pv, -sp->beta, -score, false);
            
        } else if(sp->selectivity != NO_SELECT) {
            
            score = -EG_NWS_XEndCut(threadID, sBoard, sp->pvDev+1, sp->selectivity, child_selective_cutoff, -alpha - VALUE_DISC, false);
            
            if (alpha < score && score < sp->beta)
                score = -EG_PVS_deep(threadID, sBoard, sp->pv, sp->selectivity, child_selective_cutoff, -sp->beta, -sp->alpha, false);
            
        } else {
            
            score = -EG_PVS_deep(threadID, sBoard, false, NO_SELECT, child_selective_cutoff, -alpha - VALUE_DISC, -alpha, false);
            
            if (alpha < score && score < sp->beta)
                score = -EG_PVS_deep(threadID, sBoard, sp->pv, NO_SELECT, child_selective_cutoff, -sp->beta, /*sp->selectivity != NO_SELECT? -sp->alpha :*/ -score, false);
            
        }
        
        sBoard.undo_move(*move);
        
        if(abort.load()  || thread_should_stop(threadID))
            break;
        
        
        
        //first without mutex
        if((score > sp->bestscore) || (!sp->selective_cutoff && child_selective_cutoff)) {
            
            //update
            pthread_mutex_lock(&(sp->lock));
            
            sp->child_selective_cutoff = child_selective_cutoff;
            
            if(sp->child_selective_cutoff)
                sp->selective_cutoff = true;
            
            // New best move?
            if(score > sp->bestscore) {
                sp->bestscore = score;
                sp->bestmove = move->position;
                if(score > sp->alpha) {
                    
                    if(score >= sp->beta) {
                        sp->explored =true;
                    } else {
                        sp->alpha = score;
                    }
                }
            }
            
            pthread_mutex_unlock(&(sp->lock));
        }
        
    }
    
    pthread_mutex_lock(&(sp->lock));
    
    sp->sBoard->board.n_nodes += board.n_nodes;
    
    
    sp->slaves[threadID] = false;
    sp->n_Slaves--;
    
    pthread_mutex_unlock(&(sp->lock));
}


/*
 Null Window Search  + XProbCut
 */
int RXEngine::EG_NWS_XEndCut(int threadID, RXBBPatterns& sBoard, const int pvDev, const int selectivity, bool& selective_cutoff, int alpha, const bool passed) {
    
    if(abort.load()  || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    //time gestion
    if(dependent_time && get_current_dependentTime() > time_limit()) {
        abort.store(true);
        return INTERRUPT_SEARCH;
    }
    
    RXBitBoard& board = sBoard.board;
    
    selective_cutoff = false;
    
    int bestmove = NOMOVE;
    
    
    //synchronized acces
    RXHashValue entry;
    const unsigned long long hash_code = board.hashcode();
    if(hTable->get(hash_code, type_hashtable, entry)) {
        
        if(entry.selectivity >= selectivity && entry.depth>=board.n_empties) {
            
            
            if(entry.lower > alpha) {
                if(entry.selectivity != NO_SELECT)
                    selective_cutoff = true;
                
                return entry.lower;
            }
            
            if(entry.upper <= alpha) {
                if(entry.selectivity != NO_SELECT)
                    selective_cutoff = true;
                
                return  entry.upper;
            }
            
            bestmove = entry.move;
            
        }
        
        
    }
    
    if(USE_STABILITY && bestmove == NOMOVE) {
        if ( alpha+VALUE_DISC >= stability_threshold[board.n_empties] ) {
            
            int stability_bound = 64*VALUE_DISC - 2 * board.get_stability(board.player^1);
            if ( stability_bound <= alpha )
                return stability_bound;
        }
    }
    
    //param mpc
    int lower_probcut, upper_probcut;
    int probcut_depth = (board.n_empties/4)*2 + (board.n_empties&1);
    probcut_bounds(board, selectivity, board.n_empties, pvDev, alpha, lower_probcut, upper_probcut);
    
    
    if(bestmove != NOMOVE && entry.selectivity >= selectivity && entry.depth>=probcut_depth) {
        
        if(entry.lower >= upper_probcut) {
            selective_cutoff = true;
            return alpha + VALUE_DISC; //9/02/2025
        }
#ifdef USE_PROBCUT_ALPHA
        if(entry.upper <= lower_probcut) {
            selective_cutoff = true;
            return alpha;
        }
#endif
        
    }
    
    
    RXMove* list = threads[threadID]._move[board.n_empties];
    list->next = NULL;
    
    if(bestmove != PASS) {
        
        RXMove* move = list + 1;
        RXMove* previous = list;
        
        unsigned long long  hashcode_after_move;
        
        //ENHANCED TRANSPOSITION CUTOFF
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            board.n_nodes++;
            
            if(USE_ETC) {
                hashcode_after_move = board.hashcode_after_move(move);
                hTable->entry_prefetch(hashcode_after_move, type_hashtable);
            }
            
            
            if (USE_ENHANCED_STABLILITY && alpha <= -stability_threshold[board.n_empties-1]  ) {
                const unsigned long long d_player = board.discs[board.player] | (move->flipped | move->square);
                int stability_bound = 2 * board.get_stability(d_player, board.discs[board.player^1] ^ move->flipped) - 64*VALUE_DISC;
                if ( stability_bound > alpha ) {
                    return stability_bound; // alpha
                }
            }
            
            //synchronized acces
            if(USE_ETC && hTable->get(hashcode_after_move, type_hashtable, entry) && entry.selectivity >= selectivity && entry.depth >= (board.n_empties-1)) {
                
                if(-entry.upper > alpha) {
                    if(entry.selectivity != NO_SELECT)
                        selective_cutoff = true;
                    
                    return -entry.upper;
                }
                
            }
            
            
            previous = previous->next = move++;
            
        }
        
        //for all empty square
        unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
        if(bestmove !=NOMOVE)
            legal_movesBB ^= 0x1ULL<<bestmove;
        
        for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
            if(legal_movesBB & 0x1ULL<<empties->position){
                
                ((board).*(board.generate_flips[empties->position]))(*move);
                board.n_nodes++;
                
                if(USE_ETC) {
                    hashcode_after_move = board.hashcode_after_move(move);
                    hTable->entry_prefetch(hashcode_after_move, type_hashtable);
                }
                
                if (USE_ENHANCED_STABLILITY && alpha <= -stability_threshold[board.n_empties-1]  ) {
                    const unsigned long long d_player = board.discs[board.player] | (move->flipped | move->square);
                    int stability_bound = 2 * board.get_stability(d_player, board.discs[board.player^1] ^ move->flipped) - 64*VALUE_DISC;
                    if ( stability_bound > alpha ) {
                        return stability_bound; // alpha
                    }
                }
                
                move->score = 0;
                
                //synchronized acces
                if(USE_ETC && hTable->get(hashcode_after_move, type_hashtable, entry) && entry.depth>=(board.n_empties-1)) {
                    
                    move->score = -3*VALUE_DISC;
                    
                    if(-entry.upper > alpha) {
                        
                        if(entry.selectivity >= selectivity ) {
                            if(entry.selectivity != NO_SELECT)
                                selective_cutoff = true;
                            
                            return -entry.upper;
                        }
                        
                        move->score = -16*VALUE_DISC;
                        
                    }
                    
                }
                
                
                
                
                previous = previous->next = move++;
                
            }
        
        previous->next = NULL;
        
        
    }
    
    int bestscore = UNDEF_SCORE;
    bool child_selective_cutoff = false;
    
    if(list->next == NULL) {
        //PASS
        if(passed) {
            board.n_nodes--;
            bestscore = board.final_score();
            hTable->update(hash_code, false, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, -MAX_SCORE, MAX_SCORE,  bestscore, bestmove);
            return bestscore;
        } else {
            board.n_nodes++;
            board.do_pass();
            bestscore = -EG_NWS_XEndCut(threadID, sBoard, pvDev, selectivity, child_selective_cutoff, -alpha-VALUE_DISC, true);
            board.do_pass();
            bestmove = PASS;
            
            if(child_selective_cutoff)
                selective_cutoff = true;
            
        }
        
    } else {
        
        //XProbcut
        int type_probcut = probcut(threadID, true, sBoard, selectivity, probcut_depth, lower_probcut, upper_probcut, list, bestmove != NOMOVE);
        if( type_probcut == BETA_CUT) {
            selective_cutoff = true;
            return alpha + VALUE_DISC; //9/02/2025
        }
#ifdef USE_PROBCUT_ALPHA
        if(type_probcut == ALPHA_CUT) {
            selective_cutoff = true;
            return alpha;
        }
#endif
        
        //interrupt search
        if(abort.load()  || thread_should_stop(threadID))
            return INTERRUPT_SEARCH;
        
        
        //fisrt move
        RXMove* move = list->next;
        
        if(board.n_empties<MIN_DEPTH_USE_ENDCUT) {
            board.do_move(*move);
            bestscore = -EG_PVS_ETC_mobility(threadID, sBoard, false, -alpha-VALUE_DISC, -alpha, false);
            board.undo_move(*move);
        } else {
            sBoard.do_move(*move);
            bestscore = -EG_NWS_XEndCut(threadID, sBoard, pvDev, selectivity, child_selective_cutoff, -alpha-VALUE_DISC, false);
            sBoard.undo_move(*move);
        }
        
        //interrupt search
        if(abort.load()  || thread_should_stop(threadID))
            return INTERRUPT_SEARCH;
        
        
        
        bestmove = move->position;
        list = list->next;
        
        if(child_selective_cutoff)
            selective_cutoff = true;
        
        
        int score;
        for(RXMove* iter = list->next; !abort.load()  && bestscore<=alpha && iter != NULL; iter = iter->next, list = list->next) {
            
            //			//assert(bestscore >= -MAX_SCORE);
            //			if(bestscore<-MAX_SCORE)
            //				std::cout << "Error" << std::endl;
            
            
            if(activeThreads > 1
               && (list->next)->next != NULL && !thread_should_stop(threadID)
               && !abort.load() && idle_thread_exists(threadID)
               && split(sBoard, false, pvDev, board.n_empties, selectivity, selective_cutoff, child_selective_cutoff,
                        alpha, (alpha + VALUE_DISC), bestscore, bestmove, list, threadID, RXSplitPoint::END_XPROBCUT))
                
                break;
            
            
            if(board.n_empties<MIN_DEPTH_USE_ENDCUT) {
                board.do_move(*iter);
                score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -alpha-VALUE_DISC, -alpha, false);
                board.undo_move(*iter);
            } else {
                sBoard.do_move(*iter);
                score = -EG_NWS_XEndCut(threadID, sBoard, pvDev+1, selectivity, child_selective_cutoff, -alpha-VALUE_DISC, false);
                sBoard.undo_move(*iter);
            }
            
            //interrupt search
            if(abort.load()  ||thread_should_stop(threadID))
                return INTERRUPT_SEARCH;
            
            
            
            if (score>bestscore) {
                bestscore = score;
                bestmove = iter->position;
            }
            
            if(child_selective_cutoff)
                selective_cutoff = true;
            
        }
    }
    
    
    //interrupt search
    if(abort.load()  || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    
    
    if(bestscore>alpha)
        selective_cutoff = child_selective_cutoff;
    
    hTable->update(hash_code, type_hashtable, selective_cutoff? selectivity : NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, bestscore, bestmove);
    if(pvDev < 4)
        hTable_PV->update(hash_code, type_hashtable, selective_cutoff? selectivity : NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, bestscore, bestmove);
    
    return bestscore;
    
    
}

// EG_SP_search_XEndcut() is used to search from a PV split point.  This function
// is called by each thread working at the split point.  It is similar to
// the normal EG_NWS_XEndCut() function, but simpler.  Because we have already
// probed the hash table and searched the first move before splitting, we
// don't have to repeat all this work in EG_SP_search_XEndcut().  We also don't
// need to store anything to the hash table here:  This is taken care of
// after we return from the split point.

void RXEngine::EG_SP_search_XEndcut(RXSplitPoint* sp, const unsigned int threadID) {
    
    //    assert(threadID >= 0 && threadID < activeThreads);
    //    assert(activeThreads > 1);
    
    RXBBPatterns& sBoard = sp->sBoardStack[threadID];
    sBoard = *(sp->sBoard); //operator=
    RXBitBoard& board = sBoard.board;
    board.n_nodes = 0;
    
    //here sp->alpha is const
    while(sp->bestscore <= sp->alpha && !abort.load()  && !thread_should_stop(threadID)) {
        
        pthread_mutex_lock(&(sp->lock));
        
        if(sp->list->next == NULL) {
            pthread_mutex_unlock(&(sp->lock));
            break;
        }
        
        RXMove* move = sp->list->next;
        sp->list = move;
        
        //bool child_selective_cutoff = sp->child_selective_cutoff;
        
        pthread_mutex_unlock(&(sp->lock));
        
        int alpha = sp->alpha; //local copy
        
        bool child_selective_cutoff = false;
        int score;
        if(board.n_empties<=MIN_DEPTH_USE_ENDCUT) {
            board.do_move(*move);
            score = -EG_PVS_ETC_mobility(threadID, sBoard, false, -alpha-VALUE_DISC, -alpha, false);
            board.undo_move(*move);
        } else {
            sBoard.do_move(*move);
            score = -EG_NWS_XEndCut(threadID, sBoard, sp->pvDev+1, sp->selectivity, child_selective_cutoff, -alpha-VALUE_DISC, false);
            sBoard.undo_move(*move);
        }
        
        
        if(abort.load() || thread_should_stop(threadID))
            break;
        
        
        //first without mutex
        if((score > sp->bestscore) || (!sp->selective_cutoff && child_selective_cutoff)) {
            
            pthread_mutex_lock(&(sp->lock));
            
            //update SplitPoint
            
            sp->child_selective_cutoff = child_selective_cutoff;
            
            if(sp->child_selective_cutoff)
                sp->selective_cutoff = true;
            
            // New best move?
            if(score > sp->bestscore) {
                sp->bestscore = score;
                sp->bestmove = move->position;
                if(score > sp->alpha) {
                    sp->explored = true;
                }
            }
            
            
            pthread_mutex_unlock(&(sp->lock));
        }
    }
    
    pthread_mutex_lock(&(sp->lock));
    
    sp->sBoard->board.n_nodes += board.n_nodes;
    
    sp->slaves[threadID] = false;
    sp->n_Slaves--;
    
    pthread_mutex_unlock(&(sp->lock));
}

/*!
 * \brief Principal Variation Search algorithm at the root of the tree.
 *
 * This function solves the position provided within the limits set by the alpha
 * and beta bounds. The list parameter is updated so that the bestmove is the
 * first of the list when the search ended.
 *
 * \param sBoard      	sBoard.
 * \param selectivity 	selectivity.
 * \param alpha      	lower bound.
 * \param beta       	upper bound.
 * \param list   		List of legal moves (should actually contain moves !).
 */
void RXEngine::EG_PVS_root(RXBBPatterns& sBoard, const int selectivity, int alpha, int beta, RXMove* list)
{
    
    //	assert(alpha>=-64*VALUE_DISC && beta<=64*VALUE_DISC);
    
    
    /*
     *log << "[" << alpha << " ; " << beta << "]" << std::endl;
     */
    
    //*log << "                  [EndGame @" << CONFIDENCE[selectivity] << " alpha : " << alpha << " beta : " << beta << "]" << std::endl;
    
    RXMove* iter = list->next;
    
    int bestmove = iter->position;
    
    RXBitBoard& board = sBoard.board;
    
    int lower = alpha;
    const int upper = beta;
    
    bool selective_cutoff = false;
    bool child_selective_cutoff = false;
    
    /* first move */
    first_move.store(true);
    
    sBoard.do_move(*iter);
    
    
    int bestscore;
    if (board.n_empties == 0) {
        bestscore = -board.final_score();
    } else if (board.n_empties == 1) {
        bestscore = -board.final_score_1();
    } else if (board.n_empties == 2) {
        bestscore = -board.final_score_2(-upper, -lower);
    } else if (board.n_empties == 3) {
        bestscore = -board.final_score_3(-upper, -lower);
    } else if (board.n_empties == 4) {
        bestscore = -board.final_score_4(true, -upper, -lower, false);
    } else if (board.n_empties < EG_MEDIUM_TO_SHALLOW) {
        bestscore = -EG_alphabeta_parity(0, board, true, -upper, -lower, false);
    } else if (board.n_empties < EG_MEDIUM_HI_TO_LOW) {
        bestscore = -EG_PVS_hash_mobility(0, board, true, -upper, -lower, false);
    } else  if (board.n_empties < EG_DEEP_TO_MEDIUM) {
        bestscore = -EG_PVS_ETC_mobility(0, sBoard, true, -upper, -lower, false);
    } else {
        bestscore = -EG_PVS_deep(0, sBoard, true, selectivity, child_selective_cutoff, -upper, -lower, false);
    }
    
    sBoard.undo_move(*iter);
    
    if(!abort.load() && std::abs(bestscore) != INTERRUPT_SEARCH) {
        
        if(search_client == RXSearch::kGGSMode) {	// GGS mode
            
            if(dependent_time && board.n_empties>19)
                manager->sendMsg(showBestmove(board.n_empties, selectivity, lower, upper, bestscore, bestmove));
        }
        
        if (bestscore > lower)
            lower = bestscore;
        else {
            extra_time = 1;
            //            *log << "                  [extra time <=:" << extra_time << "]" << std::endl;
        }
        
        if (child_selective_cutoff)
            selective_cutoff = true;
        
        /* other moves : try to refute the first/best one */
        int score;
        first_move.store(false);
        
        for (iter = iter->next; !abort.load()  && lower < upper && iter != NULL; iter = iter->next) {
            
#ifdef USE_SPLIT_AT_ROOT
            
            if(activeThreads > 1 && iter->next != NULL && board.n_empties >= EG_DEEP_TO_MEDIUM
               && !abort.load() && idle_thread_exists(0) && !thread_should_stop(0)
               && split(sBoard, true, 0, board.n_empties, selectivity, selective_cutoff, child_selective_cutoff,
                        lower, upper, bestscore, bestmove, iter, 0, RXSplitPoint::END_ROOT)) {
                
                
                break;
            }
#endif
            
            sBoard.do_move(*iter);
            
            if (board.n_empties == 1) {
                score = -board.final_score_1();
            } else if (board.n_empties == 2) {
                score = -board.final_score_2(-upper, -lower);
            } else if (board.n_empties == 3) {
                score = -board.final_score_3(-upper, -lower);
            } else if (board.n_empties == 4) {
                score = -board.final_score_4(true, -upper, -lower, false);
            } else {
                
                //				//multi_pv PV == true
                //				if (board.n_empties < EG_MEDIUM_TO_SHALLOW)
                //					score = -EG_alphabeta_parity(0, board, -lower - VALUE_DISC, -lower, false);
                //				else if (board.n_empties < EG_MEDIUM_HI_TO_LOW)
                //					score = -EG_PVS_hash_mobility(0, board, true, -lower - VALUE_DISC, -lower, false);
                //				else if (board.n_empties < EG_DEEP_TO_MEDIUM)
                //					score = -EG_PVS_ETC_mobility(0, sBoard, true, -lower - VALUE_DISC, -lower, false); //multi-PV pv == true ????
                //				else
                //					score = -EG_PVS_deep(0, sBoard, true, selectivity, child_selective_cutoff, -lower - VALUE_DISC, -lower, false); //multi-PV pv == true ????
                
                //simple_pv PV == false
                if (board.n_empties < EG_MEDIUM_TO_SHALLOW)
                    score = -EG_alphabeta_parity(0, board, false, -lower - VALUE_DISC, -lower, false);
                else if (board.n_empties < EG_MEDIUM_HI_TO_LOW)
                    score = -EG_PVS_hash_mobility(0, board, false, -lower - VALUE_DISC, -lower, false);
                else if (board.n_empties < EG_DEEP_TO_MEDIUM)
                    score = -EG_PVS_ETC_mobility(0, sBoard, false, -lower - VALUE_DISC, -lower, false); //simple-PV pv == false ????
                else
                    score = -EG_PVS_deep(0, sBoard, false, selectivity, child_selective_cutoff, -lower - VALUE_DISC, -lower, false); //simple-PV pv == false ????
                
                
                if (!abort.load() && board.n_empties > 4 && lower < score && score < upper) {
                    
                    if(search_client == RXSearch::kGGSMode) {	// GGS mode
                        
                        if(dependent_time && board.n_empties>19)
                            manager->sendMsg("         " + RXMove::index_to_coord(iter->position) + " maybe better? ");
                        
                    }
                    
                    if (board.n_empties < EG_MEDIUM_TO_SHALLOW)
                        score = -EG_alphabeta_parity(0, board, true, -upper, -score, false);
                    else if (board.n_empties < EG_MEDIUM_HI_TO_LOW)
                        score = -EG_PVS_hash_mobility(0, board, true, -upper, -score, false);
                    else if (board.n_empties < EG_DEEP_TO_MEDIUM)
                        score = -EG_PVS_ETC_mobility(0, sBoard, true, -upper, -score, false);
                    else {
                        extra_time++;
                        //                        *log << "                  [extra time > :" << extra_time << "]" << std::endl;
                        
                        if(selectivity != NO_SELECT)
                            score = -EG_PVS_deep(0, sBoard, true, selectivity, child_selective_cutoff, -upper, child_selective_cutoff? -lower : -score, false);
                        else
                            score = -EG_PVS_deep(0, sBoard, true, selectivity, child_selective_cutoff, -upper, -score, false);
                        
                        if(search_client == RXSearch::kGGSMode && !abort.load()) {    // GGS mode
                            if(dependent_time && board.n_empties>19 && score <= bestscore)
                                manager->sendMsg("         " + RXMove::index_to_coord(iter->position) + " refuted ");
                        }
                        
                        
                        extra_time--;
                        //                        *log << "                  [extra time end :" << extra_time << "]" << std::endl;
                        
                    }
                    
                    
                }
                
                
                
            }
            
            
            
            sBoard.undo_move(*iter);
            
            
            if (!abort.load()  && std::abs(score) != INTERRUPT_SEARCH) {
                
                if(score > bestscore) {
                    
                    bestmove = iter->position;
                    bestscore = score;
                    
                    if(search_client == RXSearch::kGGSMode)    // GGS mode
                        if(dependent_time && board.n_empties>19)
                            manager->sendMsg(showBestmove(board.n_empties, selectivity, lower, upper, bestscore, bestmove));
                    
                    if (bestscore > lower) {
                        lower = bestscore;
                        extra_time = 0;
                    }
                    
                    
                }
            }
            
            if (child_selective_cutoff)
                selective_cutoff = true;
            
            
        }
        
        extra_time = 0;
        
        if(bestscore>=upper)
            selective_cutoff = child_selective_cutoff;
        
        list->sort_bestmove(bestmove);
        list->next->score = bestscore;
        list->next->selectivity = selective_cutoff? selectivity : NO_SELECT;
        list->next->depth = board.n_empties;
        
        //        *log << "                  [score " << bestscore << " ]" << std::endl;
        
        
        hTable->update(sBoard.board.hashcode(), true, type_hashtable, selective_cutoff? selectivity : NO_SELECT, DEPTH_BOOSTER+board.n_empties, alpha, upper, bestscore, bestmove);
        
        
    }
    
}

// EG_SP_search_root() is used to search from a PV split point.  This function
// is called by each thread working at the split point.  It is similar to
// the normal EG_PVS_root() function, but simpler.  Because we have already
// probed the hash table and searched the first move before splitting, we
// don't have to repeat all this work in EG_SP_search_DEEP().  We also don't
// need to store anything to the hash table here:  This is taken care of
// after we return from the split point.

void RXEngine::EG_SP_search_root(RXSplitPoint* sp, const unsigned int threadID) {
    
    //    assert(threadID >= 0 && threadID < activeThreads);
    //    assert(activeThreads > 1);
    
    
    RXBBPatterns& sBoard = sp->sBoardStack[threadID];
    sBoard = *(sp->sBoard); //operator=
    RXBitBoard& board = sBoard.board;
    board.n_nodes = 0;
    
    //here sp->beta is const
    while(sp->alpha < sp->beta && !abort.load() && !thread_should_stop(threadID)) {
        
        pthread_mutex_lock(&(sp->lock));
        
        
        if(sp->list == NULL) {
            pthread_mutex_unlock(&(sp->lock));
            break;
        }
        
        
        RXMove* move = sp->list;
        sp->list = move->next;
        
        bool child_selective_cutoff = sp->child_selective_cutoff;
        
        pthread_mutex_unlock(&(sp->lock));
        
        
        sBoard.do_move(*move);
        
        int alpha = sp->alpha; //local copy
        
        int score = -EG_PVS_deep(threadID, sBoard, false, sp->selectivity, child_selective_cutoff, -alpha - VALUE_DISC, -alpha, false);
        
        if (!abort.load()  && alpha < score && score < sp->beta) {
            
            extra_time++;
            
            if(dependent_time && board.n_empties>19)
                manager->sendMsg("         " + RXMove::index_to_coord(move->position) + " maybe better? ");
            
            if(sp->selectivity != NO_SELECT)
                score = -EG_PVS_deep(threadID, sBoard, true, sp->selectivity, child_selective_cutoff, -sp->beta, (child_selective_cutoff? -sp->alpha : -score), false);
            else
                score = -EG_PVS_deep(threadID, sBoard, true, sp->selectivity, child_selective_cutoff, -sp->beta, -score, false);
            
            if(search_client == RXSearch::kGGSMode && !(abort.load() || thread_should_stop(threadID))) {    // GGS mode
                if(dependent_time && board.n_empties>19 && score <= sp->bestscore)
                    manager->sendMsg("         " + RXMove::index_to_coord(move->position) + " refuted ");
            }
            
            
            extra_time--; //thread-safe?
            
        }
        
        sBoard.undo_move(*move);
        
        if(abort.load() || thread_should_stop(threadID))
            break;
        
        
        
        
        
        //first without mutex
        if((score > sp->bestscore) || (!sp->selective_cutoff && child_selective_cutoff)) {
            
            //update
            pthread_mutex_lock(&(sp->lock));
            
            sp->child_selective_cutoff = child_selective_cutoff;
            
            if(sp->child_selective_cutoff)
                sp->selective_cutoff = true;
            
            // New best move?
            if(score > sp->bestscore) {
                sp->bestscore = score;
                sp->bestmove = move->position;
                
                if(dependent_time && board.n_empties>19)
                    manager->sendMsg(showBestmove(board.n_empties, sp->selectivity, sp->alpha, sp->beta, sp->bestscore, sp->bestmove));
                
                if(score > sp->alpha) {
                    
                    if(score >= sp->beta) {
                        sp->explored = true;
                    } else {
                        sp->alpha = score;
                    }
                    
                }
            }
            
            pthread_mutex_unlock(&(sp->lock));
        }
        
    }
    
    pthread_mutex_lock(&(sp->lock));
    
    sp->sBoard->board.n_nodes += board.n_nodes;
    
    sp->slaves[threadID] = false;
    sp->n_Slaves--;
    
    pthread_mutex_unlock(&(sp->lock));
}

/*!
 * \brief Driver for solver.
 *
 * This function reduced and moved the search window
 *
 * \param sBoard      	sBoard.
 * \param selectivity 	selectivity.
 * \param list   		List of legal moves (should actually contain moves !).
 */

void RXEngine::EG_driver(RXBBPatterns& sBoard, int selectivity, int end_selectivity, RXMove* list) {
    
    set_type_search(ENDGAME);
    
    
    //utiliser directement "list->next->score" provoque un bug d'affichage 25/03/2025
    int score = list->next->score;
    int modulo = score % (2*VALUE_DISC);
    if( modulo != 0) {
        score -= modulo;
        if(modulo > VALUE_DISC)
            score += 2*VALUE_DISC;
    }
    
    score = std::min(64*VALUE_DISC, std::max(-64*VALUE_DISC, score));
    
    
    int s_alpha = search_alpha;
    if(s_alpha<0) {
        if(s_alpha%(2*VALUE_DISC) != 0)
            s_alpha -= 2*VALUE_DISC + s_alpha%(2*VALUE_DISC);
    } else {
        s_alpha -= s_alpha%(2*VALUE_DISC);
    }
    
    int s_beta  = search_beta;
    if(s_beta <0) {
        s_beta  -= s_beta%(2*VALUE_DISC);
    } else {
        if(s_beta%(2*VALUE_DISC) != 0)
            s_beta  += 2*VALUE_DISC - s_beta%(2*VALUE_DISC);
    }
    
    s_alpha = std::max(-64*VALUE_DISC, s_alpha);
    s_beta  = std::min( 64*VALUE_DISC, s_beta);
    
    //for record fforum-40-59 setting
    //for(selectivity = 2; !abort.load()  && selectivity <= end_selectivity; selectivity+=4) {
    
    for(; !abort.load()  && selectivity <= end_selectivity; selectivity++) {
        
        selectivity = std::max(std::min(NO_SELECT, std::max(EG_HIGH_SELECT, 28-sBoard.board.n_empties)), std::max(EG_HIGH_SELECT, selectivity));
        
        set_select_search(selectivity);
        
        if(dependent_time)
            determine_move_time(sBoard.board);
        
        new_search = false;
        
        int eTime_start_level = get_current_time();
        
        time_startLevel = eTime_start_level;
        
        
        sBoard.board.n_nodes = 0;
        
        
        int alpha = std::max(s_alpha, std::min(s_beta-VALUE_DISC, score - VALUE_DISC));
        int beta  = std::min(s_beta, std::max(s_alpha+VALUE_DISC, score + VALUE_DISC));
        
        //always even window
        if(alpha<0) {
            if(alpha%(2*VALUE_DISC) != 0)
                alpha -= 2*VALUE_DISC + alpha%(2*VALUE_DISC) ;
        } else {
            alpha -= alpha%(2*VALUE_DISC);
        }
        
        if(beta <0) {
            beta  -= beta%(2*VALUE_DISC);
        } else {
            if(beta%(2*VALUE_DISC) != 0)
                beta  += 2*VALUE_DISC - beta%(2*VALUE_DISC);
        }
        
        //high score >= 48 skip selectivity stage
        if (selectivity == EG_HIGH_SELECT && abs(alpha) > 48*VALUE_DISC) {
            selectivity = 2;
        } else  if (selectivity != EG_HIGH_SELECT && abs(alpha) > 51*VALUE_DISC ) {
            selectivity = std::min(++selectivity, NO_SELECT);
            if (abs(alpha) > 59*VALUE_DISC )
                selectivity = NO_SELECT;
        }
        
        EG_PVS_root(sBoard, selectivity, alpha, beta, list);
        score = list->next->score;
        
        int left = 2;
        int right = 2;
        
        while (!abort.load() && !(alpha < score && score < beta)) {
            
            //*log << "                  EG research : [" << alpha << " < " << list->next->score << " < " << beta << "]" <<std::endl;
            
            if (score <= alpha) {
                
                if(alpha <= s_alpha)
                    break;
                
                left *=2;
                
                alpha = score - left*VALUE_DISC;
                
            } else if (score >= beta) {
                
                if(beta >= s_beta)
                    break;
                
                right *=2;
                
                beta  = score + right*VALUE_DISC;
            }
            
            
            alpha = std::max(s_alpha, std::min(s_beta-VALUE_DISC, alpha));
            beta  = std::min(s_beta, std::max(s_alpha+VALUE_DISC, beta ));
            
            //search window has always peers limits
            if(alpha<0) {
                if(alpha%(2*VALUE_DISC) != 0)
                    alpha -= 2*VALUE_DISC + alpha%(2*VALUE_DISC) ;
            } else {
                alpha -= alpha%(2*VALUE_DISC);
            }
            
            if(beta <0) {
                beta  -= beta%(2*VALUE_DISC);
            } else {
                if(beta%(2*VALUE_DISC) != 0)
                    beta  += 2*VALUE_DISC - beta%(2*VALUE_DISC);
            }
            
            EG_PVS_root(sBoard, selectivity, alpha, beta, list);
            score = list->next->score;
            
            
        };
        
        
        extra_time = 0;
        
        //        //check PV at 100%
        //        if(!abort.load() && selectivity == NO_SELECT && s_alpha < list->next->score && list->next->score < s_beta) {
        //            RXSearch::t_client save_client = search_client;
        //            search_client = RXSearch::kPrivate;
        //            check_PV(search_sBoard, list->next->score, list->next->score-VALUE_DISC, list->next->score+VALUE_DISC);
        //            search_client = save_client;
        //        }
        
        best_answer.position = list->next->position;
        best_answer.score = list->next->score;
        best_answer.depth = list->next->depth;
        best_answer.selectivity = CONFIDENCE[list->next->selectivity];
        std::ostringstream buf;
        hTable->mainline(buf, sBoard.board, type_hashtable);
        best_answer.mainLine = buf.str();
        
        best_answer.nodes += sBoard.board.n_nodes;
        
        
        int eTime = get_current_time();
        
        
        //unsynchronized acces
        RXHashValue entry;
        if(hTable->get(sBoard.board, type_hashtable, entry) && entry.depth >= sBoard.board.n_empties && entry.selectivity>=selectivity) {
            int type = EXACT;
            
            int score = list->next->score;
            if(entry.lower == entry.upper) {
                if(abort.load())
                    type = INTERRUPT;
            } else if(list->next->score == entry.lower || entry.upper ==  MAX_SCORE) {
                type = SUPERIOR;
                score = entry.lower;
            } else if(list->next->score == entry.upper || entry.lower == -MAX_SCORE){
                type = INFERIOR;
                score = entry.upper;
            }
            
            
            *log << display(sBoard.board, type, selectivity, score, eTime, eTime - eTime_start_level) << std::endl;
            
        }
        
        if(abort.load() )
            break;
        
        time_nextLevel = pTime_next_level(sBoard.board, eTime - eTime_start_level);
        
        if(probable_timeout(time_nextLevel)) {
            abort.store(true);
            *log << "                  EG Driver : likely timeout" << std::endl;
            
            break;
        }
        
        
        
    }
    
}


/*!
 * \brief Extension of research until the solution.
 *
 * This function prepare parameters for solver
 *
 * \param sBoard      	sBoard.
 * \param alpha      	lower bound.
 * \param beta       	upper bound.
 */


void RXEngine::check_PV(RXBBPatterns& sBoard, const int score, const int alpha, const int beta) {
    
    RXBitBoard& board = sBoard.board;
    
    if(board.n_empties>6) { //hash limit
        
        //move in hash
        int movePV = NOMOVE;
        RXHashValue entry;
        const unsigned long long hash_code = board.hashcode();
        if(hTable->get(hash_code, type_hashtable, entry)) {
            
            if(entry.depth >= board.n_empties && entry.selectivity == NO_SELECT) {
                
                if(entry.lower == entry.upper) {
                    movePV = entry.move;
                } else if(entry.lower == score && entry.upper >= beta) {
                    if(!(alpha < entry.lower && entry.lower < beta))
                        movePV = entry.move;
                } else if(entry.upper == score && entry.lower <= alpha) {
                    if(!(alpha < entry.upper && entry.upper < beta))
                        movePV = entry.move;
                }
                
            }
        }
        
        if(movePV != NOMOVE) {
            
            //check value;
            RXHashValue entry;
            if(hTable->get(hash_code, type_hashtable, entry)) {
                
                if(entry.depth >= board.n_empties && entry.selectivity == NO_SELECT) {
                    
                    
                    if(entry.lower>=beta) {
                        if (entry.upper<entry.lower) {
                            manager->sendMsg("ERROR ON PV 1\n");
                        }
                    } else if(entry.upper<=alpha) {
                        if(entry.upper<entry.lower) {
                            manager->sendMsg("ERROR ON PV 2\n");
                        }
                    } else if(!(alpha < entry.lower && entry.upper < beta && entry.lower == entry.upper && entry.lower == score)) {
                        
                        std::ostringstream buffer;
                        buffer << "[ " << alpha << " ; " << beta << " ] ";
                        buffer << "[ " << entry.lower << " ; " << entry.upper << " ] ";
                        buffer << "score : " << score << " move : " << RXMove::index_to_coord(entry.move) << std::endl;
                        
                        manager->sendMsg(buffer.str());
                        
                        manager->sendMsg("ERROR ON PV 3\n");
                    }
                    
                }
                
            } else {
                manager->sendMsg("ERROR ON PV 4\n");
            }
            
            
            if(movePV == PASS) {
                board.do_pass();
                check_PV(sBoard, -score, -beta, -alpha);
                board.do_pass();
                
            } else {
                RXMove move;
                
                ((board).*(board.generate_flips[movePV]))(move);
                ((sBoard).*(sBoard.update_patterns[movePV][board.player]))(move);
                sBoard.do_move(move);
                check_PV(sBoard, -score, -beta, -alpha);
                sBoard.undo_move(move);
            }
            
        }
        
        //find move
        if (movePV == NOMOVE) {
            
            int lower = alpha;
            int upper = beta;
            
            if(score <= lower)
                upper = lower + VALUE_DISC;
            
            if (score >= upper)
                lower = upper - VALUE_DISC;
            
            
            RXMove list[32];
            board.moves_producing(list);
            
            int value = UNDEF_SCORE;
            if(list->next != NULL) {
                
                for(RXMove* iter = list->next; iter != NULL; iter= iter->next)
                    ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                
                EG_PVS_root(sBoard, NO_SELECT, lower, upper, list);
                movePV = list->next->position;
                value  = list->next->score;
                
            } else {
                
                board.do_pass();
                board.moves_producing(list);
                if(list->next != NULL) {
                    for(RXMove* iter = list->next; iter != NULL; iter = iter->next)
                        ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                    
                    EG_PVS_root(sBoard, NO_SELECT, -upper, -lower, list);
                    movePV = PASS;
                    value = -list->next->score;
                }
                board.do_pass();
                
            }
            
            if(movePV != NOMOVE)
                hTable->update(hash_code, true, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, lower, upper, value, movePV);
            
            
        }
        
    }
    
}
