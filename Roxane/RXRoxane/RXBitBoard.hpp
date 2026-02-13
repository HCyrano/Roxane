/*
 *  RXBitBoard.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 27/06/05.
 *  many ideas from Toshihiko Okuhara [http://www.amy.hi-ho.ne.jp/okuhara/edaxopt.htm]
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXBITBOARD_HPP
#define RXBITBOARD_HPP

#include <iostream>
#include <string>
#include <vector>

#include "RXConstantes.hpp"
#include "RXMove.hpp"
#include "RXTools.hpp"
#include "RXSetting.hpp"

#ifdef __ARM_NEON
#include "arm_neon.h"
#endif

class RXSquareList {
    
    public :
    
    int position;
    RXSquareList *previous;
    RXSquareList *next;

    RXSquareList(): position(NOMOVE), previous(NULL), next(NULL) {};

};

class RXBitBoard {
        
    private :
    static const unsigned long long hashSquare[64][2];
    
    static unsigned long long hashcodeTable_lines1_2[2][65536];
    static unsigned long long hashcodeTable_lines3_4[2][65536];
    static unsigned long long hashcodeTable_lines5_6[2][65536];
    static unsigned long long hashcodeTable_lines7_8[2][65536];
    
    static unsigned char EDGE_STABILITY[256*256]; //unsigned char
    static int find_edge_stable(const int old_P, const int old_O, int stable);
    
    static void init_hashcodeTable();
    static void edge_stability_init();

    // move functions
#define func(pos) static unsigned long long do_flips_##pos(const unsigned long long& discs_player, const unsigned long long& discs_opponent); \
void generate_flips_##pos(RXMove& move) const \

    func(A1); func(B1); func(C1); func(D1); func(E1); func(F1); func(G1); func(H1);
    func(A2); func(B2); func(C2); func(D2); func(E2); func(F2); func(G2); func(H2);
    func(A3); func(B3); func(C3); func(D3); func(E3); func(F3); func(G3); func(H3);
    func(A4); func(B4); func(C4);                      func(F4); func(G4); func(H4);
    func(A5); func(B5); func(C5);                      func(F5); func(G5); func(H5);
    func(A6); func(B6); func(C6); func(D6); func(E6); func(F6); func(G6); func(H6);
    func(A7); func(B7); func(C7); func(D7); func(E7); func(F7); func(G7); func(H7);
    func(A8); func(B8); func(C8); func(D8); func(E8); func(F8); func(G8); func(H8);
    
#undef func
        
#ifdef __ARM_NEON
    static const unsigned char OUTFLANK_3[];
    static const unsigned char OUTFLANK_4[];
    static const unsigned long long FLIPPED_3_H[];
    static const unsigned long long FLIPPED_4_H[];
    
    static const unsigned char COUNT_FLIP[8][256];
    static const uint64x2_t mask_dvhd[64][2];
    
    
#else
    
    static const unsigned char COUNT_A[];
    static const unsigned char COUNT_B[];
    static const unsigned char COUNT_C[];
    static const unsigned char COUNT_D[];
    static const unsigned char COUNT_E[];
    static const unsigned char COUNT_F[];
    static const unsigned char COUNT_G[];
    static const unsigned char COUNT_H[];
    
#define func(pos) static int count_flips_##pos(const unsigned long long& discs_player)\

    func(A1); func(B1); func(C1); func(D1); func(E1); func(F1); func(G1); func(H1);
    func(A2); func(B2); func(C2); func(D2); func(E2); func(F2); func(G2); func(H2);
    func(A3); func(B3); func(C3); func(D3); func(E3); func(F3); func(G3); func(H3);
    func(A4); func(B4); func(C4);                     func(F4); func(G4); func(H4);
    func(A5); func(B5); func(C5);                     func(F5); func(G5); func(H5);
    func(A6); func(B6); func(C6); func(D6); func(E6); func(F6); func(G6); func(H6);
    func(A7); func(B7); func(C7); func(D7); func(E7); func(F7); func(G7); func(H7);
    func(A8); func(B8); func(C8); func(D8); func(E8); func(F8); func(G8); func(H8);
    
#undef func
    
    typedef int (*type_count_flips)(const unsigned long long& discs_player);


    // presorted squares
    static const unsigned long long PRESORTED_POSITION_BITS[];


    
#endif


    public :
    
    static void static_init();

    
    //neighborhood for each square
    static const unsigned long long NEIGHBOR[];
    
    static const int PRESORTED_POSITION[];

    
    /*! a quadrant id for each square */
    static const unsigned long long QUADRANT_MASK[];
    static const int QUADRANT_SHITF[];
    static const int QUADRANT_ID[];
    
    
    //structure board
    unsigned long long discs[2];
    int player;
    int n_empty;
    int parity;
    RXSquareList empties_list[62];
    RXSquareList *position_to_empties[64];
    mutable unsigned long long n_nodes;
    
    //public :
    
    void reset();
    
    unsigned long long get_n_nodes() const {
        return n_nodes;
    }
    
    RXBitBoard();
    
    //constructeur par copie
    RXBitBoard(const RXBitBoard& src);
    
    RXBitBoard& operator=(const RXBitBoard& src);
    
    void build(const std::string& init);
    
    friend std::ostream& operator<<(std::ostream& os, RXBitBoard& board);
    
    bool squareIsEmpty(const int position) const;
    bool isPassed();
    bool isEndGame();
        
    typedef unsigned long long  (*type_do_flips)(const unsigned long long& discs_player, const unsigned long long& discs_opponent);
    static type_do_flips const do_flips[];
    void (RXBitBoard::*generate_flips[64])(RXMove& move) const;
    void init_generate_flips();
    
    
#ifdef __ARM_NEON
    
    int count_flips(int pos, unsigned long long P) const;
    
#else
    
    static type_count_flips const count_flips[];
    
#endif
    
    
    /* bool check_move(const int position, const int color) const; */

    unsigned int n_moves() const;
    void moves_producing(RXMove* start) const;
    
    //    static uint64_t calc_legal(const uint64_t P, const uint64_t O);
    inline unsigned long long get_legal_moves() const;
    static unsigned long long get_legal_moves(const unsigned long long discs_player, const unsigned long long discs_opponent);
    static inline uint64x2_t dual_legal_moves(const unsigned long long p, const unsigned long long o);
    inline uint64x2_t dual_count_legal_moves() const;
    static inline uint64x2_t dual_count_legal_moves(const unsigned long long p, const unsigned long long o);
    
    //inline bool is_quiet();

    
    bool isValid_square(const unsigned int pos) const;
    static bool dir_valid_shl(unsigned long long square, unsigned long long p_discs, unsigned long long o_discs, int shift, unsigned long long mask);
    static bool dir_valid_shr(unsigned long long square, unsigned long long p_discs, unsigned long long o_discs, int shift, unsigned long long mask);

    
    static int count_potential_moves(const unsigned long long p_discs, const unsigned long long o_discs);
    static void dual_potential_mobility(const unsigned long long p_discs, const unsigned long long o_discs, int &p_pmob, int &o_pmob);

    
    
    static inline int get_mobility(const unsigned long long discs_player, const unsigned long long discs_opponent);
    static inline int get_corner_stability(const unsigned long long& discs_player);
    inline int get_edge_stability(const int player) const;
    static unsigned long long get_stable_edge(const unsigned long long  discs_player, const unsigned long long discs_opponent);
    inline int get_stability(const int player) const;
    static inline unsigned int count_stable_edge(const unsigned long long discs_player, const unsigned long long discs_opponent);
    static inline int get_stability(const unsigned long long discs_player, const unsigned long long discs_opponent);
    
    
    void do_move(const RXMove& move);
    void undo_move(const RXMove& move);
    void do_pass();
    
    
    int final_score() const;
    int final_score_1() const;
    int final_score_2(int alpha, const int beta) const;
    int final_score_2(const unsigned long long discs_player, const unsigned long long discs_opponent, const int alpha, const int beta, const int idSquare1, const int idSquare2) const;
    int final_score_3(int alpha, const int beta) const ;
    int final_score_3(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, int beta, const unsigned int shuf3, const unsigned int empties3) const;
    int	final_score_4(int alpha, int beta, const bool passed) const;
    int	final_score_4(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, const int beta, const bool passed, const unsigned int shuf4, const unsigned int empties4) const;
    
    std::string cassio_script() const;
    
    /* DEBUG */
    void print_empties_list() const;
    //void check_empties_list() const;
    static void print_64bits(unsigned long long n);
    void print_Board();
    std::string string_rawdata();
    static void print_Board(unsigned long long P, unsigned long long O);
    void print_moves_list(RXMove* MovesList) const;
    
    unsigned long long hashcode() const ;
    unsigned long long hashcode_after_move(RXMove* move)  const;
    
    /* test */
    //static unsigned long long cntbset(unsigned long long n);
    //void build(const unsigned long long disc_player, const unsigned long long disc_opp, const int color);
    
    
};

