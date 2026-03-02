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

__attribute__((always_inline))
inline RXBBPatterns& RXBBPatterns::operator=(const RXBBPatterns& src) {

    // Optimization: Skip self-assignment check to avoid branch misprediction.
    // Safe because 'pattern' pointers are stable (allocated at startup,
    // never reassigned or deleted during search).
//    if(this != &src) {
    
        board = src.board;
    
        *pattern = *(src.pattern); //copy

//    }
    
    return *this;
}


__attribute__((always_inline))
inline void RXBBPatterns::do_move(RXMove& move) {
    board.do_move(move);
    move.undo_pattern = pattern;
    pattern = move.pattern;
}

__attribute__((always_inline))
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
    
    const int* __restrict const p = pattern->patt;
    
    const int stage = 60-board.n_empty;
    const int color = 1 - 2*board.player;

    const auto& tab_eval = RXEvaluation::eval[stage];

    const short* __restrict const mob_P    = tab_eval[0];
    const short* __restrict const mob_O    = tab_eval[1];
    const short* __restrict const diag5    = tab_eval[2];
    const short* __restrict const diag6    = tab_eval[3];
    const short* __restrict const diag7    = tab_eval[4];
    const short* __restrict const diag8    = tab_eval[5];
    const short* __restrict const edge1    = tab_eval[6];
    const short* __restrict const edge2    = tab_eval[7];
    const short* __restrict const edge3    = tab_eval[8];
    const short* __restrict const hv2      = tab_eval[9];
    const short* __restrict const hv3      = tab_eval[10];
    const short* __restrict const hv4      = tab_eval[11];
    const short* __restrict const corner1  = tab_eval[12];
    const short* __restrict const corner2  = tab_eval[13];
    const short* __restrict const corner3  = tab_eval[14];
    const short* __restrict const corner4  = tab_eval[15];

    //mobility
    uint64x2_t mobilities = board.dual_count_legal_moves();
    int mob_player   = std::min(23, static_cast<int>(vgetq_lane_u64(mobilities, 0)));
    int mob_opponent = std::min(23, static_cast<int>(vgetq_lane_u64(mobilities, 1)));
    
    unsigned long long filled  = board.discs[board.player] | board.discs[board.player^1];
    
    uint64_t mask0 = filled & 0x8040000000000000ULL;
    uint64_t mask1 = filled & 0x0102000000000000ULL;
    uint64_t mask2 = filled & 0x0000000000000201ULL;
    uint64_t mask3 = filled & 0x0000000000004080ULL;

    const short* table0 = mask0 ? corner1 : corner2;
    const short* table1 = mask1 ? corner1 : corner2;
    const short* table2 = mask2 ? corner1 : corner2;
    const short* table3 = mask3 ? corner1 : corner2;

    const short* table4 = mask0 ? corner3 : corner4;
    const short* table5 = mask1 ? corner3 : corner4;
    const short* table6 = mask2 ? corner3 : corner4;
    const short* table7 = mask3 ? corner3 : corner4;

    int eval;
    
    //mobility player
    eval  = mob_P[mob_player];
    //mobility opponent
    eval += mob_O[mob_opponent];

    
    //diag 5
    eval += diag5[color*p[0]];
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

    //diag
    eval += diag8[color*p[12]];
    eval += diag8[color*p[13]];
    
    //edge + 2X
    eval += edge1[color*p[14]];
    eval += edge1[color*p[15]];
    eval += edge1[color*p[16]];
    eval += edge1[color*p[17]];

    //edge 2*(3/2)
    eval += edge2[color*p[18]];
    eval += edge2[color*p[19]];
    eval += edge2[color*p[20]];
    eval += edge2[color*p[21]];

    //edge 2*5
    eval += edge3[color*p[22]];
    eval += edge3[color*p[23]];
    eval += edge3[color*p[24]];
    eval += edge3[color*p[25]];
    eval += edge3[color*p[26]];
    eval += edge3[color*p[27]];
    eval += edge3[color*p[28]];
    eval += edge3[color*p[29]];
    
    //hv 2
    eval += hv2[color*p[30]];
    eval += hv2[color*p[31]];
    eval += hv2[color*p[32]];
    eval += hv2[color*p[33]];

    //hv 3
    eval += hv3[color*p[34]];
    eval += hv3[color*p[35]];
    eval += hv3[color*p[36]];
    eval += hv3[color*p[37]];

    //hv 4
    eval += hv4[color*p[38]];
    eval += hv4[color*p[39]];
    eval += hv4[color*p[40]];
    eval += hv4[color*p[41]];

    //corner
    eval += table0[color * p[mask0 ? 42 : 46]];
    eval += table1[color * p[mask1 ? 43 : 47]];
    eval += table2[color * p[mask2 ? 44 : 48]];
    eval += table3[color * p[mask3 ? 45 : 49]];
    
    eval += table4[color * p[mask0 ? 50 : 54]];
    eval += table5[color * p[mask1 ? 51 : 55]];
    eval += table6[color * p[mask2 ? 52 : 56]];
    eval += table7[color * p[mask3 ? 53 : 57]];


    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;

}

