// Copyleft 2001 Chris Welty
//	All Rights Reserved

#include "types.hpp"

#include "ODKStream.hpp"
#include "GGSMessage.hpp"
#include "OsMessage.hpp"
#include "RXRoxane.hpp"

#include <iostream>
#include <unistd.h>
using namespace std;

//affiche le message dans la console
void CODKStream::HandleGGS(const CMsg* pmsg) {
	cout << pmsg->sRawText << "\n";
}

void CODKStream::HandleGGSLogin() {
	BaseGGSLogin();
	(*this) << "mso\n";
	flush();
}

void CODKStream::HandleGGSTell(const CMsgGGSTell* pmsg) {
	cout << pmsg->sFrom << " " << pmsg->sText << "\n";
	
    //join
	/**********************************************************************/
	if(	(pmsg->sFrom=="romano"
		 || pmsg->sFrom=="ohr"
		 || pmsg->sFrom=="delorme") && ((pmsg->sText.substr(0,11) == "t /td join ") || (pmsg->sText.substr(0,14) == "tell /td join "))) {
		
		(*this) << pmsg->sText << "\n";
		flush();
	}
	/**********************************************************************/
	
	if(pmsg->sFrom=="HCyrano") {
		if (pmsg->sText=="quit") {
			pComputer->resume();
			Logout();
		} else {
			(*this) << pmsg->sText << "\n";
			flush();
		}
	}
}

void CODKStream::HandleGGSUnknown(const CMsgGGSUnknown* pmsg) {
	cout << "Unknown GGS message: \n";
	HandleGGS(pmsg);
}

void CODKStream::HandleOsJoin(const CMsgOsJoin* pmsg) {
	ggsstream::BaseOsJoin(pmsg);
	GetMoveIfNeeded(pmsg->idg);
}

void CODKStream::HandleOsLogin() {
	BaseOsLogin();
	(*this) << "ts trust +\n"
			<< "tell /os open 0\n"; /* open 0 for tournament */
	flush();
}

/*

// handler from Roxane:
void CODKStream::HandleOsMatchDelta(const CMsgOsMatchDelta* pmsg) {
	if (pmsg->fPlus) {
		Roxane::start_match(pmsg);
	} else {
		string idg;
		COsGame* pgame;

		// add games to book
		idg=pmsg->match.idm;
		pgame=PGame(idg);
		if (pgame)
			pComputer->EndGame(*pgame);

		idg=pmsg->match.idm+".0";
		pgame=PGame(idg);
		if (pgame)
			pComputer->EndGame(*pgame);

		idg=pmsg->match.idm+".1";
		pgame=PGame(idg);
		if (pgame)
			pComputer->EndGame(*pgame);
	}
	BaseOsMatchDelta(pmsg);
}
*/

//void CODKStream::HandleOsMatchDelta(const CMsgOsMatchDelta* pmsg) {
//	if (!pmsg->fPlus) {
//		(*this) << "t /os ask s8r18 5:00 piglet\n";
//		flush();
//	}
//	BaseOsMatchDelta(pmsg);
//}

//message .end
void CODKStream::HandleOsEnd(const CMsgOsEnd *pmsg) {
    
    BaseOsEnd(pmsg);
    COsGame* pgame=PGame(pmsg->idg);
    if (pgame!=NULL) {
        pComputer->stop_engine(pmsg->idg, pgame);
    }
}

void CODKStream::HandleOsTimeout(const CMsgOsTimeout* pmsg){
    
    std::cout  << "timeout: " << pmsg->idg << " " << pmsg->sLogin  << std::endl;
    
    //COsGame* pgame=PGame(pmsg->idg);
    //Adjournes [.match]
    if(pmsg->sLogin == GetLogin()) {
        (*this) << "t /os break " << pmsg->idg << "\n";
        //BaseOsGameOver(pmsg->idg);
    } /*else if (pgame!=NULL) {
        pComputer->stop_engine(pmsg->idg, pgame);
    }*/
    

}


  // Example handler from Roxane:
void CODKStream::HandleOsRequestDelta(const CMsgOsRequestDelta* pmsg) {
	BaseOsRequestDelta(pmsg);

	if (pmsg->fPlus && pmsg->IAmChallenged()) {
		if (pmsg->RequireBoardSize(8) && pmsg->RequireKomi(false) && pmsg->RequireAnti(false) &&
			pmsg->RequireRated(true) && pmsg->RequireSynch(true) &&
			pmsg->RequireRand(true) && pmsg->RequireMaxRandDiscs(24) && pmsg->RequireMinRandDiscs(14) &&
			pmsg->RequireMaxOpponentClock(COsClock(30*60,0,2*60)) &&
			pmsg->RequireMinMyClock(COsClock(60,0,0)))
			(*this) << "t /os accept " << pmsg->idr << "\n";
		else
			(*this) << "t /os decline " << pmsg->idr << "\n";

		flush();
	}
}



void CODKStream::HandleOsGameOver(const CMsgOsMatchDelta* pmsg,const string& idg) {
        if (pmsg->match.IsPlaying(GetLogin()))
			pComputer->resume();
		BaseOsGameOver(idg);
}


void CODKStream::HandleOsUnknown(const CMsgOsUnknown* pmsg) {
	cout << "Unknown /os message: ";
	HandleOs(pmsg);
}

void CODKStream::HandleOsUpdate(const CMsgOsUpdate* pmsg) {
	BaseOsUpdate(pmsg);
	GetMoveIfNeeded(pmsg->idg);
}

// helper function for join and update messages
void CODKStream::GetMoveIfNeeded(const string& idg) {
        
	COsGame* pgame=PGame(idg);
	if (pgame!=NULL) {
	

		bool fMyMove=pgame->ToMove(GetLogin());
		//COsMoveListItem mli;

		if (fMyMove) {
		
			pComputer->get_move(idg, pgame);
		
		/*
			pComputer->get_move(*pgame, mli);
			(*this) << "tell /os play " << idg << " " << mli << "\n";
			flush();
		*/
		}
	}
	else
		_ASSERT(0);
}

void CODKStream::SendMove(const string& idg, COsMoveListItem& mli) {
	(*this) << "tell /os play " << idg << " " << mli << "\n";
	flush();

}

void CODKStream::SendMsg(const string& msg) {

	if(IsConnected()) {
		(*this) << "tell ." << GetLogin() << " " << msg << std::endl;
		flush();
	} else {
		std::cout << msg << std::endl;
	}

}