//Clang on Apple Silicon will compile this into a SUBS instruction followed by a CSEL (Conditional Select).
//This is the 'Holy Grail' of ARM optimization: 2 cycles, 0 branches.
//Set all bits below the sole outflank bit if outfrank != 0
static inline unsigned long long OutflankToFlipmask(unsigned long long outflank) {
    return outflank ? (outflank - 1) : 0;
//    return -(long long)outflank >> 63 & (outflank - 1);
}

// Strictly, (long long) >> 64 is undefined in C, but either 0 bit (no change)
// or 64 bit (zero out) shift will lead valid result (i.e. flipped == 0).
#define    outflank_right(O,maskr)    (0x8000000000000000ULL >> __builtin_clzll(~(O) & (maskr)))

// in case continuous from MSB
#define    outflank_right_H(O)    (0x80000000u >> __builtin_clz(~(O)))


#define not_O_in_mask(mask,O)    vbicq_u64((mask), vdupq_n_u64(O))

//rotl8
#define rotl8(x,y)    __builtin_rotateleft8((x),(y))

#define    unpackA2A7(x)    ((((x) & 0x7e) * 0x0000040810204080) & 0x0001010101010100)
#define    unpackH2H7(x)    ((((x) & 0x7e) * 0x0002040810204000) & 0x0080808080808000)
#define    packA1A8(X)      ((((X) & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56)
#define    packH1H8(X)      ((((X) & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56)



inline void RXBitBoard::moves_producing(RXMove* start) const {
    
    RXMove *list = start + 1, *previous = start;
    
    const unsigned long long legal_movesBB = get_legal_moves(discs[player], discs[player^1]);
    
    for(RXSquareList* empties = empties_list->next; empties->position != NOMOVE; empties = empties->next)
        if(legal_movesBB & 0x1ULL<<empties->position) {
            ((this)->*(generate_flips[empties->position]))(*list);
            list->score = 0;
            previous = previous->next = list++;
        }
    
    previous->next = NULL;
}



inline void RXBitBoard::do_move(const RXMove& move) {
    
    discs[player] |= (move.flipped | move.square);
    player ^=1;
    discs[player] ^= move.flipped;
    
    --n_empty;
    parity ^= QUADRANT_ID[move.position];
    
    const RXSquareList *remove = position_to_empties[move.position];
    remove->previous->next = remove->next;
    remove->next->previous = remove->previous;
    
    ++n_nodes;
}

inline void RXBitBoard::undo_move(const RXMove& move) {
    
    RXSquareList *insert = position_to_empties[move.position];
    insert->previous->next = insert;
    insert->next->previous = insert;
    
    parity ^= QUADRANT_ID[move.position];
    ++n_empty;
    
    discs[player] |= move.flipped;
    player ^=1;
    discs[player] ^= (move.flipped | move.square);
    
}

inline void RXBitBoard::do_pass() {
    player ^= 1;
}

inline unsigned long long RXBitBoard::get_legal_moves() const {
    return get_legal_moves(discs[player], discs[player^1]);
}


/*
 @brief count all legal moves with twice for the corners
 
 @param P                    a bitboard representing player
 @param O                    a bitboard representing opponent
 @return count all legal moves
 */

inline int RXBitBoard::get_mobility(const unsigned long long p_discs, const unsigned long long o_discs) {
    
    const unsigned long long legals = get_legal_moves(p_discs, o_discs);
    
    //    unsigned long long bonus = ((legals & 0x8000000000000000ULL) >> 27)
    //                             | ((legals & 0x0100000000000000ULL) >> 21)
    //                             | ((legals & 0x80ULL) << 21)
    //                             | ((legals & 0x01ULL) << 27);
    //    return __builtin_popcountll(legals | bonus);
    
    return __builtin_popcountll(legals)  +  __builtin_popcountll(legals & 0x8100000000000081ULL);
}





/// retourne un pseudo (sous evalué) score de pions stables
/// la 1ere partie determine les lignes (dans les 4 directions) pleines
/// si un pions est dans les 4 lignes et appartient a la color, il est stable
/// la deuxieme partie trouve les pions stables adgacents au pions stables precedents (dans les 4 directions)
/// - Parameters:
///   - player: joueur
inline int RXBitBoard::get_stability(const int player) const {
    return RXBitBoard::get_stability(discs[player], discs[player^1]);
}

inline int RXBitBoard::get_edge_stability(const int player) const {
    return __builtin_popcountll(RXBitBoard::get_stable_edge(discs[player], discs[player^1]));
}

/**
 * @brief Get stable edge.
 *
 * This function uses precomputed exact stable edge table to accelerate
 * the computation.
 *
 * @param P bitboard with player's discs.
 * @param O bitboard with opponent's discs.
 * @return a bitboard with (some of) player's stable discs.
 *
 */
inline unsigned long long RXBitBoard::get_stable_edge(const unsigned long long P, const unsigned long long O) {
    
    // compute the exact stable edges (from precomputed tables)
    return EDGE_STABILITY[(P & 0xff) * 256 + (O & 0xff)]
    |  ((uint64_t)EDGE_STABILITY[(P >> 56) * 256 + (O >> 56)]) << 56
    |  unpackA2A7(EDGE_STABILITY[packA1A8(P) * 256 + packA1A8(O)])
    |  unpackH2H7(EDGE_STABILITY[packH1H8(P) * 256 + packH1H8(O)]);
    
    
}



inline int RXBitBoard::get_corner_stability(const unsigned long long& discs_player) {
    
    unsigned long long stables = discs_player & 0x8100000000000081ULL;
    
    stables |= (discs_player & (stables << 1)) & 0x0200000000000002ULL;
    stables |= (discs_player & (stables >> 1)) & 0x4000000000000040ULL;
    stables |= (discs_player & (stables << 8)) & 0x0000000000008100ULL;
    stables |= (discs_player & (stables >> 8)) & 0x0081000000000000ULL;
    
    return __builtin_popcountll(stables);
    
}


inline int RXBitBoard::final_score_2(int alpha, const int beta) const {
    --n_nodes; // removes the duplicate
    return final_score_2(discs[player], discs[player^1], alpha, beta,  empties_list->next->position,  empties_list->next->next->position);
}

#ifdef __ARM_NEON

/*
inline bool RXBitBoard::is_quiet() {
    
    const unsigned long long p_discs = discs[player];
    const unsigned long long o_discs = discs[player^1];

    uint64x2_t legals = RXBitBoard::dual_legal_moves(p_discs, o_discs);
    unsigned long long  p_mob  = vgetq_lane_u64(legals, 0);
    unsigned long long  o_mob  = vgetq_lane_u64(legals, 1);
    
    // possibility corner
    if((p_mob | o_mob) & 0x8100000000000081ULL)
        return false;
    
    int mob_p = __builtin_popcountll(p_mob);
    int mob_o = __builtin_popcountll(o_mob);

    if(std::abs(mob_p - mob_o) > 6)
        return false;
    
    int p_pot, o_pot;
    RXBitBoard::dual_potential_mobility(p_discs, o_discs, p_pot, o_pot);
    
    if(std::abs(p_pot - o_pot) > 12)
        return false;
        
    return true;
    
}
*/

inline unsigned int RXBitBoard::count_stable_edge(const unsigned long long P, const unsigned long long O) {
    return __builtin_popcountll(RXBitBoard::get_stable_edge(P, O));
}




