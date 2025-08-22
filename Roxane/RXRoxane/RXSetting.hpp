//
//  RXSetting.hpp
//  Roxane
//
//  Created by Causse Bruno on 15/07/2025.
//


#ifndef RXSETTING_HPP
#define RXSETTING_HPP


#ifdef __ARM_ACLE
#define NEW_EVAL //-h 18 -t 1 fforum-40-59 -5% midgame-1-10
#endif

/*
 Matchs avec / sans alpha_cut
 91/157/49 [297]
 #define USE_PROBCUT_ALPHA
 */

#define USE_IMPROVE_HELPFUL_MASTER_CONCEPT

//for record fforum-40-59 setting
//Matchs avec / sans :
//69/319/58 [446] s8r14 2:00
//15,5%/71,5%/13%
#define USE_SPLIT_AT_ROOT

//not efficient
//#define USE_SPLIT_AT_ETC_MOBILITY


#define USE_ETC

#define USE_STABILITY
#define USE_ENHANCED_STABLILITY

//for record solver
//#define SOLVER_DRIVER
//#define SOLVER_SETTING

//check pv in endgame at 100%
//#define EG_CHECK_PV

//generate data for probcut
//#define TUNE_PROBCUT_MID
//#define TUNE_PROBCUT_END

//
//#define PROBCUT_x2 // si non defini use PROBCUT_x3

#endif
