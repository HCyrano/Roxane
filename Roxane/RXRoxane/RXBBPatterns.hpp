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

#include "RXConstantes.hpp"
#include "RXBitBoard.hpp"
#include "RXPattern.hpp"
#include "RXEvaluation.hpp"


class RXBBPatterns {
    
    friend class RXRoxane;
    friend class RXEngine;
    
    static const int QUANTA = VALUE_DISC;
    
    void set_BLACK(const int pos);
    void set_WHITE(const int pos);
    
    RXBitBoard board;
    RXPattern* pattern;
    
    public :
    
    
    RXBBPatterns();
    
    //constructeur par copie
    RXBBPatterns(const RXBBPatterns& src);
    
    RXBBPatterns& operator=(const RXBBPatterns& src);
    
    ~RXBBPatterns();
    
    void build(const std::string& init);
    
    void do_move(RXMove& move);
    void undo_move(const RXMove& move);
    
    // move functions
#define func(pos)   void update_patterns_BLACK_##pos(RXMove& move) const; \
void update_patterns_WHITE_##pos(RXMove& move) const
    
    func(A1); func(B1); func(C1); func(D1); func(E1); func(F1); func(G1); func(H1);
    func(A2); func(B2); func(C2); func(D2); func(E2); func(F2); func(G2); func(H2);
    func(A3); func(B3); func(C3); func(D3); func(E3); func(F3); func(G3); func(H3);
    func(A4); func(B4); func(C4);			          func(F4); func(G4); func(H4);
    func(A5); func(B5); func(C5);			          func(F5); func(G5); func(H5);
    func(A6); func(B6); func(C6); func(D6); func(E6); func(F6); func(G6); func(H6);
    func(A7); func(B7); func(C7); func(D7); func(E7); func(F7); func(G7); func(H7);
    func(A8); func(B8); func(C8); func(D8); func(E8); func(F8); func(G8); func(H8);
#undef func
    
    
    void (RXBBPatterns::*update_patterns[64][2])(RXMove& move) const;
    void init_update_patterns();
    
    double get_n_nodes() const { return board.get_n_nodes(); }
    int get_n_empties() const { return board.n_empties; }
    
    int final_score() const;
    int get_score() const;
    int get_score(RXMove& move) const;
    
//    inline int verif_score() const;

    
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
        return (-MAX_SCORE+VALUE_DISC) + (score + 64*VALUE_DISC);
    return (MAX_SCORE-VALUE_DISC) + (score-64*VALUE_DISC);
}

#ifndef NEW_EVAL


