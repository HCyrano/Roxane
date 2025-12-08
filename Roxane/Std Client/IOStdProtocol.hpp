/*
 *  IOStdProtocol.h
 *  Roxane
 *
 *  Created by BrunoCausse on 08/12/2025.
 *  Copyright 2025 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef STANDART_PROTOCOL_H
#define STANDART_PROTOCOL_H

#include "RXRoxane.hpp"

class RXRoxane;

class IOStdProtocol {
	
	RXRoxane* engine;
	
	volatile int running;
	
	void InterpretCommand(int nargs, char *args[]);
	void SyntaxError(int nargs, char *args[]);
	
	//synchro std::cout
	mutable pthread_mutex_t IOSync;

public:
	
	IOStdProtocol(RXRoxane* engine);
	
	void MainLoop(void);
	void Print(const std::string msg) const;


};

#endif
