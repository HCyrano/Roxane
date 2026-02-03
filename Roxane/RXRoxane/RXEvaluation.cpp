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
    
    std::ifstream from("/Users/caussebruno/Documents/developpement/Roxane/build/weight_v9.bin", std::ios::binary);
    if(from) {
        
        //chargement des donnees & decalage des tables
        for(unsigned int iStage = 0; iStage<60; iStage++) {

            //mobility player id_patt = 0
            eval[iStage][0] = new short[24];
            from.read(reinterpret_cast<char*> (eval[iStage][0]), sizeof(short)*24);

            //mobility opponent id_patt = 1
            eval[iStage][1] = new short[24];
            from.read(reinterpret_cast<char*> (eval[iStage][1]), sizeof(short)*24);

            //diag 5 id_patt = 2
            eval[iStage][2] = new short[243];
            from.read(reinterpret_cast<char*> (eval[iStage][2]), sizeof(short)*243);
            eval[iStage][2] += 243/2;

            //diag 6 id_patt = 3
            eval[iStage][3] = new short[729];
            from.read(reinterpret_cast<char*> (eval[iStage][3]), sizeof(short)*729);
            eval[iStage][3] += 729/2;
            
            //diag 7 id_patt = 4
            eval[iStage][4] = new short[2187];
            from.read(reinterpret_cast<char*> (eval[iStage][4]), sizeof(short)*2187);
            eval[iStage][4] += 2187/2;
            
            //diag 8 id_patt = 5
            eval[iStage][5] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][5]), sizeof(short)*6561);
            eval[iStage][5] += 6561/2;
            
            //edge+2X id_patt = 6
            eval[iStage][6] = new short[59049];
            from.read(reinterpret_cast<char*> (eval[iStage][6]), sizeof(short)*59049);
            eval[iStage][6] += 59049/2;

            //edge alternate 6+4 id_patt = 7
            eval[iStage][7] = new short[59049];
            from.read(reinterpret_cast<char*> (eval[iStage][7]), sizeof(short)*59049);
            eval[iStage][7] += 59049/2;
            
            //edge 2*(3/2) id_patt = 8
            eval[iStage][8] = new short[59049];
            from.read(reinterpret_cast<char*> (eval[iStage][8]), sizeof(short)*59049);
            eval[iStage][8] += 59049/2;
            
            //edge 2*5 id_patt = 9
            eval[iStage][9] = new short[59049];
            from.read(reinterpret_cast<char*> (eval[iStage][9]), sizeof(short)*59049);
            eval[iStage][9] += 59049/2;

            //hv2 patt = 10
            eval[iStage][10] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][10]), sizeof(short)*6561);
            eval[iStage][10] += 6561/2;

            //hv_3 id_patt = 11
            eval[iStage][11] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][11]), sizeof(short)*6561);
            eval[iStage][11] += 6561/2;
            
            //hv_4 id_patt = 12
            eval[iStage][12] = new short[6561];
            from.read(reinterpret_cast<char*> (eval[iStage][12]), sizeof(short)*6561);
            eval[iStage][12] += 6561/2;
            
            // corner 4/3/3/1 id_patt = 13
            eval[iStage][13] = new short[177147];
            from.read(reinterpret_cast<char*> (eval[iStage][13]), sizeof(short)*177147);
            eval[iStage][13] += 177147/2;
                        

        }
    } else {
        std::cout << "erreur chargement eval" << std::endl;
    }

    from.close();

    
};