/*
inline int RXBBPatterns::verif_score() const {
    
    const int stage = 60-board.n_empties;
    
    const unsigned long long filled = (board.discs[BLACK] | board.discs[WHITE]);
    
    int eval;
    const short* value;
    const short* value_b;
    
    if(board.player == BLACK) {
        
        value = RXEvaluation::DIAG_5[stage];
        std::cout << "     pattern->diag_5a = " << pattern->diag_5a << std::endl;
        std::cout << "diag_5a = " << value[pattern->diag_5a] << std::endl;
        eval  = value[pattern->diag_5a];

        std::cout << "     pattern->diag_5b = " << pattern->diag_5b << std::endl;
        std::cout << "diag_5b = " << value[pattern->diag_5b] << std::endl;
        eval += value[pattern->diag_5b];
        
        std::cout << "     pattern->diag_5c = " << pattern->diag_5c << std::endl;
        std::cout << "diag_5c = " << value[pattern->diag_5c] << std::endl;
        eval += value[pattern->diag_5c];
        
        std::cout << "     pattern->diag_5d = " << pattern->diag_5d << std::endl;
        std::cout << "diag_5d = " << value[pattern->diag_5d] << std::endl;
        eval += value[pattern->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        std::cout << "diag_6a = " << value[pattern->diag_6a] << std::endl;
        eval += value[pattern->diag_6a];
        
        std::cout << "diag_6b = " << value[pattern->diag_6b] << std::endl;
        eval += value[pattern->diag_6b];
        
        std::cout << "diag_6c = " << value[pattern->diag_6c] << std::endl;
        eval += value[pattern->diag_6c];
        
        std::cout << "diag_6d = " << value[pattern->diag_6d] << std::endl;
        eval += value[pattern->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        std::cout << "diag_7a = " << value[pattern->diag_7a] << std::endl;
        eval += value[pattern->diag_7a];
        
        std::cout << "diag_7b = " << value[pattern->diag_7b] << std::endl;
        eval += value[pattern->diag_7b];
        
        std::cout << "diag_7c = " << value[pattern->diag_7c] << std::endl;
        eval += value[pattern->diag_7c];
        
        std::cout << "diag_7d = " << value[pattern->diag_7d] << std::endl;
        eval += value[pattern->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        std::cout << "diag_8a = " << value[pattern->diag_8a] << std::endl;
        eval += value[pattern->diag_8a];
        
        std::cout << "diag_8b = " << value[pattern->diag_8b] << std::endl;
        eval += value[pattern->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        std::cout << "hv4_a = " << value[pattern->hv_4a] << std::endl;
        eval += value[pattern->hv_4a];
        
        std::cout << "hv4_b = " << value[pattern->hv_4b] << std::endl;
        eval += value[pattern->hv_4b];
        
        std::cout << "hv4_c = " << value[pattern->hv_4c] << std::endl;
        eval += value[pattern->hv_4c];
        
        std::cout << "hv4_d = " << value[pattern->hv_4d] << std::endl;
        eval += value[pattern->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        std::cout << "hv3_a = " << value[pattern->hv_3a] << std::endl;
        eval += value[pattern->hv_3a];
        
        std::cout << "hv3_b = " << value[pattern->hv_3b] << std::endl;
        eval += value[pattern->hv_3b];
        
        std::cout << "hv3_c = " << value[pattern->hv_3c] << std::endl;
        eval += value[pattern->hv_3c];
        
        std::cout << "hv3_d = " << value[pattern->hv_3d] << std::endl;
        eval += value[pattern->hv_3d];
        
        value = RXEvaluation::HV_2[stage];
        int edge_d = value[pattern->hv_2a];
        eval += value[pattern->hv_2a];
        int edge_b = value[pattern->hv_2b];
        eval += value[pattern->hv_2b];
        std::cout << "hv_2c = " << value[pattern->hv_2c] << std::endl;
 //       std::cout << "     patern->hv_2c = " << pattern->hv_2c << std::endl;
        int edge_a = value[pattern->hv_2c];
        eval += value[pattern->hv_2c];
        int edge_c = value[pattern->hv_2d];
        eval += value[pattern->hv_2d];
        
        value = RXEvaluation::CORNER_11[stage];
        std::cout << "corner11a = " << value[pattern->corner11a] << std::endl;
        eval += value[pattern->corner11a];
        
        std::cout << "corner11b = " << value[pattern->corner11b] << std::endl;
        eval += value[pattern->corner11b];
        
        std::cout << "corner11c = " << value[pattern->corner11c] << std::endl;
        eval += value[pattern->corner11c];
        
        std::cout << "corner11d = " << value[pattern->corner11d] << std::endl;
        eval += value[pattern->corner11d];
        
        value = RXEvaluation::corner2x5[stage];
        std::cout << "corner2x5a = " << value[pattern->corner2x5a] << std::endl;
//        std::cout << "     pattern->corner2x5a = " << pattern->corner2x5a << std::endl;

        edge_a += value[pattern->corner2x5a];
        eval += value[pattern->corner2x5a];
        edge_b += value[pattern->corner2x5b];
        eval += value[pattern->corner2x5b];
        edge_c += value[pattern->corner2x5c];
        eval += value[pattern->corner2x5c];
        edge_d += value[pattern->corner2x5d];
        eval += value[pattern->corner2x5d];
        
        edge_d += value[pattern->corner2x5e];
        eval += value[pattern->corner2x5e];
        std::cout << "corner2x5f = " << value[pattern->corner2x5f] << std::endl;
//        std::cout << "     pattern->corner2x5f = " << pattern->corner2x5f << std::endl;
        edge_a += value[pattern->corner2x5f];
        eval += value[pattern->corner2x5f];
        edge_b += value[pattern->corner2x5g];
        eval += value[pattern->corner2x5g];
        edge_c += value[pattern->corner2x5h];
        eval += value[pattern->corner2x5h];
        
        value = RXEvaluation::EDGE_2XC[stage];
        value_b = RXEvaluation::EDGE_6_4[stage];
        if(filled & 0x8142000000000000ULL) { //A1 H1 B2 G2
            std::cout << "edge2XCa = " << value[pattern->edge2XCa] << std::endl;
 //           std::cout << "     pattern->edge2XCa = " << pattern->edge2XCa << std::endl;

            edge_a += value[pattern->edge2XCa];
            eval += value[pattern->edge2XCa];
        } else {
            std::cout << "edge64a = " << value[pattern->edge64a] << std::endl;
//            std::cout << "     pattern->edge64a = " << pattern->edge64a << std::endl;

            edge_a += value_b[pattern->edge64a];
            eval += value_b[pattern->edge64a];
        }

        if(filled & 0x0102000000000201ULL) {//H1 G2 G7 H8
            edge_b += value[pattern->edge2XCb];
            eval += value[pattern->edge2XCb];
        } else {
            edge_b += value_b[pattern->edge64b];
            eval += value_b[pattern->edge64b];
        }

        if(filled & 0x0000000000004281ULL) {   //B7 G7 A8 H8
            edge_c += value[pattern->edge2XCc];
            eval += value[pattern->edge2XCc];
        } else {
            edge_c += value_b[pattern->edge64c];
            eval += value_b[pattern->edge64c];
        }

        if(filled & 0x8040000000004080ULL) {  //A1 B2 B7 A8
            edge_d += value[pattern->edge2XCd];
            eval += value[pattern->edge2XCd];
        } else {
            edge_d += value_b[pattern->edge64d];
            eval += value_b[pattern->edge64d];
        }
        
        std::cout << "edge_a = " << edge_a << std::endl;
        std::cout << "edge_b = " << edge_b << std::endl;
        std::cout << "edge_c = " << edge_c << std::endl;
        std::cout << "edge_d = " << edge_d << std::endl;

    } else {
        
        value = RXEvaluation::DIAG_5[stage];
        std::cout << "diag_5a = " << value[-pattern->diag_5a] << std::endl;
        eval  = value[-pattern->diag_5a];

        std::cout << "diag_5b = " << value[-pattern->diag_5b] << std::endl;
        eval += value[-pattern->diag_5b];
        

        std::cout << "diag_5c = " << value[-pattern->diag_5c] << std::endl;
        eval += value[-pattern->diag_5c];
        
        std::cout << "diag_5d = " << value[-pattern->diag_5d] << std::endl;
        eval += value[-pattern->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        std::cout << "diag_6a = " << value[-pattern->diag_6a] << std::endl;
        eval += value[-pattern->diag_6a];
        
        std::cout << "diag_6b = " << value[-pattern->diag_6b] << std::endl;
        eval += value[-pattern->diag_6b];
        
        std::cout << "diag_6c = " << value[-pattern->diag_6c] << std::endl;
        eval += value[-pattern->diag_6c];
        
        std::cout << "diag_6d = " << value[-pattern->diag_6d] << std::endl;
        eval += value[-pattern->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        std::cout << "diag_7a = " << value[-pattern->diag_7a] << std::endl;
        eval += value[-pattern->diag_7a];
        
        std::cout << "diag_7b = " << value[-pattern->diag_7b] << std::endl;
        eval += value[-pattern->diag_7b];
        
        std::cout << "diag_7c = " << value[-pattern->diag_7c] << std::endl;
        eval += value[-pattern->diag_7c];
        
        std::cout << "diag_7d = " << value[-pattern->diag_7d] << std::endl;
        eval += value[-pattern->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        std::cout << "diag_8a = " << value[-pattern->diag_8a] << std::endl;
        eval += value[-pattern->diag_8a];
        
        std::cout << "diag_8b = " << value[-pattern->diag_8b] << std::endl;
        eval += value[-pattern->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        std::cout << "hv4_a = " << value[-pattern->hv_4a] << std::endl;
        eval += value[-pattern->hv_4a];
        
        std::cout << "hv4_b = " << value[-pattern->hv_4b] << std::endl;
        eval += value[-pattern->hv_4b];
        
        std::cout << "hv4_c = " << value[-pattern->hv_4c] << std::endl;
        eval += value[-pattern->hv_4c];
        
        std::cout << "hv4_d = " << value[-pattern->hv_4d] << std::endl;
        eval += value[-pattern->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        std::cout << "hv3_a = " << value[-pattern->hv_3a] << std::endl;
        eval += value[-pattern->hv_3a];
        
        std::cout << "hv3_b = " << value[-pattern->hv_3b] << std::endl;
        eval += value[-pattern->hv_3b];
        
        std::cout << "hv3_c = " << value[-pattern->hv_3c] << std::endl;
        eval += value[-pattern->hv_3c];
        
        std::cout << "hv3_d = " << value[-pattern->hv_3d] << std::endl;
        eval += value[-pattern->hv_3d];
        
        value = RXEvaluation::HV_2[stage];
        int edge_d = value[-pattern->hv_2a];
        eval += value[-pattern->hv_2a];
        int edge_b = value[-pattern->hv_2b];
        eval += value[-pattern->hv_2b];
        int edge_a = value[-pattern->hv_2c];
        eval += value[-pattern->hv_2c];
        int edge_c = value[-pattern->hv_2d];
        eval += value[-pattern->hv_2d];
        
        value = RXEvaluation::CORNER_11[stage];
        std::cout << "corner11a = " << value[-pattern->corner11a] << std::endl;
        eval += value[-pattern->corner11a];
        
        std::cout << "corner11b = " << value[-pattern->corner11b] << std::endl;
        eval += value[-pattern->corner11b];
        
        std::cout << "corner11c = " << value[-pattern->corner11c] << std::endl;
        eval += value[-pattern->corner11c];
        
        std::cout << "corner11d = " << value[-pattern->corner11d] << std::endl;
        eval += value[-pattern->corner11d];
        
        value = RXEvaluation::corner2x5[stage];
        edge_a += value[-pattern->corner2x5a];
        eval += value[-pattern->corner2x5a];
        edge_b += value[-pattern->corner2x5b];
        eval += value[-pattern->corner2x5b];
        edge_c += value[-pattern->corner2x5c];
        eval += value[-pattern->corner2x5c];
        edge_d += value[-pattern->corner2x5d];
        eval += value[-pattern->corner2x5d];
        
        edge_d += value[-pattern->corner2x5e];
        eval += value[-pattern->corner2x5e];
        edge_a += value[-pattern->corner2x5f];
        eval += value[-pattern->corner2x5f];
        edge_b += value[-pattern->corner2x5g];
        eval += value[-pattern->corner2x5g];
        edge_c += value[-pattern->corner2x5h];
        eval += value[-pattern->corner2x5h];
        
        value = RXEvaluation::EDGE_2XC[stage];
        value_b = RXEvaluation::EDGE_6_4[stage];
        if(filled & 0x8142000000000000ULL) { //A1 H1 B2 G2
            edge_a += value[-pattern->edge2XCa];
            eval += value[-pattern->edge2XCa];
        } else {
            edge_a += value_b[-pattern->edge64a];
            eval += value_b[-pattern->edge64a];
        }

        if(filled & 0x0102000000000201ULL) {//H1 G2 G7 H8
            edge_b += value[-pattern->edge2XCb];
            eval += value[-pattern->edge2XCb];
        } else {
            edge_b += value_b[-pattern->edge64b];
            eval += value_b[-pattern->edge64b];
        }

        if(filled & 0x0000000000004281ULL) {   //B7 G7 A8 H8
            edge_c += value[-pattern->edge2XCc];
            eval += value[-pattern->edge2XCc];
        } else {
            edge_c += value_b[-pattern->edge64c];
            eval += value_b[-pattern->edge64c];
        }

        if(filled & 0x8040000000004080ULL) {  //A1 B2 B7 A8
            edge_d += value[-pattern->edge2XCd];
            eval += value[-pattern->edge2XCd];
        } else {
            edge_d += value_b[-pattern->edge64d];
            eval += value_b[-pattern->edge64d];
        }
        
        std::cout << "edge_a = " << edge_a << std::endl;
        std::cout << "edge_b = " << edge_b << std::endl;
        std::cout << "edge_c = " << edge_c << std::endl;
        std::cout << "edge_d = " << edge_d << std::endl;
    }
    
    std::cout << "Eval = " << eval << std::endl;
    
    // theorie des quantas
    eval >>= 3;
    if(eval < 0)
        return QUANTA * ((eval+1 -QUANTA/2)/QUANTA);
    else
        return QUANTA * ((eval   +QUANTA/2)/QUANTA);
    
    
}
 */



