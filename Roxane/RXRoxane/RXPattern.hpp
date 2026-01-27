/*
 *  RXPattern.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 20/09/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXPATTERN_HPP
#define RXPATTERN_HPP

#include <iostream>



class alignas(16) RXPattern {
    
    static const unsigned int N_PATTERNS = 38;
        
public:
    
    int patt[N_PATTERNS];

    
    //set Black disc
    void set_BLACK_A1();  void set_BLACK_B1();  void set_BLACK_C1();  void set_BLACK_D1();  void set_BLACK_E1();  void set_BLACK_F1();  void set_BLACK_G1();  void set_BLACK_H1();
    void set_BLACK_A2();  void set_BLACK_B2();  void set_BLACK_C2();  void set_BLACK_D2();  void set_BLACK_E2();  void set_BLACK_F2();  void set_BLACK_G2();  void set_BLACK_H2();
    void set_BLACK_A3();  void set_BLACK_B3();  void set_BLACK_C3();  void set_BLACK_D3();  void set_BLACK_E3();  void set_BLACK_F3();  void set_BLACK_G3();  void set_BLACK_H3();
    void set_BLACK_A4();  void set_BLACK_B4();  void set_BLACK_C4();  void set_BLACK_D4();  void set_BLACK_E4();  void set_BLACK_F4();  void set_BLACK_G4();  void set_BLACK_H4();
    void set_BLACK_A5();  void set_BLACK_B5();  void set_BLACK_C5();  void set_BLACK_D5();  void set_BLACK_E5();  void set_BLACK_F5();  void set_BLACK_G5();  void set_BLACK_H5();
    void set_BLACK_A6();  void set_BLACK_B6();  void set_BLACK_C6();  void set_BLACK_D6();  void set_BLACK_E6();  void set_BLACK_F6();  void set_BLACK_G6();  void set_BLACK_H6();
    void set_BLACK_A7();  void set_BLACK_B7();  void set_BLACK_C7();  void set_BLACK_D7();  void set_BLACK_E7();  void set_BLACK_F7();  void set_BLACK_G7();  void set_BLACK_H7();
    void set_BLACK_A8();  void set_BLACK_B8();  void set_BLACK_C8();  void set_BLACK_D8();  void set_BLACK_E8();  void set_BLACK_F8();  void set_BLACK_G8();  void set_BLACK_H8();
    
    //set White disc
    void set_WHITE_A1();  void set_WHITE_B1();  void set_WHITE_C1();  void set_WHITE_D1();  void set_WHITE_E1();  void set_WHITE_F1();  void set_WHITE_G1();  void set_WHITE_H1();
    void set_WHITE_A2();  void set_WHITE_B2();  void set_WHITE_C2();  void set_WHITE_D2();  void set_WHITE_E2();  void set_WHITE_F2();  void set_WHITE_G2();  void set_WHITE_H2();
    void set_WHITE_A3();  void set_WHITE_B3();  void set_WHITE_C3();  void set_WHITE_D3();  void set_WHITE_E3();  void set_WHITE_F3();  void set_WHITE_G3();  void set_WHITE_H3();
    void set_WHITE_A4();  void set_WHITE_B4();  void set_WHITE_C4();  void set_WHITE_D4();  void set_WHITE_E4();  void set_WHITE_F4();  void set_WHITE_G4();  void set_WHITE_H4();
    void set_WHITE_A5();  void set_WHITE_B5();  void set_WHITE_C5();  void set_WHITE_D5();  void set_WHITE_E5();  void set_WHITE_F5();  void set_WHITE_G5();  void set_WHITE_H5();
    void set_WHITE_A6();  void set_WHITE_B6();  void set_WHITE_C6();  void set_WHITE_D6();  void set_WHITE_E6();  void set_WHITE_F6();  void set_WHITE_G6();  void set_WHITE_H6();
    void set_WHITE_A7();  void set_WHITE_B7();  void set_WHITE_C7();  void set_WHITE_D7();  void set_WHITE_E7();  void set_WHITE_F7();  void set_WHITE_G7();  void set_WHITE_H7();
    void set_WHITE_A8();  void set_WHITE_B8();  void set_WHITE_C8();  void set_WHITE_D8();  void set_WHITE_E8();  void set_WHITE_F8();  void set_WHITE_G8();  void set_WHITE_H8();
    
    //flip Black disc
                           void flip_BLACK_B1();  void flip_BLACK_C1();  void flip_BLACK_D1();  void flip_BLACK_E1();  void flip_BLACK_F1();  void flip_BLACK_G1();
    void flip_BLACK_A2();  void flip_BLACK_B2();  void flip_BLACK_C2();  void flip_BLACK_D2();  void flip_BLACK_E2();  void flip_BLACK_F2();  void flip_BLACK_G2();  void flip_BLACK_H2();
    void flip_BLACK_A3();  void flip_BLACK_B3();  void flip_BLACK_C3();  void flip_BLACK_D3();  void flip_BLACK_E3();  void flip_BLACK_F3();  void flip_BLACK_G3();  void flip_BLACK_H3();
    void flip_BLACK_A4();  void flip_BLACK_B4();  void flip_BLACK_C4();  void flip_BLACK_D4();  void flip_BLACK_E4();  void flip_BLACK_F4();  void flip_BLACK_G4();  void flip_BLACK_H4();
    void flip_BLACK_A5();  void flip_BLACK_B5();  void flip_BLACK_C5();  void flip_BLACK_D5();  void flip_BLACK_E5();  void flip_BLACK_F5();  void flip_BLACK_G5();  void flip_BLACK_H5();
    void flip_BLACK_A6();  void flip_BLACK_B6();  void flip_BLACK_C6();  void flip_BLACK_D6();  void flip_BLACK_E6();  void flip_BLACK_F6();  void flip_BLACK_G6();  void flip_BLACK_H6();
    void flip_BLACK_A7();  void flip_BLACK_B7();  void flip_BLACK_C7();  void flip_BLACK_D7();  void flip_BLACK_E7();  void flip_BLACK_F7();  void flip_BLACK_G7();  void flip_BLACK_H7();
                           void flip_BLACK_B8();  void flip_BLACK_C8();  void flip_BLACK_D8();  void flip_BLACK_E8();  void flip_BLACK_F8();  void flip_BLACK_G8();
    
    //flip White disc
                           void flip_WHITE_B1();  void flip_WHITE_C1();  void flip_WHITE_D1();  void flip_WHITE_E1();  void flip_WHITE_F1();  void flip_WHITE_G1();
    void flip_WHITE_A2();  void flip_WHITE_B2();  void flip_WHITE_C2();  void flip_WHITE_D2();  void flip_WHITE_E2();  void flip_WHITE_F2();  void flip_WHITE_G2();  void flip_WHITE_H2();
    void flip_WHITE_A3();  void flip_WHITE_B3();  void flip_WHITE_C3();  void flip_WHITE_D3();  void flip_WHITE_E3();  void flip_WHITE_F3();  void flip_WHITE_G3();  void flip_WHITE_H3();
    void flip_WHITE_A4();  void flip_WHITE_B4();  void flip_WHITE_C4();  void flip_WHITE_D4();  void flip_WHITE_E4();  void flip_WHITE_F4();  void flip_WHITE_G4();  void flip_WHITE_H4();
    void flip_WHITE_A5();  void flip_WHITE_B5();  void flip_WHITE_C5();  void flip_WHITE_D5();  void flip_WHITE_E5();  void flip_WHITE_F5();  void flip_WHITE_G5();  void flip_WHITE_H5();
    void flip_WHITE_A6();  void flip_WHITE_B6();  void flip_WHITE_C6();  void flip_WHITE_D6();  void flip_WHITE_E6();  void flip_WHITE_F6();  void flip_WHITE_G6();  void flip_WHITE_H6();
    void flip_WHITE_A7();  void flip_WHITE_B7();  void flip_WHITE_C7();  void flip_WHITE_D7();  void flip_WHITE_E7();  void flip_WHITE_F7();  void flip_WHITE_G7();  void flip_WHITE_H7();
                           void flip_WHITE_B8();  void flip_WHITE_C8();  void flip_WHITE_D8();  void flip_WHITE_E8();  void flip_WHITE_F8();  void flip_WHITE_G8();
    
    void clear();
    
    /* DEBUG */
    friend std::ostream& operator<<(std::ostream& os, RXPattern& pattern);

    
};