inline int RXBitBoard::get_stability(const unsigned long long discs_player, const unsigned long long discs_opponent) {
    
    const unsigned long long filled = discs_player | discs_opponent;
    const unsigned long long central_mask = discs_player & 0x007e7e7e7e7e7e00ULL;
    
    unsigned long long stable = get_stable_edge(discs_player, discs_opponent);
    
    uint8x8_t h8;
    uint64x2_t l79, r79;
    const uint64x2_t e790 = vdupq_n_u64(0x007f7f7f7f7f7f7f);
    const uint64x2_t e791 = vdupq_n_u64(0xfefefefefefefe00);
    const uint64x2_t e792 = vdupq_n_u64(0x00003f3f3f3f3f3f);
    const uint64x2_t e793 = vdupq_n_u64(0x0f0f0f0ff0f0f0f0);
    
    
    uint64_t h, v, d7, d9;
    
    h8 = vcreate_u8(filled);                l79 = r79 = vreinterpretq_u64_u8(vcombine_u8(h8, vrev64_u8(h8)));
    h8 = vceq_u8(h8, vdup_n_u8(0xff));      l79 = vandq_u64(l79, vornq_u64(vshrq_n_u64(l79, 9), e790));
    h = vget_lane_u64(vreinterpret_u64_u8(h8), 0);
    r79 = vandq_u64(r79, vornq_u64(vshlq_n_u64(r79, 9), e791));
    v = filled;                             l79 = vbicq_u64(l79, vbicq_u64(e792, vshrq_n_u64(l79, 18)));
    v &= (v >> 8) | (v << 56);              r79 = vbicq_u64(r79, vshlq_n_u64(vbicq_u64(e792, r79), 18));
    v &= (v >> 16) | (v << 48);             l79 = vandq_u64(vandq_u64(l79, r79), vorrq_u64(e793, vsliq_n_u64(vshrq_n_u64(l79, 36), r79, 36)));
    v &= (v >> 32) | (v << 32);             d7 = __builtin_bswap64(vgetq_lane_u64(l79, 1));
    d9 = vgetq_lane_u64(l79, 0);
    
    stable |= (h & v & d7 & d9 & central_mask);
    
    unsigned long long old_stable = 0;
    uint64_t stable_h, stable_v, stable_d7, stable_d9;
    
    while (stable != old_stable) {
        old_stable = stable;
        stable_h  = ((stable >> 1) | (stable << 1) | h);
        stable_v  = ((stable >> 8) | (stable << 8) | v);
        stable_d7 = ((stable >> 7) | (stable << 7) | d7);
        stable_d9 = ((stable >> 9) | (stable << 9) | d9);
        stable |= (stable_h & stable_v & stable_d7 & stable_d9 & central_mask);
    }
    
    return __builtin_popcountll(stable);
    
}


inline unsigned long long RXBitBoard::hashcode() const {
    
    const uint16x4_t p_lines = vcreate_u16(discs[player]);
    const uint16x4_t o_lines = vcreate_u16(discs[player^1]);
    
    unsigned long long
    hashcode  = hashcodeTable_lines1_2[PLAYER][vget_lane_u16(p_lines,3)];
    hashcode ^= hashcodeTable_lines3_4[PLAYER][vget_lane_u16(p_lines,2)];
    hashcode ^= hashcodeTable_lines5_6[PLAYER][vget_lane_u16(p_lines,1)];
    hashcode ^= hashcodeTable_lines7_8[PLAYER][vget_lane_u16(p_lines,0)];
    
    hashcode ^= hashcodeTable_lines1_2[OPPONENT][vget_lane_u16(o_lines,3)];
    hashcode ^= hashcodeTable_lines3_4[OPPONENT][vget_lane_u16(o_lines,2)];
    hashcode ^= hashcodeTable_lines5_6[OPPONENT][vget_lane_u16(o_lines,1)];
    hashcode ^= hashcodeTable_lines7_8[OPPONENT][vget_lane_u16(o_lines,0)];
    
    return hashcode;
    
}

inline unsigned long long RXBitBoard::hashcode_after_move(RXMove* move) const {
    
    const uint16x4_t p_lines = vcreate_u16(discs[player^1] ^ move->flipped);
    const uint16x4_t o_lines = vcreate_u16(discs[player] | (move->flipped | move->square));
    
    unsigned long long
    hashcode  = hashcodeTable_lines1_2[PLAYER][vget_lane_u16(p_lines,3)];
    hashcode ^= hashcodeTable_lines3_4[PLAYER][vget_lane_u16(p_lines,2)];
    hashcode ^= hashcodeTable_lines5_6[PLAYER][vget_lane_u16(p_lines,1)];
    hashcode ^= hashcodeTable_lines7_8[PLAYER][vget_lane_u16(p_lines,0)];
    
    hashcode ^= hashcodeTable_lines1_2[OPPONENT][vget_lane_u16(o_lines,3)];
    hashcode ^= hashcodeTable_lines3_4[OPPONENT][vget_lane_u16(o_lines,2)];
    hashcode ^= hashcodeTable_lines5_6[OPPONENT][vget_lane_u16(o_lines,1)];
    hashcode ^= hashcodeTable_lines7_8[OPPONENT][vget_lane_u16(o_lines,0)];
    
    return hashcode;
    
}

////3 directions NEON + 1 direction CPU
//inline unsigned long long RXBitBoard::get_legal_moves(const unsigned long long P, const unsigned long long O)
//{
//    unsigned int  mO, movesL, movesH, flip1, pre1;
//    uint64x1_t    rP, rO;
//    uint64x2_t    PP, OO, MM, flip, pre;
//
//    /*vertical_mirror in PP[1], OO[1]*/                                  mO = (unsigned int) O & 0x7e7e7e7e;
//    rP = vreinterpret_u64_u8(vrev64_u8(vcreate_u8(P)));                  flip1  = mO & ((unsigned int) P << 1);
//    PP = vcombine_u64(vcreate_u64(P), rP);                               flip1 |= mO & (flip1 << 1);
//                                                                         pre1   = mO & (mO << 1);
//    rO = vreinterpret_u64_u8(vrev64_u8(vcreate_u8(O)));                  flip1 |= pre1 & (flip1 << 2);
//    OO = vcombine_u64(vcreate_u64(O), rO);                               flip1 |= pre1 & (flip1 << 2);
//                                                                         movesL = flip1 << 1;
//
//    flip = vandq_u64(OO, vshlq_n_u64(PP, 8));                            flip1  = mO & ((unsigned int) P >> 1);
//    flip = vorrq_u64(flip, vandq_u64(OO, vshlq_n_u64(flip, 8)));         flip1 |= mO & (flip1 >> 1);
//    pre  = vandq_u64(OO, vshlq_n_u64(OO, 8));                            pre1 >>= 1;
//    flip = vorrq_u64(flip, vandq_u64(pre, vshlq_n_u64(flip, 16)));       flip1 |= pre1 & (flip1 >> 2);
//    flip = vorrq_u64(flip, vandq_u64(pre, vshlq_n_u64(flip, 16)));       flip1 |= pre1 & (flip1 >> 2);
//    MM = vshlq_n_u64(flip, 8);                                           movesL |= flip1 >> 1;
//
//    OO = vandq_u64(OO, vdupq_n_u64(0x7e7e7e7e7e7e7e7e));                 mO = (unsigned int) (O >> 32) & 0x7e7e7e7e;
//    flip = vandq_u64(OO, vshlq_n_u64(PP, 7));                            flip1  = mO & ((unsigned int) (P >> 32) << 1);
//    flip = vorrq_u64(flip, vandq_u64(OO, vshlq_n_u64(flip, 7)));         flip1 |= mO & (flip1 << 1);
//    pre  = vandq_u64(OO, vshlq_n_u64(OO, 7));                            pre1   = mO & (mO << 1);
//    flip = vorrq_u64(flip, vandq_u64(pre, vshlq_n_u64(flip, 14)));       flip1 |= pre1 & (flip1 << 2);
//    flip = vorrq_u64(flip, vandq_u64(pre, vshlq_n_u64(flip, 14)));       flip1 |= pre1 & (flip1 << 2);
//    MM = vorrq_u64(MM, vshlq_n_u64(flip, 7));                            movesH = flip1 << 1;
//
//    flip = vandq_u64(OO, vshlq_n_u64(PP, 9));                            flip1  = mO & ((unsigned int) (P >> 32) >> 1);
//    flip = vorrq_u64(flip, vandq_u64(OO, vshlq_n_u64(flip, 9)));         flip1 |= mO & (flip1 >> 1);
//    pre  = vandq_u64(OO, vshlq_n_u64(OO, 9));                            pre1 >>= 1;
//    flip = vorrq_u64(flip, vandq_u64(pre, vshlq_n_u64(flip, 18)));       flip1 |= pre1 & (flip1 >> 2);
//    flip = vorrq_u64(flip, vandq_u64(pre, vshlq_n_u64(flip, 18)));       flip1 |= pre1 & (flip1 >> 2);
//    MM = vorrq_u64(MM, vshlq_n_u64(flip, 9));                            movesH |= flip1 >> 1;
//
//    movesL |= vgetq_lane_u32(vreinterpretq_u32_u64(MM), 0) | __builtin_bswap32(vgetq_lane_u32(vreinterpretq_u32_u64(MM), 3));
//    movesH |= vgetq_lane_u32(vreinterpretq_u32_u64(MM), 1) | __builtin_bswap32(vgetq_lane_u32(vreinterpretq_u32_u64(MM), 2));
//    return (movesL | ((unsigned long long) movesH << 32)) & ~(P|O);    // mask with empties
//}



#define KOGGE_STONE_STEP(flip, shift, mask) \
    flip = vorrq_u64(flip, vandq_u64(vshlq_u64(flip, shift), mask))

