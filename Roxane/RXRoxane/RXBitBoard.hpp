/*
 *  RXBitBoard.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 27/06/05.
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
#include "RXSquareList.hpp"
#include "RXTools.hpp"

#ifdef __ARM_NEON
    #include "arm_neon.h"
#endif


class RXBitBoard {

	friend class RXRoxane;
	friend class RXHashTable;
	friend class RXHashShallow;
	friend class RXEngine;
	friend class RXBBPatterns;
	friend class RXEvaluation;
    
    private :
    static const unsigned long long hashSquare[64][2];
    
    static unsigned long long hashcodeTable_lines1_2[2][65536];
    static unsigned long long hashcodeTable_lines3_4[2][65536];
    static unsigned long long hashcodeTable_lines5_6[2][65536];
    static unsigned long long hashcodeTable_lines7_8[2][65536];

    
	public :
    
    static const unsigned long long X_TO_BIT[66];

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
    
#endif
    
    static void init_hashcodeTable();
	
	//neighborhood for each square
	static const unsigned long long NEIGHBOR[];
    
    // presorted squares
    static const unsigned long long PRESORTED_POSITION_BITS[];
	static const int PRESORTED_POSITION[];

    
	/*! a quadrant id for each square */
    static const unsigned long long QUADRANT_MASK[];
    static const int QUADRANT_SHITF[];
    static const int QUADRANT_ID[];


    //initialisation hashcodeTable
    


	unsigned long long discs[2];
	int player;
	int n_empties;
    int parity;
	RXSquareList empties_list[62];
	RXSquareList *position_to_empties[64];
	mutable unsigned long long n_nodes;
	    
    //public :
    
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
    
    // move functions
#define func(pos) static bool do_flips_##pos(unsigned long long& discs_player, unsigned long long& discs_opponent); \
bool generate_flips_##pos(RXMove& move) const \
    
    func(A1); func(B1); func(C1); func(D1); func(E1); func(F1); func(G1); func(H1);
    func(A2); func(B2); func(C2); func(D2); func(E2); func(F2); func(G2); func(H2);
    func(A3); func(B3); func(C3); func(D3); func(E3); func(F3); func(G3); func(H3);
    func(A4); func(B4); func(C4);					  func(F4); func(G4); func(H4);
    func(A5); func(B5); func(C5);					  func(F5); func(G5); func(H5);
    func(A6); func(B6); func(C6); func(D6); func(E6); func(F6); func(G6); func(H6);
    func(A7); func(B7); func(C7); func(D7); func(E7); func(F7); func(G7); func(H7);
    func(A8); func(B8); func(C8); func(D8); func(E8); func(F8); func(G8); func(H8);
    
#undef func
    
    typedef bool (*type_do_flips)(unsigned long long& discs_player, unsigned long long& discs_opponent);
    static type_do_flips const do_flips[];
    bool (RXBitBoard::*generate_flips[64])(RXMove& move) const;
    void init_generate_flips();

    
#ifdef __ARM_NEON
    
    int count_flips(int pos, unsigned long long P) const;
    
#else

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
    static type_count_flips const count_flips[];
    
#endif
    
        
    /* bool check_move(const int position, const int color) const; */
    
    void moves_producing(RXMove* start) const;
    
    static uint64_t calc_legal(const uint64_t P, const uint64_t O);
    static unsigned long long get_legal_moves(const unsigned long long discs_player, const unsigned long long discs_opponent);

    static int count_potential_moves(const unsigned long long p_discs, const unsigned long long o_discs);

    
    static inline int get_mobility(const unsigned long long discs_player, const unsigned long long discs_opponent);
    static inline int get_corner_stability(const unsigned long long& discs_player);
    inline int get_stability(const int player) const;
    static inline int get_stability_opponent(const unsigned long long discs_player, const unsigned long long discs_opponent);
    
    
    void do_move(const RXMove& move);
    void undo_move(const RXMove& move);
    void do_pass();
    
    
    int final_score() const;
    int final_score_1() const ;
    int final_score_2(int alpha, const int beta);
    int final_score_2(const unsigned long long discs_player, const unsigned long long discs_opponent, const int alpha, const int beta, const int idSquare1, const int idSquare2);
    int final_score_3(int alpha, const int beta);
    int final_score_3(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, int beta, const int sort3, int idSquare1, int idSquare2, int idSquare3);
    int	final_score_4(const bool pv, int alpha, int beta, const bool passed);
    int	final_score_4(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, const int beta, const bool passed, const int sort3, const int idSquare1, const int idSquare2, const int idSquare3, const int idSquare4);
    
    std::string cassio_script() const;
    
    /* DEBUG */
    void print_empties_list() const;
    static void print_64bits(unsigned long long n);
    void print_Board();
    void print_moves_list(RXMove* MovesList) const;
    
    unsigned long long hashcode() const ;
    unsigned long long hashcode_after_move(RXMove* move)  const;
    
    /* test */
    //static unsigned long long cntbset(unsigned long long n);
    //void build(const unsigned long long disc_player, const unsigned long long disc_opp, const int color);
    
    
};

/*
 * Set all bits below the sole outflank bit if outfrank != 0
 */
