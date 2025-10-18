/*
 *  RXBBPatterns.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 31/07/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXBBPATTERN_HPP
#define RXBBPATTERN_HPP

#include <cmath>
#include <arm_neon.h>

#include "RXConstantes.hpp"
#include "RXBitBoard.hpp"
#include "RXPattern.hpp"
#include "RXEvaluation.hpp"


class RXBBPatterns {
    
    void set_BLACK(const int pos);
    void set_WHITE(const int pos);
    
    // move functions
#define func(pos)   void update_patterns_BLACK_##pos(RXMove& move) const; \
void update_patterns_WHITE_##pos(RXMove& move) const
    
    func(A1); func(B1); func(C1); func(D1); func(E1); func(F1); func(G1); func(H1);
    func(A2); func(B2); func(C2); func(D2); func(E2); func(F2); func(G2); func(H2);
    func(A3); func(B3); func(C3); func(D3); func(E3); func(F3); func(G3); func(H3);
    func(A4); func(B4); func(C4);                      func(F4); func(G4); func(H4);
    func(A5); func(B5); func(C5);                      func(F5); func(G5); func(H5);
    func(A6); func(B6); func(C6); func(D6); func(E6); func(F6); func(G6); func(H6);
    func(A7); func(B7); func(C7); func(D7); func(E7); func(F7); func(G7); func(H7);
    func(A8); func(B8); func(C8); func(D8); func(E8); func(F8); func(G8); func(H8);
#undef func

    public :

    RXBitBoard board;
    RXPattern* pattern;

    
    RXBBPatterns();
    
    //constructeur par copie
    RXBBPatterns(const RXBBPatterns& src);
    
    RXBBPatterns& operator=(const RXBBPatterns& src);
    
    ~RXBBPatterns();
    
    void build(const std::string& init);
    
    void do_move(RXMove& move);
    void undo_move(const RXMove& move);
    
    
    
    void (RXBBPatterns::*update_patterns[64][2])(RXMove& move) const;
    void init_update_patterns();
    
    void reset();
    
    double get_n_nodes() const { return board.get_n_nodes(); }
    int get_n_empty() const { return board.n_empty; }
    
    int final_score() const;
    int get_score() const noexcept;
    int get_score(RXMove& move) const noexcept ;
        
    /* DEBUG */
    friend std::ostream& operator<<(std::ostream& os, RXBBPatterns& sBoard);
    
};


inline void RXBBPatterns::do_move(RXMove& move) {
    board.do_move(move);
    move.undo_pattern = pattern;
    pattern = move.pattern;
}

inline void RXBBPatterns::undo_move(const RXMove& move) {
    pattern = move.undo_pattern;
    board.undo_move(move);
}

//track bug HASH 24/01/2025
inline int RXBBPatterns::final_score() const {
    int score = board.final_score();
    if(score<0)
        return (-MAX_SCORE+1) + (score + 64);
    return (MAX_SCORE-1) + (score-64);
}



#ifndef NEW_EVAL





