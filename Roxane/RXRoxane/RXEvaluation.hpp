/*
 *  RXEvaluation.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 06/08/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXEVALUATION_HPP
#define RXEVALUATION_HPP

#include <iostream>

#include "RXConstantes.hpp"
#include "RXPattern.hpp"

class alignas(64) RXEvaluation {
    
    public :
    
    static void load();
    static std::string get_version() {
            return "G1 2026-01-29";
        }

    
    alignas(64) static inline short* eval[60][16];


    ~RXEvaluation() {
        

        for(unsigned int iStage = 0; iStage<60; iStage++) {
                        
            eval[iStage][0] -= 243/2;
            delete[] eval[iStage][0];
            
            eval[iStage][1] -= 729/2;
            delete[] eval[iStage][1];

            eval[iStage][2] -= 2187/2;
            delete[] eval[iStage][2];
            
            eval[iStage][3] -= 6561/2;
            delete[] eval[iStage][3];

            eval[iStage][4] -= 59049/2;
            delete[] eval[iStage][4];
            
            eval[iStage][5] -= 59049/2;
            delete[] eval[iStage][5];
            
            eval[iStage][6] -= 59049/2;
            delete[] eval[iStage][6];
            
            eval[iStage][7] -= 59049/2;
            delete[] eval[iStage][7];
            
            eval[iStage][8] -= 59049/2;
            delete[] eval[iStage][8];
            
            eval[iStage][9] -= 6561/2;
            delete[] eval[iStage][9];

            eval[iStage][10] -= 6561/2;
            delete[] eval[iStage][10];

            eval[iStage][11] -= 6561/2;
            delete[] eval[iStage][11];

            eval[iStage][12] -= 59049/2;
            delete[] eval[iStage][12];

            eval[iStage][13] -= 59049/2;
            delete[] eval[iStage][13];

            eval[iStage][14] -= 177147/2;
            delete[] eval[iStage][14];

            eval[iStage][15] -= 177147/2;
            delete[] eval[iStage][15];

        }

    };


        
};




#endif