inline void RXPattern::clear() {
    
    for(int i = 0; i< N_PATTERNS; ++i)
        patt[i] = 0;
    
}


inline void RXPattern::set_BLACK_A1() { patt[12]  -=       1; patt[14]  -=      27; patt[18]  -=      81; patt[21]  -=  177147; patt[34]  -=      81; };
inline void RXPattern::set_BLACK_B1() { patt[ 9]  -=       1; patt[14]  -=    2187; patt[18]  -=     243; patt[21]  -=  531441; patt[34]  -=    2187; };
inline void RXPattern::set_BLACK_C1() { patt[ 5]  -=       1; patt[18]  -=     729; patt[22]  -=      27; patt[29]  -=    2187; patt[34]  -=    6561; };
inline void RXPattern::set_BLACK_D1() { patt[ 1]  -=       1; patt[18]  -=    2187; patt[22]  -=      81; patt[33]  -=    2187; patt[34]  -=   59049; };
inline void RXPattern::set_BLACK_E1() { patt[ 0]  -=      81; patt[18]  -=    6561; patt[22]  -=     243; patt[31]  -=       1; patt[35]  -=       1; };
inline void RXPattern::set_BLACK_F1() { patt[ 4]  -=     243; patt[18]  -=   19683; patt[22]  -=     729; patt[27]  -=       1; patt[35]  -=       9; };
inline void RXPattern::set_BLACK_G1() { patt[ 8]  -=     729; patt[15]  -=       9; patt[18]  -=   59049; patt[19]  -=      27; patt[35]  -=      27; };
inline void RXPattern::set_BLACK_H1() { patt[13]  -=    2187; patt[15]  -=      27; patt[18]  -=  177147; patt[19]  -=      81; patt[35]  -=      81; };

inline void RXPattern::set_BLACK_A2() { patt[11]  -=     729; patt[14]  -=       9; patt[18]  -=      27; patt[21]  -=   59049; patt[34]  -=      27; };
inline void RXPattern::set_BLACK_B2() { patt[12]  -=       3; patt[14]  -=      81; patt[18]  -=       9; patt[21]  -= 1594323; patt[34]  -=     243; };
inline void RXPattern::set_BLACK_C2() { patt[ 9]  -=       3; patt[14]  -=    6561; patt[18]  -=       3; patt[29]  -=     729; patt[34]  -=   19683; };
inline void RXPattern::set_BLACK_D2() { patt[ 0]  -=      27; patt[ 5]  -=       3; patt[18]  -=       1; patt[22]  -=       9; patt[33]  -=     729; };
inline void RXPattern::set_BLACK_E2() { patt[ 1]  -=       3; patt[ 4]  -=      81; patt[18]  -= 14348907; patt[22]  -=    2187; patt[31]  -=       3; };
inline void RXPattern::set_BLACK_F2() { patt[ 8]  -=     243; patt[15]  -=       3; patt[18]  -= 4782969; patt[27]  -=       3; patt[35]  -=       3; };
inline void RXPattern::set_BLACK_G2() { patt[13]  -=     729; patt[15]  -=      81; patt[18]  -= 1594323; patt[19]  -=       9; patt[35]  -=     243; };
inline void RXPattern::set_BLACK_H2() { patt[10]  -=       1; patt[15]  -=    2187; patt[18]  -=  531441; patt[19]  -=     243; patt[35]  -=    2187; };

inline void RXPattern::set_BLACK_A3() { patt[ 7]  -=     243; patt[21]  -=   19683; patt[25]  -=     729; patt[26]  -=       1; patt[34]  -=       9; };
inline void RXPattern::set_BLACK_B3() { patt[11]  -=     243; patt[14]  -=       3; patt[21]  -= 4782969; patt[26]  -=       3; patt[34]  -=       3; };
inline void RXPattern::set_BLACK_C3() { patt[ 0]  -=       9; patt[12]  -=       9; patt[14]  -=     243; patt[22]  -=       3; patt[25]  -=    6561; patt[26]  -=       9; patt[29]  -=     243; patt[34]  -=     729; };
inline void RXPattern::set_BLACK_D3() { patt[ 4]  -=      27; patt[ 9]  -=       9; patt[14]  -=   19683; patt[22]  -=       1; patt[26]  -=      27; patt[33]  -=     243; };
inline void RXPattern::set_BLACK_E3() { patt[ 5]  -=       9; patt[ 8]  -=      81; patt[15]  -=       1; patt[22]  -=   19683; patt[26]  -=      81; patt[31]  -=       9; };
inline void RXPattern::set_BLACK_F3() { patt[ 1]  -=       9; patt[13]  -=     243; patt[15]  -=     243; patt[22]  -=    6561; patt[23]  -=       3; patt[26]  -=     243; patt[27]  -=       9; patt[35]  -=     729; };
inline void RXPattern::set_BLACK_G3() { patt[10]  -=       3; patt[15]  -=    6561; patt[19]  -=       3; patt[26]  -=     729; patt[35]  -=   19683; };
inline void RXPattern::set_BLACK_H3() { patt[ 6]  -=       1; patt[19]  -=     729; patt[23]  -=      27; patt[26]  -=    2187; patt[35]  -=    6561; };

inline void RXPattern::set_BLACK_A4() { patt[ 3]  -=      81; patt[21]  -=    6561; patt[25]  -=     243; patt[30]  -=       1; patt[34]  -=       1; };
inline void RXPattern::set_BLACK_B4() { patt[ 0]  -=       3; patt[ 7]  -=      81; patt[21]  -= 14348907; patt[25]  -=    2187; patt[30]  -=       3; };
inline void RXPattern::set_BLACK_C4() { patt[ 4]  -=       9; patt[11]  -=      81; patt[14]  -=       1; patt[25]  -=   19683; patt[29]  -=      81; patt[30]  -=       9; };
inline void RXPattern::set_BLACK_D4() { patt[ 8]  -=      27; patt[12]  -=      27; patt[14]  -=     729; patt[30]  -=      27; patt[33]  -=      81; };
inline void RXPattern::set_BLACK_E4() { patt[ 9]  -=      27; patt[13]  -=      81; patt[15]  -=     729; patt[30]  -=      81; patt[31]  -=      27; };
inline void RXPattern::set_BLACK_F4() { patt[ 5]  -=      27; patt[10]  -=       9; patt[15]  -=   19683; patt[23]  -=       1; patt[27]  -=      27; patt[30]  -=     243; };
inline void RXPattern::set_BLACK_G4() { patt[ 1]  -=      27; patt[ 6]  -=       3; patt[19]  -=       1; patt[23]  -=       9; patt[30]  -=     729; };
inline void RXPattern::set_BLACK_H4() { patt[ 2]  -=       1; patt[19]  -=    2187; patt[23]  -=      81; patt[30]  -=    2187; patt[35]  -=   59049; };

