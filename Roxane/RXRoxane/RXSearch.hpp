/*
 *  RXSearch.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 07/10/09.
 *  Copyleft 2009-2025 personnel.
 *
 */

#ifndef RXSEARCH_HPP
#define RXSEARCH_HPP

#include <string>
#include "RXBBPatterns.hpp"
#include "RXHashTable.hpp"
#include "OsObjects.hpp"

class RXBestMove {

	friend class RXRoxane;
	friend class RXEngine;

	unsigned char position;
	int depth;
	int score;
	int selectivity;
	double tElapsed;
	std::string mainLine;
	
	unsigned long long nodes;
	
	void to_COsMoveListItem(COsMoveListItem& mli);
	
};


class RXSearch {
	
	friend class RXRoxane;
	friend class RXEngine;
	
	enum t_client {kPrivate, kGGSMode, kZOOMode, kCassioMode};
	
	//parameters Engine
	t_client clientMode;
	
	unsigned int idEngine;
	int nThreads;
	RXHashTable* htable;
	RXHashTable* main_PV;
	RXHashTable* expected_PV;
	

	int depth_incr;

	bool search_on_opponent_time;
	
	
	//parameters times
	bool dependent_time;

	int tMatch;
	int tRemaining;
	
	//parameters search
	RXBBPatterns sBoard;
	int alpha;
	int beta;
	int selectivity;
	
	int depth;
	
	//parameters best move
	RXBestMove bestMove;	
	
};





#endif