#if __has_builtin(__builtin_subcll)
static inline unsigned long long OutflankToFlipmask(unsigned long long outflank) {
    unsigned long long flipmask, cy;
    flipmask = __builtin_subcll(outflank, 1, 0, &cy);
    return __builtin_addcll(flipmask, 0, cy, &cy);
}
#else
    #define OutflankToFlipmask(outflank)    ((outflank) - (unsigned int) ((outflank) != 0))
#endif

// Strictly, (long long) >> 64 is undefined in C, but either 0 bit (no change)
// or 64 bit (zero out) shift will lead valid result (i.e. flipped == 0).
#define    outflank_right(O,maskr)    (0x8000000000000000ULL >> __builtin_clzll(~(O) & (maskr)))

// in case continuous from MSB
#define    outflank_right_H(O)    (0x80000000u >> __builtin_clz(~(O)))


#ifdef __clang__    // poor optimization for vbicq(const,x) (ndk-r15)
#define not_O_in_mask(mask,O)    vandq_u64((mask), vdupq_n_u64(~(O)))
#else
#define not_O_in_mask(mask,O)    vbicq_u64((mask), vdupq_n_u64(O))
#endif

//rotl8
#if __has_builtin(__builtin_rotateleft8)
    #define rotl8(x,y)    __builtin_rotateleft8((x),(y))
#elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)) && (defined(__x86_64__) || defined(__i386__))
    #define rotl8(x,y)    __builtin_ia32_rolqi((x),(y))
#endif




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
	
	n_empties--;
    parity ^= QUADRANT_ID[move.position];

	const RXSquareList *remove = position_to_empties[move.position];
	remove->previous->next = remove->next;
	remove->next->previous = remove->previous;
		
	n_nodes++;
}

inline void RXBitBoard::undo_move(const RXMove& move) {

	RXSquareList *insert = position_to_empties[move.position];
	insert->previous->next = insert;
	insert->next->previous = insert;
	
    parity ^= QUADRANT_ID[move.position];
    n_empties++;
	
	discs[player] |= move.flipped;
	player ^=1;
	discs[player] ^= (move.flipped | move.square);

}