inline void RXPattern::set_BLACK_A5() { patt[ 0]  -=       1; patt[21]  -=    2187; patt[25]  -=      81; patt[32]  -=    2187; patt[37]  -=   59049; };
inline void RXPattern::set_BLACK_B5() { patt[ 3]  -=      27; patt[ 4]  -=       3; patt[21]  -=       1; patt[25]  -=       9; patt[32]  -=     729; };
inline void RXPattern::set_BLACK_C5() { patt[ 7]  -=      27; patt[ 8]  -=       9; patt[17]  -=   19683; patt[25]  -=       1; patt[29]  -=      27; patt[32]  -=     243; };
inline void RXPattern::set_BLACK_D5() { patt[11]  -=      27; patt[13]  -=      27; patt[17]  -=     729; patt[32]  -=      81; patt[33]  -=      27; };
inline void RXPattern::set_BLACK_E5() { patt[10]  -=      27; patt[12]  -=      81; patt[16]  -=     729; patt[31]  -=      81; patt[32]  -=      27; };
inline void RXPattern::set_BLACK_F5() { patt[ 6]  -=       9; patt[ 9]  -=      81; patt[16]  -=       1; patt[23]  -=   19683; patt[27]  -=      81; patt[32]  -=       9; };
inline void RXPattern::set_BLACK_G5() { patt[ 2]  -=       3; patt[ 5]  -=      81; patt[19]  -= 14348907; patt[23]  -=    2187; patt[32]  -=       3; };
inline void RXPattern::set_BLACK_H5() { patt[ 1]  -=      81; patt[19]  -=    6561; patt[23]  -=     243; patt[32]  -=       1; patt[36]  -=       1; };

inline void RXPattern::set_BLACK_A6() { patt[ 4]  -=       1; patt[21]  -=     729; patt[25]  -=      27; patt[28]  -=    2187; patt[37]  -=    6561; };
inline void RXPattern::set_BLACK_B6() { patt[ 8]  -=       3; patt[17]  -=    6561; patt[21]  -=       3; patt[28]  -=     729; patt[37]  -=   19683; };
inline void RXPattern::set_BLACK_C6() { patt[ 3]  -=       9; patt[13]  -=       9; patt[17]  -=     243; patt[24]  -=    6561; patt[25]  -=       3; patt[28]  -=     243; patt[29]  -=       9; patt[37]  -=     729; };
inline void RXPattern::set_BLACK_D6() { patt[ 7]  -=       9; patt[10]  -=      81; patt[17]  -=       1; patt[24]  -=   19683; patt[28]  -=      81; patt[33]  -=       9; };
inline void RXPattern::set_BLACK_E6() { patt[ 6]  -=      27; patt[11]  -=       9; patt[16]  -=   19683; patt[24]  -=       1; patt[28]  -=      27; patt[31]  -=     243; };
inline void RXPattern::set_BLACK_F6() { patt[ 2]  -=       9; patt[12]  -=     243; patt[16]  -=     243; patt[23]  -=    6561; patt[24]  -=       3; patt[27]  -=     243; patt[28]  -=       9; patt[36]  -=     729; };
inline void RXPattern::set_BLACK_G6() { patt[ 9]  -=     243; patt[16]  -=       3; patt[19]  -= 4782969; patt[28]  -=       3; patt[36]  -=       3; };
inline void RXPattern::set_BLACK_H6() { patt[ 5]  -=     243; patt[19]  -=   19683; patt[23]  -=     729; patt[28]  -=       1; patt[36]  -=       9; };

inline void RXPattern::set_BLACK_A7() { patt[ 8]  -=       1; patt[17]  -=    2187; patt[20]  -=  531441; patt[21]  -=     243; patt[37]  -=    2187; };
inline void RXPattern::set_BLACK_B7() { patt[13]  -=       3; patt[17]  -=      81; patt[20]  -= 1594323; patt[21]  -=       9; patt[37]  -=     243; };
inline void RXPattern::set_BLACK_C7() { patt[10]  -=     243; patt[17]  -=       3; patt[20]  -= 4782969; patt[29]  -=       3; patt[37]  -=       3; };
inline void RXPattern::set_BLACK_D7() { patt[ 3]  -=       3; patt[ 6]  -=      81; patt[20]  -= 14348907; patt[24]  -=    2187; patt[33]  -=       3; };
inline void RXPattern::set_BLACK_E7() { patt[ 2]  -=      27; patt[ 7]  -=       3; patt[20]  -=       1; patt[24]  -=       9; patt[31]  -=     729; };
inline void RXPattern::set_BLACK_F7() { patt[11]  -=       3; patt[16]  -=    6561; patt[20]  -=       3; patt[27]  -=     729; patt[36]  -=   19683; };
inline void RXPattern::set_BLACK_G7() { patt[12]  -=     729; patt[16]  -=      81; patt[19]  -= 1594323; patt[20]  -=       9; patt[36]  -=     243; };
inline void RXPattern::set_BLACK_H7() { patt[ 9]  -=     729; patt[16]  -=       9; patt[19]  -=   59049; patt[20]  -=      27; patt[36]  -=      27; };

inline void RXPattern::set_BLACK_A8() { patt[13]  -=       1; patt[17]  -=      27; patt[20]  -=  177147; patt[21]  -=      81; patt[37]  -=      81; };
inline void RXPattern::set_BLACK_B8() { patt[10]  -=     729; patt[17]  -=       9; patt[20]  -=   59049; patt[21]  -=      27; patt[37]  -=      27; };
inline void RXPattern::set_BLACK_C8() { patt[ 6]  -=     243; patt[20]  -=   19683; patt[24]  -=     729; patt[29]  -=       1; patt[37]  -=       9; };
inline void RXPattern::set_BLACK_D8() { patt[ 2]  -=      81; patt[20]  -=    6561; patt[24]  -=     243; patt[33]  -=       1; patt[37]  -=       1; };
inline void RXPattern::set_BLACK_E8() { patt[ 3]  -=       1; patt[20]  -=    2187; patt[24]  -=      81; patt[31]  -=    2187; patt[36]  -=   59049; };
inline void RXPattern::set_BLACK_F8() { patt[ 7]  -=       1; patt[20]  -=     729; patt[24]  -=      27; patt[27]  -=    2187; patt[36]  -=    6561; };
inline void RXPattern::set_BLACK_G8() { patt[11]  -=       1; patt[16]  -=    2187; patt[19]  -=  531441; patt[20]  -=     243; patt[36]  -=    2187; };
inline void RXPattern::set_BLACK_H8() { patt[12]  -=    2187; patt[16]  -=      27; patt[19]  -=  177147; patt[20]  -=      81; patt[36]  -=      81; };


inline void RXPattern::set_WHITE_A1() { patt[12]  +=       1; patt[14]  +=      27; patt[18]  +=      81; patt[21]  +=  177147; patt[34]  +=      81; };
inline void RXPattern::set_WHITE_B1() { patt[ 9]  +=       1; patt[14]  +=    2187; patt[18]  +=     243; patt[21]  +=  531441; patt[34]  +=    2187; };
inline void RXPattern::set_WHITE_C1() { patt[ 5]  +=       1; patt[18]  +=     729; patt[22]  +=      27; patt[29]  +=    2187; patt[34]  +=    6561; };
inline void RXPattern::set_WHITE_D1() { patt[ 1]  +=       1; patt[18]  +=    2187; patt[22]  +=      81; patt[33]  +=    2187; patt[34]  +=   59049; };
inline void RXPattern::set_WHITE_E1() { patt[ 0]  +=      81; patt[18]  +=    6561; patt[22]  +=     243; patt[31]  +=       1; patt[35]  +=       1; };
inline void RXPattern::set_WHITE_F1() { patt[ 4]  +=     243; patt[18]  +=   19683; patt[22]  +=     729; patt[27]  +=       1; patt[35]  +=       9; };
inline void RXPattern::set_WHITE_G1() { patt[ 8]  +=     729; patt[15]  +=       9; patt[18]  +=   59049; patt[19]  +=      27; patt[35]  +=      27; };
inline void RXPattern::set_WHITE_H1() { patt[13]  +=    2187; patt[15]  +=      27; patt[18]  +=  177147; patt[19]  +=      81; patt[35]  +=      81; };