inline int RXBBPatterns::get_score() const {
    
    const int stage = 60-board.n_empty;
    
    const unsigned long long filled = (board.discs[BLACK] | board.discs[WHITE]);
    
    int eval;
    const short* value;
    const short* value_b;
    
    if(board.player == BLACK) {
        
        value = RXEvaluation::DIAG_5[stage];
        eval  = value[pattern->diag_5a];
        eval += value[pattern->diag_5b];
        eval += value[pattern->diag_5c];
        eval += value[pattern->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        eval += value[pattern->diag_6a];
        eval += value[pattern->diag_6b];
        eval += value[pattern->diag_6c];
        eval += value[pattern->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        eval += value[pattern->diag_7a];
        eval += value[pattern->diag_7b];
        eval += value[pattern->diag_7c];
        eval += value[pattern->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        eval += value[pattern->diag_8a];
        eval += value[pattern->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        eval += value[pattern->hv_4a];
        eval += value[pattern->hv_4b];
        eval += value[pattern->hv_4c];
        eval += value[pattern->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        eval += value[pattern->hv_3a];
        eval += value[pattern->hv_3b];
        eval += value[pattern->hv_3c];
        eval += value[pattern->hv_3d];
        
        value = RXEvaluation::HV_2[stage];
        eval += value[pattern->hv_2a];
        eval += value[pattern->hv_2b];
        eval += value[pattern->hv_2c];
        eval += value[pattern->hv_2d];
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[pattern->corner11a];
        eval += value[pattern->corner11b];
        eval += value[pattern->corner11c];
        eval += value[pattern->corner11d];
        
        value = RXEvaluation::corner2x5[stage];
        eval += value[pattern->corner2x5a];
        eval += value[pattern->corner2x5b];
        eval += value[pattern->corner2x5c];
        eval += value[pattern->corner2x5d];
        eval += value[pattern->corner2x5e];
        eval += value[pattern->corner2x5f];
        eval += value[pattern->corner2x5g];
        eval += value[pattern->corner2x5h];
        
        value = RXEvaluation::EDGE_2XC[stage];
        value_b = RXEvaluation::EDGE_6_4[stage];
        if(filled & 0x8142000000000000ULL)	//A1 H1 B2 G2
            eval += value[pattern->edge2XCa];
        else
            eval += value_b[pattern->edge64a];
        
        if(filled & 0x0102000000000201ULL) //H1 G2 G7 H8
            eval += value[pattern->edge2XCb];
        else
            eval += value_b[pattern->edge64b];
        
        if(filled & 0x0000000000004281ULL)	//B7 G7 A8 H8
            eval += value[pattern->edge2XCc];
        else
            eval += value_b[pattern->edge64c];
        
        if(filled & 0x8040000000004080ULL)	//A1 B2 B7 A8
            eval += value[pattern->edge2XCd];
        else
            eval += value_b[pattern->edge64d];
        
    } else {
        
        value = RXEvaluation::DIAG_5[stage];
        eval  = value[-pattern->diag_5a];
        eval += value[-pattern->diag_5b];
        eval += value[-pattern->diag_5c];
        eval += value[-pattern->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        eval += value[-pattern->diag_6a];
        eval += value[-pattern->diag_6b];
        eval += value[-pattern->diag_6c];
        eval += value[-pattern->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        eval += value[-pattern->diag_7a];
        eval += value[-pattern->diag_7b];
        eval += value[-pattern->diag_7c];
        eval += value[-pattern->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        eval += value[-pattern->diag_8a];
        eval += value[-pattern->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        eval += value[-pattern->hv_4a];
        eval += value[-pattern->hv_4b];
        eval += value[-pattern->hv_4c];
        eval += value[-pattern->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        eval += value[-pattern->hv_3a];
        eval += value[-pattern->hv_3b];
        eval += value[-pattern->hv_3c];
        eval += value[-pattern->hv_3d];
        
        value = RXEvaluation::HV_2[stage];
        eval += value[-pattern->hv_2a];
        eval += value[-pattern->hv_2b];
        eval += value[-pattern->hv_2c];
        eval += value[-pattern->hv_2d];
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[-pattern->corner11a];
        eval += value[-pattern->corner11b];
        eval += value[-pattern->corner11c];
        eval += value[-pattern->corner11d];
        
        value = RXEvaluation::corner2x5[stage];
        eval += value[-pattern->corner2x5a];
        eval += value[-pattern->corner2x5b];
        eval += value[-pattern->corner2x5c];
        eval += value[-pattern->corner2x5d];
        eval += value[-pattern->corner2x5e];
        eval += value[-pattern->corner2x5f];
        eval += value[-pattern->corner2x5g];
        eval += value[-pattern->corner2x5h];
        
        value = RXEvaluation::EDGE_2XC[stage];
        value_b = RXEvaluation::EDGE_6_4[stage];
        if(filled & 0x8142000000000000ULL)	//A1 H1 B2 G2
            eval += value[-pattern->edge2XCa];
        else
            eval += value_b[-pattern->edge64a];
        
        if(filled & 0x0102000000000201ULL) //H1 G2 G7 H8
            eval += value[-pattern->edge2XCb];
        else
            eval += value_b[-pattern->edge64b];
        
        if(filled & 0x0000000000004281ULL)	//B7 G7 A8 H8
            eval += value[-pattern->edge2XCc];
        else
            eval += value_b[-pattern->edge64c];
        
        if(filled & 0x8040000000004080ULL)	//A1 B2 B7 A8
            eval += value[-pattern->edge2XCd];
        else
            eval += value_b[-pattern->edge64d];
        
    }
    
    // arrondi(eval/8)/100.0f
    return std::round((eval>>3)/100.0f);

}


inline int RXBBPatterns::get_score(RXMove& move) const {
    
    const RXPattern* const p = move.pattern;
    
    const int stage = 61-board.n_empty;
    
    const unsigned long long filled = (board.discs[BLACK] | board.discs[WHITE] | move.square);
    
    int eval;
    const short* value;
    const short* value_b;
    
    if(board.player == WHITE) {
        
        value = RXEvaluation::DIAG_5[stage];
        eval  = value[p->diag_5a];
        eval += value[p->diag_5b];
        eval += value[p->diag_5c];
        eval += value[p->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        eval += value[p->diag_6a];
        eval += value[p->diag_6b];
        eval += value[p->diag_6c];
        eval += value[p->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        eval += value[p->diag_7a];
        eval += value[p->diag_7b];
        eval += value[p->diag_7c];
        eval += value[p->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        eval += value[p->diag_8a];
        eval += value[p->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        eval += value[p->hv_4a];
        eval += value[p->hv_4b];
        eval += value[p->hv_4c];
        eval += value[p->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        eval += value[p->hv_3a];
        eval += value[p->hv_3b];
        eval += value[p->hv_3c];
        eval += value[p->hv_3d];
        
        value = RXEvaluation::HV_2[stage];
        eval += value[p->hv_2a];
        eval += value[p->hv_2b];
        eval += value[p->hv_2c];
        eval += value[p->hv_2d];
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[p->corner11a];
        eval += value[p->corner11b];
        eval += value[p->corner11c];
        eval += value[p->corner11d];
        
        value = RXEvaluation::corner2x5[stage];
        eval += value[p->corner2x5a];
        eval += value[p->corner2x5b];
        eval += value[p->corner2x5c];
        eval += value[p->corner2x5d];
        eval += value[p->corner2x5e];
        eval += value[p->corner2x5f];
        eval += value[p->corner2x5g];
        eval += value[p->corner2x5h];
        
        value = RXEvaluation::EDGE_2XC[stage];
        value_b = RXEvaluation::EDGE_6_4[stage];
        if(filled & 0x8142000000000000ULL)	//A1 H1 B2 G2
            eval += value[p->edge2XCa];
        else
            eval += value_b[p->edge64a];
        
        if(filled & 0x0102000000000201ULL) //H1 G2 G7 H8
            eval += value[p->edge2XCb];
        else
            eval += value_b[p->edge64b];
        
        if(filled & 0x0000000000004281ULL)	//B7 G7 A8 H8
            eval += value[p->edge2XCc];
        else
            eval += value_b[p->edge64c];
        
        if(filled & 0x8040000000004080ULL)	//A1 B2 B7 A8
            eval += value[p->edge2XCd];
        else
            eval += value_b[p->edge64d];
        
    } else {
        
        value = RXEvaluation::DIAG_5[stage];
        eval  = value[-p->diag_5a];
        eval += value[-p->diag_5b];
        eval += value[-p->diag_5c];
        eval += value[-p->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        eval += value[-p->diag_6a];
        eval += value[-p->diag_6b];
        eval += value[-p->diag_6c];
        eval += value[-p->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        eval += value[-p->diag_7a];
        eval += value[-p->diag_7b];
        eval += value[-p->diag_7c];
        eval += value[-p->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        eval += value[-p->diag_8a];
        eval += value[-p->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        eval += value[-p->hv_4a];
        eval += value[-p->hv_4b];
        eval += value[-p->hv_4c];
        eval += value[-p->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        eval += value[-p->hv_3a];
        eval += value[-p->hv_3b];
        eval += value[-p->hv_3c];
        eval += value[-p->hv_3d];
        
        value = RXEvaluation::HV_2[stage];
        eval += value[-p->hv_2a];
        eval += value[-p->hv_2b];
        eval += value[-p->hv_2c];
        eval += value[-p->hv_2d];
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[-p->corner11a];
        eval += value[-p->corner11b];
        eval += value[-p->corner11c];
        eval += value[-p->corner11d];
        
        value = RXEvaluation::corner2x5[stage];
        eval += value[-p->corner2x5a];
        eval += value[-p->corner2x5b];
        eval += value[-p->corner2x5c];
        eval += value[-p->corner2x5d];
        eval += value[-p->corner2x5e];
        eval += value[-p->corner2x5f];
        eval += value[-p->corner2x5g];
        eval += value[-p->corner2x5h];
        
        value = RXEvaluation::EDGE_2XC[stage];
        value_b = RXEvaluation::EDGE_6_4[stage];
        if(filled & 0x8142000000000000ULL)	//A1 H1 B2 G2
            eval += value[-p->edge2XCa];
        else
            eval += value_b[-p->edge64a];
        
        if(filled & 0x0102000000000201ULL) //H1 G2 G7 H8
            eval += value[-p->edge2XCb];
        else
            eval += value_b[-p->edge64b];
        
        if(filled & 0x0000000000004281ULL)	//B7 G7 A8 H8
            eval += value[-p->edge2XCc];
        else
            eval += value_b[-p->edge64c];
        
        if(filled & 0x8040000000004080ULL)	//A1 B2 B7 A8
            eval += value[-p->edge2XCd];
        else
            eval += value_b[-p->edge64d];
        
    }
    
    // arrondi(eval/8)/100.0f
    return std::round((eval>>3)/100.0f);

}

#else



inline int RXBBPatterns::get_score() const noexcept {
    
    const int stage = 60-board.n_empty;
    const int color = 1 - 2*board.player;
    
    // Précharge des pointeurs locaux pour limiter les accès mémoire indirects
    const short* __restrict diag5  = RXEvaluation::DIAG_5[stage];
    const short* __restrict diag6  = RXEvaluation::DIAG_6[stage];
    const short* __restrict diag7  = RXEvaluation::DIAG_7[stage];
    const short* __restrict diag8  = RXEvaluation::DIAG_8[stage];
    const short* __restrict hv4    = RXEvaluation::HV_4[stage];
    const short* __restrict hv3    = RXEvaluation::HV_3[stage];
    const short* __restrict corner = RXEvaluation::CORNER_11[stage];
    const short* __restrict edge   = RXEvaluation::EDGE[stage];

    int
    eval  = diag5[color*pattern->diag_5a];
    eval += diag5[color*pattern->diag_5b];
    eval += diag5[color*pattern->diag_5c];
    eval += diag5[color*pattern->diag_5d];
    
    eval += diag6[color*pattern->diag_6a];
    eval += diag6[color*pattern->diag_6b];
    eval += diag6[color*pattern->diag_6c];
    eval += diag6[color*pattern->diag_6d];
    
    eval += diag7[color*pattern->diag_7a];
    eval += diag7[color*pattern->diag_7b];
    eval += diag7[color*pattern->diag_7c];
    eval += diag7[color*pattern->diag_7d];
    
    eval += diag8[color*pattern->diag_8a];
    eval += diag8[color*pattern->diag_8b];
    
    eval += hv4[color*pattern->hv_4a];
    eval += hv4[color*pattern->hv_4b];
    eval += hv4[color*pattern->hv_4c];
    eval += hv4[color*pattern->hv_4d];
    
    eval += hv3[color*pattern->hv_3a];
    eval += hv3[color*pattern->hv_3b];
    eval += hv3[color*pattern->hv_3c];
    eval += hv3[color*pattern->hv_3d];
    
    eval += corner[color*pattern->corner11a];
    eval += corner[color*pattern->corner11b];
    eval += corner[color*pattern->corner11c];
    eval += corner[color*pattern->corner11d];
    
    eval += edge[color*pattern->edge_1];
    eval += edge[color*pattern->edge_2];
    eval += edge[color*pattern->edge_3];
    eval += edge[color*pattern->edge_4];
        
    //return std::round((eval>>3)/100.0f);
    //trick
    return (eval + 400 - (eval>>31 & 800))/800;

}
 
 inline int RXBBPatterns::get_score(RXMove& move) const noexcept {
     
     const RXPattern* const pattern = move.pattern;
     
     const int stage = 61-board.n_empty;
     const int color = 2*board.player-1;

     // Précharge des pointeurs locaux pour limiter les accès mémoire indirects
     const short* __restrict diag5  = RXEvaluation::DIAG_5[stage];
     const short* __restrict diag6  = RXEvaluation::DIAG_6[stage];
     const short* __restrict diag7  = RXEvaluation::DIAG_7[stage];
     const short* __restrict diag8  = RXEvaluation::DIAG_8[stage];
     const short* __restrict hv4    = RXEvaluation::HV_4[stage];
     const short* __restrict hv3    = RXEvaluation::HV_3[stage];
     const short* __restrict corner = RXEvaluation::CORNER_11[stage];
     const short* __restrict edge   = RXEvaluation::EDGE[stage];

     int
     eval  = diag5[color*pattern->diag_5a];
     eval += diag5[color*pattern->diag_5b];
     eval += diag5[color*pattern->diag_5c];
     eval += diag5[color*pattern->diag_5d];
     
     eval += diag6[color*pattern->diag_6a];
     eval += diag6[color*pattern->diag_6b];
     eval += diag6[color*pattern->diag_6c];
     eval += diag6[color*pattern->diag_6d];
     
     eval += diag7[color*pattern->diag_7a];
     eval += diag7[color*pattern->diag_7b];
     eval += diag7[color*pattern->diag_7c];
     eval += diag7[color*pattern->diag_7d];
     
     eval += diag8[color*pattern->diag_8a];
     eval += diag8[color*pattern->diag_8b];
     
     eval += hv4[color*pattern->hv_4a];
     eval += hv4[color*pattern->hv_4b];
     eval += hv4[color*pattern->hv_4c];
     eval += hv4[color*pattern->hv_4d];
     
     eval += hv3[color*pattern->hv_3a];
     eval += hv3[color*pattern->hv_3b];
     eval += hv3[color*pattern->hv_3c];
     eval += hv3[color*pattern->hv_3d];
     
     eval += corner[color*pattern->corner11a];
     eval += corner[color*pattern->corner11b];
     eval += corner[color*pattern->corner11c];
     eval += corner[color*pattern->corner11d];
     
     eval += edge[color*pattern->edge_1];
     eval += edge[color*pattern->edge_2];
     eval += edge[color*pattern->edge_3];
     eval += edge[color*pattern->edge_4];
         
         
     //return std::round((eval>>3)/100.0f);
     //trick
     return (eval + 400 - (eval>>31 & 800))/800;

 }

/*
inline int RXBBPatterns::get_score() const noexcept {
    
    const int stage = 60 - board.n_empty;
    const int color = 1 - 2*board.player;
    const auto& p = *pattern;
    
    // Tableaux précentrés
    const short* __restrict diag5   = RXEvaluation::DIAG_5[stage];
    const short* __restrict diag6   = RXEvaluation::DIAG_6[stage];
    const short* __restrict diag7   = RXEvaluation::DIAG_7[stage];
    const short* __restrict diag8   = RXEvaluation::DIAG_8[stage];
    const short* __restrict hv4     = RXEvaluation::HV_4[stage];
    const short* __restrict hv3     = RXEvaluation::HV_3[stage];
    const short* __restrict corner  = RXEvaluation::CORNER_11[stage];
    const short* __restrict edge    = RXEvaluation::EDGE[stage];
    
    
    // ----- DIAG 7 & 8 -----
    int16x8_t values = vdupq_n_s16(0);
    values = vld1q_lane_s16(&diag7[color*p.diag_7a], values, 0);
    values = vld1q_lane_s16(&diag7[color*p.diag_7b], values, 1);
    values = vld1q_lane_s16(&diag7[color*p.diag_7c], values, 2);
    values = vld1q_lane_s16(&diag7[color*p.diag_7d], values, 3);
    values = vld1q_lane_s16(&diag8[color*p.diag_8a], values, 4);
    values = vld1q_lane_s16(&diag8[color*p.diag_8b], values, 5);
 
    int32x4_t acc = vpaddlq_s16(values);
 
    // ----- DIAG 5 & 6 -----
    values = vld1q_lane_s16(&diag5[color*p.diag_5a], values, 0);
    values = vld1q_lane_s16(&diag5[color*p.diag_5b], values, 1);
    values = vld1q_lane_s16(&diag5[color*p.diag_5c], values, 2);
    values = vld1q_lane_s16(&diag5[color*p.diag_5d], values, 3);
    values = vld1q_lane_s16(&diag6[color*p.diag_6a], values, 4);
    values = vld1q_lane_s16(&diag6[color*p.diag_6b], values, 5);
    values = vld1q_lane_s16(&diag6[color*p.diag_6c], values, 6);
    values = vld1q_lane_s16(&diag6[color*p.diag_6d], values, 7);

    acc = vaddq_s32(acc, vpaddlq_s16(values));
    
    // ----- HV4 & HV3 -----
    values = vld1q_lane_s16(&hv3[color*p.hv_3a], values, 0);
    values = vld1q_lane_s16(&hv3[color*p.hv_3b], values, 1);
    values = vld1q_lane_s16(&hv3[color*p.hv_3c], values, 2);
    values = vld1q_lane_s16(&hv3[color*p.hv_3d], values, 3);
    values = vld1q_lane_s16(&hv4[color*p.hv_4a], values, 4);
    values = vld1q_lane_s16(&hv4[color*p.hv_4b], values, 5);
    values = vld1q_lane_s16(&hv4[color*p.hv_4c], values, 6);
    values = vld1q_lane_s16(&hv4[color*p.hv_4d], values, 7);

    acc = vaddq_s32(acc, vpaddlq_s16(values));
    
    // ----- CORNER & EDGE -----
    values = vld1q_lane_s16(&corner[color*p.corner11a], values, 0);
    values = vld1q_lane_s16(&corner[color*p.corner11b], values, 1);
    values = vld1q_lane_s16(&corner[color*p.corner11c], values, 2);
    values = vld1q_lane_s16(&corner[color*p.corner11d], values, 3);
    values = vld1q_lane_s16(&edge[color*p.edge_1], values, 4);
    values = vld1q_lane_s16(&edge[color*p.edge_2], values, 5);
    values = vld1q_lane_s16(&edge[color*p.edge_3], values, 6);
    values = vld1q_lane_s16(&edge[color*p.edge_4], values, 7);

    acc = vaddq_s32(acc, vpaddlq_s16(values));

    
    int eval = vaddvq_s32(acc);
    
    // ----- Branchless rounding / normalization -----
    return (eval + 400 - ((eval >> 31) & 800)) / 800;
}

inline int RXBBPatterns::get_score(RXMove& move) const noexcept {
    
    const int stage = 61 - board.n_empty;
    const int color = 2*board.player-1;
    const auto& p = *move.pattern;
    
    // Tableaux précentrés
    const short* __restrict diag5   = RXEvaluation::DIAG_5[stage];
    const short* __restrict diag6   = RXEvaluation::DIAG_6[stage];
    const short* __restrict diag7   = RXEvaluation::DIAG_7[stage];
    const short* __restrict diag8   = RXEvaluation::DIAG_8[stage];
    const short* __restrict hv4     = RXEvaluation::HV_4[stage];
    const short* __restrict hv3     = RXEvaluation::HV_3[stage];
    const short* __restrict corner  = RXEvaluation::CORNER_11[stage];
    const short* __restrict edge    = RXEvaluation::EDGE[stage];
    
    
    // ----- DIAG 7 & 8 -----
    int16x8_t values = vdupq_n_s16(0);
    values = vld1q_lane_s16(&diag7[color*p.diag_7a], values, 0);
    values = vld1q_lane_s16(&diag7[color*p.diag_7b], values, 1);
    values = vld1q_lane_s16(&diag7[color*p.diag_7c], values, 2);
    values = vld1q_lane_s16(&diag7[color*p.diag_7d], values, 3);
    values = vld1q_lane_s16(&diag8[color*p.diag_8a], values, 4);
    values = vld1q_lane_s16(&diag8[color*p.diag_8b], values, 5);
 
    int32x4_t acc = vpaddlq_s16(values);
 
    // ----- DIAG 5 & 6 -----
    values = vld1q_lane_s16(&diag5[color*p.diag_5a], values, 0);
    values = vld1q_lane_s16(&diag5[color*p.diag_5b], values, 1);
    values = vld1q_lane_s16(&diag5[color*p.diag_5c], values, 2);
    values = vld1q_lane_s16(&diag5[color*p.diag_5d], values, 3);
    values = vld1q_lane_s16(&diag6[color*p.diag_6a], values, 4);
    values = vld1q_lane_s16(&diag6[color*p.diag_6b], values, 5);
    values = vld1q_lane_s16(&diag6[color*p.diag_6c], values, 6);
    values = vld1q_lane_s16(&diag6[color*p.diag_6d], values, 7);

    acc = vaddq_s32(acc, vpaddlq_s16(values));
    
    // ----- HV4 & HV3 -----
    values = vld1q_lane_s16(&hv3[color*p.hv_3a], values, 0);
    values = vld1q_lane_s16(&hv3[color*p.hv_3b], values, 1);
    values = vld1q_lane_s16(&hv3[color*p.hv_3c], values, 2);
    values = vld1q_lane_s16(&hv3[color*p.hv_3d], values, 3);
    values = vld1q_lane_s16(&hv4[color*p.hv_4a], values, 4);
    values = vld1q_lane_s16(&hv4[color*p.hv_4b], values, 5);
    values = vld1q_lane_s16(&hv4[color*p.hv_4c], values, 6);
    values = vld1q_lane_s16(&hv4[color*p.hv_4d], values, 7);

    acc = vaddq_s32(acc, vpaddlq_s16(values));
    
    // ----- CORNER & EDGE -----
    values = vld1q_lane_s16(&corner[color*p.corner11a], values, 0);
    values = vld1q_lane_s16(&corner[color*p.corner11b], values, 1);
    values = vld1q_lane_s16(&corner[color*p.corner11c], values, 2);
    values = vld1q_lane_s16(&corner[color*p.corner11d], values, 3);
    values = vld1q_lane_s16(&edge[color*p.edge_1], values, 4);
    values = vld1q_lane_s16(&edge[color*p.edge_2], values, 5);
    values = vld1q_lane_s16(&edge[color*p.edge_3], values, 6);
    values = vld1q_lane_s16(&edge[color*p.edge_4], values, 7);

    acc = vaddq_s32(acc, vpaddlq_s16(values));

    
    int eval = vaddvq_s32(acc);
    
    // ----- Branchless rounding / normalization -----
    return (eval + 400 - ((eval >> 31) & 800)) / 800;
}
 */
 

#endif


#endif
