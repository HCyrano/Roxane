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
    func(A4); func(B4); func(C4);                     func(F4); func(G4); func(H4);
    func(A5); func(B5); func(C5);                     func(F5); func(G5); func(H5);
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
    int get_score() const;
    int get_score(RXMove& move) const;
        
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

inline int RXBBPatterns::get_score() const {
    
    unsigned long long filled = board.discs[BLACK] | board.discs[WHITE];
        
    const int* __restrict const p = pattern->patt;
    
    const int stage = 60-board.n_empty;
    const int color = 1 - 2*board.player;

    const auto& tab_eval = RXEvaluation::eval[stage];

    const short* __restrict const diag5    = tab_eval[0];
    const short* __restrict const diag6    = tab_eval[1];
    const short* __restrict const diag7    = tab_eval[2];
    const short* __restrict const diag8    = tab_eval[3];
    const short* __restrict const hyp_dg   = tab_eval[4];
    const short* __restrict const edge1    = tab_eval[5];
    const short* __restrict const edge2    = tab_eval[6];
    const short* __restrict const edge3    = tab_eval[7];
    const short* __restrict const edge4    = tab_eval[8];
    const short* __restrict const hv2      = tab_eval[9];
    const short* __restrict const hv3      = tab_eval[10];
    const short* __restrict const hv4      = tab_eval[11];
    const short* __restrict const corner1  = tab_eval[12];
    const short* __restrict const corner2  = tab_eval[13];

    int eval;

    //diag 5
    eval  = diag5[color*p[0]];
    eval += diag5[color*p[1]];
    eval += diag5[color*p[2]];
    eval += diag5[color*p[3]];
    
    //diag 6
    eval += diag6[color*p[4]];
    eval += diag6[color*p[5]];
    eval += diag6[color*p[6]];
    eval += diag6[color*p[7]];
    
    //diag 7
    eval += diag7[color*p[8]];
    eval += diag7[color*p[9]];
    eval += diag7[color*p[10]];
    eval += diag7[color*p[11]];
    
    //diag 8
    eval += diag8[color*p[12]];
    eval += diag8[color*p[13]];
    
    //hyper diag
    eval += hyp_dg[color*p[14]];
    eval += hyp_dg[color*p[15]];
    eval += hyp_dg[color*p[16]];
    eval += hyp_dg[color*p[17]];

    //edge+2X or edde 6+4
    if(filled & 0x8142000000000000ULL)
        eval += edge1[color*p[18]];
    else
        eval += edge2[color*p[22]];

    if(filled & 0x0102000000000201ULL)
        eval += edge1[color*p[19]];
    else
        eval += edge2[color*p[23]];

    if(filled & 0x0000000000004281ULL)
        eval += edge1[color*p[20]];
    else
        eval += edge2[color*p[24]];

    if(filled & 0x8040000000004080ULL)
        eval += edge1[color*p[21]];
    else
        eval += edge2[color*p[25]];

    //edge 4/2/4
    eval += edge3[color*p[26]];
    eval += edge3[color*p[27]];
    eval += edge3[color*p[28]];
    eval += edge3[color*p[29]];

    //edge 2*(3/2)
    eval += edge4[color*p[30]];
    eval += edge4[color*p[31]];
    eval += edge4[color*p[32]];
    eval += edge4[color*p[33]];

    //hv 2
    eval += hv2[color*p[34]];
    eval += hv2[color*p[35]];
    eval += hv2[color*p[36]];
    eval += hv2[color*p[37]];
    
    //hv 3
    eval += hv3[color*p[38]];
    eval += hv3[color*p[39]];
    eval += hv3[color*p[40]];
    eval += hv3[color*p[41]];

    //hv 4
    eval += hv4[color*p[42]];
    eval += hv4[color*p[43]];
    eval += hv4[color*p[44]];
    eval += hv4[color*p[45]];

    //corner 4/3/3/1
    eval += corner1[color*p[46]];
    eval += corner1[color*p[47]];
    eval += corner1[color*p[48]];
    eval += corner1[color*p[49]];

    //corner 2*5
    eval += corner2[color*p[50]];
    eval += corner2[color*p[51]];
    eval += corner2[color*p[52]];
    eval += corner2[color*p[53]];
    eval += corner2[color*p[54]];
    eval += corner2[color*p[55]];
    eval += corner2[color*p[56]];
    eval += corner2[color*p[57]];

    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;

}

