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
    
    const int stage = 60-board.n_empty;
    const int color = 1 - 2*board.player;
    
    int eval;

    //diag 5
    const short* value = RXEvaluation::eval[stage][0];
    eval  = value[color*pattern->patt[0]];
    eval += value[color*pattern->patt[1]];
    eval += value[color*pattern->patt[2]];
    eval += value[color*pattern->patt[3]];
    
    //diag 6
    value = RXEvaluation::eval[stage][1];
    eval += value[color*pattern->patt[4]];
    eval += value[color*pattern->patt[5]];
    eval += value[color*pattern->patt[6]];
    eval += value[color*pattern->patt[7]];
    
    //diag 7
    value = RXEvaluation::eval[stage][2];
    eval += value[color*pattern->patt[8]];
    eval += value[color*pattern->patt[9]];
    eval += value[color*pattern->patt[10]];
    eval += value[color*pattern->patt[11]];

    //diag 8
    value = RXEvaluation::eval[stage][3];
    eval += value[color*pattern->patt[12]];
    eval += value[color*pattern->patt[13]];

    //edge+2X or edge 6+4
    value = RXEvaluation::eval[stage][4];
    const short* value_b = RXEvaluation::eval[stage][5];
    if(filled & 0x8142000000000000ULL)    //A1 H1 B2 G2
        eval += value[color*pattern->patt[14]];
    else
        eval += value_b[color*pattern->patt[18]];
    
    if(filled & 0x0102000000000201ULL) //H1 G2 G7 H8
        eval += value[color*pattern->patt[15]];
    else
        eval += value_b[color*pattern->patt[19]];
    
    if(filled & 0x0000000000004281ULL)    //B7 G7 A8 H8
        eval += value[color*pattern->patt[16]];
    else
        eval += value_b[color*pattern->patt[20]];
    
    if(filled & 0x8040000000004080ULL)    //A1 B2 B7 A8
        eval += value[color*pattern->patt[17]];
    else
        eval += value_b[color*pattern->patt[21]];

    //hv 2
    value = RXEvaluation::eval[stage][6];
    eval += value[color*pattern->patt[22]];
    eval += value[color*pattern->patt[23]];
    eval += value[color*pattern->patt[24]];
    eval += value[color*pattern->patt[25]];

    //hv 3
    value = RXEvaluation::eval[stage][7];
    eval += value[color*pattern->patt[26]];
    eval += value[color*pattern->patt[27]];
    eval += value[color*pattern->patt[28]];
    eval += value[color*pattern->patt[29]];

    //hv 4
    value = RXEvaluation::eval[stage][8];
    eval += value[color*pattern->patt[30]];
    eval += value[color*pattern->patt[31]];
    eval += value[color*pattern->patt[32]];
    eval += value[color*pattern->patt[33]];

    //2 bords5 + X
    value = RXEvaluation::eval[stage][9];
    value_b = RXEvaluation::eval[stage][10];
    if(filled & 0x8040000000000000ULL)    //A1 B2
        eval += value[color*pattern->patt[34]];
    else
        eval += value_b[color*pattern->patt[38]];
                        
    if(filled & 0x0102000000000000ULL) //H1 G2
        eval += value[color*pattern->patt[35]];
    else
        eval += value_b[color*pattern->patt[39]];
                        
    if(filled & 0x0000000000000201ULL)    //G7 H8
        eval += value[color*pattern->patt[36]];
    else
        eval += value_b[color*pattern->patt[40]];
                        
    if(filled & 0x0000000000004080ULL)    //B7 A8
        eval += value[color*pattern->patt[37]];
    else
        eval += value_b[color*pattern->patt[41]];


    //corner 2*5
    value = RXEvaluation::eval[stage][11];
    eval += value[color*pattern->patt[42]];
    eval += value[color*pattern->patt[43]];
    eval += value[color*pattern->patt[44]];
    eval += value[color*pattern->patt[45]];
    eval += value[color*pattern->patt[46]];
    eval += value[color*pattern->patt[47]];
    eval += value[color*pattern->patt[48]];
    eval += value[color*pattern->patt[49]];

    //hyper diag
    value = RXEvaluation::eval[stage][12];
    eval += value[color*pattern->patt[50]];
    eval += value[color*pattern->patt[51]];
    eval += value[color*pattern->patt[52]];
    eval += value[color*pattern->patt[53]];

    //corner 4/3/3/1 or corner ALT
    value = RXEvaluation::eval[stage][13];
    value_b = RXEvaluation::eval[stage][14];
    if(filled & 0x8040000000000000ULL)    //A1 B2
        eval += value[color*pattern->patt[54]];
    else
        eval += value_b[color*pattern->patt[58]];
                        
    if(filled & 0x0102000000000000ULL) //H1 G2
        eval += value[color*pattern->patt[55]];
    else
        eval += value_b[color*pattern->patt[59]];
                        
    if(filled & 0x0000000000000201ULL)    //G7 H8
        eval += value[color*pattern->patt[56]];
    else
        eval += value_b[color*pattern->patt[60]];
                        
    if(filled & 0x0000000000004080ULL)    //B7 A8
        eval += value[color*pattern->patt[57]];
    else
        eval += value_b[color*pattern->patt[61]];

    //edge 4/2/4
    value = RXEvaluation::eval[stage][15];
    eval += value[color*pattern->patt[62]];
    eval += value[color*pattern->patt[63]];
    eval += value[color*pattern->patt[64]];
    eval += value[color*pattern->patt[65]];

    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;

}

