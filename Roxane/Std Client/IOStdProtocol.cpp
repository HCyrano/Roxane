/*
 *  IOStdProtocol.cpp
 *  Roxane
 *
 *  Created by BrunoCausse on 08/12/2025.
 *  Copyright 2025 __MyCompanyName__. All rights reserved.
 *
 */

#include "IOStdProtocol.hpp"

extern "C" {
#include "StdInput.h"
}


/*
 *******************************************************************************
 *                                                                             *
 *   Constructeur                                                              *
 *                                                                             *
 *******************************************************************************
 */

IOStdProtocol::IOStdProtocol(RXRoxane* e) : engine(e) {
}

/*
 *******************************************************************************
 *                                                                             *
 *   MainLoop() : la boucle principale de notre implementation.                *
 *   On lit l'entree standard ligne par ligne, et si la ligne commence par le  *
 *   mot cle special ENGINE-PROTOCOL, on execute la commande.                  *
 *   Cette implementation prend 0% du temps CPU tant que la ligne ne contient  *
 *   pas de commande.                                                          *
 *                                                                             *
 *******************************************************************************
 */
void IOStdProtocol::MainLoop(void) 
{ 
	int readstatus, nargs;
	running = 1;
	
	SetReadStream(stdin);
	
	while (running) {
		
		
		readstatus = Read(line_buffer);  // blocking read of stdin
				
		
		if (readstatus > 0) {
			
			nargs = ReadParse(line_buffer, args, " ");
			if (nargs > 0) {
				if (strstr(args[0], "IOStd"))
					InterpretCommand(nargs, args);
				else
					SyntaxError(nargs, args);
			} else {
				//ligne vide le moteur tourne?
                /*
				if(engine->is_running())
					Print("ok.\n");
				else
					Print("ready.\n");
				*/
                
                Print("ready.\n");
 
			}
		} else if (readstatus <0) {
			break;
		}
		
	}
	
	//engine->stop();
	running = 0;
}



/*
 *******************************************************************************
 *                                                                             *
 *   InterpretCommand() : interpretation d'une commande recue    *
 *   par le moteur sur l'entree standard.                                      *
 *                                                                             *
 *******************************************************************************
 */
void IOStdProtocol::InterpretCommand(int nargs, char *args[])
{ 
	if (strstr(args[1], "init")) {
		engine->resume(); //engine->init(); // not implemented
		Print("ready.\n");
	}
	
	else if (strstr(args[1], "stop")) {
		engine->stop_engine();
	}
	
	else if (strstr(args[1], "quit"))  { //engine->free();	// not implemented
		engine->stop_engine();
		running = 0;
	}

	else if (strstr(args[1], "empty-hash"))  {
		engine->resume();
		Print("ready.\n");
	}
		
	else if (strstr(args[1], "search")) {

		if(nargs == 5) {
			char *position;
			int depth, precision;
			
			position  = args[2];
			depth     = atoi(args[3]);
			precision = atoi(args[4]);
			
			engine->get_move(position, depth, precision);
		}
	}
		
	else
		SyntaxError(nargs,args);
}

/*
 *******************************************************************************
 *                                                                             *
 *   SyntaxError() : aide au debugage, affice les lexemes recus                *
 *   sur la derniere ligne de l'entree standard, en cas d'erreur de syntaxe    *
 *                                                                             *
 *******************************************************************************
 */

void IOStdProtocol::SyntaxError(int nargs, char *args[])
{int i;
	
	Print("\nSYNTAX ERROR :\n");
	for (i = 0; i < nargs; i++) {
		std::ostringstream line;
		line  << "Token[" << i << "] = " << args[i];
		Print(line.str());
	}
	Print("\n");
}


/*
 *******************************************************************************
 *                                                                             *
 *   Print() : ecrit sur la sortie standart de facon synchronisée              *
 *                                                                             *
 *******************************************************************************
 */

void IOStdProtocol::Print(const std::string msg) const {
    pthread_mutex_lock(&IOSync);
	std::cout << msg << std::endl;
    pthread_mutex_unlock(&IOSync);
}

