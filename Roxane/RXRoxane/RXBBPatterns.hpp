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
    
    const unsigned long long filled = board.discs[BLACK] | board.discs[WHITE];
    
    const unsigned long long mask0 = filled & 0x8142000000000000ULL;
    const unsigned long long mask1 = filled & 0x0102000000000201ULL;
    const unsigned long long mask2 = filled & 0x0000000000004281ULL;
    const unsigned long long mask3 = filled & 0x8040000000004080ULL;

    const unsigned long long mask4 = filled & 0x8040000000000000ULL;
    const unsigned long long mask5 = filled & 0x0102000000000000ULL;
    const unsigned long long mask6 = filled & 0x0000000000000201ULL;
    const unsigned long long mask7 = filled & 0x0000000000004080ULL;

    const int* __restrict const p = pattern->patt;
    
    const int stage = 60-board.n_empty;
    const int color = 1 - 2*board.player;

    const auto& tab_eval = RXEvaluation::eval[stage];

    const short* __restrict const diag5    = tab_eval[0];
    const short* __restrict const diag6    = tab_eval[1];
    const short* __restrict const diag7    = tab_eval[2];
    const short* __restrict const diag8    = tab_eval[3];
    
    const short* __restrict const edge1    = tab_eval[4];
    const short* __restrict const edge2    = tab_eval[5];
    const short* __restrict const edge3    = tab_eval[6];
    const short* __restrict const edge4    = tab_eval[7];
    const short* __restrict const edge5    = tab_eval[8];

    const short* __restrict const hv2      = tab_eval[9];
    const short* __restrict const hv3      = tab_eval[10];
    const short* __restrict const hv4      = tab_eval[11];
    
    const short* __restrict const corner1  = tab_eval[12];
    const short* __restrict const corner2  = tab_eval[13];
    const short* __restrict const corner3  = tab_eval[14];
    const short* __restrict const corner4  = tab_eval[15];

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
    

    const short* __restrict const table0 = mask0 ? edge1 : edge2;
    const short* __restrict const table1 = mask1 ? edge1 : edge2;
    const short* __restrict const table2 = mask2 ? edge1 : edge2;
    const short* __restrict const table3 = mask3 ? edge1 : edge2;
    
    eval += table0[color * p[mask0 ? 14 : 18]];
    eval += table1[color * p[mask1 ? 15 : 19]];
    eval += table2[color * p[mask2 ? 16 : 20]];
    eval += table3[color * p[mask3 ? 17 : 21]];

    //edge 2*(3/2)
    eval += edge3[color*p[22]];
    eval += edge3[color*p[23]];
    eval += edge3[color*p[24]];
    eval += edge3[color*p[25]];

    //edge 1/4/1 + 4
    eval += edge4[color*p[26]];
    eval += edge4[color*p[27]];
    eval += edge4[color*p[28]];
    eval += edge4[color*p[29]];

    //edge 2*5
    eval += edge5[color*p[30]];
    eval += edge5[color*p[31]];
    eval += edge5[color*p[32]];
    eval += edge5[color*p[33]];
    eval += edge5[color*p[34]];
    eval += edge5[color*p[35]];
    eval += edge5[color*p[36]];
    eval += edge5[color*p[37]];

    //hv 2
    eval += hv2[color*p[38]];
    eval += hv2[color*p[39]];
    eval += hv2[color*p[40]];
    eval += hv2[color*p[41]];

    //hv 3
    eval += hv3[color*p[42]];
    eval += hv3[color*p[43]];
    eval += hv3[color*p[44]];
    eval += hv3[color*p[45]];

    //hv 4
    eval += hv4[color*p[46]];
    eval += hv4[color*p[47]];
    eval += hv4[color*p[48]];
    eval += hv4[color*p[49]];

    const short* __restrict const table4 = mask4 ? corner1 : corner2;
    const short* __restrict const table5 = mask5 ? corner1 : corner2;
    const short* __restrict const table6 = mask6 ? corner1 : corner2;
    const short* __restrict const table7 = mask7 ? corner1 : corner2;

    //corner 2 bords +X
    eval += table4[color*p[mask4 ? 50 : 54]];
    eval += table5[color*p[mask5 ? 51 : 55]];
    eval += table6[color*p[mask6 ? 52 : 56]];
    eval += table7[color*p[mask7 ? 53 : 57]];

    const short* __restrict const table8 = mask4 ? corner3 : corner4;
    const short* __restrict const table9 = mask5 ? corner3 : corner4;
    const short* __restrict const tableA = mask6 ? corner3 : corner4;
    const short* __restrict const tableB = mask7 ? corner3 : corner4;

    //corner 2 bords +X
    eval += table8[color*p[mask4 ? 58 : 62]];
    eval += table9[color*p[mask5 ? 59 : 63]];
    eval += tableA[color*p[mask6 ? 60 : 64]];
    eval += tableB[color*p[mask7 ? 61 : 65]];

    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;

}