inline void RXPattern::set_WHITE_A2() { patt[11]  +=     729; patt[14]  +=       9; patt[18]  +=      27; patt[21]  +=   59049; patt[34]  +=      27; };
inline void RXPattern::set_WHITE_B2() { patt[12]  +=       3; patt[14]  +=      81; patt[18]  +=       9; patt[21]  += 1594323; patt[34]  +=     243; };
inline void RXPattern::set_WHITE_C2() { patt[ 9]  +=       3; patt[14]  +=    6561; patt[18]  +=       3; patt[29]  +=     729; patt[34]  +=   19683; };
inline void RXPattern::set_WHITE_D2() { patt[ 0]  +=      27; patt[ 5]  +=       3; patt[18]  +=       1; patt[22]  +=       9; patt[33]  +=     729; };
inline void RXPattern::set_WHITE_E2() { patt[ 1]  +=       3; patt[ 4]  +=      81; patt[18]  += 14348907; patt[22]  +=    2187; patt[31]  +=       3; };
inline void RXPattern::set_WHITE_F2() { patt[ 8]  +=     243; patt[15]  +=       3; patt[18]  += 4782969; patt[27]  +=       3; patt[35]  +=       3; };
inline void RXPattern::set_WHITE_G2() { patt[13]  +=     729; patt[15]  +=      81; patt[18]  += 1594323; patt[19]  +=       9; patt[35]  +=     243; };
inline void RXPattern::set_WHITE_H2() { patt[10]  +=       1; patt[15]  +=    2187; patt[18]  +=  531441; patt[19]  +=     243; patt[35]  +=    2187; };

inline void RXPattern::set_WHITE_A3() { patt[ 7]  +=     243; patt[21]  +=   19683; patt[25]  +=     729; patt[26]  +=       1; patt[34]  +=       9; };
inline void RXPattern::set_WHITE_B3() { patt[11]  +=     243; patt[14]  +=       3; patt[21]  += 4782969; patt[26]  +=       3; patt[34]  +=       3; };
inline void RXPattern::set_WHITE_C3() { patt[ 0]  +=       9; patt[12]  +=       9; patt[14]  +=     243; patt[22]  +=       3; patt[25]  +=    6561; patt[26]  +=       9; patt[29]  +=     243; patt[34]  +=     729; };
inline void RXPattern::set_WHITE_D3() { patt[ 4]  +=      27; patt[ 9]  +=       9; patt[14]  +=   19683; patt[22]  +=       1; patt[26]  +=      27; patt[33]  +=     243; };
inline void RXPattern::set_WHITE_E3() { patt[ 5]  +=       9; patt[ 8]  +=      81; patt[15]  +=       1; patt[22]  +=   19683; patt[26]  +=      81; patt[31]  +=       9; };
inline void RXPattern::set_WHITE_F3() { patt[ 1]  +=       9; patt[13]  +=     243; patt[15]  +=     243; patt[22]  +=    6561; patt[23]  +=       3; patt[26]  +=     243; patt[27]  +=       9; patt[35]  +=     729; };
inline void RXPattern::set_WHITE_G3() { patt[10]  +=       3; patt[15]  +=    6561; patt[19]  +=       3; patt[26]  +=     729; patt[35]  +=   19683; };
inline void RXPattern::set_WHITE_H3() { patt[ 6]  +=       1; patt[19]  +=     729; patt[23]  +=      27; patt[26]  +=    2187; patt[35]  +=    6561; };

inline void RXPattern::set_WHITE_A4() { patt[ 3]  +=      81; patt[21]  +=    6561; patt[25]  +=     243; patt[30]  +=       1; patt[34]  +=       1; };
inline void RXPattern::set_WHITE_B4() { patt[ 0]  +=       3; patt[ 7]  +=      81; patt[21]  += 14348907; patt[25]  +=    2187; patt[30]  +=       3; };
inline void RXPattern::set_WHITE_C4() { patt[ 4]  +=       9; patt[11]  +=      81; patt[14]  +=       1; patt[25]  +=   19683; patt[29]  +=      81; patt[30]  +=       9; };
inline void RXPattern::set_WHITE_D4() { patt[ 8]  +=      27; patt[12]  +=      27; patt[14]  +=     729; patt[30]  +=      27; patt[33]  +=      81; };
inline void RXPattern::set_WHITE_E4() { patt[ 9]  +=      27; patt[13]  +=      81; patt[15]  +=     729; patt[30]  +=      81; patt[31]  +=      27; };
inline void RXPattern::set_WHITE_F4() { patt[ 5]  +=      27; patt[10]  +=       9; patt[15]  +=   19683; patt[23]  +=       1; patt[27]  +=      27; patt[30]  +=     243; };
inline void RXPattern::set_WHITE_G4() { patt[ 1]  +=      27; patt[ 6]  +=       3; patt[19]  +=       1; patt[23]  +=       9; patt[30]  +=     729; };
inline void RXPattern::set_WHITE_H4() { patt[ 2]  +=       1; patt[19]  +=    2187; patt[23]  +=      81; patt[30]  +=    2187; patt[35]  +=   59049; };

inline void RXPattern::set_WHITE_A5() { patt[ 0]  +=       1; patt[21]  +=    2187; patt[25]  +=      81; patt[32]  +=    2187; patt[37]  +=   59049; };
inline void RXPattern::set_WHITE_B5() { patt[ 3]  +=      27; patt[ 4]  +=       3; patt[21]  +=       1; patt[25]  +=       9; patt[32]  +=     729; };
inline void RXPattern::set_WHITE_C5() { patt[ 7]  +=      27; patt[ 8]  +=       9; patt[17]  +=   19683; patt[25]  +=       1; patt[29]  +=      27; patt[32]  +=     243; };
inline void RXPattern::set_WHITE_D5() { patt[11]  +=      27; patt[13]  +=      27; patt[17]  +=     729; patt[32]  +=      81; patt[33]  +=      27; };
inline void RXPattern::set_WHITE_E5() { patt[10]  +=      27; patt[12]  +=      81; patt[16]  +=     729; patt[31]  +=      81; patt[32]  +=      27; };
inline void RXPattern::set_WHITE_F5() { patt[ 6]  +=       9; patt[ 9]  +=      81; patt[16]  +=       1; patt[23]  +=   19683; patt[27]  +=      81; patt[32]  +=       9; };
inline void RXPattern::set_WHITE_G5() { patt[ 2]  +=       3; patt[ 5]  +=      81; patt[19]  += 14348907; patt[23]  +=    2187; patt[32]  +=       3; };
inline void RXPattern::set_WHITE_H5() { patt[ 1]  +=      81; patt[19]  +=    6561; patt[23]  +=     243; patt[32]  +=       1; patt[36]  +=       1; };

