// Copyleft 2001 Chris Welty
//	All Rights Reserved

#include <fstream>
#include <iomanip>

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cassert>

#include "sockbuf.hpp"
#include "types.hpp"


sockbuf::sockbuf() {
	// log incoming data to file?
	fplog=NULL;
	loglast=kLogNone;
	if (fLogging) {
		fplog=new std::ofstream("recv.txt");
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
        delete fplog;
	}
    
	if (buf)
		delete [] buf;
}

int sockbuf::connect(const std::string& sServer, int nPort) {
    
	hostent *hostent;
	protoent *protoent;
	sockaddr_in sa;
	const std::string sProtocol="tcp";

	if (err)
		return err;

	if (fConnected) {
		return kErrAlreadyConnected;
	}

	// get connection location and type
	if (!(hostent=gethostbyname(sServer.c_str())))
		return kErrNoHost;
	if (!(protoent=getprotobyname(sProtocol.c_str())))
		return kErrNoProtocol;
        
	sa.sin_family=AF_INET;
    sa.sin_port=htons(nPort);
	sa.sin_addr.s_addr=* reinterpret_cast< unsigned int* > (hostent->h_addr_list[0]);

	// get socket
    sock=socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    if(sock == -1)
		return kErrNoSocket;

    // ═══════════════════════════════════════════════════════════════════════════
    // CONFIGURATION MINIMALE - Options POSIX standard uniquement
    // Compatible avec toutes les versions de macOS et Unix
    // ═══════════════════════════════════════════════════════════════════════════
    
    std::cout << "\n[SOCKBUF] ═══════════════════════════════════════" << std::endl;
    std::cout << "[SOCKBUF] Configuring socket for network detection" << std::endl;
    std::cout << "[SOCKBUF] ═══════════════════════════════════════\n" << std::endl;
    
    // ────────────────────────────────────────────────────────────────────────
    // OPTION 1 : SO_RCVTIMEO - LA CLÉ POUR DÉTECTER LES COUPURES
    // ────────────────────────────────────────────────────────────────────────
    // C'est l'option la plus importante !
    // recv() retournera une erreur si aucune donnée n'arrive pendant ce délai
    
    struct timeval recv_timeout;
    recv_timeout.tv_sec = 90;   // 90 secondes
    recv_timeout.tv_usec = 0;
    
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout)) == 0) {
        std::cout << "[SOCKBUF] ✓ SO_RCVTIMEO = " << recv_timeout.tv_sec << " seconds" << std::endl;
        std::cout << "[SOCKBUF]   recv() will timeout after " << recv_timeout.tv_sec
                  << "s with no data" << std::endl;
    } else {
        std::cerr << "[SOCKBUF] ✗ Failed to set SO_RCVTIMEO: " << strerror(errno) << std::endl;
        std::cerr << "[SOCKBUF]   WARNING: Network failures may not be detected!" << std::endl;
    }
    
    // ────────────────────────────────────────────────────────────────────────
    // OPTION 2 : SO_SNDTIMEO - Timeout pour send()
    // ────────────────────────────────────────────────────────────────────────
    
    struct timeval send_timeout;
    send_timeout.tv_sec = 10;   // 10 secondes
    send_timeout.tv_usec = 0;
    
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, sizeof(send_timeout)) == 0) {
        std::cout << "[SOCKBUF] ✓ SO_SNDTIMEO = " << send_timeout.tv_sec << " seconds" << std::endl;
    } else {
        std::cerr << "[SOCKBUF] ✗ Failed to set SO_SNDTIMEO: " << strerror(errno) << std::endl;
    }
    
    // ────────────────────────────────────────────────────────────────────────
    // OPTION 3 : SO_KEEPALIVE - Keepalive TCP basique
    // ────────────────────────────────────────────────────────────────────────
    // Active le keepalive avec les paramètres système par défaut
    // Sur macOS, par défaut : 2 heures d'inactivité avant détection
    
    int keepalive = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == 0) {
        std::cout << "[SOCKBUF] ✓ SO_KEEPALIVE enabled (system defaults)" << std::endl;
        std::cout << "[SOCKBUF]   Note: System default is typically ~2 hours" << std::endl;
    } else {
        std::cerr << "[SOCKBUF] ✗ Failed to enable SO_KEEPALIVE: " << strerror(errno) << std::endl;
    }
    
    // ────────────────────────────────────────────────────────────────────────
    // FIN DE LA CONFIGURATION
    // ────────────────────────────────────────────────────────────────────────
    
    std::cout << "\n[SOCKBUF] ═══════════════════════════════════════" << std::endl;
    std::cout << "[SOCKBUF] Detection Strategy:" << std::endl;
    std::cout << "[SOCKBUF] ───────────────────────────────────────" << std::endl;
    std::cout << "[SOCKBUF]  1. Process() continuously reads data" << std::endl;
    std::cout << "[SOCKBUF]  2. If network fails, no data arrives" << std::endl;
    std::cout << "[SOCKBUF]  3. After " << recv_timeout.tv_sec << "s, recv() times out" << std::endl;
    std::cout << "[SOCKBUF]  4. recv() returns -1 (errno=EAGAIN)" << std::endl;
    std::cout << "[SOCKBUF]  5. underflow() detects error" << std::endl;
    std::cout << "[SOCKBUF]  6. Process() exits read loop" << std::endl;
    std::cout << "[SOCKBUF]  7. TryReconnect() is called!" << std::endl;
    std::cout << "[SOCKBUF] ═══════════════════════════════════════\n" << std::endl;
    
    // ═══════════════════════════════════════════════════════════════════════════

	// connect
	if (::connect(sock,(const sockaddr*)&sa,sizeof(sa))) {
        
		close(sock);
        //if(fplog) fplog->close();
		return kErrCantConnect;
	}


	fConnected=true;
	return 0;
}

