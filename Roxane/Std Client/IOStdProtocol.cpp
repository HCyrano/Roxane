/*
 *  IOStdProtocol.cpp
 *  Roxane
 *
 *  Created by BrunoCausse on 08/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
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
				if (strstr(args[0], "IOStd-protocol"))
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
		//engine->stop();
	}
	
	else if (strstr(args[1], "quit"))  { //engine->free();	// not implemented
		//engine->stop();
		running = 0;
	}

	else if (strstr(args[1], "new-position"))  {
		; //engine->new_search();
		Print("ready.\n");
	}

	else if (strstr(args[1], "empty-hash"))  {
		engine->resume();
//		Print("ready.\n");
	}
	
	else if (strstr(args[1], "feed-hash")) {
		
		if(nargs == 8) {
			
			char *position, *pv = NULL;
			double alpha, beta;
			int depth, precision;
			
			position  = args[2];
			alpha     = atof(args[3]);
			beta      = atof(args[4]);
			depth     = atoi(args[5]);
			precision = atoi(args[6]);
			pv		  = args[7];
			
			//engine->feed_hash(position, alpha, beta, depth, precision, pv);
		}
	}
	
	else if (strstr(args[1], "midgame-search")) {

		if(nargs == 7) {
			char *position;
			double alpha, beta;
			int depth, precision;
			
			position  = args[2];
			alpha     = atof(args[3]);
			beta      = atof(args[4]);
			depth     = atoi(args[5]);
			precision = atoi(args[6]);
			
			//engine->midgame_search(position, alpha, beta, depth, precision);
		}
	}
	
	else if (strstr(args[1], "endgame-search")) {
		
		if(nargs == 6) {
			char *position;
			int alpha, beta, precision;
			
			position  = args[2];
			alpha     = atoi(args[3]);
			beta      = atoi(args[4]);
			precision = atoi(args[5]);
			
			//engine->endgame_search(position, alpha, beta, precision);
		}
	}
	
	else if(strstr(args[1], "get-search-infos")) {

		//engine->get_search_infos();
		
	}
	
	else if(strstr(args[1], "get-version")) {
		
		//engine->get_version();
		Print("ready.\n");
		
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

