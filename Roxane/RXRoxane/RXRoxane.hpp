/*
 *  RXRoxane.hpp
 *  BitBoard
 *
 *  Created by Bruno Causse on 01/09/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef RXROXANE_HPP
#define RXROXANE_HPP

#include <string>
#include <atomic>

#include "OsObjects.hpp"
#include "ODKStream.hpp"

#include "RXConstantes.hpp"
#include "RXBBPatterns.hpp"
#include "RXHashTable.hpp"
#include "RXEngine.hpp"
#include "RXHelper.hpp"
#include "RXSearch.hpp"

extern "C"
void* init_process(void* pt);

class CODKStream;
class CassioProtocol;

class RXRoxane: public Runnable {

	RXHashTable* hTable;
	RXHashTable* main_PV;
	RXHashTable* expected_PV;

	RXEngine* engine[2]; //BLACK and WHITE
	RXSearch search;

	pthread_t process[1];
	
	pthread_mutex_t mutex;
	
	//volatile bool resume_flag;
    std::atomic_bool resume_flag;
    
	//interface GGS server
	CODKStream* GGSClient;
	COsPosition game[2];
	string idg;

	unsigned long long hash_opening[60];
	char move_opening[60];

	

public :
    
    int max_threads;


	RXRoxane(int size_hashtable = 20, int max_thread = 1);	
	~RXRoxane();
	
	
	void resume();

	virtual void run();
    
    RXEngine* getEngine(const int color) const;

	
	//privateProtocol
	void get_move(const std::string& file_name);


	//wthorProtocol
	void check_allWTHOR();
	void check_WTHOR(const std::string& file_name, const std::string& file_error);
	
	
	//GGSProtocol
	void connectGGS(CODKStream* client);
	void imposed_opening(const std::string& line);
    
    void stop_engine(COsGame* game);

	void get_move(const std::string& _idg, COsGame* game);

	void sendMsg(std::string msg);
	
	

};


#endif

