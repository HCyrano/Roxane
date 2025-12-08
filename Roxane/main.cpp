/*
 *  main.cpp
 *  Roxane
 *
 *  Created by B Causse modify 12/09/2024.
 *  Copyleft 2005-2025 personnel.
 *
 */

#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <pthread.h>


#include "types.hpp"

#include "ODKStream.hpp"
#include "IOStdProtocol.hpp"
#include "RXRoxane.hpp"
#include "RXBBPatterns.hpp"

#include "RXEngine.hpp"
#include "RXTools.hpp"
#include "RXSetting.hpp"


int main (int argc, char * const argv[]) {
        
#ifdef __ARM_FEATURE_SVE
    std::cout << "ARM SVE available" << std::endl;
#else
    std::cout << "ARM SVE unavailable" << std::endl;
#endif
#ifdef __ARM_NEON
    std::cout << "ARM NEON available" << std::endl;
#else
    std::cout << "ARM NEON unavailable" << std::endl;
#endif


	std::string login, password, file_name, mode, imposed_opening = "";
	unsigned int nBitsTable = 20;
	unsigned int nThreads = 1;

    unsigned int offset_start = 0;
    unsigned int n_games = 1000;

	for(int i = 1; i<argc; i++) {
		
		std::string arg(argv[i]);
		if(arg == "-login" && i+1<argc) {
			login = argv[++i];
		} else if(arg == "-passw" && i+1<argc) {
			password =  argv[++i];
		} else if(arg == "-fixeline" && i+1<argc) {
			imposed_opening = argv[++i];
		}  else if(arg == "-h" && i+1<argc) {
			std::istringstream iss(argv[++i]);
			iss >> nBitsTable;
		} else if(arg == "-t" && i+1<argc) {
			std::istringstream iss(argv[++i]);
			iss >> nThreads;
        } else if(arg == "-rawdata" && i+2<argc) {
            std::istringstream iss_offset(argv[++i]);
            iss_offset >> offset_start;
            std::istringstream iss_game(argv[++i]);
            iss_game >> n_games;
        } else if(arg == "-mode" && i+1<argc) {
			mode =  argv[++i];
		} else if(file_name.empty()) {
			file_name = argv[i];
		}
	}

    std::string version = "build-08-12-2025 15h45";
    
	std::cout << "Version Roxane " << version << std::endl;
	std::cout << "Number of threads: " << nThreads << std::endl;
	std::cout << "Size hashTable: 2^" << nBitsTable << std::endl;
    std::cout << file_name << std::endl;
        
	RXRoxane roxane(nBitsTable, nThreads);
    

#ifdef TUNE_PROBCUT_MID
    
    roxane.get_probcut_mid_data();
    
#else
    
    
#ifdef TUNE_PROBCUT_END
    
    roxane.get_probcut_end_data();
    
    
#else
    
#ifdef TUNE_PROBCUT_END2
    
    if(!file_name.empty())
        roxane.get_probcut_end2_data(file_name);
    
    
#else

#ifdef GENERATE_RAWDATA

    //base de donnée
    if(!file_name.empty())
        roxane.rawdata(file_name, offset_start, n_games);
    
#else
    
    //warm up
    if(!file_name.empty())
        roxane.get_move(file_name);
    
#endif
     
    
    if(imposed_opening != "")
        roxane.imposed_opening(imposed_opening);
    
    
    //	if(mode == "wthor")
    //		roxane.check_allWTHOR(); //try for robust endgame
    
    
    if(mode == "ggs" && !login.empty() && !password.empty()) {
        
        int err;
        CODKStream gs;
        
        //liaison Roxane GGS
        roxane.connectGGS(&gs);
        gs.pComputer = &roxane;
        
        // Connect(server, port)
        if ((err = gs.Connect("www.skatgame.net",5000))) {
            std::cout << "erreur conection" << std::endl;
            
            cerr << gs.ErrText(err) << "\n";
            return err;
        }
        
        // Login(name, password)
        if ((err = gs.Login(login.c_str(), password.c_str()))) {
            cerr << gs.ErrText(err) << "\n";
            gs.Disconnect();
            return err;
        }
        
        gs.SendMsg("version : " + version);


        gs.Process();			// receive, parse, and pass on messages
        
    }
    
    if(mode == "IOStd") {
        
        IOStdProtocol StdIO(&roxane);
        roxane.connectIOStd(&StdIO);
        
        StdIO.MainLoop();
        
    }

    
    
#endif
#endif
#endif
    
    return 0;
}

