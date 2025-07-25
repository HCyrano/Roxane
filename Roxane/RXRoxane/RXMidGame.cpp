/*
 *  RXMidGame.cpp
 *  BitBoard
 *
 *  Created by Bruno Causse on 13/08/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#include <cstdlib>
#include <cmath>
#include <sstream>

#include "RXEngine.hpp"
#include "RXRoxane.hpp"



const int RXEngine::MG_SELECT = 1; //72%

const int RXEngine::MIN_DEPTH_USE_PROBCUT = 4; // DO NOT CHANGE

const int RXEngine::MIN_DEPTH_SPLITPOINT = 7;

const int RXEngine::MG_DEEP_TO_SHALLOW = 4;
const int RXEngine::MG_MOVING_WINDOW = 4; //4

// 21/06/2025 : desabled PV EXTENSION
// with vs without : 503 games s8r14 2:00
// W        D       L
// 85       293     125
// 16,9%    58,25%  24,85%
const bool RXEngine::USE_PV_EXTENSION = false;
const int RXEngine::PV_EXTENSION_DEPTH = 14;
const int RXEngine::MIN_DEPTH_USE_PV_EXTENSION = 16;


void RXEngine::iterative_deepening(RXBBPatterns& sBoard, RXMove* list, int depth, const int max_depth) {
    
    set_type_search(MIDGAME);
    
    if(dependent_time)
        determine_move_time(sBoard.board);
    
    int eTime = get_current_time();
    
    do {
        
        time_startLevel= eTime;
        
        sBoard.board.n_nodes = 0;
        
        use_pv_ext = false;
        
        if(USE_PV_EXTENSION && depth >= MIN_DEPTH_USE_PV_EXTENSION) {
            
            
            depth_pv_extension = PV_EXTENSION_DEPTH + (depth & 1);
            
            if(abs(list->next->score) > 18*VALUE_DISC)
                depth_pv_extension -= 2;
            
            if(sBoard.board.n_empties-depth <= depth_pv_extension) {
                use_pv_ext = true;
                *log << "                  Use Pv Extension" << std::endl;
            }
            
            
        }
        
        
        aspiration_search(sBoard, depth, list);
        
        eTime = get_current_time();
        
        RXHashValue entry;
        if(hTable->get(sBoard.board, type_hashtable, entry) && entry.depth>=depth) {
            int type = EXACT;
            
            int score = list->next->score;
            
            if(entry.lower == entry.upper) {
                if(abort.load())
                    type = INTERRUPT;
                
                if(score >= 64*VALUE_DISC){
                    score = 64*VALUE_DISC;
                    type = SUPERIOR;
                } else if (score <= -64*VALUE_DISC) {
                    score = -64*VALUE_DISC;
                    type = INFERIOR;
                }
                
            } else if(list->next->score == entry.lower || entry.upper == MAX_SCORE) {
                type = SUPERIOR;
                score = entry.lower;
            } else if(list->next->score == entry.upper || entry.lower == -MAX_SCORE) {
                type = INFERIOR;
                score = entry.upper;
            }
            
            *log << display(sBoard.board, type, depth, score, eTime, eTime - time_startLevel) << std::endl;
        }
        
        best_answer.position = list->next->position;
        best_answer.score = list->next->score;
        best_answer.depth = list->next->depth;
        best_answer.selectivity = CONFIDENCE[list->next->selectivity];
        std::ostringstream buf;
        hTable->mainline(buf, sBoard.board, type_hashtable);
        best_answer.mainLine = buf.str();
        
        best_answer.nodes += sBoard.board.n_nodes;
        
        if(abort.load() )
            break;
        
        //update probable time for next depth
        
        time_nextLevel = pTime_next_level(sBoard.board, eTime - time_startLevel, depth, depth+2);
        
        if(depth>6)
            if(probable_timeout(time_nextLevel)) {
                abort.store(true);
                *log << "                  MG driver : likely timeout" << std::endl;
                
                break;
            }
        
        //stop conditions EndGame
        if(fabs(list->next->score) >= ((MAX_SCORE-1) - 64*VALUE_DISC)) {
            break;
        }
        
        
        if(fabs(list->next->score)>48*VALUE_DISC && depth>=18 && sBoard.board.n_empties<38)
            break;
        
        
    } while((depth +=2)<=max_depth);
    
}


void RXEngine::aspiration_search(RXBBPatterns& sBoard, const int depth, RXMove* list) {
    
    const int s_alpha = (search_alpha <= -64*VALUE_DISC? -MAX_SCORE: search_alpha);
    const int s_beta  = (search_beta  >=  64*VALUE_DISC?  MAX_SCORE: search_beta);
    
    int alpha = std::max(s_alpha, std::min(s_beta-VALUE_DISC, list->next->score - MG_MOVING_WINDOW*VALUE_DISC));
    int beta  = std::min(s_beta, std::max(s_alpha+VALUE_DISC, list->next->score + MG_MOVING_WINDOW*VALUE_DISC));
    
    
    MG_PVS_root(sBoard, depth, alpha, beta, list);
    
    int left = 2;
    int right = 2;
    
    while (!abort.load()  && !(alpha < list->next->score && list->next->score < beta)) {
        
        //        *log << "                  MG research : [" << alpha << " < " << list->next->score << " < " << beta << "]" <<std::endl;
        
        
        if (list->next->score <= alpha) {
            
            if(alpha <= s_alpha)
                break;
            
            alpha = list->next->score - (MG_MOVING_WINDOW*left)*VALUE_DISC;
            left *=2;
            
        } else if (list->next->score >= beta) {
            
            if(beta >= s_beta)
                break;
            
            beta  = list->next->score + (MG_MOVING_WINDOW*right)*VALUE_DISC;
            right *=2;
        }
        
        
        alpha = std::max(s_alpha, std::min(s_beta-VALUE_DISC, alpha));
        beta  = std::min(s_beta, std::max(s_alpha+VALUE_DISC, beta ));
        
        MG_PVS_root(sBoard, depth, alpha, beta, list);
        
    };
    
}


// avec la selectivite et de l'extension de recherche, on ne peut faire totalement confiance a
// un coup etudie en null window (pas d'extension de recherche)
// c.a.d. on ne profite pas de la recherche NWS pour trouver un meilleur coup sauf si score>beta.
//
// root

void RXEngine::MG_PVS_root(RXBBPatterns& sBoard, const int depth,  const int alpha, const int beta, RXMove* list) {
    
    //assert(alpha>=-MAX_SCORE && beta<=MAX_SCORE);
    
    //*log << "                  [MidGame @" << CONFIDENCE[MG_SELECT] << " alpha : " << alpha << " beta : " << beta << "]" << std::endl;
    
    int selectivity = MG_SELECT;
    
    RXMove* iter = list->next;
    int bestmove = iter->position;
    
    int lower = alpha;
    const int upper = beta;
    
    bool selective_cutoff = false;
    bool child_selective_cutoff = false;
    
    //first move
    first_move.store(true);
    
    sBoard.do_move(*iter);
    
    int bestscore = -MG_PVS_deep(0, sBoard, true, selectivity, depth-1, child_selective_cutoff, -upper, -lower, false);
    
    sBoard.undo_move(*iter);
    
    if(!abort.load()  && std::abs(bestscore) != INTERRUPT_SEARCH) {
        
        
        if(search_client == RXSearch::kGGSMode) {	// GGS mode
            if(dependent_time && depth>13) {
                manager->sendMsg(showBestmove(depth, selectivity, lower, upper, bestscore, bestmove));
            }
        }
        
        if(bestscore > lower)
            lower = bestscore;
        else {
            extra_time = 1;
            //            *log << "                  [extra time <=:" << extra_time << "]" << std::endl;
        }
        
        selective_cutoff |= child_selective_cutoff;
        
        //other moves
        first_move.store(false);
        int score;
        for(iter = iter->next; !abort.load()  && lower<upper && iter != NULL; iter = iter->next) {
            
            
#ifdef USE_SPLIT_AT_ROOT
            
            // Split?
            if(activeThreads > 1 && iter->next != NULL && depth>(MIN_DEPTH_SPLITPOINT+3) && !abort.load()
               && !thread_should_stop(0) && idle_thread_exists(0)
               && split(sBoard, true, 0, depth, selectivity, selective_cutoff,
                        lower, upper, bestscore, bestmove, iter, 0, RXSplitPoint::MID_ROOT)) {
                
                break;
            }
#endif
            
            sBoard.do_move(*iter);
            
            //simple_PV pv == false ???
            score = -MG_PVS_deep(0, sBoard, false, selectivity, depth-1, child_selective_cutoff, -lower-VALUE_DISC, -lower, false); //change
            
            
            if(!abort.load()  && lower < score && score < upper) {
                
                if(search_client == RXSearch::kGGSMode) {	// GGS mode
                    if(dependent_time && depth>13)
                        manager->sendMsg("         " + RXMove::index_to_coord(iter->position) + " maybe better? ");
                }
                
                
                extra_time++;
                //                *log << "                  [extra time >:" << extra_time << "]" << std::endl;
                score = -MG_PVS_deep(0, sBoard, true, selectivity, depth-1, child_selective_cutoff, -upper, (child_selective_cutoff? -lower : -score), false);
                extra_time--;
                //                *log << "                  [extra time end :" << extra_time << "]" << std::endl;
                
                if(search_client == RXSearch::kGGSMode && !abort.load()) {    // GGS mode
                    if(dependent_time && depth>13 && score <= bestscore)
                        manager->sendMsg("         " + RXMove::index_to_coord(iter->position) + " refuted ");
                }
                
                
            }
            
            
            
            sBoard.undo_move(*iter);
            
            if (!abort.load() && std::abs(score) != INTERRUPT_SEARCH) {
                
                if(score > bestscore) {
                    
                    extra_time = 0;
                    
                    bestmove = iter->position;
                    bestscore = score;
                    
                    if(search_client == RXSearch::kGGSMode) {	// GGS mode
                        if(dependent_time && depth>13) {
                            manager->sendMsg(showBestmove(depth, selectivity, lower, upper, bestscore, bestmove));
                        }
                    }
                    
                    if (bestscore > lower)
                        lower = bestscore;
                    
                }
            }
            
            selective_cutoff |= child_selective_cutoff;
        }
        
        extra_time = 0;
        
        if(bestscore>=upper)
            selective_cutoff = child_selective_cutoff;
        
        
        //move to front bestmove
        list->sort_bestmove(bestmove);
        list->next->score = bestscore;
        list->next->selectivity = selective_cutoff? MG_SELECT: NO_SELECT;
        list->next->depth = depth;
        
        
        
        hTable->update(sBoard.board.hashcode(), true, type_hashtable, selective_cutoff? MG_SELECT: NO_SELECT, depth, alpha, upper, bestscore, bestmove);
        
        
    }
    
}

void RXEngine::MG_SP_search_root(RXSplitPoint* sp, const unsigned int threadID) {
    
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
        
        pthread_mutex_unlock(&(sp->lock));
        
        int score;
        int alpha = sp->alpha; //local copy
        int depth = sp->depth;
        bool child_selective_cutoff = false;
        
        sBoard.do_move(*move);
        
        
        score = -MG_PVS_deep(threadID, sBoard, false, sp->selectivity, depth-1, child_selective_cutoff, -alpha-VALUE_DISC, -alpha, false); //change
        
        if (!(abort.load() || thread_should_stop(threadID)) && alpha < score && score < sp->beta) {
            
            extra_time++;
            
            if(dependent_time && depth>13)
                manager->sendMsg("         " + RXMove::index_to_coord(move->position) + " maybe better? ");
            
            score = -MG_PVS_deep(threadID, sBoard, true, sp->selectivity, depth-1, child_selective_cutoff, -sp->beta, (child_selective_cutoff? -alpha : -score), false);
            
            
            if(search_client == RXSearch::kGGSMode && !(abort.load() || thread_should_stop(threadID))) {    // GGS mode
                if(dependent_time && depth>13 && score <= sp->bestscore)
                    manager->sendMsg("         " + RXMove::index_to_coord(move->position) + " refuted ");
            }
            
            
            extra_time--; //atomic = thread-safe
            
        }
        
        sBoard.undo_move(*move);
        
        if(abort.load() || thread_should_stop(threadID))
            break;
        
        //first without mutex
        if((score > sp->bestscore) || (!sp->selective_cutoff && child_selective_cutoff)) {
            
            //update
            pthread_mutex_lock(&(sp->lock));
            
            sp->selective_cutoff |= child_selective_cutoff;
            
            // New best move?
            if(score > sp->bestscore) {
                sp->bestscore = score;
                sp->bestmove = move->position;
                
                if(dependent_time && depth>13)
                    manager->sendMsg(showBestmove(depth, sp->selectivity, sp->alpha, sp->beta, sp->bestscore, sp->bestmove));
                
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


int RXEngine::MG_PVS_deep(const unsigned int threadID, RXBBPatterns& sBoard, const bool pv, const int selectivity, const int depth, bool& selective_cutoff, int alpha, const int beta, const bool passed) {
    
    //assert(alpha>=-MAX_SCORE && beta<=MAX_SCORE);
    
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
    int lower = alpha;
    int upper = beta;
    
    //PV EXTENSION
    if (USE_PV_EXTENSION && pv && use_pv_ext && (board.n_empties) <= depth_pv_extension) {
        
        if (board.n_empties <= EG_MEDIUM_HI_TO_LOW)
            return EG_PVS_hash_mobility(threadID, board, true, lower, upper, passed);
        
        return EG_PVS_ETC_mobility(threadID, sBoard, true, lower, upper, passed);
        
    }
    
    
    //synchronized acces
    RXHashValue entry;
    const unsigned long long hash_code = board.hashcode();
    if(hTable->get(hash_code, type_hashtable, entry)) {
        
        if(!pv && entry.depth >= depth) {
            
            if(entry.lower > lower) {
                
                lower = entry.lower;
                if(lower >= upper) {
                    
                    selective_cutoff = (entry.selectivity != NO_SELECT);
                    
                    return lower;
                }
            }
            
            if(entry.upper < upper) {
                
                upper = entry.upper;
                if(upper <= lower) {
                    
                    selective_cutoff = (entry.selectivity != NO_SELECT);
                    
                    return  upper;
                }
            }
            
        }
        
        bestmove = entry.move;
    }
    
    // IID
    if(pv && bestmove == NOMOVE && depth > 8) {
        bool child_selective_cutoff = false;
        
        MG_PVS_deep(threadID, sBoard, pv, selectivity, depth - 2, child_selective_cutoff, lower, upper, passed);
        
        if(abort.load() || thread_should_stop(threadID))
            return INTERRUPT_SEARCH;
        
        if(hTable->get(hash_code, type_hashtable, entry))
            bestmove = entry.move;
    }
    
    
    RXMove* list = threads[threadID]._move[board.n_empties];
    list->next = NULL;
    
    if(bestmove != PASS) {
        
        RXMove* move = list + 1;
        RXMove* previous = list;
        
        //ENHANCED TRANSPOSITION CUTOFF
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            board.n_nodes++;
            
            //synchronized acces
#ifdef USE_ETC
            if(!pv && hTable->get(board.hashcode_after_move(move), type_hashtable, entry) && entry.depth>=depth-1) {
                
                if(-entry.upper >= upper) {
                    selective_cutoff = (entry.selectivity != NO_SELECT);
                    return -entry.upper ;
                }
                
            }
#endif
            
            previous = previous->next = move++;
        }
        
        //for all empty square
        unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
        if(bestmove != NOMOVE)
            legal_movesBB ^= 0x1ULL<<bestmove;
        
        for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next) {
            if(legal_movesBB & 0x1ULL<<empties->position) {
                
                ((board).*(board.generate_flips[empties->position]))(*move);
                board.n_nodes++;
                
                move->score = 0;
                
                //synchronized acces
#ifdef USE_ETC
                
                if(!pv && hTable->get(board.hashcode_after_move(move), type_hashtable, entry)  && entry.depth>=depth-1) {
                    
                    if(-entry.upper >= upper) {
                        selective_cutoff = (entry.selectivity != NO_SELECT);
                        return -entry.upper ;
                    }
                    
                    move->score = -3*VALUE_DISC;
                    
                    if(-entry.lower<=lower)
                        move->score = 2*VALUE_DISC;
                    
                    
                }
#endif
                
                previous = previous->next = move++;
                
            }
        }
        
        previous->next = NULL;
        
    }
    
    int bestscore = UNDEF_SCORE;
    bool child_selective_cutoff = false;
    
    if (list->next == NULL) {
        
        if (passed) {
            bestscore = board.final_score();
            alpha = -(upper = +MAX_SCORE);
            bestmove = NOMOVE;
        } else {
            board.do_pass();
            
            if(depth <= MG_DEEP_TO_SHALLOW)
                bestscore = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -lower, true);
            else
                bestscore = -MG_PVS_deep(threadID, sBoard, pv, selectivity, depth-1, child_selective_cutoff, -upper, -lower, true);
            
            board.do_pass();
            bestmove = PASS;
            
            selective_cutoff |= child_selective_cutoff;
        }
        
    } else {
        
        
        
        if(bestmove != NOMOVE) {
            
            /* first move */
            list = list->next;
            
            ((sBoard).*(sBoard.update_patterns[list->position][board.player]))(*list);
            
            sBoard.do_move(*list);
            
            if(depth <= MG_DEEP_TO_SHALLOW)
                bestscore = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -lower, false);
            else
                bestscore = -MG_PVS_deep(threadID, sBoard, pv, selectivity, depth-1, child_selective_cutoff, -upper, -lower, false);
            
            sBoard.undo_move(*list);
            
            
            //interrupt search
            if(abort.load()  || thread_should_stop(threadID))
                return INTERRUPT_SEARCH;
            
            
            
            bestmove = list->position;
            
            if(bestscore > lower)
                lower = bestscore;
            
            selective_cutoff |= child_selective_cutoff;
        }
        
        if(lower < upper) {
            
            if(list->next != NULL) {
                
                if((list->next)->next != NULL) {
                    //sort
                    
                    if(depth>=10) {
                        
                        int lower_probcut = -MAX_SCORE;
                        int upper_probcut =  MAX_SCORE;
                        probcut_bounds(board, 3, (4 + depth/4 + depth&0x1UL), 0, (lower+upper)/2, lower_probcut, upper_probcut); //selectivity 3 = 91%
                        
                        if(lower_probcut<= sBoard.get_score()) { // && eval_position<=(beta+upper_probcut*4)) { //alpha 95% / beta 99%
                            
                            //stable position -> sorting on evaluation
                            
                            for(RXMove* iter = list->next; iter!=NULL; iter = iter->next) {
                                
                                ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                                
                                //stable move -> sorting on evaluation
                                sBoard.do_move(*iter);
                                
                                if(depth >= 30) {
                                    
                                    if((depth & 0x1UL) == 0)
                                        iter->score += PVS_last_ply(threadID, sBoard, 6, -upper_probcut , -lower_probcut, false);
                                    else
                                        iter->score += PVS_last_ply(threadID, sBoard, 5, -upper_probcut , -lower_probcut, false);
                                    
                                } else if(depth >= 24) {
                                    
                                    if((depth & 0x1UL) == 0)
                                        iter->score += PVS_last_ply(threadID, sBoard, 4, -upper_probcut , -lower_probcut, false);
                                    else
                                        iter->score += alphabeta_last_three_ply(threadID, sBoard, -upper_probcut , -lower_probcut, false);
                                    
                                } else if(depth >= 18) {
                                    
                                    if((depth & 0x1UL) == 0)
                                        iter->score += alphabeta_last_two_ply(threadID, sBoard, -upper_probcut , -lower_probcut, false);
                                    else
                                        iter->score += alphabeta_last_three_ply(threadID, sBoard, -upper_probcut , -lower_probcut, false);
                                    
                                } else  if((depth & 0x1UL) == 0) {
                                    
                                    iter->score += alphabeta_last_two_ply(threadID, sBoard, -upper_probcut , -lower_probcut, false);
                                    
                                } else {
                                    
                                    int bestscore1 = UNDEF_SCORE; //masquage
                                    
                                    const unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
                                    if(legal_movesBB) {
                                        
                                        RXMove& lastMove = threads[threadID]._move[board.n_empties][1];
                                        for(RXSquareList* empties = board.empties_list->next; bestscore1<-lower_probcut && empties->position != NOMOVE; empties = empties->next)
                                            if(legal_movesBB & 0x1ULL<<empties->position) {
                                                ((board).*(board.generate_flips[empties->position]))(lastMove);
                                                ((sBoard).*(sBoard.update_patterns[empties->position][board.player]))(lastMove);
                                                board.n_nodes++;
                                                
                                                int score= -sBoard.get_score(lastMove);
                                                if (score>bestscore1) {
                                                    bestscore1 = score;
                                                }
                                                
                                            }
                                        
                                    } else  {
                                        //PASS
                                        sBoard.board.do_pass();
                                        iter->score -= sBoard.get_score();
                                        sBoard.board.do_pass();
                                        
                                    }
                                    iter->score += bestscore1;
                                }
                                
                                sBoard.undo_move(*iter);
                                
                                
                            }
                            
                        } else {
                            
                            for(RXMove* iter = list->next; iter!=NULL; iter = iter->next) {
                                board.n_nodes++;
                                
                                ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                                iter->score += sBoard.get_score(*iter);
                            }
                        }
                        
                    } else {
                        
                        for(RXMove* iter = list->next; iter!=NULL; iter = iter->next) {
                            board.n_nodes++;
                            
                            ((sBoard).*(sBoard.update_patterns[iter->position][board.player]))(*iter);
                            iter->score += sBoard.get_score(*iter);
                        }
                        
                    }
                    
                } else {
                    ((sBoard).*(sBoard.update_patterns[list->next->position][board.player]))(*(list->next));
                }
            }
            
            if(bestscore == UNDEF_SCORE) { //first move
                
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
                
                bestmove = move->position;
                
                sBoard.do_move(*move);
                if(depth <= MG_DEEP_TO_SHALLOW)
                    bestscore = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -lower, false);
                else
                    bestscore = -MG_PVS_deep(threadID, sBoard, pv, selectivity, depth-1, child_selective_cutoff, -upper, -lower, false);
                sBoard.undo_move(*move);
                
                //interrupt search
                if(abort.load()  || thread_should_stop(threadID))
                    return INTERRUPT_SEARCH;
                
                
                
                if(bestscore>lower)
                    lower = bestscore;
                
                selective_cutoff |= child_selective_cutoff;
                
                // next move
                list = list->next;
            }
            
            int score;
            for(;!abort.load()  && lower < upper && list->next != NULL; list = list->next) {
                
                RXMove* move = list->next;
                
                if(move->next != NULL) {	//more 1 move
                    
                    // Split?
                    if(activeThreads > 1 && depth>MIN_DEPTH_SPLITPOINT && !abort.load()
                       && !thread_should_stop(threadID) && idle_thread_exists(threadID)
                       && split(sBoard, pv, 0, depth, selectivity, selective_cutoff,
                                lower, upper, bestscore, bestmove, list, threadID, RXSplitPoint::MID_PVS))
                        
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
                
                if(depth <= MG_DEEP_TO_SHALLOW) {
                    score = -MG_PVS_shallow(threadID, sBoard, false, depth-1, -lower-VALUE_DISC, -lower, false);
                    if(lower < score && score < upper) {
                        score = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -score, false);
                    }
                } else {
                    
                    score = -MG_NWS_XProbCut(threadID, sBoard, 0, selectivity, depth-1, child_selective_cutoff, -lower-VALUE_DISC, false);
                    
                    if(lower < score && score < upper)
                        score = -MG_PVS_deep(threadID, sBoard, pv, selectivity, depth-1, child_selective_cutoff, -upper, -lower, false);
                    
                }
                
                sBoard.undo_move(*move);
                
                //interrupt search
                if(abort.load()  || thread_should_stop(threadID))
                    return INTERRUPT_SEARCH;
                
                
                
                if (score>bestscore) {
                    bestmove = move->position;
                    bestscore = score;
                    if (bestscore>lower)
                        lower = bestscore;
                }
                
                //                if ( child_selective_cutoff )
                //                    selective_cutoff = true;
                
                selective_cutoff |= child_selective_cutoff;
                
            }
        }
        
    }
    
    
    //interrupt search
    if(abort.load()  || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    
    
    if(bestscore>=upper)
        selective_cutoff = child_selective_cutoff;
    
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(   hash_code, pv, type_hashtable, selective_cutoff? MG_SELECT : NO_SELECT, depth, alpha, upper,  bestscore, bestmove);
    /*if(pv)*/
    hTable_PV->update(hash_code, pv, type_hashtable, selective_cutoff? MG_SELECT : NO_SELECT, depth, alpha, upper,  bestscore, bestmove);
    
    return bestscore;
    
}