inline void RXPattern::set_WHITE_A6() { patt[ 4]  +=       1; patt[21]  +=     729; patt[25]  +=      27; patt[28]  +=    2187; patt[37]  +=    6561; };
inline void RXPattern::set_WHITE_B6() { patt[ 8]  +=       3; patt[17]  +=    6561; patt[21]  +=       3; patt[28]  +=     729; patt[37]  +=   19683; };
inline void RXPattern::set_WHITE_C6() { patt[ 3]  +=       9; patt[13]  +=       9; patt[17]  +=     243; patt[24]  +=    6561; patt[25]  +=       3; patt[28]  +=     243; patt[29]  +=       9; patt[37]  +=     729; };
inline void RXPattern::set_WHITE_D6() { patt[ 7]  +=       9; patt[10]  +=      81; patt[17]  +=       1; patt[24]  +=   19683; patt[28]  +=      81; patt[33]  +=       9; };
inline void RXPattern::set_WHITE_E6() { patt[ 6]  +=      27; patt[11]  +=       9; patt[16]  +=   19683; patt[24]  +=       1; patt[28]  +=      27; patt[31]  +=     243; };
inline void RXPattern::set_WHITE_F6() { patt[ 2]  +=       9; patt[12]  +=     243; patt[16]  +=     243; patt[23]  +=    6561; patt[24]  +=       3; patt[27]  +=     243; patt[28]  +=       9; patt[36]  +=     729; };
inline void RXPattern::set_WHITE_G6() { patt[ 9]  +=     243; patt[16]  +=       3; patt[19]  += 4782969; patt[28]  +=       3; patt[36]  +=       3; };
inline void RXPattern::set_WHITE_H6() { patt[ 5]  +=     243; patt[19]  +=   19683; patt[23]  +=     729; patt[28]  +=       1; patt[36]  +=       9; };

inline void RXPattern::set_WHITE_A7() { patt[ 8]  +=       1; patt[17]  +=    2187; patt[20]  +=  531441; patt[21]  +=     243; patt[37]  +=    2187; };
inline void RXPattern::set_WHITE_B7() { patt[13]  +=       3; patt[17]  +=      81; patt[20]  += 1594323; patt[21]  +=       9; patt[37]  +=     243; };
inline void RXPattern::set_WHITE_C7() { patt[10]  +=     243; patt[17]  +=       3; patt[20]  += 4782969; patt[29]  +=       3; patt[37]  +=       3; };
inline void RXPattern::set_WHITE_D7() { patt[ 3]  +=       3; patt[ 6]  +=      81; patt[20]  += 14348907; patt[24]  +=    2187; patt[33]  +=       3; };
inline void RXPattern::set_WHITE_E7() { patt[ 2]  +=      27; patt[ 7]  +=       3; patt[20]  +=       1; patt[24]  +=       9; patt[31]  +=     729; };
inline void RXPattern::set_WHITE_F7() { patt[11]  +=       3; patt[16]  +=    6561; patt[20]  +=       3; patt[27]  +=     729; patt[36]  +=   19683; };
inline void RXPattern::set_WHITE_G7() { patt[12]  +=     729; patt[16]  +=      81; patt[19]  += 1594323; patt[20]  +=       9; patt[36]  +=     243; };
inline void RXPattern::set_WHITE_H7() { patt[ 9]  +=     729; patt[16]  +=       9; patt[19]  +=   59049; patt[20]  +=      27; patt[36]  +=      27; };

inline void RXPattern::set_WHITE_A8() { patt[13]  +=       1; patt[17]  +=      27; patt[20]  +=  177147; patt[21]  +=      81; patt[37]  +=      81; };
inline void RXPattern::set_WHITE_B8() { patt[10]  +=     729; patt[17]  +=       9; patt[20]  +=   59049; patt[21]  +=      27; patt[37]  +=      27; };
inline void RXPattern::set_WHITE_C8() { patt[ 6]  +=     243; patt[20]  +=   19683; patt[24]  +=     729; patt[29]  +=       1; patt[37]  +=       9; };
inline void RXPattern::set_WHITE_D8() { patt[ 2]  +=      81; patt[20]  +=    6561; patt[24]  +=     243; patt[33]  +=       1; patt[37]  +=       1; };
inline void RXPattern::set_WHITE_E8() { patt[ 3]  +=       1; patt[20]  +=    2187; patt[24]  +=      81; patt[31]  +=    2187; patt[36]  +=   59049; };
inline void RXPattern::set_WHITE_F8() { patt[ 7]  +=       1; patt[20]  +=     729; patt[24]  +=      27; patt[27]  +=    2187; patt[36]  +=    6561; };
inline void RXPattern::set_WHITE_G8() { patt[11]  +=       1; patt[16]  +=    2187; patt[19]  +=  531441; patt[20]  +=     243; patt[36]  +=    2187; };
inline void RXPattern::set_WHITE_H8() { patt[12]  +=    2187; patt[16]  +=      27; patt[19]  +=  177147; patt[20]  +=      81; patt[36]  +=      81; };


inline void RXPattern::flip_BLACK_B1() { patt[ 9]  -=       2; patt[14]  -=    4374; patt[18]  -=     486; patt[21]  -= 1062882; patt[34]  -=    4374; };
inline void RXPattern::flip_BLACK_C1() { patt[ 5]  -=       2; patt[18]  -=    1458; patt[22]  -=      54; patt[29]  -=    4374; patt[34]  -=   13122; };
inline void RXPattern::flip_BLACK_D1() { patt[ 1]  -=       2; patt[18]  -=    4374; patt[22]  -=     162; patt[33]  -=    4374; patt[34]  -=  118098; };
inline void RXPattern::flip_BLACK_E1() { patt[ 0]  -=     162; patt[18]  -=   13122; patt[22]  -=     486; patt[31]  -=       2; patt[35]  -=       2; };
inline void RXPattern::flip_BLACK_F1() { patt[ 4]  -=     486; patt[18]  -=   39366; patt[22]  -=    1458; patt[27]  -=       2; patt[35]  -=      18; };
inline void RXPattern::flip_BLACK_G1() { patt[ 8]  -=    1458; patt[15]  -=      18; patt[18]  -=  118098; patt[19]  -=      54; patt[35]  -=      54; };

inline void RXPattern::flip_BLACK_A2() { patt[11]  -=    1458; patt[14]  -=      18; patt[18]  -=      54; patt[21]  -=  118098; patt[34]  -=      54; };
inline void RXPattern::flip_BLACK_B2() { patt[12]  -=       6; patt[14]  -=     162; patt[18]  -=      18; patt[21]  -= 3188646; patt[34]  -=     486; };
inline void RXPattern::flip_BLACK_C2() { patt[ 9]  -=       6; patt[14]  -=   13122; patt[18]  -=       6; patt[29]  -=    1458; patt[34]  -=   39366; };
inline void RXPattern::flip_BLACK_D2() { patt[ 0]  -=      54; patt[ 5]  -=       6; patt[18]  -=       2; patt[22]  -=      18; patt[33]  -=    1458; };
inline void RXPattern::flip_BLACK_E2() { patt[ 1]  -=       6; patt[ 4]  -=     162; patt[18]  -= 28697814; patt[22]  -=    4374; patt[31]  -=       6; };
inline void RXPattern::flip_BLACK_F2() { patt[ 8]  -=     486; patt[15]  -=       6; patt[18]  -= 9565938; patt[27]  -=       6; patt[35]  -=       6; };
inline void RXPattern::flip_BLACK_G2() { patt[13]  -=    1458; patt[15]  -=     162; patt[18]  -= 3188646; patt[19]  -=      18; patt[35]  -=     486; };
inline void RXPattern::flip_BLACK_H2() { patt[10]  -=       2; patt[15]  -=    4374; patt[18]  -= 1062882; patt[19]  -=     486; patt[35]  -=    4374; };