inline void RXBitBoard::do_pass() {
	player ^= 1;
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


/*
    @brief Get a bitboard representing all legal moves

    @param P                    a bitboard representing player
    @param O                    a bitboard representing opponent
    @return all legal moves as a bitboard
*/
// original code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
// modified by Nyanyan
// version EDAX version identique a Roxane (presentation pour la vectorisation)
inline uint64_t RXBitBoard::calc_legal(const uint64_t P, const uint64_t O){
    uint64_t moves, mO;
    uint64_t flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    mO = O & 0x7e7e7e7e7e7e7e7eULL;
    flip1 = mO & (P << 1);         flip7  = mO & (P << 7);        flip9  = mO & (P << 9);        flip8  = O & (P << 8);
    flip1 |= mO & (flip1 << 1);    flip7 |= mO & (flip7 << 7);    flip9 |= mO & (flip9 << 9);    flip8 |= O & (flip8 << 8);
    pre1 = mO & (mO << 1);         pre7 = mO & (mO << 7);         pre9 = mO & (mO << 9);         pre8 = O & (O << 8);
    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;            moves |= flip7 << 7;           moves |= flip9 << 9;           moves |= flip8 << 8;
    flip1 = mO & (P >> 1);         flip7  = mO & (P >> 7);        flip9  = mO & (P >> 9);        flip8  = O & (P >> 8);
    flip1 |= mO & (flip1 >> 1);    flip7 |= mO & (flip7 >> 7);    flip9 |= mO & (flip9 >> 9);    flip8 |= O & (flip8 >> 8);
    pre1 >>= 1;                    pre7 >>= 7;                    pre9 >>= 9;                    pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;           moves |= flip7 >> 7;           moves |= flip9 >> 9;           moves |= flip8 >> 8;
    return moves & ~(P | O);
}

/// retourne un pseudo (sous evalué) score de pions stables
/// la 1ere partie determine les lignes (dans les 4 directions) pleines
/// si un pions est dans les 4 lignes et appartient a la color, il est stable
/// la deuxieme partie trouve les pions stables adgacents au pions stables precedents (dans les 4 directions)
/// - Parameters:
///   - color: couleur du joueur
///   - n_stables_cut: valeur de coupure (type alpha, beta)
inline int RXBitBoard::get_stability(const int player) const {
    return RXBitBoard::get_stability_opponent(discs[player^1], discs[player]);
}




#ifdef __ARM_NEON


//inline int RXBitBoard::get_corner_stability(const unsigned long long& discs_player) {
//    
//    static const int64x2_t shift[] = {{-1,-8}, { 1, 8}};
//    static const uint64x2_t mask[] = {
//        {0x4000000000000040ULL, 0x0081000000000000ULL},
//        {0x0200000000000002ULL, 0x0000000000008100ULL}};
//    
//    const uint64x2_t discs  = vdupq_n_u64(discs_player);
//    uint64x2_t stable = vdupq_n_u64(discs_player & 0x8100000000000081ULL);
// 
//    stable = vorrq_u64(vandq_u64(vandq_u64(vshlq_u64(stable, shift[0]), discs), mask[0]), stable);
//    stable = vorrq_u64(vandq_u64(vandq_u64(vshlq_u64(stable, shift[1]), discs), mask[1]), stable);
//
//    return __builtin_popcountll(vgetq_lane_u64(stable, 0) | vgetq_lane_u64(stable, 1));
//
//}


    
    
inline int RXBitBoard::get_stability_opponent(const unsigned long long discs_player, const unsigned long long discs_opponent) {

    //static const for all directions full lines
    
    static const int64x2_t shift[] = {
        {-4,-32}, {64, 32}, {-2,-16}, {64, 16}, {-1, -8}, {64,  8},
        {-28,-36}, { 28, 36}, {-14,-18}, { 14, 18},
        {-1,-8}, { 1, 8}, {-7,-9}, { 7, 9}
    };
    
    static const uint64x2_t mask[] = {
        { 0x8181818181818181ULL, 0xFF000000000000FFULL},
        { 0x00000000F0F0F0F0ULL, 0x000000000F0F0F0FULL},
        { 0x0F0F0F0F00000000ULL, 0xF0F0F0F000000000ULL},
        { 0xF0F0F0F00F0F0F0FULL, 0x0F0F0F0FF0F0F0F0ULL},
        { 0x0000FCFCFCFCFCFCULL, 0x00003F3F3F3F3F3FULL},
        { 0x3F3F3F3F3F3F0000ULL, 0xFCFCFCFCFCFC0000ULL},
        { 0xC0C0000000000303ULL, 0x030300000000C0C0ULL},
        { 0xFF818181818181FFULL, 0xFF818181818181FFULL}
    };
    
    //--------------------------------------------------------------------------------------------
    
    const unsigned long long filled = discs_player | discs_opponent;
    const uint64x2_t dd_color = vdupq_n_u64(discs_opponent);


    //horizontals and verticals full lines
    uint64x2_t hv = vdupq_n_u64(filled);
    
    hv = vandq_u64( hv, vorrq_u64(vshlq_u64(hv, shift[0]), vshlq_u64(hv, shift[1])));
    hv = vandq_u64( hv, vorrq_u64(vshlq_u64(hv, shift[2]), vshlq_u64(hv, shift[3])));
    hv = vandq_u64( hv, vorrq_u64(vshlq_u64(hv, shift[4]), vshlq_u64(hv, shift[5])));

    hv = vcombine_u64(((vget_low_u64(hv) & 0x0101010101010101ULL) * 0xFFULL), vget_high_u64(hv));
    hv = vorrq_u64(hv, mask[0]);

    
    //2 * diagonals full lines
    uint64x2_t dg = vdupq_n_u64(filled);
    
    uint64x2_t
    temp = vandq_u64(vshlq_u64(dg, shift[6]), mask[1]);
    temp = vorrq_u64(temp , vandq_u64(vshlq_u64(dg, shift[7]), mask[2]));
    dg = vandq_u64(dg, vorrq_u64(temp, mask[3]));

    temp = vandq_u64(vshlq_u64(dg, shift[8]), mask[4]);
    temp = vorrq_u64(temp , vandq_u64(vshlq_u64(dg, shift[9]), mask[5]));
    dg = vandq_u64(dg, vorrq_u64(temp, mask[6]));

    dg = vandq_u64(dg, vandq_u64(vshlq_u64(dg, shift[12]), vshlq_u64(dg, shift[13])));
    dg = vorrq_u64(dg, mask[7]);
    
    
    // mix full lines and discs color
    temp = vandq_u64(hv, dg);
    temp = vandq_u64(vandq_u64(temp, vcombine_u64(vget_high_u64(temp), vget_low_u64(temp))), dd_color);
    
    
    unsigned long long stable = vgetq_lane_u64(temp, 0);


    if(stable == 0)
        return 0;
        

    unsigned long long old_stable;
    uint64x2_t dir_hv;
    uint64x2_t dir_dg;
    
           
    do {
        
        old_stable = stable;
        
        dir_hv = vorrq_u64(vorrq_u64(vshlq_u64(temp, shift[10]), vshlq_u64(temp, shift[11])), hv);
        dir_dg = vorrq_u64(vorrq_u64(vshlq_u64(temp, shift[12]), vshlq_u64(temp, shift[13])), dg);

        temp = vandq_u64(dir_hv, dir_dg);
        temp = vandq_u64(vandq_u64(temp, vcombine_u64(vget_high_u64(temp), vget_low_u64(temp))), dd_color);

        stable = vgetq_lane_u64(temp, 0);

    } while(stable != old_stable);

//    if(stable == 0)
//        return 0;
        
    return VALUE_DISC * __builtin_popcountll(stable);
 
}

#else



/// retourne un pseudo (sous evalué) score de pions stables
/// - Parameters:
///   - color: couleur du joueur
///   - n_stables_cut: valeur de coupure (type alpha, beta)
inline int RXBitBoard::get_stability_opponent(const unsigned long long discs_player, const unsigned long long discs_opponent) {
    
    
    unsigned long long filled = discs_player | discs_opponent;
    
    unsigned long long left_right, up_down, diag_a, diag_b;

    left_right = filled;
    left_right &= left_right >> 4;
    left_right &= left_right >> 2;
    left_right &= left_right >> 1;
    left_right &= 0x0101010101010101ULL;
    
    //trick multiplication par 255 (remplit le lignes)
    left_right = (left_right << 8) - left_right; //*=255
    left_right |= 0x8181818181818181ULL;
     
    up_down = filled;
    up_down &= (up_down >> 32) | (up_down << 32);
    up_down &= (up_down >> 16) | (up_down << 16);
    up_down &= (up_down >>  8) | (up_down <<  8);
    
    up_down |= 0xFF000000000000FFULL;
        
    diag_a = filled;
    diag_a &= ((diag_a>>28) & 0x00000000F0F0F0F0ULL) | ((diag_a<<28) & 0x0F0F0F0F00000000ULL) | 0xF0F0F0F00F0F0F0FULL;
    diag_a &= ((diag_a>>14) & 0x0000FCFCFCFCFCFCULL) | ((diag_a<<14) & 0x3F3F3F3F3F3F0000ULL) | 0xC0C0000000000303ULL;
    diag_a &= (diag_a>> 7) & (diag_a<< 7);
    
    diag_a |= 0xFF818181818181FFULL;
    
    diag_b = filled;
    //diag_b &= ((diag_b>>36) & 0x000000000F0F0F0FULL) | ((diag_b<<36) & 0xF0F0F0F000000000ULL) | 0x0F0F0F0FF0F0F0F0ULL;
    diag_b &= (diag_b>>36) | (diag_b<<36) | 0x0F0F0F0FF0F0F0F0ULL;
    diag_b &= ((diag_b>>18) & 0x00003F3F3F3F3F3FULL) | ((diag_b<<18) & 0xFCFCFCFCFCFC0000ULL) | 0x030300000000C0C0ULL;
    diag_b &= (diag_b>> 9) & (diag_b<< 9);
    
    diag_b |= 0xFF818181818181FFULL;
    
    unsigned long long stable = left_right & up_down & diag_a & diag_b & discs_opponent;


    if(stable == 0)
        return 0;

    unsigned long long old_stable, dir_1, dir_2, dir_3, dir_4;
    
    
    do {

        old_stable = stable;

        dir_1 = (stable << 1) | (stable >> 1 ) | left_right;
        dir_2 = (stable << 8) | (stable >> 8 ) | up_down;
        dir_3 = (stable << 7) | (stable >> 7 ) | diag_a;
        dir_4 = (stable << 9) | (stable >> 9 ) | diag_b;

        stable = dir_1 & dir_2 & dir_3 & dir_4 & discs_opponent;


    } while(stable != old_stable);
    

//    if(stable == 0)
//        return 0;
        
    return VALUE_DISC * __builtin_popcountll(stable);

 
}

#endif

inline int RXBitBoard::get_corner_stability(const unsigned long long& discs_player) {

    unsigned long long stables = discs_player & 0x8100000000000081ULL;

    stables |= (discs_player & (stables << 1)) & 0x0200000000000002ULL;
    stables |= (discs_player & (stables >> 1)) & 0x4000000000000040ULL;
    stables |= (discs_player & (stables << 8)) & 0x0000000000008100ULL;
    stables |= (discs_player & (stables >> 8)) & 0x0081000000000000ULL;

    return __builtin_popcountll(stables);
    
//    unsigned int P2187 = static_cast<unsigned int>((discs_player >> 48) | (discs_player << 16));    // ror 48
//    unsigned int stable = 0x00818100 & P2187;
//    stable |= ((((stable * 5) >> 1) & 0x00424200) | (stable << 8) | (stable >> 8)) & P2187;    // 1-8 alias does not matter since corner is stable anyway
//    return __builtin_popcount(stable);

}


inline int RXBitBoard::final_score_2(int alpha, const int beta) {
	return final_score_2(discs[player], discs[player^1], alpha/VALUE_DISC, beta/VALUE_DISC,  empties_list->next->position,  empties_list->next->next->position)*VALUE_DISC;
}

#ifdef __ARM_NEON

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


//unroll
inline int RXBitBoard::final_score_2(const unsigned long long discs_player, const unsigned long long discs_opponent, const int alpha, const int beta, const int idSquare1, const int idSquare2) {
    
    unsigned long long d_player = discs_player;
    unsigned long long d_opponent = discs_opponent;
            
    int n_flips, bestscore = UNDEF_SCORE;
            
    // try to play on the first available square
    if((d_opponent & NEIGHBOR[idSquare1]) && (do_flips[idSquare1](d_player, d_opponent))) {
        n_nodes++;
        
        bestscore = 62 - 2*__builtin_popcountll(d_opponent);

        n_flips = count_flips(idSquare2, d_opponent);
        if(n_flips !=0) {
            bestscore -= n_flips;
        } else {
            if(bestscore >= 0) {
                bestscore += 2;
                if(bestscore < beta) {
                    bestscore += count_flips(idSquare2, d_player);
                }
            } else {
                if(bestscore < beta) {
                    n_flips = count_flips(idSquare2, d_player);

                    if(n_flips != 0)
                        bestscore += n_flips + 2;
                }
            }
        }

        if(bestscore >= beta)
            return bestscore;

        d_player = discs_player;
        d_opponent = discs_opponent;
            
    }
                                
    // if needed, try to play on the second & last available square
    if((d_opponent & NEIGHBOR[idSquare2]) && (do_flips[idSquare2](d_player, d_opponent))) {
        n_nodes++;
        
        int score = 62 - 2*__builtin_popcountll(d_opponent);

        
        n_flips = count_flips(idSquare1, d_opponent);
        if(n_flips !=0) {
            score -= n_flips;
        } else {
            if(score >= 0) {
                score += 2;
                if(score < beta) {
                    score += count_flips(idSquare1, d_player);
                }
            } else {
                if(score < beta) {
                    n_flips = count_flips(idSquare1, d_player);
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
        n_nodes++; //PASS
        
        if((do_flips[idSquare1](d_opponent, d_player))) {
            n_nodes++;
            
            bestscore = 62 - 2*__builtin_popcountll(d_player);
            
            n_flips = count_flips(idSquare2, d_player);
            if(n_flips !=0) {
                bestscore -= n_flips;
            } else {
                if(bestscore >= 0) {
                    bestscore += 2;
                    if(bestscore < -alpha) {
                        bestscore += count_flips(idSquare2, d_opponent);
                    }
                } else {
                    if(bestscore < -alpha) {
                        n_flips = count_flips(idSquare2, d_opponent);
                        
                        if(n_flips != 0)
                            bestscore += n_flips + 2;
                    }
                }
            }
            
            if(bestscore >= -alpha)
                return -bestscore;
            
            d_player = discs_player;
            d_opponent = discs_opponent;
            
        }
        
        // if needed, try to play on the second & last available square
        if((do_flips[idSquare2](d_opponent, d_player))) {
            n_nodes++;
            
            int score = 62 - 2*__builtin_popcountll(d_player);
            
            
            n_flips = count_flips(idSquare1, d_player);
            if(n_flips !=0) {
                score -= n_flips;
            } else {
                if(score >= 0) {
                    score += 2;
                    if(score < -alpha) {
                        score += count_flips(idSquare1, d_opponent);
                    }
                } else {
                    if(score < -alpha) {
                        n_flips = count_flips(idSquare1, d_opponent);
                        if(n_flips != 0)
                            score += n_flips + 2;
                    }
                }
            }
            
            if(score > bestscore)
                return -score;
 
        }
        
        if(bestscore == UNDEF_SCORE) {
            
            n_nodes--; //undo PASS
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


inline int RXBitBoard::final_score_2(const unsigned long long discs_player, const unsigned long long discs_opponent, const int alpha, const int beta, const int idSquare1, const int idSquare2) {
    
    unsigned long long d_player = discs_player;
    unsigned long long d_opponent = discs_opponent;
            
    int n_flips, bestscore = UNDEF_SCORE;
            
    // try to play on the first available square
    if((d_opponent & NEIGHBOR[idSquare1]) && (do_flips[idSquare1](d_player, d_opponent))) {
        n_nodes++;
        
        bestscore = 62 - 2*__builtin_popcountll(d_opponent);

        n_flips = count_flips[idSquare2](d_opponent);
        if(n_flips !=0) {
            bestscore -= n_flips;
        } else {
            if(bestscore >= 0) {
                bestscore += 2;
                if(bestscore < beta) {
                    bestscore += count_flips[idSquare2](d_player);
                }
            } else {
                if(bestscore < beta) {
                    n_flips = count_flips[idSquare2](d_player);

                    if(n_flips != 0)
                        bestscore += n_flips + 2;
                }
            }
        }

        if(bestscore >= beta)
            return bestscore;

        d_player = discs_player;
        d_opponent = discs_opponent;
            
    }
                                
    // if needed, try to play on the second & last available square
    if((d_opponent & NEIGHBOR[idSquare2]) && (do_flips[idSquare2](d_player, d_opponent))) {
        n_nodes++;
        
        int score = 62 - 2*__builtin_popcountll(d_opponent);

        
        n_flips = count_flips[idSquare1](d_opponent);
        if(n_flips !=0) {
            score -= n_flips;
        } else {
            if(score >= 0) {
                score += 2;
                if(score < beta) {
                    score += count_flips[idSquare1](d_player);
                }
            } else {
                if(score < beta) {
                    n_flips = count_flips[idSquare1](d_player);
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
        n_nodes++; //PASS
        
        if((do_flips[idSquare1](d_opponent, d_player))) {
            n_nodes++;
            
            bestscore = 62 - 2*__builtin_popcountll(d_player);
            
            n_flips = count_flips[idSquare2](d_player);
            if(n_flips !=0) {
                bestscore -= n_flips;
            } else {
                if(bestscore >= 0) {
                    bestscore += 2;
                    if(bestscore < -alpha) {
                        bestscore += count_flips[idSquare2](d_opponent);
                    }
                } else {
                    if(bestscore < -alpha) {
                        n_flips = count_flips[idSquare2](d_opponent);
                        
                        if(n_flips != 0)
                            bestscore += n_flips + 2;
                    }
                }
            }
            
            if(bestscore >= -alpha)
                return -bestscore;
            
            d_player = discs_player;
            d_opponent = discs_opponent;
            
        }
        
        // if needed, try to play on the second & last available square
        if((do_flips[idSquare2](d_opponent, d_player))) {
            n_nodes++;
            
            int score = 62 - 2*__builtin_popcountll(d_player);
            
            
            n_flips = count_flips[idSquare1](d_player);
            if(n_flips !=0) {
                score -= n_flips;
            } else {
                if(score >= 0) {
                    score += 2;
                    if(score < -alpha) {
                        score += count_flips[idSquare1](d_opponent);
                    }
                } else {
                    if(score < -alpha) {
                        n_flips = count_flips[idSquare1](d_opponent);
                        if(n_flips != 0)
                            score += n_flips + 2;
                    }
                }
            }
            
            if(score > bestscore)
                return -score;
 
        }
        
        if(bestscore == UNDEF_SCORE) {
            n_nodes--; //undo PASS
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


inline int RXBitBoard::final_score_3(int alpha, const int beta) {
	return final_score_3(discs[player], discs[player^1], alpha/VALUE_DISC, beta/VALUE_DISC, 0,  empties_list->next->position,  empties_list->next->next->position,  empties_list->next->next->next->position)*VALUE_DISC;
}

//unroll
inline int RXBitBoard::final_score_3(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, int beta, const int sort3, int idSquare1, int idSquare2, int idSquare3) {

	int score, bestscore = UNDEF_SCORE;
	
	unsigned long long d_player = discs_player;
	unsigned long long d_opponent = discs_opponent;
    
    // parity based move sorting
    switch (sort3 & 0x03) {
        case 1:
            std::swap(idSquare1, idSquare2);
            break;
        case 2:
            std::swap(idSquare1, idSquare3);
            //carefull not break
        case 3:
            std::swap(idSquare2, idSquare3);
            break;
    }

		
	if ((d_opponent & NEIGHBOR[idSquare1]) && (do_flips[idSquare1](d_player, d_opponent))){ 
		n_nodes++;
		
		bestscore = -final_score_2(d_opponent, d_player, -beta, -alpha, idSquare2, idSquare3);
										
		if(bestscore>=beta)
			return bestscore;

		if(bestscore>alpha)
			alpha = bestscore;
		
		d_player = discs_player;
		d_opponent = discs_opponent;

	}

	if ((d_opponent & NEIGHBOR[idSquare2]) && (do_flips[idSquare2](d_player, d_opponent))){ 
		n_nodes++;
		
		score = -final_score_2(d_opponent, d_player, -beta, -alpha, idSquare1, idSquare3);
		
		if(score>=beta)
			return score;
			
		if (score > bestscore) {
			bestscore = score;
			if (bestscore > alpha)
				alpha = bestscore;
		}
		
		d_player = discs_player;
		d_opponent = discs_opponent;
		
	}
	
	if ((d_opponent & NEIGHBOR[idSquare3]) && (do_flips[idSquare3](d_player, d_opponent))){ 
		n_nodes++;
		
		score = -final_score_2(d_opponent, d_player, -beta, -alpha, idSquare1, idSquare2);
		
		if(score > bestscore)
			return score;
			
		return bestscore;
	}
    
    if (bestscore == UNDEF_SCORE) {
        
        n_nodes++; //PASS
        
        if (do_flips[idSquare1](d_opponent, d_player)){
            n_nodes++;
            
            bestscore = -final_score_2(d_player, d_opponent, alpha, beta, idSquare2, idSquare3);
                                            
            if(bestscore>=-alpha)
                return -bestscore;

            if(bestscore>-beta)
                beta = -bestscore;
            
            d_player = discs_player;
            d_opponent = discs_opponent;

        }

        if (do_flips[idSquare2](d_opponent, d_player)){
            n_nodes++;
            
            score = -final_score_2(d_player, d_opponent, alpha, beta, idSquare1, idSquare3);
            
            if(score>=-alpha)
                return -score;
                
            if (score > bestscore) {
                bestscore = score;
                if (bestscore > -beta)
                    beta = -bestscore;
            }
            
            d_player = discs_player;
            d_opponent = discs_opponent;
            
        }
        
        if (do_flips[idSquare3](d_opponent, d_player)){
            n_nodes++;
            
            score = -final_score_2(d_player, d_opponent, alpha, beta, idSquare1, idSquare2);
            
            if(score > bestscore)
                return -score;
        }
 
        if(bestscore == UNDEF_SCORE) {
            n_nodes--; //undo PASS
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
inline int RXBitBoard::final_score_4(const bool pv, int alpha, int beta, const bool passed) {
    
    
    if(!pv) {
        //stability Cutoff
        int diff_discs = 2*__builtin_popcountll(discs[player]) - 60;
        
        if (beta >= 6*VALUE_DISC || (beta >= 0 && (diff_discs*VALUE_DISC <= beta - 6*VALUE_DISC))) {
            
            int stability_bound = 64*VALUE_DISC - 2 * get_stability(player^1);
            if ( stability_bound <= alpha )
                return stability_bound; //alpha
            
            if ( stability_bound < beta )
                beta = stability_bound;
            
            
        } else  if (beta <= -8*VALUE_DISC || (beta <= 0 && (diff_discs*VALUE_DISC >= beta + 8*VALUE_DISC))) {
            
            int stability_bound = 2 * get_stability(player) - 64*VALUE_DISC;
            if ( stability_bound >= beta )
                return stability_bound; //beta
            
        }
    }
    

	
	int sq_1 = empties_list->next->position;
	int sq_2 = empties_list->next->next->position;
	int sq_3 = empties_list->next->next->next->position;
	int sq_4 = empties_list->next->next->next->next->position;
    
    //Edax's sorting on the parity (4 empties square) **Bright
	
    // parity sort
    static const unsigned char parity_case[64] = {    /* sq_4sq_3sq_2sq_1 = */
        /*0000*/  0, /*0001*/  0, /*0010*/  1, /*0011*/  9, /*0100*/  2, /*0101*/ 10, /*0110*/ 11, /*0111*/  3,
        /*0002*/  0, /*0003*/  0, /*0012*/  0, /*0013*/  0, /*0102*/  4, /*0103*/  4, /*0112*/  5, /*0113*/  5,
        /*0020*/  1, /*0021*/  0, /*0030*/  1, /*0031*/  0, /*0120*/  6, /*0121*/  7, /*0130*/  6, /*0131*/  7,
        /*0022*/  9, /*0023*/  0, /*0032*/  0, /*0033*/  9, /*0122*/  8, /*0123*/  0, /*0132*/  0, /*0133*/  8,
        /*0200*/  2, /*0201*/  4, /*0210*/  6, /*0211*/  8, /*0300*/  2, /*0301*/  4, /*0310*/  6, /*0311*/  8,
        /*0202*/ 10, /*0203*/  4, /*0212*/  7, /*0213*/  0, /*0302*/  4, /*0303*/ 10, /*0312*/  0, /*0313*/  7,
        /*0220*/ 11, /*0221*/  5, /*0230*/  6, /*0231*/  0, /*0320*/  6, /*0321*/  0, /*0330*/ 11, /*0331*/  5,
        /*0222*/  3, /*0223*/  5, /*0232*/  7, /*0233*/  8, /*0322*/  8, /*0323*/  7, /*0332*/  5, /*0333*/  3
    };
    
    static const short sort3_shuf[] = {
        0x0000,    //  0: 1(sq_1) 3(sq_2 sq_3 sq_4),      1(sq_1) 1(sq_2) 2(sq_3 sq_4), 1 1 1 1, 4                              sq_4sq_1sq_2sq_3-sq_3sq_1sq_2sq_4-sq_2sq_1sq_3sq_4-sq_1sq_2sq_3sq_4
        0x1100,    //  1: 1(sq_2) 3(sq_1 sq_3 sq_4)       sq_4sq_2sq_1sq_3-sq_3sq_2sq_1sq_4-sq_2sq_1sq_3sq_4-sq_1sq_2sq_3sq_4
        0x2011,    //  2: 1(sq_3) 3(sq_1 sq_2 sq_4)       sq_4sq_3sq_1sq_2-sq_3sq_1sq_2sq_4-sq_2sq_3sq_1sq_4-sq_1sq_3sq_2sq_4
        0x0222,    //  3: 1(sq_4) 3(sq_1 sq_2 sq_3)       sq_4sq_1sq_2sq_3-sq_3sq_4sq_1sq_2-sq_2sq_4sq_1sq_3-sq_1sq_4sq_2sq_3
        0x3000,    //  4: 1(sq_1) 1(sq_3) 2(sq_2 sq_4)    sq_4sq_1sq_2sq_3-sq_2sq_1sq_3sq_4-sq_3sq_1sq_2sq_4-sq_1sq_3sq_2sq_4 <- sq_4sq_1sq_3sq_2-sq_2sq_1sq_3sq_4-sq_3sq_1sq_2sq_4-sq_1sq_3sq_2sq_4
        0x3300,    //  5: 1(sq_1) 1(sq_4) 2(sq_2 sq_3)    sq_3sq_1sq_2sq_4-sq_2sq_1sq_3sq_4-sq_4sq_1sq_2sq_3-sq_1sq_4sq_2sq_3 <- sq_3sq_1sq_4sq_2-sq_2sq_1sq_4sq_3-sq_4sq_1sq_2sq_3-sq_1sq_4sq_2sq_3
        0x2000,    //  6: 1(sq_2) 1(sq_3) 2(sq_1 sq_4)    sq_4sq_1sq_2sq_3-sq_1sq_2sq_3sq_4-sq_3sq_2sq_1sq_4-sq_2sq_3sq_1sq_4 <- sq_4sq_2sq_3sq_1-sq_1sq_2sq_3sq_4-sq_3sq_2sq_1sq_4-sq_2sq_3sq_1sq_4
        0x2300,    //  7: 1(sq_2) 1(sq_4) 2(sq_1 sq_3)    sq_3sq_1sq_2sq_4-sq_1sq_2sq_3sq_4-sq_4sq_2sq_1sq_3-sq_2sq_4sq_1sq_3 <- sq_3sq_2sq_4sq_1-sq_1sq_2sq_4sq_3-sq_4sq_2sq_1sq_3-sq_2sq_4sq_1sq_3
        0x2200,    //  8: 1(sq_3) 1(sq_4) 2(sq_1 sq_2)    sq_2sq_1sq_3sq_4-sq_1sq_2sq_3sq_4-sq_4sq_3sq_1sq_2-sq_3sq_4sq_1sq_2 <- sq_2sq_3sq_4sq_1-sq_1sq_3sq_4sq_2-sq_4sq_3sq_1sq_2-sq_3sq_4sq_1sq_2
        0x2200,    //  9: 2(sq_1 sq_2) 2(sq_3 sq_4)       sq_4sq_3sq_1sq_2-sq_3sq_4sq_1sq_2-sq_2sq_1sq_3sq_4-sq_1sq_2sq_3sq_4
        0x1021,    // 10: 2(sq_1 sq_3) 2(sq_2 sq_4)       sq_4sq_2sq_1sq_3-sq_3sq_1sq_2sq_4-sq_2sq_4sq_1sq_3-sq_1sq_3sq_2sq_4
        0x0112     // 11: 2(sq_1 sq_4) 2(sq_2 sq_3)       sq_4sq_1sq_2sq_3-sq_3sq_2sq_1sq_4-sq_2sq_3sq_1sq_4-sq_1sq_4sq_2sq_3
    };
    
    // parity based move sorting.
    // The following hole sizes are possible:
    //    4 - 1 3 - 2 2 - 1 1 2 - 1 1 1 1
    // Only the 1 1 2 case needs move sorting on this ply.

    int paritysort = parity_case[((sq_3 ^ sq_4) & 0x24) + (((sq_2 ^ sq_4) & 0x24) >> 1) + (((sq_1 ^ sq_4) & 0x24) >> 2)];
    switch (paritysort) {
        case 4: // case 1(sq_1) 1(sq_3) 2(sq_2 sq_4)
            std::swap(sq_2, sq_3);
            break;
        case 5: // case 1(sq_1) 1(sq_4) 2(sq_2 sq_3)
            std::swap(sq_2, sq_4);
            std::swap(sq_3, sq_4);
            break;
        case 6:    // case 1(sq_2) 1(sq_3) 2(sq_1 sq_4)
            std::swap(sq_1, sq_3);
            std::swap(sq_1, sq_2);
            break;
        case 7: // case 1(sq_2) 1(sq_4) 2(sq_1 sq_3)
            std::swap(sq_1, sq_2);
            std::swap(sq_2, sq_4);
            std::swap(sq_3, sq_4);
            break;
        case 8:    // case 1(sq_3) 1(sq_4) 2(sq_1 sq_2)
            std::swap(sq_1, sq_3);
            std::swap(sq_2, sq_4);
            break;
    }


    
    
    return final_score_4(discs[player], discs[player^1], alpha/VALUE_DISC, beta/VALUE_DISC, passed, sort3_shuf[paritysort], sq_1,  sq_2, sq_3, sq_4)*VALUE_DISC;
}

inline int RXBitBoard::final_score_4(const unsigned long long discs_player, const unsigned long long discs_opponent, int alpha, const int beta, const bool passed, const int sort3, const int idSquare1, const int idSquare2, const int idSquare3, const int idSquare4) {

		
	int score, bestscore = UNDEF_SCORE;
		
	unsigned long long d_player = discs_player;
	unsigned long long d_opponent = discs_opponent;
	
	if ((d_opponent & NEIGHBOR[idSquare1]) && (do_flips[idSquare1](d_player, d_opponent))){ 
		n_nodes++;
        			 
		bestscore = -final_score_3(d_opponent, d_player, -beta, -alpha, sort3, idSquare2, idSquare3, idSquare4);
				
		if(bestscore>=beta)
			return bestscore;

		if(bestscore>alpha)
			alpha = bestscore;
		
		d_player = discs_player;
		d_opponent = discs_opponent;
		
	}

	if ((d_opponent & NEIGHBOR[idSquare2]) && (do_flips[idSquare2](d_player, d_opponent))){ 
		n_nodes++;
		
		score = -final_score_3(d_opponent, d_player, -beta, -alpha, sort3>>4, idSquare1, idSquare3, idSquare4);
		
		if(score>=beta)
			return score;
			
		if (score > bestscore) {
			bestscore = score;
			if (bestscore > alpha)
				alpha = bestscore;
		}

		d_player = discs_player;
		d_opponent = discs_opponent;
		
	}
	
	if ((d_opponent & NEIGHBOR[idSquare3]) && (do_flips[idSquare3](d_player, d_opponent))) {
		n_nodes++;
		
		score = -final_score_3(d_opponent, d_player, -beta, -alpha, sort3>>8, idSquare1, idSquare2, idSquare4);
		
		if(score>=beta)
			return score;
			
		if (score > bestscore) {
			bestscore = score;
			if (bestscore > alpha)
				alpha = bestscore;
		}

		d_player = discs_player;
		d_opponent = discs_opponent;
		
	}

	if ((d_opponent & NEIGHBOR[idSquare4]) && (do_flips[idSquare4](d_player, d_opponent))){ 
		n_nodes++;
		
		score = -final_score_3(d_opponent, d_player, -beta, -alpha, sort3>>12, idSquare1, idSquare2, idSquare3);

		if(score > bestscore)
			return score;
			
		return bestscore;
			
	}

	// if no _move4 were available
	if(bestscore == UNDEF_SCORE) {
		if (passed) {
 			n_nodes--;
			bestscore = 60-2*__builtin_popcountll(discs_opponent);
			if(bestscore>0)
				bestscore+=4;
			else if (bestscore<0)
				bestscore-=4;
								
		} else {
			n_nodes++;
			bestscore = -final_score_4(d_opponent, d_player, -beta, -alpha, true, sort3, idSquare1, idSquare2, idSquare3, idSquare4);
		}
	}
	
	return bestscore;
}


#endif
