/*
 *  RXEvaluation.cpp
 *  Roxane
 *
 *  Created by Bruno Causse on 06/08/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#include <fstream>
#include <cmath>

#include "RXEvaluation.hpp"


short* RXEvaluation::DIAG_5[61];

short* RXEvaluation::DIAG_6[61];

short* RXEvaluation::DIAG_7[61];

short* RXEvaluation::DIAG_8[61];

short* RXEvaluation::HV_4[61];

short* RXEvaluation::HV_3[61];

short* RXEvaluation::CORNER_11[61];


#ifndef NEW_EVAL

short* RXEvaluation::HV_2[61];

short* RXEvaluation::EDGE_6_4[61];

short* RXEvaluation::corner2x5[61];

short* RXEvaluation::EDGE_2XC[61];


void RXEvaluation::load() {

	//create tables
    //std::ifstream from("/Users/caussebruno/Documents/developpement/Roxane/build/Coefficientsi386.data", std::ios::binary);
    //i5 2,7ghz
    std::ifstream from("../build/Coefficientsi386.data", std::ios::binary);
	if(from) {
		//chargement des donnees & decalage des tables
		for(unsigned int iStage = START; iStage<END; iStage++) {

			DIAG_5[iStage] = new short[243];
			from.read(reinterpret_cast<char*> (DIAG_5[iStage]), sizeof(short)*243);
			DIAG_5[iStage] += 121;
			
			DIAG_6[iStage] = new short[729];
			from.read(reinterpret_cast<char*> (DIAG_6[iStage]), sizeof(short)*729);
			DIAG_6[iStage] += 364;

			DIAG_7[iStage] = new short[2187];
			from.read(reinterpret_cast<char*> (DIAG_7[iStage]), sizeof(short)*2187);
			DIAG_7[iStage] += 1093;

			DIAG_8[iStage] = new short[6561];
			from.read(reinterpret_cast<char*> (DIAG_8[iStage]), sizeof(short)*6561);
			DIAG_8[iStage] += 3280;

			HV_4[iStage] = new short[6561];
			from.read(reinterpret_cast<char*> (HV_4[iStage]), sizeof(short)*6561);
			HV_4[iStage] += 3280;

			HV_3[iStage] = new short[6561];
			from.read(reinterpret_cast<char*> (HV_3[iStage]), sizeof(short)*6561);
			HV_3[iStage] += 3280;

			HV_2[iStage] = new short[6561];
			from.read(reinterpret_cast<char*> (HV_2[iStage]), sizeof(short)*6561);
			HV_2[iStage] += 3280;

			EDGE_6_4[iStage] = new short[59049];
			from.read(reinterpret_cast<char*> (EDGE_6_4[iStage]), sizeof(short)*59049);
			EDGE_6_4[iStage] += 29524;

			corner2x5[iStage] = new short[59049];
			from.read(reinterpret_cast<char*> (corner2x5[iStage]), sizeof(short)*59049);
			corner2x5[iStage] += 29524;

			CORNER_11[iStage] = new short[177147];
			from.read(reinterpret_cast<char*> (CORNER_11[iStage]), sizeof(short)*177147);
			CORNER_11[iStage] += 88573;

			EDGE_2XC[iStage] = new short[531441];
			from.read(reinterpret_cast<char*> (EDGE_2XC[iStage]), sizeof(short)*531441);
			EDGE_2XC[iStage] += 265720;
			
		}
	}
	from.close();
    

	// recopie des extremites
	
	//du stage 0 au START
	for(unsigned int iStage = 0; iStage<START; iStage++) {

		DIAG_5[iStage] = DIAG_5[START];
		DIAG_6[iStage] = DIAG_6[START];
		DIAG_7[iStage] = DIAG_7[START];
		DIAG_8[iStage] = DIAG_8[START];
		HV_4[iStage] = HV_4[START];
		HV_3[iStage] = HV_3[START];
		HV_2[iStage] = HV_2[START];
		EDGE_6_4[iStage] = EDGE_6_4[START];
		corner2x5[iStage] = corner2x5[START];
		CORNER_11[iStage] = CORNER_11[START];
		EDGE_2XC[iStage] = EDGE_2XC[START];
		
	}
	
	//du Stage END au 61
	for(unsigned int iStage = END; iStage<61; iStage++) {

		DIAG_5[iStage] = DIAG_5[END-1];
		DIAG_6[iStage] = DIAG_6[END-1];
		DIAG_7[iStage] = DIAG_7[END-1];
		DIAG_8[iStage] = DIAG_8[END-1];
		HV_4[iStage] = HV_4[END-1];
		HV_3[iStage] = HV_3[END-1];
		HV_2[iStage] = HV_2[END-1];
		EDGE_6_4[iStage] = EDGE_6_4[END-1];
		corner2x5[iStage] = corner2x5[END-1];
		CORNER_11[iStage] = CORNER_11[END-1];
		EDGE_2XC[iStage] = EDGE_2XC[END-1];
		
	}
    
}

#else

short* RXEvaluation::EDGE[61];


void RXEvaluation::load() {

    //create tables
    std::ifstream from("/Users/caussebruno/Documents/developpement/Roxane/build/CoefficientsARM.data", std::ios::binary);
    //std::ifstream from("../build/CoefficientsARM.data", std::ios::binary);

    if(from) {
        //chargement des donnees & decalage des tables
        for(unsigned int iStage = START; iStage<END; iStage++) {

            DIAG_5[iStage] = new short[243];
            from.read(reinterpret_cast<char*> (DIAG_5[iStage]), sizeof(short)*243);
            DIAG_5[iStage] += 121;
            
            DIAG_6[iStage] = new short[729];
            from.read(reinterpret_cast<char*> (DIAG_6[iStage]), sizeof(short)*729);
            DIAG_6[iStage] += 364;

            DIAG_7[iStage] = new short[2187];
            from.read(reinterpret_cast<char*> (DIAG_7[iStage]), sizeof(short)*2187);
            DIAG_7[iStage] += 1093;

            DIAG_8[iStage] = new short[6561];
            from.read(reinterpret_cast<char*> (DIAG_8[iStage]), sizeof(short)*6561);
            DIAG_8[iStage] += 3280;

            HV_4[iStage] = new short[6561];
            from.read(reinterpret_cast<char*> (HV_4[iStage]), sizeof(short)*6561);
            HV_4[iStage] += 3280;

            HV_3[iStage] = new short[6561];
            from.read(reinterpret_cast<char*> (HV_3[iStage]), sizeof(short)*6561);
            HV_3[iStage] += 3280;

            CORNER_11[iStage] = new short[177147];
            from.read(reinterpret_cast<char*> (CORNER_11[iStage]), sizeof(short)*177147);
            CORNER_11[iStage] += 88573;

            EDGE[iStage] = new short[43046721];
            from.read(reinterpret_cast<char*> (EDGE[iStage]), sizeof(short)*43046721);
            EDGE[iStage] += 21523360;
            
        }
    }
    
    from.close();

    
    // recopie des extremites
    
    //du stage 0 au START
    for(unsigned int iStage = 0; iStage<START; iStage++) {

        DIAG_5[iStage] = DIAG_5[START];
        DIAG_6[iStage] = DIAG_6[START];
        DIAG_7[iStage] = DIAG_7[START];
        DIAG_8[iStage] = DIAG_8[START];
        HV_4[iStage] = HV_4[START];
        HV_3[iStage] = HV_3[START];
        CORNER_11[iStage] = CORNER_11[START];
        EDGE[iStage] = EDGE[START];
        
    }
    
    //du Stage END au 61
    for(unsigned int iStage = END; iStage<61; iStage++) {

        DIAG_5[iStage] = DIAG_5[END-1];
        DIAG_6[iStage] = DIAG_6[END-1];
        DIAG_7[iStage] = DIAG_7[END-1];
        DIAG_8[iStage] = DIAG_8[END-1];
        HV_4[iStage] = HV_4[END-1];
        HV_3[iStage] = HV_3[END-1];
        CORNER_11[iStage] = CORNER_11[END-1];
        EDGE[iStage] = EDGE[END-1];
        
    }
    
}

#endif

/*
void RXEvaluation::new_eval() {
 
 
    std::ofstream to("/Users/caussebruno/Documents/othello/MPRoxane[2024-07-21]/build/CoefficientsARM.data", std::ios::binary);
    if(to) {
        for(unsigned int iStage = START; iStage<END; iStage++) {
            
            EDGE[iStage] = new short[43046721];
            EDGE[iStage] += 21523360;
                        
            for(int D2 = -1; D2<2; D2++) {
                for(int C2 = -1; C2<2; C2++) {
                    for(int B2 = -1; B2<2; B2++) {
                        for(int A2 = -1; A2<2; A2++) {
                            for(int A1 = -1; A1<2; A1++) {
                                for(int B1 = -1; B1<2; B1++) {
                                    for(int C1 = -1; C1<2; C1++) {
                                        for(int D1 = -1; D1<2; D1++) {
                                            for(int E1 = -1; E1<2; E1++) {
                                                for(int F1 = -1; F1<2; F1++) {
                                                    for(int G1 = -1; G1<2; G1++) {
                                                        for(int H1 = -1; H1<2; H1++) {
                                                            for(int H2 = -1; H2<2; H2++) {
                                                                for(int G2 = -1; G2<2; G2++) {
                                                                    for(int F2 = -1; F2<2; F2++) {
                                                                        for(int E2 = -1; E2<2; E2++) {
                                                                            
                                                                            int Edge2XC = ((((((((((((G2*3)+H2)*3+H1)*3+G1)*3+F1)*3)+E1)*3+D1)*3+C1)*3+B1)*3+A1)*3+A2)*3+B2;
                                                                            
                                                                            int Edge64 =  (((((((((E2*3)+F2)*3+G1)*3+F1)*3+E1)*3+D1)*3+C1)*3+B1)*3+C2)*3+D2;
                                                                            
                                                                            int Corner1 = (((((((((A2*3)+B2)*3+C2)*3+D2)*3+E2)*3+E1)*3+D1)*3+C1)*3+B1)*3+A1;
                                                                            
                                                                            int Corner2 = (((((((((H2*3)+G2)*3+F2)*3+E2)*3+D2)*3+D1)*3+E1)*3+F1)*3+G1)*3+H1;
                                                                            
                                                                            int hv2 = (((((((A2*3)+B2)*3+C2)*3+D2)*3+E2)*3+F2)*3+G2)*3+H2;
                                                                            
                                                                            int Edge = (((((((((((((((E2*3)+F2)*3+G2)*3+H2)*3+H1)*3+G1)*3+F1)*3+E1)*3+D1)*3+C1)*3+B1)*3+A1)*3+A2)*3+B2)*3+C2)*3+D2;
                                                                            
                                                                            EDGE[iStage][Edge] = corner2x5[iStage][Corner1] + corner2x5[iStage][Corner2] + HV_2[iStage][hv2];
                                                                            
                                                                            if(B2==0 && A1==0 && H1==0 && G2==0)
                                                                                EDGE[iStage][Edge] += EDGE_6_4[iStage][Edge64];
                                                                            else
                                                                                EDGE[iStage][Edge] += EDGE_2XC[iStage][Edge2XC];
                                                                            
                                                                            //verification Bornes short
                                                                            if(-32768 > EDGE[iStage][Edge] || EDGE[iStage][Edge] > 32767)
                                                                                std::cout << "hors bornes : " << EDGE[iStage][Edge] << std::endl;
                                                                            
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            //enregistrement
            DIAG_5[iStage] -= 121;
            to.write(reinterpret_cast<char*> (DIAG_5[iStage]), sizeof(short)*243);
            
            DIAG_6[iStage] -= 364;
            to.write(reinterpret_cast<char*> (DIAG_6[iStage]), sizeof(short)*729);

            DIAG_7[iStage] -= 1093;
            to.write(reinterpret_cast<char*> (DIAG_7[iStage]), sizeof(short)*2187);
            
            DIAG_8[iStage] -= 3280;
            to.write(reinterpret_cast<char*> (DIAG_8[iStage]), sizeof(short)*6561);

            HV_4[iStage] -= 3280;
            to.write(reinterpret_cast<char*> (HV_4[iStage]), sizeof(short)*6561);

            HV_3[iStage] -= 3280;
            to.write(reinterpret_cast<char*> (HV_3[iStage]), sizeof(short)*6561);

            CORNER_11[iStage] -= 88573;
            to.write(reinterpret_cast<char*> (CORNER_11[iStage]), sizeof(short)*177147);
            
            EDGE[iStage] -= 21523360;
            to.write(reinterpret_cast<char*> (EDGE[iStage]), sizeof(short)*43046721);

            
        }
        
    }
    
    to.close();
    
    
}

*/





