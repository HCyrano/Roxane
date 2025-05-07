/*
 *  RXThreadHelper.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 10/09/08.
 *  Copyleft 2008-2025 personnel.
 *
 */
#ifndef RXHELPER_HPP
#define RXHELPER_HPP

#include <pthread.h>

// Interface
class RXHelper {
	public :
	virtual void idle_loop() = 0;
};

// Interface
class Runnable {
	public :
	virtual void run() = 0;
};

#endif