inline void RXPattern::flip_BLACK_A3() { patt[ 7]  -=     486; patt[21]  -=   39366; patt[25]  -=    1458; patt[26]  -=       2; patt[34]  -=      18; };
inline void RXPattern::flip_BLACK_B3() { patt[11]  -=     486; patt[14]  -=       6; patt[21]  -= 9565938; patt[26]  -=       6; patt[34]  -=       6; };
inline void RXPattern::flip_BLACK_C3() { patt[ 0]  -=      18; patt[12]  -=      18; patt[14]  -=     486; patt[22]  -=       6; patt[25]  -=   13122; patt[26]  -=      18; patt[29]  -=     486; patt[34]  -=    1458; };
inline void RXPattern::flip_BLACK_D3() { patt[ 4]  -=      54; patt[ 9]  -=      18; patt[14]  -=   39366; patt[22]  -=       2; patt[26]  -=      54; patt[33]  -=     486; };
inline void RXPattern::flip_BLACK_E3() { patt[ 5]  -=      18; patt[ 8]  -=     162; patt[15]  -=       2; patt[22]  -=   39366; patt[26]  -=     162; patt[31]  -=      18; };
inline void RXPattern::flip_BLACK_F3() { patt[ 1]  -=      18; patt[13]  -=     486; patt[15]  -=     486; patt[22]  -=   13122; patt[23]  -=       6; patt[26]  -=     486; patt[27]  -=      18; patt[35]  -=    1458; };
inline void RXPattern::flip_BLACK_G3() { patt[10]  -=       6; patt[15]  -=   13122; patt[19]  -=       6; patt[26]  -=    1458; patt[35]  -=   39366; };
inline void RXPattern::flip_BLACK_H3() { patt[ 6]  -=       2; patt[19]  -=    1458; patt[23]  -=      54; patt[26]  -=    4374; patt[35]  -=   13122; };

inline void RXPattern::flip_BLACK_A4() { patt[ 3]  -=     162; patt[21]  -=   13122; patt[25]  -=     486; patt[30]  -=       2; patt[34]  -=       2; };
inline void RXPattern::flip_BLACK_B4() { patt[ 0]  -=       6; patt[ 7]  -=     162; patt[21]  -= 28697814; patt[25]  -=    4374; patt[30]  -=       6; };
inline void RXPattern::flip_BLACK_C4() { patt[ 4]  -=      18; patt[11]  -=     162; patt[14]  -=       2; patt[25]  -=   39366; patt[29]  -=     162; patt[30]  -=      18; };
inline void RXPattern::flip_BLACK_D4() { patt[ 8]  -=      54; patt[12]  -=      54; patt[14]  -=    1458; patt[30]  -=      54; patt[33]  -=     162; };
inline void RXPattern::flip_BLACK_E4() { patt[ 9]  -=      54; patt[13]  -=     162; patt[15]  -=    1458; patt[30]  -=     162; patt[31]  -=      54; };
inline void RXPattern::flip_BLACK_F4() { patt[ 5]  -=      54; patt[10]  -=      18; patt[15]  -=   39366; patt[23]  -=       2; patt[27]  -=      54; patt[30]  -=     486; };
inline void RXPattern::flip_BLACK_G4() { patt[ 1]  -=      54; patt[ 6]  -=       6; patt[19]  -=       2; patt[23]  -=      18; patt[30]  -=    1458; };
inline void RXPattern::flip_BLACK_H4() { patt[ 2]  -=       2; patt[19]  -=    4374; patt[23]  -=     162; patt[30]  -=    4374; patt[35]  -=  118098; };

inline void RXPattern::flip_BLACK_A5() { patt[ 0]  -=       2; patt[21]  -=    4374; patt[25]  -=     162; patt[32]  -=    4374; patt[37]  -=  118098; };
inline void RXPattern::flip_BLACK_B5() { patt[ 3]  -=      54; patt[ 4]  -=       6; patt[21]  -=       2; patt[25]  -=      18; patt[32]  -=    1458; };
inline void RXPattern::flip_BLACK_C5() { patt[ 7]  -=      54; patt[ 8]  -=      18; patt[17]  -=   39366; patt[25]  -=       2; patt[29]  -=      54; patt[32]  -=     486; };
inline void RXPattern::flip_BLACK_D5() { patt[11]  -=      54; patt[13]  -=      54; patt[17]  -=    1458; patt[32]  -=     162; patt[33]  -=      54; };
inline void RXPattern::flip_BLACK_E5() { patt[10]  -=      54; patt[12]  -=     162; patt[16]  -=    1458; patt[31]  -=     162; patt[32]  -=      54; };
inline void RXPattern::flip_BLACK_F5() { patt[ 6]  -=      18; patt[ 9]  -=     162; patt[16]  -=       2; patt[23]  -=   39366; patt[27]  -=     162; patt[32]  -=      18; };
inline void RXPattern::flip_BLACK_G5() { patt[ 2]  -=       6; patt[ 5]  -=     162; patt[19]  -= 28697814; patt[23]  -=    4374; patt[32]  -=       6; };
inline void RXPattern::flip_BLACK_H5() { patt[ 1]  -=     162; patt[19]  -=   13122; patt[23]  -=     486; patt[32]  -=       2; patt[36]  -=       2; };

inline void RXPattern::flip_BLACK_A6() { patt[ 4]  -=       2; patt[21]  -=    1458; patt[25]  -=      54; patt[28]  -=    4374; patt[37]  -=   13122; };
inline void RXPattern::flip_BLACK_B6() { patt[ 8]  -=       6; patt[17]  -=   13122; patt[21]  -=       6; patt[28]  -=    1458; patt[37]  -=   39366; };
inline void RXPattern::flip_BLACK_C6() { patt[ 3]  -=      18; patt[13]  -=      18; patt[17]  -=     486; patt[24]  -=   13122; patt[25]  -=       6; patt[28]  -=     486; patt[29]  -=      18; patt[37]  -=    1458; };
inline void RXPattern::flip_BLACK_D6() { patt[ 7]  -=      18; patt[10]  -=     162; patt[17]  -=       2; patt[24]  -=   39366; patt[28]  -=     162; patt[33]  -=      18; };
inline void RXPattern::flip_BLACK_E6() { patt[ 6]  -=      54; patt[11]  -=      18; patt[16]  -=   39366; patt[24]  -=       2; patt[28]  -=      54; patt[31]  -=     486; };
inline void RXPattern::flip_BLACK_F6() { patt[ 2]  -=      18; patt[12]  -=     486; patt[16]  -=     486; patt[23]  -=   13122; patt[24]  -=       6; patt[27]  -=     486; patt[28]  -=      18; patt[36]  -=    1458; };
inline void RXPattern::flip_BLACK_G6() { patt[ 9]  -=     486; patt[16]  -=       6; patt[19]  -= 9565938; patt[28]  -=       6; patt[36]  -=       6; };
inline void RXPattern::flip_BLACK_H6() { patt[ 5]  -=     486; patt[19]  -=   39366; patt[23]  -=    1458; patt[28]  -=       2; patt[36]  -=      18; };