// MG_SP_search_deep() is used to search from a PV split point.  This function
// is called by each thread working at the split point.  It is similar to
// the normal EG_PVS_deep() function, but simpler.  Because we have already
// probed the hash table and searched the first move before splitting, we
// don't have to repeat all this work in MG_SP_search_deep().  We also don't
// need to store anything to the hash table here:  This is taken care of
// after we return from the split point.

void RXEngine::MG_SP_search_deep(RXSplitPoint* sp, const unsigned int threadID) {
    
    
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
        
        int score;
        int alpha = sp->alpha; //local copy
        bool child_selective_cutoff = false;
        
        sBoard.do_move(*move);
        
        if(sp->depth <= MG_DEEP_TO_SHALLOW) {
            
            score = -MG_PVS_shallow(threadID, sBoard, false, sp->depth-1, -alpha-VALUE_DISC, -alpha, false);
            
            if(alpha < score && score < sp->beta) {
                score = -MG_PVS_shallow(threadID, sBoard, sp->pv, sp->depth-1, -sp->beta, -score, false);
            }
        } else {
            
            score = -MG_NWS_XProbCut(threadID, sBoard, sp->pvDev+1, sp->selectivity, sp->depth-1, child_selective_cutoff, -alpha-VALUE_DISC, false);
            
            if(alpha < score && score < sp->beta)
                score = -MG_PVS_deep(threadID, sBoard, sp->pv, sp->selectivity, sp->depth-1, child_selective_cutoff, -sp->beta, -sp->alpha, false);
            
        }
        
        sBoard.undo_move(*move);
        
        
        if(abort.load()  || thread_should_stop(threadID))
            break;
        
        //first without mutex
        if((score > sp->bestscore) || (!sp->selective_cutoff && child_selective_cutoff)) {
            
            
            //update
            pthread_mutex_lock(&(sp->lock));
            
            
            sp->selective_cutoff |= child_selective_cutoff;
            
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


int RXEngine::MG_PVS_shallow(const unsigned int threadID, RXBBPatterns& sBoard, const bool pv, const int depth, int alpha, const int beta, const bool passed) {
    
    RXBitBoard& board = sBoard.board;
    int bestscore = UNDEF_SCORE;
    
    if(depth == 0) {
        
        return sBoard.get_score();
        
    } else if(depth == 1) {
        
        
        
        unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
        if(legal_movesBB) {
            
            RXMove& move = threads[threadID]._move[board.n_empties][1];
            for(RXSquareList* empties = board.empties_list->next; alpha < beta &&  empties->position != NOMOVE; empties = empties->next) {
                if(legal_movesBB & 0x1ULL<<empties->position) {
                    
                    ((board).*(board.generate_flips[empties->position ]))(move);
                    ((sBoard).*(sBoard.update_patterns[empties->position ][board.player]))(move);
                    board.n_nodes++;
                    int score= -sBoard.get_score(move);
                    
                    if (score>bestscore) {
                        bestscore = score;
                        if (bestscore>alpha)
                            alpha = bestscore;
                    }
                }
            }
            
        } else {
            
            if(passed) {
                bestscore = sBoard.final_score();
            } else {
                board.do_pass();
                
                bestscore = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -beta, -alpha, true);
                
                board.do_pass();
            }
            
        }
        
        return bestscore;
    }
    
    //PV EXTENSION
    if (USE_PV_EXTENSION && pv && use_pv_ext && (board.n_empties- depth) <= depth_pv_extension) {
        
        if (board.n_empties <= EG_MEDIUM_HI_TO_LOW)
            return EG_PVS_hash_mobility(threadID, board, true, alpha, beta, passed);
        
        return EG_PVS_ETC_mobility(threadID, sBoard, true, alpha, beta, passed);
        
    }
    
    
    //synchronized acces
    const unsigned long long hash_code = board.hashcode();
    hTable->entry_prefetch(hash_code, type_hashtable);
    
    int bestmove = NOMOVE;
    
    int upper = beta;
    int lower = alpha;
        
    RXHashValue entry;
    if(hTable->get(hash_code, type_hashtable, entry)) {
        
        if(!pv && entry.depth >= depth) {
            
            if(entry.lower > lower) {
                
                lower = entry.lower;
                
                if(lower >= upper)
                    return lower;
                
            }
            
            if(entry.upper < upper) {
                
                upper = entry.upper;
                
                if(upper <= lower)
                    return  upper;
                
            }
            //modification 17/03/2025
            bestmove = entry.move;
            
        }
        
    }
    
    
    if(bestmove != PASS) {
        
        RXMove* list = threads[threadID]._move[board.n_empties];
        RXMove* move = list + 1;
        
        if(bestmove != NOMOVE) {
            
            ((board).*(board.generate_flips[bestmove]))(*move);
            ((sBoard).*(sBoard.update_patterns[bestmove][board.player]))(*move);
            
            sBoard.do_move(*move);
            bestscore = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -lower, false);
            sBoard.undo_move(*move);
            
            if (bestscore>lower)
                lower = bestscore;
            
        }
        
        if(lower<upper) {
            
            
            unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
            if(bestmove != NOMOVE)
                legal_movesBB ^= 0x1ULL<<bestmove;
            
            
            //sort on score
            if(legal_movesBB) {
                
                RXMove *previous = list;
                
                const int p = board.player;
                
                for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
                    if(legal_movesBB & 0x1ULL<<empties->position) {
                        ((board).*(board.generate_flips[empties->position]))(*move);
                        ((sBoard).*(sBoard.update_patterns[move->position][p]))(*move);
                        
                        board.n_nodes++;
                        
                        previous = previous->next = move++;
                    }
                
                previous->next = NULL;
                
                if((list->next)->next != NULL) { //nb moves > 1
                    
                    //sort list by evaluation
                    for(RXMove* iter = list->next; iter != NULL; iter = iter->next) {
                        iter->score = sBoard.get_score(*iter);
                    }
                    
                }
                
                
                int score;
                for(;  lower < upper && list->next != NULL; list = list->next) {
                    
                    RXMove* move = list->next;
                    
                    if(move->next != NULL) {    //more 1 move
                        
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
                    
                    if(bestscore == UNDEF_SCORE) {
                        score = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -lower, false);
                    } else {
                        score = -MG_PVS_shallow(threadID, sBoard, false, depth-1, -lower-VALUE_DISC, -lower, false);
                        
                        if(lower < score && score < upper)
                            score = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -score, false);
                    }
                    
                    sBoard.undo_move(*move);
                    
                    
                    if (score>bestscore) {
                        bestmove = move->position;
                        bestscore = score;
                        if (bestscore>lower)
                            lower = bestscore;
                    }
                    
                    
                }
                
            }
        }
    }
    
    if(bestscore == UNDEF_SCORE) {
        
        if(passed) {
            alpha = -MAX_SCORE;
            upper = MAX_SCORE;
            bestmove = NOMOVE;
            bestscore = sBoard.final_score();
        } else {
            board.do_pass();
            
            bestscore = -MG_PVS_shallow(threadID, sBoard, pv, depth-1, -upper, -lower, true);
            
            board.do_pass();
            bestmove = PASS;
            
        }
    }
    
    //en test 21/01/2025 suspision bug (bestscore >= upper mais stocker comme < beta)
    hTable->update(hash_code, pv, type_hashtable, NO_SELECT, depth, alpha, upper,  bestscore, bestmove);
    
    return bestscore;
    
}