inline int RXBBPatterns::get_score() const {
    
    const int stage = 60-board.n_empties;
    
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
    
    // theorie des quantas
    eval >>= 3; 
//    if(eval < 0)
//        return QUANTA * ((eval+1 -QUANTA/2)/QUANTA);
//    else
//        return QUANTA * ((eval   +QUANTA/2)/QUANTA);
    
    return QUANTA * ((eval + (eval<0)*(1-QUANTA) + QUANTA/2)/QUANTA);

}


inline int RXBBPatterns::get_score(RXMove& move) const {
    
    const RXPattern* const p = move.pattern;
    
    const int stage = 61-board.n_empties;
    
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
    
    // theorie des quantas
    eval >>= 3; 
//    if(eval <  0)
//        return QUANTA * ((eval+1 -QUANTA/2)/QUANTA);
//    else
//        return QUANTA * ((eval   +QUANTA/2)/QUANTA);
    
    return QUANTA * ((eval + (eval<0)*(1-QUANTA) + QUANTA/2)/QUANTA);

}

#else





inline int RXBBPatterns::get_score() const {
    
    const int stage = 60-board.n_empties;
    
    int eval;
    const short* value;

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
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[pattern->corner11a];
        eval += value[pattern->corner11b];
        eval += value[pattern->corner11c];
        eval += value[pattern->corner11d];
 
        value = RXEvaluation::EDGE[stage];
        eval += value[pattern->edge_1];
        eval += value[pattern->edge_2];
        eval += value[pattern->edge_3];
        eval += value[pattern->edge_4];
        
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
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[-pattern->corner11a];
        eval += value[-pattern->corner11b];
        eval += value[-pattern->corner11c];
        eval += value[-pattern->corner11d];
        
        value = RXEvaluation::EDGE[stage];
        eval += value[-pattern->edge_1];
        eval += value[-pattern->edge_2];
        eval += value[-pattern->edge_3];
        eval += value[-pattern->edge_4];
        
        
    }
    
    // theorie des quantas
    eval >>= 3;