inline void RXPattern::flip_BLACK_A7() { patt[ 8]  -=       2; patt[17]  -=    4374; patt[20]  -= 1062882; patt[21]  -=     486; patt[37]  -=    4374; };
inline void RXPattern::flip_BLACK_B7() { patt[13]  -=       6; patt[17]  -=     162; patt[20]  -= 3188646; patt[21]  -=      18; patt[37]  -=     486; };
inline void RXPattern::flip_BLACK_C7() { patt[10]  -=     486; patt[17]  -=       6; patt[20]  -= 9565938; patt[29]  -=       6; patt[37]  -=       6; };
inline void RXPattern::flip_BLACK_D7() { patt[ 3]  -=       6; patt[ 6]  -=     162; patt[20]  -= 28697814; patt[24]  -=    4374; patt[33]  -=       6; };
inline void RXPattern::flip_BLACK_E7() { patt[ 2]  -=      54; patt[ 7]  -=       6; patt[20]  -=       2; patt[24]  -=      18; patt[31]  -=    1458; };
inline void RXPattern::flip_BLACK_F7() { patt[11]  -=       6; patt[16]  -=   13122; patt[20]  -=       6; patt[27]  -=    1458; patt[36]  -=   39366; };
inline void RXPattern::flip_BLACK_G7() { patt[12]  -=    1458; patt[16]  -=     162; patt[19]  -= 3188646; patt[20]  -=      18; patt[36]  -=     486; };
inline void RXPattern::flip_BLACK_H7() { patt[ 9]  -=    1458; patt[16]  -=      18; patt[19]  -=  118098; patt[20]  -=      54; patt[36]  -=      54; };

inline void RXPattern::flip_BLACK_B8() { patt[10]  -=    1458; patt[17]  -=      18; patt[20]  -=  118098; patt[21]  -=      54; patt[37]  -=      54; };
inline void RXPattern::flip_BLACK_C8() { patt[ 6]  -=     486; patt[20]  -=   39366; patt[24]  -=    1458; patt[29]  -=       2; patt[37]  -=      18; };
inline void RXPattern::flip_BLACK_D8() { patt[ 2]  -=     162; patt[20]  -=   13122; patt[24]  -=     486; patt[33]  -=       2; patt[37]  -=       2; };
inline void RXPattern::flip_BLACK_E8() { patt[ 3]  -=       2; patt[20]  -=    4374; patt[24]  -=     162; patt[31]  -=    4374; patt[36]  -=  118098; };
inline void RXPattern::flip_BLACK_F8() { patt[ 7]  -=       2; patt[20]  -=    1458; patt[24]  -=      54; patt[27]  -=    4374; patt[36]  -=   13122; };
inline void RXPattern::flip_BLACK_G8() { patt[11]  -=       2; patt[16]  -=    4374; patt[19]  -= 1062882; patt[20]  -=     486; patt[36]  -=    4374; };


inline void RXPattern::flip_WHITE_B1() { patt[ 9]  +=       2; patt[14]  +=    4374; patt[18]  +=     486; patt[21]  += 1062882; patt[34]  +=    4374; };
inline void RXPattern::flip_WHITE_C1() { patt[ 5]  +=       2; patt[18]  +=    1458; patt[22]  +=      54; patt[29]  +=    4374; patt[34]  +=   13122; };
inline void RXPattern::flip_WHITE_D1() { patt[ 1]  +=       2; patt[18]  +=    4374; patt[22]  +=     162; patt[33]  +=    4374; patt[34]  +=  118098; };
inline void RXPattern::flip_WHITE_E1() { patt[ 0]  +=     162; patt[18]  +=   13122; patt[22]  +=     486; patt[31]  +=       2; patt[35]  +=       2; };
inline void RXPattern::flip_WHITE_F1() { patt[ 4]  +=     486; patt[18]  +=   39366; patt[22]  +=    1458; patt[27]  +=       2; patt[35]  +=      18; };
inline void RXPattern::flip_WHITE_G1() { patt[ 8]  +=    1458; patt[15]  +=      18; patt[18]  +=  118098; patt[19]  +=      54; patt[35]  +=      54; };

inline void RXPattern::flip_WHITE_A2() { patt[11]  +=    1458; patt[14]  +=      18; patt[18]  +=      54; patt[21]  +=  118098; patt[34]  +=      54; };
inline void RXPattern::flip_WHITE_B2() { patt[12]  +=       6; patt[14]  +=     162; patt[18]  +=      18; patt[21]  += 3188646; patt[34]  +=     486; };
inline void RXPattern::flip_WHITE_C2() { patt[ 9]  +=       6; patt[14]  +=   13122; patt[18]  +=       6; patt[29]  +=    1458; patt[34]  +=   39366; };
inline void RXPattern::flip_WHITE_D2() { patt[ 0]  +=      54; patt[ 5]  +=       6; patt[18]  +=       2; patt[22]  +=      18; patt[33]  +=    1458; };
inline void RXPattern::flip_WHITE_E2() { patt[ 1]  +=       6; patt[ 4]  +=     162; patt[18]  += 28697814; patt[22]  +=    4374; patt[31]  +=       6; };
inline void RXPattern::flip_WHITE_F2() { patt[ 8]  +=     486; patt[15]  +=       6; patt[18]  += 9565938; patt[27]  +=       6; patt[35]  +=       6; };
inline void RXPattern::flip_WHITE_G2() { patt[13]  +=    1458; patt[15]  +=     162; patt[18]  += 3188646; patt[19]  +=      18; patt[35]  +=     486; };
inline void RXPattern::flip_WHITE_H2() { patt[10]  +=       2; patt[15]  +=    4374; patt[18]  += 1062882; patt[19]  +=     486; patt[35]  +=    4374; };

inline void RXPattern::flip_WHITE_A3() { patt[ 7]  +=     486; patt[21]  +=   39366; patt[25]  +=    1458; patt[26]  +=       2; patt[34]  +=      18; };
inline void RXPattern::flip_WHITE_B3() { patt[11]  +=     486; patt[14]  +=       6; patt[21]  += 9565938; patt[26]  +=       6; patt[34]  +=       6; };
inline void RXPattern::flip_WHITE_C3() { patt[ 0]  +=      18; patt[12]  +=      18; patt[14]  +=     486; patt[22]  +=       6; patt[25]  +=   13122; patt[26]  +=      18; patt[29]  +=     486; patt[34]  +=    1458; };
inline void RXPattern::flip_WHITE_D3() { patt[ 4]  +=      54; patt[ 9]  +=      18; patt[14]  +=   39366; patt[22]  +=       2; patt[26]  +=      54; patt[33]  +=     486; };
inline void RXPattern::flip_WHITE_E3() { patt[ 5]  +=      18; patt[ 8]  +=     162; patt[15]  +=       2; patt[22]  +=   39366; patt[26]  +=     162; patt[31]  +=      18; };
inline void RXPattern::flip_WHITE_F3() { patt[ 1]  +=      18; patt[13]  +=     486; patt[15]  +=     486; patt[22]  +=   13122; patt[23]  +=       6; patt[26]  +=     486; patt[27]  +=      18; patt[35]  +=    1458; };
inline void RXPattern::flip_WHITE_G3() { patt[10]  +=       6; patt[15]  +=   13122; patt[19]  +=       6; patt[26]  +=    1458; patt[35]  +=   39366; };
inline void RXPattern::flip_WHITE_H3() { patt[ 6]  +=       2; patt[19]  +=    1458; patt[23]  +=      54; patt[26]  +=    4374; patt[35]  +=   13122; };