/*
 Null Window Search  + XProbCut
 */
int RXEngine::MG_NWS_XProbCut(const unsigned int threadID, RXBBPatterns& sBoard, const int pvDev, const int selectivity, const int depth, bool& selective_cutoff, const int alpha, const bool passed) {
    
    //assert(alpha>=-MAX_SCORE);
    
    if(abort.load() || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    //time gestion
    if (dependent_time && get_current_dependentTime() > time_limit()) {
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
        
        if(entry.depth >= depth) {
            
            if(entry.lower > alpha) {
                
                selective_cutoff = (entry.selectivity != NO_SELECT);
                
                return entry.lower;
            }
            
            if(entry.upper <= alpha) {
                
                selective_cutoff = (entry.selectivity != NO_SELECT);
                
                return  entry.upper;
            }
            
            
        }
        
        //if(entry.depth >= depth-2)
        bestmove = entry.move;
    }
    
    
    //param mpc
    int lower_probcut, upper_probcut;
    probcut_bounds(board, selectivity, depth, pvDev, alpha, lower_probcut, upper_probcut);
    int probcut_depth = (depth/4)*2 + (depth & 0x1UL);
    
    
    if(bestmove != NOMOVE && entry.selectivity >= selectivity && entry.depth>=probcut_depth) {
        
        if(entry.lower >= upper_probcut) {
            selective_cutoff = true;
            return alpha+VALUE_DISC;
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
        
        if(depth > 8) { //ETC depth 10 bestmove != NOMOVE ||
            
            RXMove* move = list + 1;
            RXMove* previous = list;
            
            //ENHANCED TRANSPOSITION CUTOFF
            if(bestmove != NOMOVE) {
                
                ((board).*(board.generate_flips[bestmove]))(*move);
                board.n_nodes++;
                
                //synchronized acces
#ifdef USE_ETC
                
                if(hTable->get(board.hashcode_after_move(move), type_hashtable, entry) && entry.depth >= depth-1) {
                    
                    if(-entry.upper > alpha) {
                        selective_cutoff = (entry.selectivity != NO_SELECT);
                        
                        return -entry.upper ;
                    }
                }
#endif
                
                previous = previous->next = move++;
                
            }
            
            //for all empty square
            unsigned long long legal_movesBB = RXBitBoard::get_legal_moves(board.discs[board.player], board.discs[board.player^1]);
            if(bestmove != NOMOVE)
                legal_movesBB ^= 0x1ULL<<bestmove;
            
            for(RXSquareList* empties = board.empties_list->next; empties->position != NOMOVE; empties = empties->next)
                if(legal_movesBB & 0x1ULL<<empties->position) {
                    
                    ((board).*(board.generate_flips[empties->position]))(*move);
                    board.n_nodes++;
                    
                    move->score = 0;
                    
                    //synchronized acces
#ifdef USE_ETC
                    
                    if(hTable->get(board.hashcode_after_move(move), type_hashtable, entry) && entry.depth>=depth-1) {
                        
                        if(-entry.upper > alpha) {
                            selective_cutoff = (entry.selectivity != NO_SELECT);
                            
                            return -entry.upper ;
                        }
                        
                        move->score = ((-entry.lower<=alpha)*5-2)*VALUE_DISC;
                        
                        
                    }
#endif
                    
                    previous = previous->next = move++;
                    
                }
            
            previous->next = NULL;
            
        } else {
            
            board.moves_producing(list);
            if(bestmove != NOMOVE)
                list->sort_bestmove(bestmove);
        }
        
    }
    
    int bestscore = UNDEF_SCORE;
    bool child_selective_cutoff = false;
    
    
    if(list->next == NULL) {
        //PASS
        if(passed) {
            bestscore = sBoard.final_score();
            hTable->update(hash_code, false, type_hashtable, NO_SELECT, DEPTH_BOOSTER+board.n_empties, -MAX_SCORE, MAX_SCORE,  bestscore, bestmove);
            return bestscore;
        } else {
            board.do_pass();
            if(depth > MIN_DEPTH_USE_PROBCUT) {
                bestscore = -MG_NWS_XProbCut(threadID, sBoard, pvDev, selectivity, depth-1, child_selective_cutoff, -alpha-VALUE_DISC, true);
            } else {
                bestscore = -alphabeta_last_three_ply(threadID, sBoard, -alpha-VALUE_DISC, -alpha, true);
            }
            
            board.do_pass();
            bestmove = PASS;
        }
        
    } else {
        
        //XProbcut
        int type_probcut = probcut(threadID, false, sBoard, selectivity, probcut_depth, lower_probcut, upper_probcut, list, bestmove != NOMOVE);
        if(type_probcut == BETA_CUT) {
            selective_cutoff = true;
            return alpha + VALUE_DISC;
        }
#ifdef USE_PROBCUT_ALPHA
        if(type_probcut == ALPHA_CUT) {
            selective_cutoff = true;
            return alpha;
        }
#endif
        
        //interrupt search
        if(abort.load() || thread_should_stop(threadID))
            return INTERRUPT_SEARCH;
        
        
        //fisrt move
        RXMove* move = list->next;
        
        sBoard.do_move(*move);
        
        if(depth > MIN_DEPTH_USE_PROBCUT) {
            bestscore = -MG_NWS_XProbCut(threadID, sBoard, pvDev, selectivity, depth-1, child_selective_cutoff, -alpha-VALUE_DISC, false);
        } else {
            bestscore = -alphabeta_last_three_ply(threadID, sBoard, -alpha-VALUE_DISC, -alpha, false);
        }
        
        sBoard.undo_move(*move);
        
        //interrupt search
        if(abort.load() || thread_should_stop(threadID))
            return INTERRUPT_SEARCH;
        
        
        
        bestmove = move->position;
        list = list->next;
        
        selective_cutoff |= child_selective_cutoff;
        
        int score;
        for(RXMove* iter = list->next;!abort.load()  && bestscore<=alpha && iter != NULL; iter = iter->next, list = list->next) {
            
            //		//assert(bestscore >= -MAX_SCORE);
            //		if(bestscore<-MAX_SCORE)
            //			std::cout << "Error" << std::endl;
            
            // Split?
            if(activeThreads > 1 && depth>MIN_DEPTH_SPLITPOINT && iter->next != NULL && !abort.load()
               && idle_thread_exists(threadID) && !thread_should_stop(threadID)
               && split(sBoard, false, pvDev, depth, selectivity, selective_cutoff,
                        alpha, (alpha+VALUE_DISC), bestscore, bestmove, list, threadID, RXSplitPoint::MID_XPROBCUT))
                
                break;
            
            
            sBoard.do_move(*iter);
            
            if(depth > MIN_DEPTH_USE_PROBCUT) {
                score = -MG_NWS_XProbCut(threadID, sBoard, pvDev+1,selectivity, depth-1, child_selective_cutoff, -alpha-VALUE_DISC, false);
            } else {
                score = -alphabeta_last_three_ply(threadID, sBoard, -alpha-VALUE_DISC, -alpha, false);
            }
            
            sBoard.undo_move(*iter);
            
            //interrupt search
            if(abort.load() || thread_should_stop(threadID))
                return INTERRUPT_SEARCH;
            
            
            
            if (score>bestscore) {
                bestscore = score;
                bestmove = iter->position;
            }
            
            selective_cutoff |= child_selective_cutoff;
            
            
        }
    }
    
    
    //interrupt search
    if(abort.load()  || thread_should_stop(threadID))
        return INTERRUPT_SEARCH;
    
    
    
    if(bestscore>alpha)
        selective_cutoff = child_selective_cutoff;
    
    hTable->update(hash_code, type_hashtable, selective_cutoff? MG_SELECT : NO_SELECT, depth, alpha, bestscore, bestmove);
    if(pvDev < 4)
        hTable_PV->update(hash_code, type_hashtable, selective_cutoff? MG_SELECT : NO_SELECT, depth, alpha, bestscore, bestmove);
    
    return bestscore;
    
    
}

// MG_SP_search_XProbcut() is used to search from a PV split point.  This function
// is called by each thread working at the split point.  It is similar to
// the normal EG_NWS_XEndCut() function, but simpler.  Because we have already
// probed the hash table and searched the first move before splitting, we
// don't have to repeat all this work in MG_SP_search_XProbcut().  We also don't
// need to store anything to the hash table here:  This is taken care of
// after we return from the split point.

void RXEngine::MG_SP_search_XProbcut(RXSplitPoint* sp, const unsigned int threadID) {
    
    //    assert(threadID >= 0 && threadID < activeThreads);
    //    assert(activeThreads > 1);
    
    RXBBPatterns& sBoard = sp->sBoardStack[threadID];
    sBoard = *(sp->sBoard);                                 //operator=
    RXBitBoard& board = sBoard.board;
    board.n_nodes = 0;
    
    //here sp->alpha is const
    while(sp->bestscore <= sp->alpha && !abort.load()  && !thread_should_stop(threadID)) {
        
        
        //verouillage du splitpoint
        pthread_mutex_lock(&(sp->lock));
        
        if(sp->list->next == NULL) {
            pthread_mutex_unlock(&(sp->lock));
            break;
        }
        
        RXMove* move = sp->list->next;
        sp->list = move;
        
        pthread_mutex_unlock(&(sp->lock));
        
        //deverouillage de splitpoint
        
        int alpha = sp->alpha; //local copy
        bool child_selective_cutoff = false;
        
        int score;
        sBoard.do_move(*move);
        
        if(sp->depth > MIN_DEPTH_USE_PROBCUT) {
            score = -MG_NWS_XProbCut(threadID, sBoard, sp->pvDev+1, sp->selectivity, sp->depth-1, child_selective_cutoff, -alpha-VALUE_DISC, false);
        } else {
            score = -alphabeta_last_three_ply(threadID, sBoard, -alpha-VALUE_DISC, -alpha, false);
        }
        
        sBoard.undo_move(*move);
        
        if(abort.load()  || thread_should_stop(threadID))
            break;
        
        
        //first without mutex
        if((score > sp->bestscore) || (!sp->selective_cutoff && child_selective_cutoff)) {
            
            //update
            pthread_mutex_lock(&(sp->lock));
            
            sp->selective_cutoff |= child_selective_cutoff;
            
            // New best move?
            if(score > sp->bestscore) {
                sp->bestscore = score;
                sp->bestmove = move->position;
                if(score > sp->alpha) {
                    sp->explored =true;
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