int sockbuf::disconnect() {
	if (fConnected) {
		close(sock);
        //fplog->close();
		fConnected=false;
		return 0;
	}
	else {
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
        return EOF; // *p0;
	}
	
    // ═══════════════════════════════════════════════════════════════════════════
    // APPEL CRITIQUE : recv()
    // Avec SO_RCVTIMEO configuré, recv() retournera -1 après timeout
    // ═══════════════════════════════════════════════════════════════════════════
    
    long nrecv=recv(sock, p0, nGetSize, 0);
    
    if (nrecv == SOCKET_ERROR || nrecv < 0) {
        
        std::cout << "[DEBUG] recv() returned: " << nrecv << ", errno=" << errno << std::endl;

        // Analyser errno
        switch(errno) {
            case EAGAIN:
            #if EAGAIN != EWOULDBLOCK
            case EWOULDBLOCK:
            #endif
                // ⚡ TIMEOUT ! C'est CE cas qui détecte la coupure réseau
                err = kErrConnectionReset;
                std::cerr << "\n";
                std::cerr << "════════════════════════════════════════════════════════════════" << std::endl;
                std::cerr << "⚠️  ⚠️  ⚠️  NETWORK FAILURE DETECTED  ⚠️  ⚠️  ⚠️" << std::endl;
                std::cerr << "════════════════════════════════════════════════════════════════" << std::endl;
                std::cerr << "[SOCKBUF] recv() timeout" << std::endl;
                std::cerr << "[SOCKBUF] No data received for 90 seconds" << std::endl;
                std::cerr << "[SOCKBUF] Connection is considered DEAD" << std::endl;
                std::cerr << "[SOCKBUF] Returning EOF to trigger reconnection..." << std::endl;
                std::cerr << "════════════════════════════════════════════════════════════════" << std::endl;
                std::cerr << "\n";
                break;
                
            case ECONNRESET:
                err = kErrConnectionReset;
                std::cerr << "[SOCKBUF] ⚠️  Connection reset by peer" << std::endl;
                break;
                
            case ETIMEDOUT:
                err = kErrConnectionReset;
                std::cerr << "[SOCKBUF] ⚠️  Connection timed out" << std::endl;
                break;
                
            case ENETUNREACH:
            case EHOSTUNREACH:
                err = kErrConnectionReset;
                std::cerr << "[SOCKBUF] ⚠️  Network unreachable" << std::endl;
                break;
                
            case EPIPE:
            case ENOTCONN:
                err = kErrConnectionClosed;
                std::cerr << "[SOCKBUF] ⚠️  Socket not connected" << std::endl;
                break;
                
            default:
                err = kErrUnknown;
                std::cerr << "[SOCKBUF] ⚠️  recv() error: " << strerror(errno)
                          << " (errno=" << errno << ")" << std::endl;
                break;
        }
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
		if (fplog && fplog->is_open()) {
			if (loglast!=kLogRecv) {
				loglast=kLogRecv;
               fplog->write("[recv]",6);
			}
            fplog->write(p0, nrecv);
			*fplog << std::flush;
		}
		return *p0;
	}
}

int sockbuf::overflow(int c) {
	if (!fConnected || err)
		return EOF;

	long nSend=pptr()-pbase();
	long nSent=send(sock, pbase(), nSend,0);


    if (nSent == SOCKET_ERROR || nSent < 0) {
        switch(errno) {
            case EAGAIN:
            #if EAGAIN != EWOULDBLOCK
            case EWOULDBLOCK:
            #endif
                err = kErrConnectionReset;
                std::cerr << "[SOCKBUF] ⚠️  send() timeout (buffer full)" << std::endl;
                break;
            case EPIPE:
                err = kErrConnectionClosed;
                std::cerr << "[SOCKBUF] ⚠️  send() failed: Broken pipe" << std::endl;
                break;
            case ECONNRESET:
                err = kErrConnectionReset;
                std::cerr << "[SOCKBUF] ⚠️  send() failed: Connection reset" << std::endl;
                break;
            case ETIMEDOUT:
                err = kErrConnectionReset;
                std::cerr << "[SOCKBUF] ⚠️  send() failed: Timeout" << std::endl;
                break;
            default:
                err = kErrUnknown;
                std::cerr << "[SOCKBUF] ⚠️  send() error: " << strerror(errno) << std::endl;
                break;
        }
        return EOF;
    }

	bool fOK=nSend==nSent;
    
    assert(fOK);
	if (fplog && fplog->is_open()) {
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
		if (fplog && fplog->is_open()) {
			fplog->write(&cc,1);
		}
	}
	if (fplog && fplog->is_open())
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

const char* sockbuf::ErrText(int err) {
    switch(err) {
        case 0:
            return "No error";
        case kErrCantStartup:
            return "Failed to initialize network library (Winsock/Socket startup)";
        case kErrNoHost:
            return "Host not found (DNS lookup failed)";
        case kErrNoProtocol:
            return "Network protocol not supported";
        case kErrNoSocket:
            return "Could not create network socket";
        case kErrCantConnect:
            return "Connection refused or server unreachable";
        case kErrConnectionReset:
            return "Connection reset by peer (the server dropped the link)";
        case kErrConnectionClosed:
            return "Connection closed gracefully by the server";
        case kErrNotConnected:
            return "Operation failed: Socket is not connected";
        case kErrAlreadyConnected:
            return "Operation failed: Socket is already connected";
        case kErrUnknown:
            return "An unknown socket error occurred";
        default:
            return "Unspecified network error";
 }
}