//    if(eval < 0)
//        return QUANTA * ((eval+1 -QUANTA/2)/QUANTA);
//    else
//        return QUANTA * ((eval   +QUANTA/2)/QUANTA);
    
    return QUANTA * ((eval + (eval<0)*(1-QUANTA) + QUANTA/2)/QUANTA);

}


inline int RXBBPatterns::get_score(RXMove& move) const {
    
    const RXPattern* const p = move.pattern;
    
    const int stage = 61-board.n_empties;
    
    int eval;
    const short* value;
    
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
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[p->corner11a];
        eval += value[p->corner11b];
        eval += value[p->corner11c];
        eval += value[p->corner11d];
        
        value = RXEvaluation::EDGE[stage];
        eval += value[p->edge_1];
        eval += value[p->edge_2];
        eval += value[p->edge_3];
        eval += value[p->edge_4];
        
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
        
        value = RXEvaluation::CORNER_11[stage];
        eval += value[-p->corner11a];
        eval += value[-p->corner11b];
        eval += value[-p->corner11c];
        eval += value[-p->corner11d];

        value = RXEvaluation::EDGE[stage];
        eval += value[-p->edge_1];
        eval += value[-p->edge_2];
        eval += value[-p->edge_3];
        eval += value[-p->edge_4];
        
    }
        
    // theorie des quantas
    eval >>= 3;