inline int RXBBPatterns::get_score(RXMove& move) const {
    
    unsigned long long filled = board.discs[BLACK] | board.discs[WHITE] | move.square;

    const int* __restrict const p = move.pattern->patt;
    
    const int stage = 61-board.n_empty;
    const int color = 2*board.player-1;

    const auto& tab_eval = RXEvaluation::eval[stage];

    const short* __restrict const diag5    = tab_eval[0];
    const short* __restrict const diag6    = tab_eval[1];
    const short* __restrict const diag7    = tab_eval[2];
    const short* __restrict const diag8    = tab_eval[3];
    const short* __restrict const hyp_dg   = tab_eval[4];
    const short* __restrict const edge1    = tab_eval[5];
    const short* __restrict const edge2    = tab_eval[6];
    const short* __restrict const edge3    = tab_eval[7];
    const short* __restrict const edge4    = tab_eval[8];
    const short* __restrict const hv2      = tab_eval[9];
    const short* __restrict const hv3      = tab_eval[10];
    const short* __restrict const hv4      = tab_eval[11];
    const short* __restrict const corner1  = tab_eval[12];
    const short* __restrict const corner2  = tab_eval[13];

    int eval;

    //diag 5
    eval  = diag5[color*p[0]];
    eval += diag5[color*p[1]];
    eval += diag5[color*p[2]];
    eval += diag5[color*p[3]];
    
    //diag 6
    eval += diag6[color*p[4]];
    eval += diag6[color*p[5]];
    eval += diag6[color*p[6]];
    eval += diag6[color*p[7]];
    
    //diag 7
    eval += diag7[color*p[8]];
    eval += diag7[color*p[9]];
    eval += diag7[color*p[10]];
    eval += diag7[color*p[11]];
    
    //diag 8
    eval += diag8[color*p[12]];
    eval += diag8[color*p[13]];
    
    //hyper diag
    eval += hyp_dg[color*p[14]];
    eval += hyp_dg[color*p[15]];
    eval += hyp_dg[color*p[16]];
    eval += hyp_dg[color*p[17]];

    //edge+2X or edde 6+4
    if(filled & 0x8142000000000000ULL)
        eval += edge1[color*p[18]];
    else
        eval += edge2[color*p[22]];

    if(filled & 0x0102000000000201ULL)
        eval += edge1[color*p[19]];
    else
        eval += edge2[color*p[23]];

    if(filled & 0x0000000000004281ULL)
        eval += edge1[color*p[20]];
    else
        eval += edge2[color*p[24]];

    if(filled & 0x8040000000004080ULL)
        eval += edge1[color*p[21]];
    else
        eval += edge2[color*p[25]];

    //edge 4/2/4
    eval += edge3[color*p[26]];
    eval += edge3[color*p[27]];
    eval += edge3[color*p[28]];
    eval += edge3[color*p[29]];

    //edge 2*(3/2)
    eval += edge4[color*p[30]];
    eval += edge4[color*p[31]];
    eval += edge4[color*p[32]];
    eval += edge4[color*p[33]];

    //hv 2
    eval += hv2[color*p[34]];
    eval += hv2[color*p[35]];
    eval += hv2[color*p[36]];
    eval += hv2[color*p[37]];
    
    //hv 3
    eval += hv3[color*p[38]];
    eval += hv3[color*p[39]];
    eval += hv3[color*p[40]];
    eval += hv3[color*p[41]];

    //hv 4
    eval += hv4[color*p[42]];
    eval += hv4[color*p[43]];
    eval += hv4[color*p[44]];
    eval += hv4[color*p[45]];

    //corner 4/3/3/1
    eval += corner1[color*p[46]];
    eval += corner1[color*p[47]];
    eval += corner1[color*p[48]];
    eval += corner1[color*p[49]];

    //corner 2*5
    eval += corner2[color*p[50]];
    eval += corner2[color*p[51]];
    eval += corner2[color*p[52]];
    eval += corner2[color*p[53]];
    eval += corner2[color*p[54]];
    eval += corner2[color*p[55]];
    eval += corner2[color*p[56]];
    eval += corner2[color*p[57]];

    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;

}



#endif