inline unsigned long long RXBitBoard::get_legal_moves(const unsigned long long p_discs, const unsigned long long o_discs) {
    constexpr int64x2_t S_H  = { -1,  1}, S2_H  = { -2,  2}, S4_H  = { -4,  4};
    constexpr int64x2_t S_V  = { -8,  8}, S2_V  = {-16, 16}, S4_V  = {-32, 32};
    constexpr int64x2_t S_D7 = { -7,  7}, S2_D7 = {-14, 14}, S4_D7 = {-28, 28};
    constexpr int64x2_t S_D9 = { -9,  9}, S2_D9 = {-18, 18}, S4_D9 = {-36, 36};
    
    const uint64x2_t P = vdupq_n_u64(p_discs);
    const uint64x2_t O = vdupq_n_u64(o_discs);
    const uint64x2_t O_inner = vdupq_n_u64(o_discs & 0x7E7E7E7E7E7E7E7EULL);
    
    uint64x2_t fH  = vandq_u64(vshlq_u64(P, S_H),  O_inner);
    uint64x2_t fV  = vandq_u64(vshlq_u64(P, S_V),  O);
    uint64x2_t fD7 = vandq_u64(vshlq_u64(P, S_D7), O_inner);
    uint64x2_t fD9 = vandq_u64(vshlq_u64(P, S_D9), O_inner);
    
    const uint64x2_t aH  = vandq_u64(O_inner, vshlq_u64(O_inner, S_H));
    const uint64x2_t aV  = vandq_u64(O, vshlq_u64(O, S_V));
    const uint64x2_t aD7 = vandq_u64(O_inner, vshlq_u64(O_inner, S_D7));
    const uint64x2_t aD9 = vandq_u64(O_inner, vshlq_u64(O_inner, S_D9));
    
    const uint64x2_t a2H  = vandq_u64(aH,  vshlq_u64(aH,  S2_H));
    const uint64x2_t a2V  = vandq_u64(aV,  vshlq_u64(aV,  S2_V));
    const uint64x2_t a2D7 = vandq_u64(aD7, vshlq_u64(aD7, S2_D7));
    const uint64x2_t a2D9 = vandq_u64(aD9, vshlq_u64(aD9, S2_D9));
    
    KOGGE_STONE_STEP(fH,  S_H,  O_inner);
    KOGGE_STONE_STEP(fV,  S_V,  O);
    KOGGE_STONE_STEP(fD7, S_D7, O_inner);
    KOGGE_STONE_STEP(fD9, S_D9, O_inner);
    
    KOGGE_STONE_STEP(fH,  S2_H,  aH);
    KOGGE_STONE_STEP(fV,  S2_V,  aV);
    KOGGE_STONE_STEP(fD7, S2_D7, aD7);
    KOGGE_STONE_STEP(fD9, S2_D9, aD9);
    
    KOGGE_STONE_STEP(fH,  S4_H,  a2H);
    KOGGE_STONE_STEP(fV,  S4_V,  a2V);
    KOGGE_STONE_STEP(fD7, S4_D7, a2D7);
    KOGGE_STONE_STEP(fD9, S4_D9, a2D9);
    
    const uint64x2_t legals = vorrq_u64(
        vorrq_u64(vshlq_u64(fH, S_H), vshlq_u64(fV, S_V)),
        vorrq_u64(vshlq_u64(fD7, S_D7), vshlq_u64(fD9, S_D9))
    );
    
    uint64x2_t result = vorrq_u64(legals, vextq_u64(legals, legals, 1));
    return (vgetq_lane_u64(result, 0) & ~(p_discs | o_discs));
}

#undef KOGGE_STONE_STEP

//unroll
inline int RXBitBoard::final_score_2(const unsigned long long discs_player, const unsigned long long discs_opponent, const int alpha, const int beta, const int idSquare1, const int idSquare2) const {
    
    unsigned long long flipped, n_player, n_opponent;
    
    int n_flips, bestscore = UNDEF_SCORE;
    ++n_nodes;
    
    // try to play on the first available square
    if((discs_opponent & NEIGHBOR[idSquare1]) && (flipped = do_flips[idSquare1](discs_player, discs_opponent))) {
        ++n_nodes;
        
        n_opponent = discs_opponent ^ flipped;
        
        bestscore = 62 - 2*__builtin_popcountll(n_opponent);
        
        n_flips = count_flips(idSquare2, n_opponent);
        if(n_flips !=0) {
            bestscore -= n_flips;
        } else {
            
            if(bestscore >= 0) {
                bestscore += 2;
                if(bestscore < beta) {
                    bestscore += count_flips(idSquare2, ~n_opponent);
                }
            } else {
                if(bestscore < beta) {
                    n_flips = count_flips(idSquare2, ~n_opponent);
                    
                    if(n_flips != 0)
                        bestscore += n_flips + 2;
                }
            }
        }
        
        if(bestscore >= beta)
            return bestscore;
        
    }
    
    // if needed, try to play on the second & last available square
    if((discs_opponent & NEIGHBOR[idSquare2]) && (flipped = do_flips[idSquare2](discs_player, discs_opponent))) {
        ++n_nodes;
        
        n_opponent = discs_opponent ^ flipped;
        
        int score = 62 - 2*__builtin_popcountll(n_opponent);
        
        n_flips = count_flips(idSquare1, n_opponent);
        if(n_flips !=0) {
            score -= n_flips;
        } else {
            
            if(score >= 0) {
                score += 2;
                if(score < beta) {
                    score += count_flips(idSquare1, ~n_opponent);
                }
            } else {
                if(score < beta) {
                    n_flips = count_flips(idSquare1, ~n_opponent);
                    if(n_flips != 0)
                        score += n_flips + 2;
                }
            }
        }
        
        if(score > bestscore)
            return score;
        
        return bestscore;
        
    }
    
    // if no move were available
    if(bestscore == UNDEF_SCORE) {
        
        if((flipped = do_flips[idSquare1](discs_opponent, discs_player))) {
            ++n_nodes;
            
            n_player = discs_player ^ flipped;
            
            bestscore = 62 - 2*__builtin_popcountll(n_player);
            
            n_flips = count_flips(idSquare2, n_player);
            if(n_flips !=0) {
                bestscore -= n_flips;
            } else {
                
                if(bestscore >= 0) {
                    bestscore += 2;
                    if(bestscore < -alpha) {
                        bestscore += count_flips(idSquare2, ~n_player);
                    }
                } else {
                    if(bestscore < -alpha) {
                        n_flips = count_flips(idSquare2, ~n_player);
                        
                        if(n_flips != 0)
                            bestscore += n_flips + 2;
                    }
                }
            }
            
            if(bestscore >= -alpha)
                return -bestscore;
            
            
        }
        
        // if needed, try to play on the second & last available square
        if((flipped = do_flips[idSquare2](discs_opponent, discs_player))) {
            ++n_nodes;
            
            n_player = discs_player ^ flipped;
            
            int score = 62 - 2*__builtin_popcountll(n_player);
            
            n_flips = count_flips(idSquare1, n_player);
            if(n_flips !=0) {
                score -= n_flips;
            } else {
                
                if(score >= 0) {
                    score += 2;
                    if(score < -alpha) {
                        score += count_flips(idSquare1, ~n_player);
                    }
                } else {
                    if(score < -alpha) {
                        n_flips = count_flips(idSquare1, ~n_player);
                        
                        if(n_flips != 0)
                            score += n_flips + 2;
                    }
                }
            }
            
            if(score > bestscore)
                return -score;
            
        }
        
        if(bestscore == UNDEF_SCORE) {
            
            bestscore = 62 - 2*__builtin_popcountll(discs_player);
            if(bestscore>0)
                bestscore+=2;
            else if (bestscore<0)
                bestscore-=2;
        }
        
        return -bestscore;
        
    }
    
    return bestscore;
}

#else

