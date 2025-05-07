// Copyleft 2001 Chris Welty
//	All Rights Reserved

#include <fstream>
#include <iomanip>

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "sockbuf.hpp"
#include "types.hpp"

using namespace std;

sockbuf::sockbuf() {
	// log incoming data to file?
	fplog=NULL;
	loglast=kLogNone;
	if (fLogging) {
		fplog=new ofstream("recv.txt");
        if(!fplog->is_open())
            std::cout << "unable to open log file for writing" << std::endl;
	}

	// create get buffer
	buf=new char[2*nBufSize];
	if (buf) {
		setg(buf,buf+nBufSize, buf+nBufSize);
		setp(buf+nBufSize,buf+2*nBufSize);
		err=0;
	}
	else {
		err=kErrMem;
	}

	fConnected=false;
}

sockbuf::~sockbuf() {
	if (fConnected)
		disconnect();

	if (fLogging) {
		if (fplog && fplog->is_open())
			fplog->close();
		else
			_ASSERT(0);
	}
	if (buf)
		delete [] buf;
}

int sockbuf::connect(const string& sServer, int nPort) {
    
	hostent *hostent;
	protoent *protoent;
	sockaddr_in sa;
	const string sProtocol="tcp";

	if (err)
		return err;

	if (fConnected) {
		_ASSERT(0);
		return kErrAlreadyConnected;
	}

	// get connection location and type
	if (!(hostent=gethostbyname(sServer.c_str())))
		return kErrNoHost;
	if (!(protoent=getprotobyname(sProtocol.c_str())))
		return kErrNoProtocol;
        
	sa.sin_family=AF_INET;

    sa.sin_port=htons(nPort);
    
    //sa.sin_addr.s_addr=*(u2*)hostent->h_addr_list[0];
	sa.sin_addr.s_addr=* reinterpret_cast< unsigned int* > (hostent->h_addr_list[0]);

	// get socket
	if (!(sock=socket(AF_INET, SOCK_STREAM, protoent->p_proto)))
		return kErrNoSocket;


	// connect
	if (::connect(sock,(const sockaddr*)&sa,sizeof(sa))) {
        
		//close(sock);
        fplog->close();
		return kErrCantConnect;
	}


	fConnected=true;
	return 0;
}

int sockbuf::disconnect() {
	if (fConnected) {
		//close(sock);
        fplog->close();
		fConnected=false;
		return 0;
	}
	else {
		_ASSERT(0);
		return kErrNotConnected;
	}
}

bool sockbuf::IsConnected() const {
	return fConnected;
}

int sockbuf::underflow() {
    
	char* p0 = NULL;
	int nGetSize = 0;

	if (!fConnected || err)
		return EOF;

	if (eback()) {
		p0=eback();
		nGetSize=nBufSize;
	} else {
		_ASSERT(0);
        return EOF; // *p0;
	}
	
	long nrecv=recv(sock, p0, nGetSize,0);
	if (nrecv==SOCKET_ERROR) {
		err=kErrUnknown;
		return EOF;
	}
	
	if (nrecv==0) {
		// connection closed
		err=kErrConnectionClosed;
		return EOF;
	} else {
		if (gptr() >= egptr())
			setg(p0, p0, p0+nrecv);
		//else
		//	setg(unbuf, unbuf-1, unbuf+1);
		if (fplog) {
			if (loglast!=kLogRecv) {
				loglast=kLogRecv;
				fplog->write("[recv]",6);
			}
			fplog->write(p0, nrecv);
			*fplog << flush;
		}
		return *p0;
	}
}

int sockbuf::overflow(int c) {
	if (!fConnected || err)
		return EOF;

	long nSend=pptr()-pbase();
	long nSent=send(sock, pbase(), nSend,0);
	bool fOK=nSend==nSent;

	_ASSERT(fOK);
	if (fplog) {
		if (loglast!=kLogSend) {
			loglast=kLogSend;
			fplog->write("[send]",6);
		}
		fplog->write(pbase(), pptr()-pbase());
	}
	if (fOK && c!=EOF) {
		char cc=c;
		nSent=send(sock, &cc, 1, 0);
		fOK=nSent==1;
		if (fplog) {
			fplog->write(&cc,1);
		}
	}
	if (fplog)
		fplog->flush();

	// clear put area
	setp(pbase(),epptr());

	return fOK?0:EOF;
}

int sockbuf::sync() {
	return overflow(EOF);
}

int sockbuf::Err() const {
	return err;
}