inline int RXBBPatterns::get_score(RXMove& move) const {
    
    unsigned long long filled = board.discs[BLACK] | board.discs[WHITE] | move.square;
    
    const RXPattern* const p = move.pattern;
    
    const int stage = 61-board.n_empty;
    const int color = 2*board.player-1;

    int eval;

    //diag 5
    const short* value = RXEvaluation::eval[stage][0];
    eval  = value[color*p->patt[0]];
    eval += value[color*p->patt[1]];
    eval += value[color*p->patt[2]];
    eval += value[color*p->patt[3]];
    
    //diag 6
    value = RXEvaluation::eval[stage][1];
    eval += value[color*p->patt[4]];
    eval += value[color*p->patt[5]];
    eval += value[color*p->patt[6]];
    eval += value[color*p->patt[7]];
    
    //diag 7
    value = RXEvaluation::eval[stage][2];
    eval += value[color*p->patt[8]];
    eval += value[color*p->patt[9]];
    eval += value[color*p->patt[10]];
    eval += value[color*p->patt[11]];

    //diag 8
    value = RXEvaluation::eval[stage][3];
    eval += value[color*p->patt[12]];
    eval += value[color*p->patt[13]];

    //edge+2X or edge 6+4
    value = RXEvaluation::eval[stage][4];
    const short* value_b = RXEvaluation::eval[stage][5];
    if(filled & 0x8142000000000000ULL)    //A1 H1 B2 G2
        eval += value[color*p->patt[14]];
    else
        eval += value_b[color*p->patt[18]];
    
    if(filled & 0x0102000000000201ULL) //H1 G2 G7 H8
        eval += value[color*p->patt[15]];
    else
        eval += value_b[color*p->patt[19]];
    
    if(filled & 0x0000000000004281ULL)    //B7 G7 A8 H8
        eval += value[color*p->patt[16]];
    else
        eval += value_b[color*p->patt[20]];
    
    if(filled & 0x8040000000004080ULL)    //A1 B2 B7 A8
        eval += value[color*p->patt[17]];
    else
        eval += value_b[color*p->patt[21]];

    //hv 2
    value = RXEvaluation::eval[stage][6];
    eval += value[color*p->patt[22]];
    eval += value[color*p->patt[23]];
    eval += value[color*p->patt[24]];
    eval += value[color*p->patt[25]];

    //hv 3
    value = RXEvaluation::eval[stage][7];
    eval += value[color*p->patt[26]];
    eval += value[color*p->patt[27]];
    eval += value[color*p->patt[28]];
    eval += value[color*p->patt[29]];

    //hv 4
    value = RXEvaluation::eval[stage][8];
    eval += value[color*p->patt[30]];
    eval += value[color*p->patt[31]];
    eval += value[color*p->patt[32]];
    eval += value[color*p->patt[33]];

    //2 bords5 + X
    value = RXEvaluation::eval[stage][9];
    value_b = RXEvaluation::eval[stage][10];
    if(filled & 0x8040000000000000ULL)    //A1 B2
        eval += value[color*p->patt[34]];
    else
        eval += value_b[color*p->patt[38]];
                        
    if(filled & 0x0102000000000000ULL) //H1 G2
        eval += value[color*p->patt[35]];
    else
        eval += value_b[color*p->patt[39]];
                        
    if(filled & 0x0000000000000201ULL)    //G7 H8
        eval += value[color*p->patt[36]];
    else
        eval += value_b[color*p->patt[40]];
                        
    if(filled & 0x0000000000004080ULL)    //B7 A8
        eval += value[color*p->patt[37]];
    else
        eval += value_b[color*p->patt[41]];


    //corner 2*5
    value = RXEvaluation::eval[stage][11];
    eval += value[color*p->patt[42]];
    eval += value[color*p->patt[43]];
    eval += value[color*p->patt[44]];
    eval += value[color*p->patt[45]];
    eval += value[color*p->patt[46]];
    eval += value[color*p->patt[47]];
    eval += value[color*p->patt[48]];
    eval += value[color*p->patt[49]];

    //hyper diag
    value = RXEvaluation::eval[stage][12];
    eval += value[color*p->patt[50]];
    eval += value[color*p->patt[51]];
    eval += value[color*p->patt[52]];
    eval += value[color*p->patt[53]];

    //corner 4/3/3/1 or corner ALT
    value = RXEvaluation::eval[stage][13];
    value_b = RXEvaluation::eval[stage][14];
    if(filled & 0x8040000000000000ULL)    //A1 B2
        eval += value[color*p->patt[54]];
    else
        eval += value_b[color*p->patt[58]];
                        
    if(filled & 0x0102000000000000ULL) //H1 G2
        eval += value[color*p->patt[55]];
    else
        eval += value_b[color*p->patt[59]];
                        
    if(filled & 0x0000000000000201ULL)    //G7 H8
        eval += value[color*p->patt[56]];
    else
        eval += value_b[color*p->patt[60]];
                        
    if(filled & 0x0000000000004080ULL)    //B7 A8
        eval += value[color*p->patt[57]];
    else
        eval += value_b[color*p->patt[61]];

    //edge 4/2/4
    value = RXEvaluation::eval[stage][15];
    eval += value[color*p->patt[62]];
    eval += value[color*p->patt[63]];
    eval += value[color*p->patt[64]];
    eval += value[color*p->patt[65]];

    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;
}



#endif
