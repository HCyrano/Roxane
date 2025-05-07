/*
 *  RXMove.cpp
 *  Roxane
 *
 *  Created by Bruno Causse on 27/06/05.
 *  Copyleft 2005-2025 personnel.
 *
 */
#include <cmath>

#include "RXMove.hpp"
#include "RXConstantes.hpp"

/*
void RXMove::index_to_COsMove(int index, COsMove& move) {

	if (index == PASS) {
		move.fPass = true;
		move.row = -1;
		move.col = -1;
		
	} else {
		move.fPass = false;
		move.row = 7 - static_cast<int>(floor(index/8));
		move.col = 7 - index%8;
	}
	
	
}

std::string RXMove::COsMove_to_coord(COsMove& move) {

	if (move.fPass)
		return std::string("PA");
	 
	return (std::string(1, "ABCDEFGH"[move.col]) + std::string(1, "12345678"[move.row]));

}
*/

std::string RXMove::index_to_coord(int index) {

	if (index == PASS)
		return std::string("PA");

	return (std::string(1, "HGFEDCBA"[index%8]) + std::string(1, "87654321"[index/8]));

}

int RXMove::coord_to_index(std::string coord) {

	if( coord == "PA" || coord == "PASS")
		return PASS;
		
	return (('8' - coord[1])*8 + ('h' - std::tolower(coord[0])));
}


//debug
std::ostream& operator<<(std::ostream& os, RXMove* list) {

	for(RXMove* move = list->next; move != NULL; move = move->next)
		os << RXMove::index_to_coord(move->position) << ' ';
		
	return os;
}