inline void RXPattern::flip_WHITE_A4() { patt[ 3]  +=     162; patt[21]  +=   13122; patt[25]  +=     486; patt[30]  +=       2; patt[34]  +=       2; };
inline void RXPattern::flip_WHITE_B4() { patt[ 0]  +=       6; patt[ 7]  +=     162; patt[21]  += 28697814; patt[25]  +=    4374; patt[30]  +=       6; };
inline void RXPattern::flip_WHITE_C4() { patt[ 4]  +=      18; patt[11]  +=     162; patt[14]  +=       2; patt[25]  +=   39366; patt[29]  +=     162; patt[30]  +=      18; };
inline void RXPattern::flip_WHITE_D4() { patt[ 8]  +=      54; patt[12]  +=      54; patt[14]  +=    1458; patt[30]  +=      54; patt[33]  +=     162; };
inline void RXPattern::flip_WHITE_E4() { patt[ 9]  +=      54; patt[13]  +=     162; patt[15]  +=    1458; patt[30]  +=     162; patt[31]  +=      54; };
inline void RXPattern::flip_WHITE_F4() { patt[ 5]  +=      54; patt[10]  +=      18; patt[15]  +=   39366; patt[23]  +=       2; patt[27]  +=      54; patt[30]  +=     486; };
inline void RXPattern::flip_WHITE_G4() { patt[ 1]  +=      54; patt[ 6]  +=       6; patt[19]  +=       2; patt[23]  +=      18; patt[30]  +=    1458; };
inline void RXPattern::flip_WHITE_H4() { patt[ 2]  +=       2; patt[19]  +=    4374; patt[23]  +=     162; patt[30]  +=    4374; patt[35]  +=  118098; };

inline void RXPattern::flip_WHITE_A5() { patt[ 0]  +=       2; patt[21]  +=    4374; patt[25]  +=     162; patt[32]  +=    4374; patt[37]  +=  118098; };
inline void RXPattern::flip_WHITE_B5() { patt[ 3]  +=      54; patt[ 4]  +=       6; patt[21]  +=       2; patt[25]  +=      18; patt[32]  +=    1458; };
inline void RXPattern::flip_WHITE_C5() { patt[ 7]  +=      54; patt[ 8]  +=      18; patt[17]  +=   39366; patt[25]  +=       2; patt[29]  +=      54; patt[32]  +=     486; };
inline void RXPattern::flip_WHITE_D5() { patt[11]  +=      54; patt[13]  +=      54; patt[17]  +=    1458; patt[32]  +=     162; patt[33]  +=      54; };
inline void RXPattern::flip_WHITE_E5() { patt[10]  +=      54; patt[12]  +=     162; patt[16]  +=    1458; patt[31]  +=     162; patt[32]  +=      54; };
inline void RXPattern::flip_WHITE_F5() { patt[ 6]  +=      18; patt[ 9]  +=     162; patt[16]  +=       2; patt[23]  +=   39366; patt[27]  +=     162; patt[32]  +=      18; };
inline void RXPattern::flip_WHITE_G5() { patt[ 2]  +=       6; patt[ 5]  +=     162; patt[19]  += 28697814; patt[23]  +=    4374; patt[32]  +=       6; };
inline void RXPattern::flip_WHITE_H5() { patt[ 1]  +=     162; patt[19]  +=   13122; patt[23]  +=     486; patt[32]  +=       2; patt[36]  +=       2; };

inline void RXPattern::flip_WHITE_A6() { patt[ 4]  +=       2; patt[21]  +=    1458; patt[25]  +=      54; patt[28]  +=    4374; patt[37]  +=   13122; };
inline void RXPattern::flip_WHITE_B6() { patt[ 8]  +=       6; patt[17]  +=   13122; patt[21]  +=       6; patt[28]  +=    1458; patt[37]  +=   39366; };
inline void RXPattern::flip_WHITE_C6() { patt[ 3]  +=      18; patt[13]  +=      18; patt[17]  +=     486; patt[24]  +=   13122; patt[25]  +=       6; patt[28]  +=     486; patt[29]  +=      18; patt[37]  +=    1458; };
inline void RXPattern::flip_WHITE_D6() { patt[ 7]  +=      18; patt[10]  +=     162; patt[17]  +=       2; patt[24]  +=   39366; patt[28]  +=     162; patt[33]  +=      18; };
inline void RXPattern::flip_WHITE_E6() { patt[ 6]  +=      54; patt[11]  +=      18; patt[16]  +=   39366; patt[24]  +=       2; patt[28]  +=      54; patt[31]  +=     486; };
inline void RXPattern::flip_WHITE_F6() { patt[ 2]  +=      18; patt[12]  +=     486; patt[16]  +=     486; patt[23]  +=   13122; patt[24]  +=       6; patt[27]  +=     486; patt[28]  +=      18; patt[36]  +=    1458; };
inline void RXPattern::flip_WHITE_G6() { patt[ 9]  +=     486; patt[16]  +=       6; patt[19]  += 9565938; patt[28]  +=       6; patt[36]  +=       6; };
inline void RXPattern::flip_WHITE_H6() { patt[ 5]  +=     486; patt[19]  +=   39366; patt[23]  +=    1458; patt[28]  +=       2; patt[36]  +=      18; };

inline void RXPattern::flip_WHITE_A7() { patt[ 8]  +=       2; patt[17]  +=    4374; patt[20]  += 1062882; patt[21]  +=     486; patt[37]  +=    4374; };
inline void RXPattern::flip_WHITE_B7() { patt[13]  +=       6; patt[17]  +=     162; patt[20]  += 3188646; patt[21]  +=      18; patt[37]  +=     486; };
inline void RXPattern::flip_WHITE_C7() { patt[10]  +=     486; patt[17]  +=       6; patt[20]  += 9565938; patt[29]  +=       6; patt[37]  +=       6; };
inline void RXPattern::flip_WHITE_D7() { patt[ 3]  +=       6; patt[ 6]  +=     162; patt[20]  += 28697814; patt[24]  +=    4374; patt[33]  +=       6; };
inline void RXPattern::flip_WHITE_E7() { patt[ 2]  +=      54; patt[ 7]  +=       6; patt[20]  +=       2; patt[24]  +=      18; patt[31]  +=    1458; };
inline void RXPattern::flip_WHITE_F7() { patt[11]  +=       6; patt[16]  +=   13122; patt[20]  +=       6; patt[27]  +=    1458; patt[36]  +=   39366; };
inline void RXPattern::flip_WHITE_G7() { patt[12]  +=    1458; patt[16]  +=     162; patt[19]  += 3188646; patt[20]  +=      18; patt[36]  +=     486; };
inline void RXPattern::flip_WHITE_H7() { patt[ 9]  +=    1458; patt[16]  +=      18; patt[19]  +=  118098; patt[20]  +=      54; patt[36]  +=      54; };

inline void RXPattern::flip_WHITE_B8() { patt[10]  +=    1458; patt[17]  +=      18; patt[20]  +=  118098; patt[21]  +=      54; patt[37]  +=      54; };
inline void RXPattern::flip_WHITE_C8() { patt[ 6]  +=     486; patt[20]  +=   39366; patt[24]  +=    1458; patt[29]  +=       2; patt[37]  +=      18; };
inline void RXPattern::flip_WHITE_D8() { patt[ 2]  +=     162; patt[20]  +=   13122; patt[24]  +=     486; patt[33]  +=       2; patt[37]  +=       2; };
inline void RXPattern::flip_WHITE_E8() { patt[ 3]  +=       2; patt[20]  +=    4374; patt[24]  +=     162; patt[31]  +=    4374; patt[36]  +=  118098; };
inline void RXPattern::flip_WHITE_F8() { patt[ 7]  +=       2; patt[20]  +=    1458; patt[24]  +=      54; patt[27]  +=    4374; patt[36]  +=   13122; };
inline void RXPattern::flip_WHITE_G8() { patt[11]  +=       2; patt[16]  +=    4374; patt[19]  += 1062882; patt[20]  +=     486; patt[36]  +=    4374; };
#endif
