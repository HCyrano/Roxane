/*
 *  RXEvaluation.cpp
 *  Roxane
 *
 *  Created by Bruno Causse on 06/08/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#include <fstream>

#include "RXEvaluation.hpp"


void RXEvaluation::load() {
    
    //create tables
    
    std::ifstream from("/Users/caussebruno/Documents/developpement/Roxane/build/eval_v5.1.bin", std::ios::binary);
    if(from) {
        
        //chargement des donnees & decalage des tables
        for(unsigned int iStage = 0; iStage<60; iStage++) {
            
            //diag 6 id_patt = 0
            eval[iStage][0] = new short[729];
            from.read(reinterpret_cast<char*> (eval[iStage][0]), sizeof(short)*729);
            eval[iStage][0] += 729/2;
            
            //diag 7 id_patt = 1
            eval[iStage][1] = new short[2187];
            from.read(reinterpret_cast<char*> (eval[iStage][1]), sizeof(short)*2187);
            eval[iStage][1] += 2187/2;
            
            //diag 8 id_patt = 2
            eval[iStage][2] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][2]), sizeof(short)*6561);
            eval[iStage][2] += 6561/2;
            
            //edge 8+8 id_patt = 3
            eval[iStage][3] = new short[43046721];
            from.read(reinterpret_cast<char*> (eval[iStage][3]), sizeof(short)*43046721);
            eval[iStage][3] += 43046721/2;

            //hv_3 id_patt = 4
            eval[iStage][4] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][4]), sizeof(short)*6561);
            eval[iStage][4] += 6561/2;
            
            //hv_4 id_patt = 5
            eval[iStage][5] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][5]), sizeof(short)*6561);
            eval[iStage][5] += 6561/2;
            
            // corner 5/4/3/2/1 id_patt = 6
            eval[iStage][6] = new short[14348907];
            from.read(reinterpret_cast<char*> (eval[iStage][6]), sizeof(short)*14348907);
            eval[iStage][6] += 14348907/2;
            
            //hyper diag id_patt = 7
            eval[iStage][7] = new short[59049];
            from.read(reinterpret_cast<char*> (eval[iStage][7]), sizeof(short)*59049);
            eval[iStage][7] += 59049/2;

            //edge 4/2/4 id_patt = 8
            eval[iStage][8] = new short[59049];
            from.read(reinterpret_cast<char*> (eval[iStage][8]), sizeof(short)*59049);
            eval[iStage][8] += 59049/2;

        }
    } else {
        std::cout << "erreur chargement eval" << std::endl;
    }

    from.close();

    
};