inline int RXBBPatterns::get_score(RXMove& move) const {
        
    const int* __restrict const p = move.pattern->patt;
    
    const int stage = 61-board.n_empty;
    const int color = 2*board.player-1;

    const auto& tab_eval = RXEvaluation::eval[stage];

    const short* __restrict const mob_P    = tab_eval[0];
    const short* __restrict const mob_O    = tab_eval[1];
    const short* __restrict const diag5    = tab_eval[2];
    const short* __restrict const diag6    = tab_eval[3];
    const short* __restrict const diag7    = tab_eval[4];
    const short* __restrict const diag8    = tab_eval[5];
    const short* __restrict const edge1    = tab_eval[6];
    const short* __restrict const edge2    = tab_eval[7];
    const short* __restrict const edge3    = tab_eval[8];
    const short* __restrict const hv2      = tab_eval[9];
    const short* __restrict const hv3      = tab_eval[10];
    const short* __restrict const hv4      = tab_eval[11];
    const short* __restrict const corner1  = tab_eval[12];
    const short* __restrict const corner2  = tab_eval[13];
    const short* __restrict const corner3  = tab_eval[14];
    const short* __restrict const corner4  = tab_eval[15];

    //mobility
    unsigned long long discs_player   = board.discs[board.player^1] ^ move.flipped;
    unsigned long long discs_opponent = (board.discs[board.player] ^ move.flipped) | move.square;
    
    uint64x2_t mobilities = RXBitBoard::dual_count_legal_moves(discs_player, discs_opponent);
    int mob_player   = std::min(23, static_cast<int>(vgetq_lane_u64(mobilities, 0)));
    int mob_opponent = std::min(23, static_cast<int>(vgetq_lane_u64(mobilities, 1)));
    
    unsigned long long filled  = discs_player | discs_opponent;
    
    uint64_t mask0 = filled & 0x8040000000000000ULL;
    uint64_t mask1 = filled & 0x0102000000000000ULL;
    uint64_t mask2 = filled & 0x0000000000000201ULL;
    uint64_t mask3 = filled & 0x0000000000004080ULL;

    const short* table0 = mask0 ? corner1 : corner2;
    const short* table1 = mask1 ? corner1 : corner2;
    const short* table2 = mask2 ? corner1 : corner2;
    const short* table3 = mask3 ? corner1 : corner2;

    const short* table4 = mask0 ? corner3 : corner4;
    const short* table5 = mask1 ? corner3 : corner4;
    const short* table6 = mask2 ? corner3 : corner4;
    const short* table7 = mask3 ? corner3 : corner4;

    int eval;
    
    //mobility player
    eval  = mob_P[mob_player];
    //mobility opponent
    eval += mob_O[mob_opponent];

    
    //diag 5
    eval += diag5[color*p[0]];
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

    //diag
    eval += diag8[color*p[12]];
    eval += diag8[color*p[13]];
    
    //edge + 2X
    eval += edge1[color*p[14]];
    eval += edge1[color*p[15]];
    eval += edge1[color*p[16]];
    eval += edge1[color*p[17]];

    //edge 2*(3/2)
    eval += edge2[color*p[18]];
    eval += edge2[color*p[19]];
    eval += edge2[color*p[20]];
    eval += edge2[color*p[21]];

    //edge 2*5
    eval += edge3[color*p[22]];
    eval += edge3[color*p[23]];
    eval += edge3[color*p[24]];
    eval += edge3[color*p[25]];
    eval += edge3[color*p[26]];
    eval += edge3[color*p[27]];
    eval += edge3[color*p[28]];
    eval += edge3[color*p[29]];
    
    //hv 2
    eval += hv2[color*p[30]];
    eval += hv2[color*p[31]];
    eval += hv2[color*p[32]];
    eval += hv2[color*p[33]];

    //hv 3
    eval += hv3[color*p[34]];
    eval += hv3[color*p[35]];
    eval += hv3[color*p[36]];
    eval += hv3[color*p[37]];

    //hv 4
    eval += hv4[color*p[38]];
    eval += hv4[color*p[39]];
    eval += hv4[color*p[40]];
    eval += hv4[color*p[41]];

    //corner
    eval += table0[color * p[mask0 ? 42 : 46]];
    eval += table1[color * p[mask1 ? 43 : 47]];
    eval += table2[color * p[mask2 ? 44 : 48]];
    eval += table3[color * p[mask3 ? 45 : 49]];
    
    eval += table4[color * p[mask0 ? 50 : 54]];
    eval += table5[color * p[mask1 ? 51 : 55]];
    eval += table6[color * p[mask2 ? 52 : 56]];
    eval += table7[color * p[mask3 ? 53 : 57]];

    if(eval>0) eval += 128; else eval -= 128;
    eval /= 256;
    
   return eval;
}

#endif