/// retourne un pseudo (sous evalué) score de pions stables
/// - Parameters:
///   - color: couleur du joueur
///   - n_stables_cut: valeur de coupure (type alpha, beta)
inline int RXBitBoard::get_stability(const unsigned long long discs_player, const unsigned long long discs_opponent) {
    
    
    const unsigned long long filled = discs_player | discs_opponent;
    const unsigned long long central_mask = discs_player & 0x007e7e7e7e7e7e00ULL;
    
    unsigned long long stable = get_stable_edge(discs_player, discs_opponent);

    unsigned long long h, v, d7, d9;
    
    h = filled;
    h &= h >> 4;
    h &= h >> 2;
    h &= h >> 1;
    h &= 0x0101010101010101ULL;
    
    //trick multiplication par 255 (remplit les lignes)
    h = (h << 8) - h; //*=255
    h |= 0x8181818181818181ULL;
    
    v = filled;
    v &= (v >> 32) | (v << 32);
    v &= (v >> 16) | (v << 16);
    v &= (v >>  8) | (v <<  8);
    
    v |= 0xFF000000000000FFULL;
    
    d7 = filled;
    d7 &= ((d7>>28) & 0x00000000F0F0F0F0ULL) | ((d7<<28) & 0x0F0F0F0F00000000ULL) | 0xF0F0F0F00F0F0F0FULL;
    d7 &= ((d7>>14) & 0x0000FCFCFCFCFCFCULL) | ((d7<<14) & 0x3F3F3F3F3F3F0000ULL) | 0xC0C0000000000303ULL;
    d7 &= (d7>> 7) & (d7<< 7);
    
    d7 |= 0xFF818181818181FFULL;
    
    d9 = filled;
    //d9 &= ((d9>>36) & 0x000000000F0F0F0FULL) | ((d9<<36) & 0xF0F0F0F000000000ULL) | 0x0F0F0F0FF0F0F0F0ULL;
    d9 &= (d9>>36) | (d9<<36) | 0x0F0F0F0FF0F0F0F0ULL;
    d9 &= ((d9>>18) & 0x00003F3F3F3F3F3FULL) | ((d9<<18) & 0xFCFCFCFCFCFC0000ULL) | 0x030300000000C0C0ULL;
    d9 &= (d9>> 9) & (d9<< 9);
    
    d9 |= 0xFF818181818181FFULL;
        
    stable |= (h & v & d7 & d9 & central_mask);
    
    unsigned long long old_stable = 0;
    uint64_t stable_h, stable_v, stable_d7, stable_d9;
    
    while (stable != old_stable) {
        old_stable = stable;
        stable_h  = ((stable >> 1) | (stable << 1) | h);
        stable_v  = ((stable >> 8) | (stable << 8) | v);
        stable_d7 = ((stable >> 7) | (stable << 7) | d7);
        stable_d9 = ((stable >> 9) | (stable << 9) | d9);
        stable |= (stable_h & stable_v & stable_d7 & stable_d9 & central_mask);
    }
    
    return __builtin_popcountll(stable);

    
}




inline unsigned long long RXBitBoard::hashcode() const {
    
    
    const unsigned long long p = discs[player];
    const unsigned long long o = discs[player^1];
    
    unsigned int lines1_2 = static_cast<unsigned int> ((p & 0xFFFF000000000000ULL) >> 48);
    unsigned int lines3_4 = static_cast<unsigned int> ((p & 0x0000FFFF00000000ULL) >> 32);
    unsigned int lines5_6 = static_cast<unsigned int> ((p & 0x00000000FFFF0000ULL) >> 16);
    unsigned int lines7_8 = static_cast<unsigned int> ((p & 0x000000000000FFFFULL));
    
    
    unsigned long long
    hashcode  = hashcodeTable_lines1_2[PLAYER][lines1_2];
    hashcode ^= hashcodeTable_lines3_4[PLAYER][lines3_4];
    hashcode ^= hashcodeTable_lines5_6[PLAYER][lines5_6];
    hashcode ^= hashcodeTable_lines7_8[PLAYER][lines7_8];
    
    
    lines1_2 = static_cast<unsigned int> ((o & 0xFFFF000000000000ULL) >> 48);
    lines3_4 = static_cast<unsigned int> ((o & 0x0000FFFF00000000ULL) >> 32);
    lines5_6 = static_cast<unsigned int> ((o & 0x00000000FFFF0000ULL) >> 16);
    lines7_8 = static_cast<unsigned int> ((o & 0x000000000000FFFFULL));
    
    hashcode ^= hashcodeTable_lines1_2[OPPONENT][lines1_2];
    hashcode ^= hashcodeTable_lines3_4[OPPONENT][lines3_4];
    hashcode ^= hashcodeTable_lines5_6[OPPONENT][lines5_6];
    hashcode ^= hashcodeTable_lines7_8[OPPONENT][lines7_8];
    
    
    return hashcode;
    
}



inline unsigned long long RXBitBoard::hashcode_after_move(RXMove* move)  const {
    
    const unsigned long long o = discs[player] | (move->flipped | move->square);
    const unsigned long long p = discs[player^1] ^ move->flipped;
    
    unsigned int lines1_2 = static_cast<unsigned int> ((p & 0xFFFF000000000000ULL) >> 48);
    unsigned int lines3_4 = static_cast<unsigned int> ((p & 0x0000FFFF00000000ULL) >> 32);
    unsigned int lines5_6 = static_cast<unsigned int> ((p & 0x00000000FFFF0000ULL) >> 16);
    unsigned int lines7_8 = static_cast<unsigned int> ((p & 0x000000000000FFFFULL));
    
    
    unsigned long long
    hashcode  = hashcodeTable_lines1_2[PLAYER][lines1_2];
    hashcode ^= hashcodeTable_lines3_4[PLAYER][lines3_4];
    hashcode ^= hashcodeTable_lines5_6[PLAYER][lines5_6];
    hashcode ^= hashcodeTable_lines7_8[PLAYER][lines7_8];
    
    
    lines1_2 = static_cast<unsigned int> ((o & 0xFFFF000000000000ULL) >> 48);
    lines3_4 = static_cast<unsigned int> ((o & 0x0000FFFF00000000ULL) >> 32);
    lines5_6 = static_cast<unsigned int> ((o & 0x00000000FFFF0000ULL) >> 16);
    lines7_8 = static_cast<unsigned int> ((o & 0x000000000000FFFFULL));
    
    hashcode ^= hashcodeTable_lines1_2[OPPONENT][lines1_2];
    hashcode ^= hashcodeTable_lines3_4[OPPONENT][lines3_4];
    hashcode ^= hashcodeTable_lines5_6[OPPONENT][lines5_6];
    hashcode ^= hashcodeTable_lines7_8[OPPONENT][lines7_8];
    
    
    return hashcode;
    
}

/*
 @brief Get a bitboard representing all legal moves
 
 @param P                    a bitboard representing player
 @param O                    a bitboard representing opponent
 @return all legal moves as a bitboard
 */
// original code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
// modified by Nyanyan
// version EDAX version identique a Roxane (presentation pour la vectorisation)
//inline uint64_t RXBitBoard::calc_legal(const uint64_t P, const uint64_t O){
//    uint64_t moves, mO;
//    uint64_t flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
//    mO = O & 0x7e7e7e7e7e7e7e7eULL;
//    flip1 = mO & (P << 1);         flip7  = mO & (P << 7);        flip9  = mO & (P << 9);        flip8  = O & (P << 8);
//    flip1 |= mO & (flip1 << 1);    flip7 |= mO & (flip7 << 7);    flip9 |= mO & (flip9 << 9);    flip8 |= O & (flip8 << 8);
//    pre1 = mO & (mO << 1);         pre7 = mO & (mO << 7);         pre9 = mO & (mO << 9);         pre8 = O & (O << 8);
//    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
//    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
//    moves = flip1 << 1;            moves |= flip7 << 7;           moves |= flip9 << 9;           moves |= flip8 << 8;
//    flip1 = mO & (P >> 1);         flip7  = mO & (P >> 7);        flip9  = mO & (P >> 9);        flip8  = O & (P >> 8);
//    flip1 |= mO & (flip1 >> 1);    flip7 |= mO & (flip7 >> 7);    flip9 |= mO & (flip9 >> 9);    flip8 |= O & (flip8 >> 8);
//    pre1 >>= 1;                    pre7 >>= 7;                    pre9 >>= 9;                    pre8 >>= 8;
//    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
//    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
//    moves |= flip1 >> 1;           moves |= flip7 >> 7;           moves |= flip9 >> 9;           moves |= flip8 >> 8;
//    return moves & ~(P | O);
//}