//    if(eval <  0)
//        return QUANTA * ((eval+1 -QUANTA/2)/QUANTA);
//    else
//        return QUANTA * ((eval   +QUANTA/2)/QUANTA);
    
    return QUANTA * ((eval + (eval<0)*(1-QUANTA) + QUANTA/2)/QUANTA);

}

/*
inline int RXBBPatterns::verif_score() const {
    
    const int stage = 60-board.n_empties;
    
    
    int eval;
    const short* value;
    
    if(board.player == BLACK) {
        
        value = RXEvaluation::DIAG_5[stage];
        std::cout << "     pattern->diag_5a = " << pattern->diag_5a << std::endl;
        std::cout << "diag_5a = " << value[pattern->diag_5a] << std::endl;
        eval  = value[pattern->diag_5a];

        std::cout << "     pattern->diag_5b = " << pattern->diag_5b << std::endl;
        std::cout << "diag_5b = " << value[pattern->diag_5b] << std::endl;
        eval += value[pattern->diag_5b];

        std::cout << "     pattern->diag_5c = " << pattern->diag_5c << std::endl;
        std::cout << "diag_5c = " << value[pattern->diag_5c] << std::endl;
        eval += value[pattern->diag_5c];

        std::cout << "     pattern->diag_5d = " << pattern->diag_5d << std::endl;
        std::cout << "diag_5d = " << value[pattern->diag_5d] << std::endl;
        eval += value[pattern->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        std::cout << "diag_6a = " << value[pattern->diag_6a] << std::endl;
        eval += value[pattern->diag_6a];
        
        std::cout << "diag_6b = " << value[pattern->diag_6b] << std::endl;
        eval += value[pattern->diag_6b];
        
        std::cout << "diag_6c = " << value[pattern->diag_6c] << std::endl;
        eval += value[pattern->diag_6c];
        
        std::cout << "diag_6d = " << value[pattern->diag_6d] << std::endl;
        eval += value[pattern->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        std::cout << "diag_7a = " << value[pattern->diag_7a] << std::endl;
        eval += value[pattern->diag_7a];
        
        std::cout << "diag_7b = " << value[pattern->diag_7b] << std::endl;
        eval += value[pattern->diag_7b];
        
        std::cout << "diag_7c = " << value[pattern->diag_7c] << std::endl;
        eval += value[pattern->diag_7c];
        
        std::cout << "diag_7d = " << value[pattern->diag_7d] << std::endl;
        eval += value[pattern->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        std::cout << "diag_8a = " << value[pattern->diag_8a] << std::endl;
        eval += value[pattern->diag_8a];
        
        std::cout << "diag_8b = " << value[pattern->diag_8b] << std::endl;
        eval += value[pattern->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        std::cout << "hv4_a = " << value[pattern->hv_4a] << std::endl;
        eval += value[pattern->hv_4a];
        
        std::cout << "hv4_b = " << value[pattern->hv_4b] << std::endl;
        eval += value[pattern->hv_4b];
        
        std::cout << "hv4_c = " << value[pattern->hv_4c] << std::endl;
        eval += value[pattern->hv_4c];
        
        std::cout << "hv4_d = " << value[pattern->hv_4d] << std::endl;
        eval += value[pattern->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        std::cout << "hv3_a = " << value[pattern->hv_3a] << std::endl;
        eval += value[pattern->hv_3a];
        
        std::cout << "hv3_b = " << value[pattern->hv_3b] << std::endl;
        eval += value[pattern->hv_3b];
        
        std::cout << "hv3_c = " << value[pattern->hv_3c] << std::endl;
        eval += value[pattern->hv_3c];
        
        std::cout << "hv3_d = " << value[pattern->hv_3d] << std::endl;
        eval += value[pattern->hv_3d];
               
        value = RXEvaluation::CORNER_11[stage];
        std::cout << "corner11a = " << value[pattern->corner11a] << std::endl;
        eval += value[pattern->corner11a];
        
        std::cout << "corner11b = " << value[pattern->corner11b] << std::endl;
        eval += value[pattern->corner11b];
        
        std::cout << "corner11c = " << value[pattern->corner11c] << std::endl;
        eval += value[pattern->corner11c];
        
        std::cout << "corner11d = " << value[pattern->corner11d] << std::endl;
        eval += value[pattern->corner11d];
        
        value = RXEvaluation::EDGE[stage];
        std::cout << "     edge_a = " << value[pattern->edge_1] << std::endl;
        std::cout << "     pattern->edge_a = " << pattern->edge_1 << std::endl;

        eval += value[pattern->edge_1];
        std::cout << "edge_b = " << value[pattern->edge_2] << std::endl;
        eval += value[pattern->edge_2];
        std::cout << "edge_c = " << value[pattern->edge_3] << std::endl;
        eval += value[pattern->edge_3];
        std::cout << "edge_d = " << value[pattern->edge_4] << std::endl;
        eval += value[pattern->edge_4];


    } else {
        
        value = RXEvaluation::DIAG_5[stage];
        std::cout << "diag_5a = " << value[-pattern->diag_5a] << std::endl;
        eval  = value[-pattern->diag_5a];
        
        std::cout << "diag_5b = " << value[-pattern->diag_5b] << std::endl;
        eval += value[-pattern->diag_5b];

        std::cout << "diag_5c = " << value[-pattern->diag_5c] << std::endl;
        eval += value[-pattern->diag_5c];

        std::cout << "diag_5d = " << value[-pattern->diag_5d] << std::endl;
        eval += value[-pattern->diag_5d];
        
        value = RXEvaluation::DIAG_6[stage];
        std::cout << "diag_6a = " << value[-pattern->diag_6a] << std::endl;
        eval += value[-pattern->diag_6a];
        
        std::cout << "diag_6b = " << value[-pattern->diag_6b] << std::endl;
        eval += value[-pattern->diag_6b];
        
        std::cout << "diag_6c = " << value[-pattern->diag_6c] << std::endl;
        eval += value[-pattern->diag_6c];
        
        std::cout << "diag_6d = " << value[-pattern->diag_6d] << std::endl;
        eval += value[-pattern->diag_6d];
        
        value = RXEvaluation::DIAG_7[stage];
        std::cout << "diag_7a = " << value[-pattern->diag_7a] << std::endl;
        eval += value[-pattern->diag_7a];
        
        std::cout << "diag_7b = " << value[-pattern->diag_7b] << std::endl;
        eval += value[-pattern->diag_7b];
        
        std::cout << "diag_7c = " << value[-pattern->diag_7c] << std::endl;
        eval += value[-pattern->diag_7c];
        
        std::cout << "diag_7d = " << value[-pattern->diag_7d] << std::endl;
        eval += value[-pattern->diag_7d];
        
        value = RXEvaluation::DIAG_8[stage];
        std::cout << "diag_8a = " << value[-pattern->diag_8a] << std::endl;
        eval += value[-pattern->diag_8a];
        
        std::cout << "diag_8b = " << value[-pattern->diag_8b] << std::endl;
        eval += value[-pattern->diag_8b];
        
        value = RXEvaluation::HV_4[stage];
        std::cout << "hv4_a = " << value[-pattern->hv_4a] << std::endl;
        eval += value[-pattern->hv_4a];
        
        std::cout << "hv4_b = " << value[-pattern->hv_4b] << std::endl;
        eval += value[-pattern->hv_4b];
        
        std::cout << "hv4_c = " << value[-pattern->hv_4c] << std::endl;
        eval += value[-pattern->hv_4c];
        
        std::cout << "hv4_d = " << value[-pattern->hv_4d] << std::endl;
        eval += value[-pattern->hv_4d];
        
        value = RXEvaluation::HV_3[stage];
        std::cout << "hv3_a = " << value[-pattern->hv_3a] << std::endl;
        eval += value[-pattern->hv_3a];
        
        std::cout << "hv3_b = " << value[-pattern->hv_3b] << std::endl;
        eval += value[-pattern->hv_3b];
        
        std::cout << "hv3_c = " << value[-pattern->hv_3c] << std::endl;
        eval += value[-pattern->hv_3c];
        
        std::cout << "hv3_d = " << value[-pattern->hv_3d] << std::endl;
        eval += value[-pattern->hv_3d];
               
        value = RXEvaluation::CORNER_11[stage];
        std::cout << "corner11a = " << value[-pattern->corner11a] << std::endl;
        eval += value[-pattern->corner11a];
        
        std::cout << "corner11b = " << value[-pattern->corner11b] << std::endl;
        eval += value[-pattern->corner11b];
        
        std::cout << "corner11c = " << value[-pattern->corner11c] << std::endl;
        eval += value[-pattern->corner11c];
        
        std::cout << "corner11d = " << value[-pattern->corner11d] << std::endl;
        eval += value[-pattern->corner11d];
        
        value = RXEvaluation::EDGE[stage];
        std::cout << "edge_a = " << value[-pattern->edge_1] << std::endl;
        eval += value[-pattern->edge_1];
        std::cout << "edge_b = " << value[-pattern->edge_2] << std::endl;
        eval += value[-pattern->edge_2];
        std::cout << "edge_c = " << value[-pattern->edge_3] << std::endl;
        eval += value[-pattern->edge_3];
        std::cout << "edge_d = " << value[-pattern->edge_4] << std::endl;
        eval += value[-pattern->edge_4];


    }
    
    std::cout << "Eval = " << eval << std::endl;
    
    // theorie des quantas
    eval >>= 3;
    if(eval < 0)
        return QUANTA * ((eval+1 -QUANTA/2)/QUANTA);
    else
        return QUANTA * ((eval   +QUANTA/2)/QUANTA);
    
    
}
 */

#endif






#endif