inline int RXBBPatterns::get_score(RXMove& move) const {
    
    const unsigned long long filled = board.discs[BLACK] | board.discs[WHITE] | move.square;
    
    const unsigned long long mask0 = filled & 0x8142000000000000ULL;
    const unsigned long long mask1 = filled & 0x0102000000000201ULL;
    const unsigned long long mask2 = filled & 0x0000000000004281ULL;
    const unsigned long long mask3 = filled & 0x8040000000004080ULL;

    const unsigned long long mask4 = filled & 0x8040000000000000ULL;
    const unsigned long long mask5 = filled & 0x0102000000000000ULL;
    const unsigned long long mask6 = filled & 0x0000000000000201ULL;
    const unsigned long long mask7 = filled & 0x0000000000004080ULL;

    const int* __restrict const p = move.pattern->patt;
    
    const int stage = 61-board.n_empty;
    const int color = 2*board.player-1;

    const auto& tab_eval = RXEvaluation::eval[stage];

    const short* __restrict const diag5    = tab_eval[0];
    const short* __restrict const diag6    = tab_eval[1];
    const short* __restrict const diag7    = tab_eval[2];
    const short* __restrict const diag8    = tab_eval[3];
    
    const short* __restrict const edge1    = tab_eval[4];
    const short* __restrict const edge2    = tab_eval[5];
    const short* __restrict const edge3    = tab_eval[6];
    const short* __restrict const edge4    = tab_eval[7];
    const short* __restrict const edge5    = tab_eval[8];

    const short* __restrict const hv2      = tab_eval[9];
    const short* __restrict const hv3      = tab_eval[10];
    const short* __restrict const hv4      = tab_eval[11];
    
    const short* __restrict const corner1  = tab_eval[12];
    const short* __restrict const corner2  = tab_eval[13];
    const short* __restrict const corner3  = tab_eval[14];
    const short* __restrict const corner4  = tab_eval[15];

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
    

    const short* __restrict const table0 = mask0 ? edge1 : edge2;
    const short* __restrict const table1 = mask1 ? edge1 : edge2;
    const short* __restrict const table2 = mask2 ? edge1 : edge2;
    const short* __restrict const table3 = mask3 ? edge1 : edge2;
    
    eval += table0[color * p[mask0 ? 14 : 18]];
    eval += table1[color * p[mask1 ? 15 : 19]];
    eval += table2[color * p[mask2 ? 16 : 20]];
    eval += table3[color * p[mask3 ? 17 : 21]];

    //edge 2*(3/2)
    eval += edge3[color*p[22]];
    eval += edge3[color*p[23]];
    eval += edge3[color*p[24]];
    eval += edge3[color*p[25]];

    //edge 1/4/1 + 4
    eval += edge4[color*p[26]];
    eval += edge4[color*p[27]];
    eval += edge4[color*p[28]];
    eval += edge4[color*p[29]];

    //edge 2*5
    eval += edge5[color*p[30]];
    eval += edge5[color*p[31]];
    eval += edge5[color*p[32]];
    eval += edge5[color*p[33]];
    eval += edge5[color*p[34]];
    eval += edge5[color*p[35]];
    eval += edge5[color*p[36]];
    eval += edge5[color*p[37]];

    //hv 2
    eval += hv2[color*p[38]];
    eval += hv2[color*p[39]];
    eval += hv2[color*p[40]];
    eval += hv2[color*p[41]];

    //hv 3
    eval += hv3[color*p[42]];
    eval += hv3[color*p[43]];
    eval += hv3[color*p[44]];
    eval += hv3[color*p[45]];

    //hv 4
    eval += hv4[color*p[46]];
    eval += hv4[color*p[47]];
    eval += hv4[color*p[48]];
    eval += hv4[color*p[49]];

    const short* __restrict const table4 = mask4 ? corner1 : corner2;
    const short* __restrict const table5 = mask5 ? corner1 : corner2;
    const short* __restrict const table6 = mask6 ? corner1 : corner2;
    const short* __restrict const table7 = mask7 ? corner1 : corner2;

    //corner 2 bords +X
    eval += table4[color*p[mask4 ? 50 : 54]];
    eval += table5[color*p[mask5 ? 51 : 55]];
    eval += table6[color*p[mask6 ? 52 : 56]];
    eval += table7[color*p[mask7 ? 53 : 57]];

    const short* __restrict const table8 = mask4 ? corner3 : corner4;
    const short* __restrict const table9 = mask5 ? corner3 : corner4;
    const short* __restrict const tableA = mask6 ? corner3 : corner4;
    const short* __restrict const tableB = mask7 ? corner3 : corner4;

    //corner 2 bords +X
    eval += table8[color*p[mask4 ? 58 : 62]];
    eval += table9[color*p[mask5 ? 59 : 63]];
    eval += tableA[color*p[mask6 ? 60 : 64]];
    eval += tableB[color*p[mask7 ? 61 : 65]];
    
    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;

}



#endif
