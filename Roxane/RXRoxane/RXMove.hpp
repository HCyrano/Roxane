/*
 *  RXMove.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 27/06/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXMOVE_HPP
#define RXMOVE_HPP

#include <iostream>
#include <iomanip>
#include <cstddef> // define NULL
#include <string>
#include <vector>

#include "RXConstantes.hpp"
#include "OsObjects.hpp"
#include "RXPattern.hpp"


class RXMove {

	friend class RXRoxane; //for test WTHOR

	friend class RXBitBoard;
	friend class RXBBPatterns;
	friend class RXEngine;
	friend class RXEvaluation;
	
	char position;
	unsigned long long square;
	unsigned long long flipped;
	int score;
	char selectivity;
	char depth;
	
	RXMove* next;
	
	RXPattern* pattern;
	RXPattern* undo_pattern;
	
	public :

	
		RXMove() : position(static_cast<char>(NOMOVE)), square(0ULL), flipped(0ULL), score(0), next(NULL), pattern(NULL), undo_pattern(NULL) {
			pattern = new RXPattern();
		};
		
		RXMove(const RXMove& src) {
			//std::cout << "constructeur de copie RXMove" << std::endl;
			pattern = new RXPattern();
			
			*pattern = *(src.pattern); //copy

		};
		
		RXMove& operator=(const RXMove& src) {
			//std::cout << "affectation par copie de RXMove" << std::endl;
			if(this != &src) {

				*pattern = *(src.pattern); //copy

			}
			return *this;
		};

			
		~RXMove() {
			//std::cout << "destructeur RXMove" << std::endl;
			delete pattern;
		};

		
		//static std::string COsMove_to_coord(COsMove& move);		
		//static void index_to_COsMove(int index, COsMove& move);
		static std::string index_to_coord(int index);
		static int coord_to_index(std::string coord);
				
		void sort_bestmove(const int bestmove);
		void sort_by_score();

		//debug
		friend std::ostream& operator<<(std::ostream& os, RXMove* list);

};

inline void RXMove::sort_bestmove(int bestmove) {

	RXMove* previous;
	for(RXMove* iter = (previous = this)->next; iter != NULL; iter = (previous = iter)->next)
		if(iter->position == bestmove) {
			previous->next = iter->next;
			iter->next = this->next;
			this->next = iter;
			break;
		}
}

// from smallest to biggest
inline void RXMove::sort_by_score() {
	RXMove *best, *previousBest, *previous;
	
	for(RXMove* iter = this; iter->next != NULL; iter = iter->next){
		previousBest = iter;
		for(previous = previousBest->next; previous->next != NULL; previous =previous->next)
			if(previousBest->next->score>previous->next->score)
				previousBest = previous;
				
		if(previousBest != iter) {
			best = previousBest->next;
			previousBest->next = best->next;
			best->next = iter->next;
			iter->next = best;
		}
	}		
}

#endif