inline unsigned long long RXBitBoard::get_legal_moves(const unsigned long long p_discs, const unsigned long long o_discs) {
    
    
    const unsigned long long inner_o_discs = o_discs & 0x7E7E7E7E7E7E7E7EULL;
    
    
    /* direction W */
    unsigned long long
    flipped  = (p_discs >> 1) & inner_o_discs;
    flipped |= (flipped >> 1) & inner_o_discs;
    
    unsigned long long adjacent_o_discs = inner_o_discs & (inner_o_discs >> 1);
    
    flipped |= (flipped >> 2) & adjacent_o_discs;
    flipped |= (flipped >> 2) & adjacent_o_discs;
    
    unsigned long long legals = flipped >> 1;
    
    
    //    /* direction _E*/
    //    flipped  = (p_discs << 1) & inner_o_discs;
    //    flipped |= (flipped << 1) & inner_o_discs;
    //
    //    adjacent_o_discs = inner_o_discs & (inner_o_discs << 1);
    //
    //    flipped |= (flipped << 2) & adjacent_o_discs;
    //    flipped |= (flipped << 2) & adjacent_o_discs;
    //
    //    legals |= flipped << 1;
    
    // trick
    /* direction _E */
    flipped = (p_discs << 1);
    legals |= ((flipped + inner_o_discs) & ~flipped);
    
    
    /* direction S */
    flipped  = (p_discs >>  8) & o_discs;
    flipped |= (flipped >>  8) & o_discs;
    
    adjacent_o_discs = o_discs & (o_discs >> 8);
    
    flipped |= (flipped >> 16) & adjacent_o_discs;
    flipped |= (flipped >> 16) & adjacent_o_discs;
    
    legals |= flipped >> 8;
    
    
    /* direction N */
    flipped  = (p_discs <<  8) & o_discs;
    flipped |= (flipped <<  8) & o_discs;
    
    adjacent_o_discs = o_discs & (o_discs << 8);
    
    flipped |= (flipped << 16) & adjacent_o_discs;
    flipped |= (flipped << 16) & adjacent_o_discs;
    
    legals |= flipped << 8;
    
    
    /* direction NE */
    flipped  = (p_discs >>  7) & inner_o_discs;
    flipped |= (flipped >>  7) & inner_o_discs;
    
    adjacent_o_discs = inner_o_discs & (inner_o_discs >> 7);
    
    flipped |= (flipped >> 14) & adjacent_o_discs;
    flipped |= (flipped >> 14) & adjacent_o_discs;
    
    legals |= flipped >> 7;
    
    
    /* direction SW */
    flipped  = (p_discs <<  7) & inner_o_discs;
    flipped |= (flipped <<  7) & inner_o_discs;
    
    adjacent_o_discs = inner_o_discs & (inner_o_discs << 7);
    
    flipped |= (flipped << 14) & adjacent_o_discs;
    flipped |= (flipped << 14) & adjacent_o_discs;
    
    legals |= flipped << 7;
    
    
    /* direction NW */
    flipped  = (p_discs >>  9) & inner_o_discs;
    flipped |= (flipped >>  9) & inner_o_discs;
    
    adjacent_o_discs = inner_o_discs & (inner_o_discs >> 9);
    
    flipped |= (flipped >> 18) & adjacent_o_discs;
    flipped |= (flipped >> 18) & adjacent_o_discs;
    
    legals |= flipped >> 9;
    
    
    /* direction SE */
    flipped  = (p_discs <<  9) & inner_o_discs;
    flipped |= (flipped <<  9) & inner_o_discs;
    
    adjacent_o_discs = inner_o_discs & (inner_o_discs << 9);
    
    flipped |= (flipped << 18) & adjacent_o_discs;
    flipped |= (flipped << 18) & adjacent_o_discs;
    
    legals |= flipped << 9;
    
    //Removes existing discs
    legals &= ~(p_discs | o_discs);
    
    return legals;
    
}




inline int RXBitBoard::final_score_2(const unsigned long long discs_player, const unsigned long long discs_opponent, const int alpha, const int beta, const int idSquare1, const int idSquare2) {
    
    unsigned long long flipped;
    unsigned long long n_player;
    unsigned long long n_opponent;
    
    int n_flips, bestscore = UNDEF_SCORE;
    ++n_nodes;
    
    // try to play on the first available square
    if((discs_opponent & NEIGHBOR[idSquare1]) && (flipped = do_flips[idSquare1](discs_player, discs_opponent))) {
        ++n_nodes;
        
        n_opponent = discs_opponent ^ flipped;
        
        bestscore = 62 - 2*__builtin_popcountll(n_opponent);
        
        n_flips = count_flips[idSquare2](n_opponent);
        if(n_flips !=0) {
            bestscore -= n_flips;
        } else {
            
            if(bestscore >= 0) {
                bestscore += 2;
                if(bestscore < beta) {
                    bestscore += count_flips[idSquare2](~n_opponent);
                }
            } else {
                if(bestscore < beta) {
                    n_flips = count_flips[idSquare2](~n_opponent);
                    
                    if(n_flips != 0)
                        bestscore += n_flips + 2;
                }
            }
        }
        
        if(bestscore >= beta)
            return bestscore;
        
        
    }
    
    // if needed, try to play on the second & last available square
    if((discs_opponent & NEIGHBOR[idSquare2]) && (flipped = do_flips[idSquare2](discs_player, discs_opponent))) {
        ++n_nodes;
        
        n_opponent = discs_opponent ^ flipped;
        
        int score = 62 - 2*__builtin_popcountll(n_opponent);
        
        
        n_flips = count_flips[idSquare1](n_opponent);
        if(n_flips !=0) {
            score -= n_flips;
        } else {
            
            if(score >= 0) {
                score += 2;
                if(score < beta) {
                    score += count_flips[idSquare1](~n_opponent);
                }
            } else {
                if(score < beta) {
                    n_flips = count_flips[idSquare1](~n_opponent);
                    if(n_flips != 0)
                        score += n_flips + 2;
                }
            }
        }
        
        if(score > bestscore)
            return score;
        
        return bestscore;
        
    }
    
    // if no move were available
    if(bestscore == UNDEF_SCORE) {
        
        if((flipped = do_flips[idSquare1](discs_opponent, discs_player))) {
            ++n_nodes;
            
            n_player = discs_player ^ flipped;
            
            bestscore = 62 - 2*__builtin_popcountll(n_player);
            
            n_flips = count_flips[idSquare2](n_player);
            if(n_flips !=0) {
                bestscore -= n_flips;
            } else {
                
                if(bestscore >= 0) {
                    bestscore += 2;
                    if(bestscore < -alpha) {
                        bestscore += count_flips[idSquare2](~n_player);
                    }
                } else {
                    if(bestscore < -alpha) {
                        n_flips = count_flips[idSquare2](~n_player);
                        
                        if(n_flips != 0)
                            bestscore += n_flips + 2;
                    }
                }
            }
            
            if(bestscore >= -alpha)
                return -bestscore;
            
            
        }
        
        // if needed, try to play on the second & last available square
        if((flipped = do_flips[idSquare2](discs_opponent, discs_player))) {
            ++n_nodes;
            
            n_player = discs_player ^ flipped;
            
            int score = 62 - 2*__builtin_popcountll(n_player);
            
            n_flips = count_flips[idSquare1](n_player);
            if(n_flips !=0) {
                score -= n_flips;
            } else {
                
                if(score >= 0) {
                    score += 2;
                    if(score < -alpha) {
                        score += count_flips[idSquare1](~n_player);
                    }
                } else {
                    if(score < -alpha) {
                        n_flips = count_flips[idSquare1](~n_player);
                        if(n_flips != 0)
                            score += n_flips + 2;
                    }
                }
            }
            
            if(score > bestscore)
                return -score;
            
        }
        
        if(bestscore == UNDEF_SCORE) {
            bestscore = 62 - 2*__builtin_popcountll(discs_player);
            if(bestscore>0)
                bestscore+=2;
            else if (bestscore<0)
                bestscore-=2;
        }
        
        bestscore = -bestscore;
        
    }
    
    return bestscore;
}
#endif

inline bool RXBitBoard::dir_valid_shl(unsigned long long square, unsigned long long p_discs, unsigned long long o_discs, int shift, unsigned long long mask) {
    
    unsigned long long x;
    
    x = (square << shift) & mask & o_discs;
    x |= (x << shift) & mask & o_discs;
    x |= (x << (2 * shift)) & mask & o_discs;
    x |= (x << (4 * shift)) & mask & o_discs;
    return (x << shift) & mask & p_discs;
}

inline bool RXBitBoard::dir_valid_shr(unsigned long long square, unsigned long long p_discs, unsigned long long o_discs, int shift, unsigned long long mask) {
    
    unsigned long long x;
    
    x = (square >> shift) & mask & o_discs;
    x |= (x >> shift) & mask & o_discs;
    x |= (x >> (2 * shift)) & mask & o_discs;
    x |= (x >> (4 * shift)) & mask & o_discs;
    return (x >> shift) & mask & p_discs;
}



inline int RXBitBoard::final_score_3(int alpha, const int beta) const {
    
    unsigned int
    empties3  = (empties_list->next->position << 16);
    empties3 |= (empties_list->next->next->position << 8);
    empties3 |=  empties_list->next->next->next->position;
    
    --n_nodes; // removes the duplicate
    
    return final_score_3(discs[player], discs[player^1], alpha, beta, 0xE4UL, empties3);
}

