// Copyleft 2001 Chris Welty
//	All Rights Reserved

#ifndef SOCKBUF_HPP
#define SOCKBUF_HPP

#include <sys/socket.h>
#include <iostream>
#include <fstream>

#include "types.hpp"


using namespace std;

class sockbuf : public streambuf {

	public:

		// construction/destruction
		sockbuf();
		virtual ~sockbuf();

		// overrides
		virtual int underflow();
		virtual int overflow(int c=EOF);
		virtual int sync();

		// errors
		enum { kErrUnknown=0x8600, kErrCantStartup, kErrNoHost, kErrNoProtocol, kErrNoSocket,
					kErrCantConnect, kErrConnectionReset, kErrConnectionClosed,
					kErrNotConnected, kErrAlreadyConnected };
		int Err() const;

		bool IsConnected() const;

		int connect(const string& sServer, int nPort);
		int disconnect();

	protected:
		enum { fLogging=1, nBufSize=1024 };

		bool fConnected;

		int sock; //SOCKET sock;
		ofstream *fplog;
		enum {kLogNone, kLogRecv, kLogSend} loglast;
		char *buf;
		int err;
};

#endif
