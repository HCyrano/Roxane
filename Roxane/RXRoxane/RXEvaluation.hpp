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
            return "J1 2026-02-04";
        }

    
    alignas(64) static inline short* eval[60][10];


    ~RXEvaluation() {
        

        for(unsigned int iStage = 0; iStage<60; iStage++) {
 
            delete[] eval[iStage][0];

            delete[] eval[iStage][1];

            eval[iStage][2] -= 243/2;
            delete[] eval[iStage][2];
            
            eval[iStage][3] -= 729/2;
            delete[] eval[iStage][3];

            eval[iStage][4] -= 2187/2;
            delete[] eval[iStage][4];
            
            eval[iStage][5] -= 6561/2;
            delete[] eval[iStage][5];

            eval[iStage][6] -= 43046721/2;
            delete[] eval[iStage][6];
            
            eval[iStage][7] -= 6561/2;
            delete[] eval[iStage][7];

            eval[iStage][8] -= 6561/2;
            delete[] eval[iStage][8];

            eval[iStage][9] -= 177147/2;
            delete[] eval[iStage][9];


        }

    };


        
};



#endif