//unroll
inline int RXBitBoard::final_score_3(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, int beta, const unsigned int shuf3, const unsigned int empties3) const {
    
    int score, bestscore = UNDEF_SCORE;
    ++n_nodes;
    
    unsigned long long flipped;
        
    const int idSquare1 = (empties3 >> ((shuf3 & 0x30) >> 1)) & 0xFF;
    const int idSquare2 = (empties3 >> ((shuf3 & 0x0C) << 1)) & 0xFF;
    const int idSquare3 = (empties3 >> ((shuf3 & 0x03) * 8))  & 0xFF;

    
    if ((discs_opponent & NEIGHBOR[idSquare1]) && (flipped = do_flips[idSquare1](discs_player, discs_opponent))){
        
        bestscore = -final_score_2(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare1), -beta, -alpha, idSquare2, idSquare3);
        
        if(bestscore>=beta)
            return bestscore;
        
        if(bestscore>alpha)
            alpha = bestscore;
        
    }
    
    if ((discs_opponent & NEIGHBOR[idSquare2]) && (flipped = do_flips[idSquare2](discs_player, discs_opponent))){
        
        score = -final_score_2(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare2), -beta, -alpha, idSquare1, idSquare3);
        
        if(score>=beta)
            return score;
        
        if (score > bestscore) {
            bestscore = score;
            if (bestscore > alpha)
                alpha = bestscore;
        }
        
    }
    
    if ((discs_opponent & NEIGHBOR[idSquare3]) && (flipped = do_flips[idSquare3](discs_player, discs_opponent))){
        
        score = -final_score_2(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare3), -beta, -alpha, idSquare1, idSquare2);
        
        if(score > bestscore)
            return score;
        
        return bestscore;
    }
    
    if (bestscore == UNDEF_SCORE) {
                
        if ((discs_player & NEIGHBOR[idSquare1]) && (flipped = do_flips[idSquare1](discs_opponent, discs_player))){
            
            bestscore = -final_score_2(discs_player ^ flipped , discs_opponent ^ (flipped | 0x1ULL<<idSquare1), alpha, beta, idSquare2, idSquare3);
            
            if(bestscore>=-alpha)
                return -bestscore;
            
            if(bestscore>-beta)
                beta = -bestscore;
            
        }
        
        if ((discs_player & NEIGHBOR[idSquare2]) && (flipped = do_flips[idSquare2](discs_opponent, discs_player))){
            
            score = -final_score_2(discs_player ^ flipped , discs_opponent ^ (flipped | 0x1ULL<<idSquare2), alpha, beta, idSquare1, idSquare3);
            
            if(score>=-alpha)
                return -score;
            
            if (score > bestscore) {
                bestscore = score;
                if (bestscore > -beta)
                    beta = -bestscore;
            }
            
            
        }
        
        if ((discs_player & NEIGHBOR[idSquare3]) && (flipped = do_flips[idSquare3](discs_opponent, discs_player))){
            
            score = -final_score_2(discs_player ^ flipped , discs_opponent ^ (flipped | 0x1ULL<<idSquare3), alpha, beta, idSquare1, idSquare2);
            
            if(score > bestscore)
                return -score;
        }
        
        if(bestscore == UNDEF_SCORE) {
            bestscore = 61-2*__builtin_popcountll(discs_player);
            if(bestscore>0)
                bestscore+=3;
            else
                bestscore-=3;
        }
        
        
        bestscore = -bestscore;
    }
    
    
    return bestscore;
}

/// final score at 2 empties
/// - Parameters:
///   - alpha: alpha bound
///   - beta: beta bound
///   - passed: true if  previous move is pass
inline int RXBitBoard::final_score_4(int alpha, int beta, const bool passed) const {
    
#ifdef USE_STABILITY
    
    int diff_discs = (2*__builtin_popcountll(discs[player]) - 60);
    
    if (beta >= 6|| (beta >= 0 && (diff_discs <= beta - 6))) {
        
        int stability_bound = 64- 2 * get_stability(player^1);
        if ( stability_bound <= alpha )
            return stability_bound; //alpha
        
        if ( stability_bound < beta )
            beta = stability_bound;
        
        
    } else  if (alpha <= -6|| (alpha <= 0 && (-diff_discs <= alpha + 6))) {
        
        int stability_bound = 2 * get_stability(player) - 64;
        if ( stability_bound >= beta )
            return stability_bound; //beta
        
    }

#endif
    
    
    
    const int sq_1 = empties_list->next->position;
    const int sq_2 = empties_list->next->next->position;
    const int sq_3 = empties_list->next->next->next->position;
    const int sq_4 = empties_list->next->next->next->next->position;
    
    //Sorting on the parity (4 empties square) **Bright
    
    // parity sort
    static const unsigned char parity_case[64] = {    /* idQuadrant sq_4sq_3sq_2sq_1 = */
        /*0000*/  0, /*0001*/  0, /*0010*/  1, /*0011*/  9, /*0100*/  2, /*0101*/ 10, /*0110*/ 11, /*0111*/  3,
        /*0002*/  0, /*0003*/  0, /*0012*/  0, /*0013*/  0, /*0102*/  4, /*0103*/  4, /*0112*/  5, /*0113*/  5,
        /*0020*/  1, /*0021*/  0, /*0030*/  1, /*0031*/  0, /*0120*/  6, /*0121*/  7, /*0130*/  6, /*0131*/  7,
        /*0022*/  9, /*0023*/  0, /*0032*/  0, /*0033*/  9, /*0122*/  8, /*0123*/  0, /*0132*/  0, /*0133*/  8,
        /*0200*/  2, /*0201*/  4, /*0210*/  6, /*0211*/  8, /*0300*/  2, /*0301*/  4, /*0310*/  6, /*0311*/  8,
        /*0202*/ 10, /*0203*/  4, /*0212*/  7, /*0213*/  0, /*0302*/  4, /*0303*/ 10, /*0312*/  0, /*0313*/  7,
        /*0220*/ 11, /*0221*/  5, /*0230*/  6, /*0231*/  0, /*0320*/  6, /*0321*/  0, /*0330*/ 11, /*0331*/  5,
        /*0222*/  3, /*0223*/  5, /*0232*/  7, /*0233*/  8, /*0322*/  8, /*0323*/  7, /*0332*/  5, /*0333*/  3
    };

    
    static const unsigned int sort4_shuf[] = {
        0x3978b4e4,    //  0: 1(x1) 3(x2 x3 x4),      1(x1) 1(x2) 2(x3 x4), 1 1 1 1, 4        x4x1x2x3-x3x1x2x4-x2x1x3x4-x1x2x3x4
        0x3978e4b4,    //  1: 1(x2) 3(x1 x3 x4)       x4x1x2x3-x3x1x2x4-x1x2x3x4-x2x1x3x4
        0x39b4e478,    //  2: 1(x3) 3(x1 x2 x4)       x4x1x2x3-x2x1x3x4-x1x2x3x4-x3x1x2x4
        0x78b4e439,    //  3: 1(x4) 3(x1 x2 x3)       x3x1x2x4-x2x1x3x4-x1x2x3x4-x4x1x2x3
        0x39b478d8,    //  4: 1(x1) 1(x3) 2(x2 x4)    x4x1x2x3-x2x1x3x4-x3x1x2x4-x1x3x2x4
        0x78b439c9,    //  5: 1(x1) 1(x4) 2(x2 x3)    x3x1x2x4-x2x1x3x4-x4x1x2x3-x1x4x2x3
        0x39e46c9c,    //  6: 1(x2) 1(x3) 2(x1 x4)    x4x1x2x3-x1x2x3x4-x3x2x1x4-x2x3x1x4
        0x78e42d8d,    //  7: 1(x2) 1(x4) 2(x1 x3)    x3x1x2x4-x1x2x3x4-x4x2x1x3-x2x4x1x3
        0xb4e41e4e,    //  8: 1(x3) 1(x4) 2(x1 x2)    x2x1x3x4-x1x2x3x4-x4x3x1x2-x3x4x1x2
        0x1e4eb4e4,    //  9: 2(x1 x2) 2(x3 x4)       x4x3x1x2-x3x4x1x2-x2x1x3x4-x1x2x3x4
        0x2d788dd8,    // 10: 2(x1 x3) 2(x2 x4)       x4x2x1x3-x3x1x2x4-x2x4x1x3-x1x3x2x4
        0x396c9cc9     // 11: 2(x1 x4) 2(x2 x3)       x4x1x2x3-x3x2x1x4-x2x3x1x4-x1x4x2x3
    };

    // parity based move sorting.
    // The following hole sizes are possible:
    // 4 - 1 3 - 2 2 - 1 1 2 - 1 1 1 1
    // the 1 1 2 case needs move sorting on this ply.
    // prefer 1 empty over 3 empties, 1 3 case also needs sorting.
    int paritysort = parity_case[((sq_3 ^ sq_4) & 0x24) + (((sq_2 ^ sq_4) & 0x24) >> 1) + (((sq_1 ^ sq_4) & 0x24) >> 2)];
    unsigned int shuf4 = sort4_shuf[paritysort];
    unsigned int empties4 = (sq_1 << 24) | (sq_2 << 16) | (sq_3 << 8) | sq_4;

    
    
    
    return final_score_4(discs[player], discs[player^1], alpha, beta, passed, shuf4, empties4);
}

inline int RXBitBoard::final_score_4(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, const int beta, const bool passed, const unsigned int shuf4, const unsigned int empties4) const {
    
    
    int score, bestscore = UNDEF_SCORE;
    
    unsigned long long flipped;
    
    int idSquare1 = (empties4 >> ((shuf4 >> (6 - 3)) & 0x18)) & 0xFF;
    if ((discs_opponent & NEIGHBOR[idSquare1]) && (flipped = do_flips[idSquare1](discs_player, discs_opponent))){
        
        bestscore = -final_score_3(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare1), -beta, -alpha, shuf4, empties4);
        
        if(bestscore>=beta)
            return bestscore;
        
        if(bestscore>alpha)
            alpha = bestscore;
        
        
    }
    
    int idSquare2 = (empties4 >> ((shuf4 >> (14 - 3)) & 0x18)) & 0xFF;
    if ((discs_opponent & NEIGHBOR[idSquare2]) && (flipped = do_flips[idSquare2](discs_player, discs_opponent))){
        
        score = -final_score_3(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare2), -beta, -alpha, shuf4>>8, empties4);
        
        if(score>=beta)
            return score;
        
        if (score > bestscore) {
            bestscore = score;
            if (bestscore > alpha)
                alpha = bestscore;
        }
        
        
    }
    
    int idSquare3 = (empties4 >> ((shuf4 >> (22 - 3)) & 0x18)) & 0xFF;
    if ((discs_opponent & NEIGHBOR[idSquare3]) && (flipped = do_flips[idSquare3](discs_player, discs_opponent))) {
        
        score = -final_score_3(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare3), -beta, -alpha, shuf4>>16, empties4);
        
        if(score>=beta)
            return score;
        
        if (score > bestscore) {
            bestscore = score;
            if (bestscore > alpha)
                alpha = bestscore;
        }
        
        
    }
    
    int idSquare4 = (empties4 >> ((shuf4 >> 30) * 8)) & 0xFF;
    if ((discs_opponent & NEIGHBOR[idSquare4]) && (flipped = do_flips[idSquare4](discs_player, discs_opponent))){
        
        score = -final_score_3(discs_opponent ^ flipped, discs_player ^ (flipped | 0x1ULL<<idSquare4), -beta, -alpha, shuf4>>24, empties4);
        
        if(score > bestscore)
            return score;
        
        return bestscore;
        
    }
    
    // if no _move4 were available
    if(bestscore == UNDEF_SCORE) {
        if (passed) {
            bestscore = 60-2*__builtin_popcountll(discs_opponent);
            if(bestscore>0)
                bestscore+=4;
            else if (bestscore<0)
                bestscore-=4;
            
        } else {
            bestscore = -final_score_4(discs_opponent, discs_player, -beta, -alpha, true, shuf4, empties4);
        }
    }
    
    return bestscore;
}


/**
 * Calcule les coups légaux pour les deux joueurs simultanément.
 * Lane 0 : Coups légaux pour p_discs
 * Lane 1 : Coups légaux pour o_discs
 */
template<int Shift, bool IsHorizontal>
inline uint64x2_t propagate_kogge_stone(const uint64x2_t p_vec, const uint64x2_t o_vec, const uint64x2_t mask_inner) {
    constexpr int S = (Shift > 0) ? Shift : -Shift;
    
    // Masque : le M4 fusionne très bien les opérations constantes
    const uint64x2_t mask = IsHorizontal ? mask_inner : vdupq_n_u64(0xFFFFFFFFFFFFFFFFULL);
    const uint64x2_t prop = vandq_u64(o_vec, mask);
    
    if constexpr (Shift > 0) {
        // Initialisation
        uint64x2_t g = vandq_u64(vshlq_n_u64(p_vec, S), prop);
        
        // CLEF : Calculer TOUS les prop shifts en parallèle (4 ops/cycle)
        const uint64x2_t g1 = vshlq_n_u64(g, S);
        const uint64x2_t prop1 = vshlq_n_u64(prop, S);
        const uint64x2_t prop2 = vandq_u64(prop, prop1);
        const uint64x2_t prop2_shift = vshlq_n_u64(prop2, 2*S);
        
        // Puis les accumulations (dépendances)
        g = vorrq_u64(g, vandq_u64(g1, prop));
        
        const uint64x2_t g2 = vshlq_n_u64(g, 2*S);
        const uint64x2_t prop4 = vandq_u64(prop2, prop2_shift);
        g = vorrq_u64(g, vandq_u64(g2, prop2));
        
        const uint64x2_t g4 = vshlq_n_u64(g, 4*S);
        g = vorrq_u64(g, vandq_u64(g4, prop4));
        
        return vshlq_n_u64(g, S);
    } else {
        uint64x2_t g = vandq_u64(vshrq_n_u64(p_vec, S), prop);
        
        const uint64x2_t g1 = vshrq_n_u64(g, S);
        const uint64x2_t prop1 = vshrq_n_u64(prop, S);
        const uint64x2_t prop2 = vandq_u64(prop, prop1);
        const uint64x2_t prop2_shift = vshrq_n_u64(prop2, 2*S);
        
        g = vorrq_u64(g, vandq_u64(g1, prop));
        
        const uint64x2_t g2 = vshrq_n_u64(g, 2*S);
        const uint64x2_t prop4 = vandq_u64(prop2, prop2_shift);
        g = vorrq_u64(g, vandq_u64(g2, prop2));
        
        const uint64x2_t g4 = vshrq_n_u64(g, 4*S);
        g = vorrq_u64(g, vandq_u64(g4, prop4));
        
        return vshrq_n_u64(g, S);
    }
}

inline uint64x2_t RXBitBoard::dual_count_legal_moves() const {
    return dual_count_legal_moves(discs[player], discs[player^1]);
}

inline uint64x2_t RXBitBoard::dual_count_legal_moves(const unsigned long long p, const unsigned long long o) {
    
    uint64x2_t legals = RXBitBoard::dual_legal_moves( p, o);
    
    // --- POPCOUNT NEON ---
    // 1. Compte les bits par octets
    uint8x16_t cnt8 = vcntq_u8(vreinterpretq_u8_u64(legals));
    // 2. Sommes horizontales successives (8->16, 16->32, 32->64)
    uint16x8_t sum16 = vpaddlq_u8(cnt8);
    uint32x4_t sum32 = vpaddlq_u16(sum16);
    uint64x2_t mobility = vpaddlq_u32(sum32);
    
    return mobility;
}

inline uint64x2_t RXBitBoard::dual_legal_moves(const unsigned long long p, const unsigned long long o) {
    // Préparation des registres 128 bits
    // Lane 0: P vs O | Lane 1: O vs P
    uint64x2_t p_vec = {p, o};
    uint64x2_t o_vec = {o, p};
    
    uint64x2_t mask_inner = vdupq_n_u64(0x7E7E7E7E7E7E7E7EULL);
    uint64x2_t legals = vdupq_n_u64(0);
    
    // Calcul des 8 directions en Kogge-Stone
    legals = vorrq_u64(legals, propagate_kogge_stone< 8, false>(p_vec, o_vec, mask_inner)); // N
    legals = vorrq_u64(legals, propagate_kogge_stone<-8, false>(p_vec, o_vec, mask_inner)); // S
    legals = vorrq_u64(legals, propagate_kogge_stone< 1, true >(p_vec, o_vec, mask_inner)); // E
    legals = vorrq_u64(legals, propagate_kogge_stone<-1, true >(p_vec, o_vec, mask_inner)); // W
    legals = vorrq_u64(legals, propagate_kogge_stone< 7, true >(p_vec, o_vec, mask_inner)); // NE
    legals = vorrq_u64(legals, propagate_kogge_stone<-7, true >(p_vec, o_vec, mask_inner)); // SW
    legals = vorrq_u64(legals, propagate_kogge_stone< 9, true >(p_vec, o_vec, mask_inner)); // NW
    legals = vorrq_u64(legals, propagate_kogge_stone<-9, true >(p_vec, o_vec, mask_inner)); // SE
    
    // Nettoyage final : le coup doit arriver sur une case vide
    uint64x2_t occupied = vdupq_n_u64(p | o);
    legals = vbicq_u64(legals, occupied);
    
    return legals;
}


#endif
