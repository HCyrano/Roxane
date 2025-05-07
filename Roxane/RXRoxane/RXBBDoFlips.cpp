
/*
 *  RXBBDoFlips.cpp
 *  Roxane
 *
 *  Created by Bruno Causse on 27/06/05.
 *  Copyleft 2005-2025 personnel.
 *
 */

#ifndef __ARM_NEON

#include "RXBitBoard.hpp"
#include "RXTools.hpp"

bool RXBitBoard::do_flips_A1(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SE */
	my_discs = discs_player & 0X0000201008040201ULL;
	opp_discs = (discs_opponent & 0X0040201008040200ULL) | 0X7FBFDFEFF7FBFC00ULL;
	r = ((my_discs<<9) + opp_discs) & 0X8000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0040201008040200ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000808080808080ULL;
	opp_discs = (discs_opponent & 0X0080808080808000ULL) | 0X7F7F7F7F7F7F0000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X8000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0080808080808000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X3F00000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X8000000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X8000000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B1(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SE */
	my_discs = discs_player & 0X0000100804020100ULL;
	opp_discs = (discs_opponent & 0X0020100804020000ULL) | 0X3FDFEFF7FBFC0000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X4000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0020100804020000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000404040404040ULL;
	opp_discs = (discs_opponent & 0X0040404040404000ULL) | 0X3FBFBFBFBFBF8000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X4000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0040404040404000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X1F00000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X4000000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X4000000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C1(unsigned long long& discs_player, unsigned long long& discs_opponent) {
	
	unsigned long long my_discs, opp_discs, r;
	
	/* direction _W */
	unsigned long long flipped = (discs_opponent & 0X4000000000000000ULL) & (discs_player >> 1);

	/* direction SW */
	flipped |= (discs_opponent & 0X0040000000000000ULL) & (discs_player << 7);

	/* direction S */
	my_discs = discs_player & 0X0000202020202020ULL;
	opp_discs = (discs_opponent & 0X0020202020202000ULL) | 0X1FDFDFDFDFDFC000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X2000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0020202020202000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000080402010000ULL;
	opp_discs = (discs_opponent & 0X0010080402000000ULL) | 0X1FEFF7FBFC000000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X2000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0010080402000000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0F00000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X2000000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}


	if(flipped) {
		discs_player |= (flipped | 0X2000000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_D1(unsigned long long& discs_player, unsigned long long& discs_opponent) {
	
	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction S */
	my_discs = discs_player & 0X0000101010101010ULL;
	opp_discs = (discs_opponent & 0X0010101010101000ULL) | 0X0FEFEFEFEFEFE000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X1000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0010101010101000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000040201000000ULL;
	opp_discs = (discs_opponent & 0X0008040200000000ULL) | 0X0FF7FBFC00000000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X1000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0008040200000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000408000000000ULL;
	opp_discs = (discs_opponent & 0X0020400000000000ULL) | 0X0FDF800000000000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X1000000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0020400000000000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0700000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X1000000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0XC000000000000000ULL;
	r = (discs_opponent + 0X2000000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0XE000000000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X1000000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_E1(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction S */
	my_discs = discs_player & 0X0000080808080808ULL;
	opp_discs = (discs_opponent & 0X0008080808080800ULL) | 0X07F7F7F7F7F7F000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0800000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0008080808080800ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000020100000000ULL;
	opp_discs = (discs_opponent & 0X0004020000000000ULL) | 0X07FBFC0000000000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0800000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0004020000000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000204080000000ULL;
	opp_discs = (discs_opponent & 0X0010204000000000ULL) | 0X07EFDF8000000000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0800000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0010204000000000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0300000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0800000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0XE000000000000000ULL;
	r = (discs_opponent + 0X1000000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0XF000000000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0800000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F1(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	
	/* direction _E */
	unsigned long long flipped =(discs_opponent & 0X0200000000000000ULL) & (discs_player << 1);
	
	/* direction S */
	my_discs = discs_player & 0X0000040404040404ULL;
	opp_discs = (discs_opponent & 0X0004040404040400ULL) | 0X03FBFBFBFBFBF800ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0400000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0004040404040400ULL;
	}

	/* direction SE */
	flipped |= (discs_opponent & 0X0002000000000000ULL) & (discs_player << 9);

	/* direction SW */
	my_discs = discs_player & 0X0000102040800000ULL;
	opp_discs = (discs_opponent & 0X0008102040000000ULL) | 0X03F7EFDF80000000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0400000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0008102040000000ULL;
	}


	/* direction _W */
	my_discs = discs_player & 0XF000000000000000ULL;
	r = (discs_opponent + 0X0800000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0XF800000000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0400000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G1(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SW */
	my_discs = discs_player & 0X0000081020408000ULL;
	opp_discs = (discs_opponent & 0X0004081020400000ULL) | 0X01FBF7EFDF800000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0200000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0004081020400000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0XF800000000000000ULL;
	r = (discs_opponent + 0X0400000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0XFC00000000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000020202020202ULL;
	opp_discs = (discs_opponent & 0X0002020202020200ULL) | 0X01FDFDFDFDFDFC00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0200000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0002020202020200ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0200000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H1(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SW */
	my_discs = discs_player & 0X0000040810204080ULL;
	opp_discs = (discs_opponent & 0X0002040810204000ULL) | 0X00FDFBF7EFDF8000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0100000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0002040810204000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0XFC00000000000000ULL;
	r = (discs_opponent + 0X0200000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0XFE00000000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000010101010101ULL;
	opp_discs = (discs_opponent & 0X0001010101010100ULL) | 0X00FEFEFEFEFEFE00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0100000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0001010101010100ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0100000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SE */
	my_discs = discs_player & 0X0000002010080402ULL;
	opp_discs = (discs_opponent & 0X0000402010080400ULL) | 0X007FBFDFEFF7F800ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0080000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000402010080400ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000008080808080ULL;
	opp_discs = (discs_opponent & 0X0000808080808000ULL) | 0X007F7F7F7F7F0000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0080000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000808080808000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X003F000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0080000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0080000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SE */
	my_discs = discs_player & 0X0000001008040201ULL;
	opp_discs = (discs_opponent & 0X0000201008040200ULL) | 0X003FDFEFF7FBFC00ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0040000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000201008040200ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000004040404040ULL;
	opp_discs = (discs_opponent & 0X0000404040404000ULL) | 0X003FBFBFBFBF8000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0040000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000404040404000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X001F000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0040000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0040000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SW */
	unsigned long long flipped = (discs_opponent & 0X0000400000000000ULL) & (discs_player << 7);
	
	/* direction S */
	my_discs = discs_player & 0X0000002020202020ULL;
	opp_discs = (discs_opponent & 0X0000202020202000ULL) | 0X001FDFDFDFDFC000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0020000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000202020202000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000804020100ULL;
	opp_discs = (discs_opponent & 0X0000100804020000ULL) | 0X001FEFF7FBFC0000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0020000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000100804020000ULL;
	}


	/* direction _E */
	my_discs = discs_player & 0X000F000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0020000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	flipped |= (discs_opponent & 0X0040000000000000ULL) & (discs_player >> 1);

	if(flipped) {
		discs_player |= (flipped | 0X0020000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_D2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction S */
	my_discs = discs_player & 0X0000001010101010ULL;
	opp_discs = (discs_opponent & 0X0000101010101000ULL) | 0X000FEFEFEFEFE000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0010000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000101010101000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000402010000ULL;
	opp_discs = (discs_opponent & 0X0000080402000000ULL) | 0X000FF7FBFC000000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0010000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000080402000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000004080000000ULL;
	opp_discs = (discs_opponent & 0X0000204000000000ULL) | 0X000FDF8000000000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0010000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000204000000000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0007000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0010000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0X00C0000000000000ULL;
	r = (discs_opponent + 0X0020000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00E0000000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0010000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_E2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction S */
	my_discs = discs_player & 0X0000000808080808ULL;
	opp_discs = (discs_opponent & 0X0000080808080800ULL) | 0X0007F7F7F7F7F000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0008000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000080808080800ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000201000000ULL;
	opp_discs = (discs_opponent & 0X0000040200000000ULL) | 0X0007FBFC00000000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0008000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000040200000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000002040800000ULL;
	opp_discs = (discs_opponent & 0X0000102040000000ULL) | 0X0007EFDF80000000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0008000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000102040000000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0003000000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0008000000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0X00E0000000000000ULL;
	r = (discs_opponent + 0X0010000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00F0000000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0008000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction _E */
	unsigned long long flipped = (discs_opponent & 0X0002000000000000ULL) & (discs_player << 1);
	
	/* direction SE */
	flipped |= (discs_opponent & 0X0000020000000000ULL) & (discs_player << 9);
	
	/* direction S */
	my_discs = discs_player & 0X0000000404040404ULL;
	opp_discs = (discs_opponent & 0X0000040404040400ULL) | 0X0003FBFBFBFBF800ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0004000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000040404040400ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000001020408000ULL;
	opp_discs = (discs_opponent & 0X0000081020400000ULL) | 0X0003F7EFDF800000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0004000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000081020400000ULL;
	}


	/* direction _W */
	my_discs = discs_player & 0X00F0000000000000ULL;
	r = (discs_opponent + 0X0008000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00F8000000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0004000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SW */
	my_discs = discs_player & 0X0000000810204080ULL;
	opp_discs = (discs_opponent & 0X0000040810204000ULL) | 0X0001FBF7EFDF8000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0002000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000040810204000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X00F8000000000000ULL;
	r = (discs_opponent + 0X0004000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00FC000000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000202020202ULL;
	opp_discs = (discs_opponent & 0X0000020202020200ULL) | 0X0001FDFDFDFDFC00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0002000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000020202020200ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0002000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H2(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction SW */
	my_discs = discs_player & 0X0000000408102040ULL;
	opp_discs = (discs_opponent & 0X0000020408102000ULL) | 0X0000FDFBF7EFC000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0001000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000020408102000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X00FC000000000000ULL;
	r = (discs_opponent + 0X0002000000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00FE000000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000101010101ULL;
	opp_discs = (discs_opponent & 0X0000010101010100ULL) | 0X0000FEFEFEFEFE00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0001000000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000010101010100ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0001000000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction N_ */
	unsigned long long flipped = (discs_opponent & 0X0080000000000000ULL) & (discs_player >> 8);

	/* direction NE */
	flipped |= (discs_opponent & 0X0040000000000000ULL) & (discs_player >> 7);
		
	/* direction _E */
	my_discs = discs_player & 0X00003F0000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000800000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000020100804ULL;
	opp_discs = (discs_opponent & 0X0000004020100800ULL) | 0X00007FBFDFEFF000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000800000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000004020100800ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000080808080ULL;
	opp_discs = (discs_opponent & 0X0000008080808000ULL) | 0X00007F7F7F7F0000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000800000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000008080808000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000800000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NE */
	unsigned long long flipped = (discs_opponent & 0X0020000000000000ULL) & (discs_player >> 7);

	/* direction N_ */
	flipped |= (discs_opponent & 0X0040000000000000ULL) & (discs_player >> 8);
		
	/* direction _E */
	my_discs = discs_player & 0X00001F0000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000400000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000010080402ULL;
	opp_discs = (discs_opponent & 0X0000002010080400ULL) | 0X00003FDFEFF7F800ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000400000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000002010080400ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000040404040ULL;
	opp_discs = (discs_opponent & 0X0000004040404000ULL) | 0X00003FBFBFBF8000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000400000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000004040404000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000400000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C3(unsigned long long& discs_player, unsigned long long& discs_opponent) {
	
	unsigned long long my_discs, opp_discs, r;
		
	/* direction NE */
	unsigned long long flipped = (discs_opponent & 0X0010000000000000ULL) & (discs_player >> 7);
	/* direction N_ */
	flipped |= (discs_opponent & 0X0020000000000000ULL) & (discs_player >> 8);
	
	/* direction NW */
	flipped |= (discs_opponent & 0X0040000000000000ULL) & (discs_player >> 9);
	
	/* direction SW */
	flipped |= (discs_opponent & 0X0000004000000000ULL) & (discs_player << 7);
	
	/* direction _W */
	flipped |= (discs_opponent & 0X0000400000000000ULL) & (discs_player >> 1);
	
	/* direction SE */
	my_discs = discs_player & 0X0000000008040201ULL;
	opp_discs = (discs_opponent & 0X0000001008040200ULL) | 0X00001FEFF7FBFC00ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000200000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000001008040200ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000020202020ULL;
	opp_discs = (discs_opponent & 0X0000002020202000ULL) | 0X00001FDFDFDFC000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000200000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000002020202000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X00000F0000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000200000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}


	if(flipped) {
		discs_player |= (flipped | 0X0000200000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_D3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NE */
	unsigned long long flipped = (discs_opponent & 0X0008000000000000ULL) & (discs_player >> 7);

	/* direction N_ */
	flipped |= (discs_opponent & 0X0010000000000000ULL) & (discs_player >> 8);
	
	/* direction NW */
	flipped |= (discs_opponent & 0X0020000000000000ULL) & (discs_player >> 9);
	
	/* direction SE */
	my_discs = discs_player & 0X0000000004020100ULL;
	opp_discs = (discs_opponent & 0X0000000804020000ULL) | 0X00000FF7FBFC0000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000100000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000804020000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000010101010ULL;
	opp_discs = (discs_opponent & 0X0000001010101000ULL) | 0X00000FEFEFEFE000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000100000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000001010101000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000070000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000100000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000040800000ULL;
	opp_discs = (discs_opponent & 0X0000002040000000ULL) | 0X00000FDF80000000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000100000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000002040000000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X0000C00000000000ULL;
	r = (discs_opponent + 0X0000200000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000E00000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000100000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_E3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NE */
	unsigned long long flipped = (discs_opponent & 0X0004000000000000ULL) & (discs_player >> 7);
	
	/* direction N_ */
	flipped |= (discs_opponent & 0X0008000000000000ULL) & (discs_player >> 8);
	
	/* direction NW */
	flipped |= (discs_opponent & 0X0010000000000000ULL) & (discs_player >> 9);
	
	/* direction SE */
	my_discs = discs_player & 0X0000000002010000ULL;
	opp_discs = (discs_opponent & 0X0000000402000000ULL) | 0X000007FBFC000000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000080000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000402000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000008080808ULL;
	opp_discs = (discs_opponent & 0X0000000808080800ULL) | 0X000007F7F7F7F000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000080000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000808080800ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000030000000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000080000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}


	/* direction SW */
	my_discs = discs_player & 0X0000000020408000ULL;
	opp_discs = (discs_opponent & 0X0000001020400000ULL) | 0X000007EFDF800000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000080000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000001020400000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X0000E00000000000ULL;
	r = (discs_opponent + 0X0000100000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000F00000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000080000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped = (discs_opponent & 0X0000000200000000ULL) & (discs_player << 9);

	/* direction _E */
	flipped |= (discs_opponent & 0X0000020000000000ULL) & (discs_player << 1);
	
	/* direction NE */
	flipped |= (discs_opponent & 0X0002000000000000ULL) & (discs_player >> 7);
	
	/* direction N_ */
	flipped |= (discs_opponent & 0X0004000000000000ULL) & (discs_player >> 8);
	
	/* direction NW */
	flipped |= (discs_opponent & 0X0008000000000000ULL) & (discs_player >> 9);
		
	/* direction S */
	my_discs = discs_player & 0X0000000004040404ULL;
	opp_discs = (discs_opponent & 0X0000000404040400ULL) | 0X000003FBFBFBF800ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000040000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000404040400ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000010204080ULL;
	opp_discs = (discs_opponent & 0X0000000810204000ULL) | 0X000003F7EFDF8000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000040000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000810204000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X0000F00000000000ULL;
	r = (discs_opponent + 0X0000080000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000F80000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000040000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NW */
	unsigned long long flipped = (discs_opponent & 0X0004000000000000ULL) & (discs_player >> 9);

	/* direction N_ */
	flipped |= (discs_opponent & 0X0002000000000000ULL) & (discs_player >> 8);
		
	/* direction _W */
	my_discs = discs_player & 0X0000F80000000000ULL;
	r = (discs_opponent + 0X0000040000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000FC0000000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000008102040ULL;
	opp_discs = (discs_opponent & 0X0000000408102000ULL) | 0X000001FBF7EFC000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000020000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000408102000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000002020202ULL;
	opp_discs = (discs_opponent & 0X0000000202020200ULL) | 0X000001FDFDFDFC00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000020000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000202020200ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000020000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H3(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;

	/* direction NW */
	unsigned long long flipped = (discs_opponent & 0X0002000000000000ULL) & (discs_player >> 9);

	/* direction N_ */
	flipped |= (discs_opponent & 0X0001000000000000ULL) & (discs_player >> 8);
		
	/* direction _W */
	my_discs = discs_player & 0X0000FC0000000000ULL;
	r = (discs_opponent + 0X0000020000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000FE0000000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000004081020ULL;
	opp_discs = (discs_opponent & 0X0000000204081000ULL) | 0X000000FDFBF7E000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000010000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000204081000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000001010101ULL;
	opp_discs = (discs_opponent & 0X0000000101010100ULL) | 0X000000FEFEFEFE00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000010000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000101010100ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000010000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A4(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _E */
	my_discs = discs_player & 0X0000003F00000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000008000000000ULL;
	if(r != 0) {
		flipped |= 0X0000008000000000ULL - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X1020000000000000ULL;
	opp_discs = (discs_opponent & 0X0020400000000000ULL) | 0X0FDF800000000000ULL;
	r = (opp_discs + 0X0000400000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020400000000000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000000201008ULL;
	opp_discs = (discs_opponent & 0X0000000040201000ULL) | 0X0000007FBFDFE000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000008000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000040201000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X8080000000000000ULL;
	opp_discs = (discs_opponent & 0X0080800000000000ULL) | 0X7F7F000000000000ULL;
	r = (opp_discs + 0X0000800000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0080800000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000808080ULL;
	opp_discs = (discs_opponent & 0X0000000080808000ULL) | 0X0000007F7F7F0000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000008000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000080808000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000008000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B4(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _E */
	my_discs = discs_player & 0X0000001F00000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000004000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0810000000000000ULL;
	opp_discs = (discs_opponent & 0X0010200000000000ULL) | 0X07EFC00000000000ULL;
	r = (opp_discs + 0X0000200000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010200000000000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000000100804ULL;
	opp_discs = (discs_opponent & 0X0000000020100800ULL) | 0X0000003FDFEFF000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000004000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000020100800ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X4040000000000000ULL;
	opp_discs = (discs_opponent & 0X0040400000000000ULL) | 0X3FBF800000000000ULL;
	r = (opp_discs + 0X0000400000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040400000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000404040ULL;
	opp_discs = (discs_opponent & 0X0000000040404000ULL) | 0X0000003FBFBF8000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000004000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000040404000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000004000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C4(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NW */
	unsigned long long flipped = (discs_opponent & 0X0000400000000000ULL) & (discs_player >> 9);

	/* direction SW */
	flipped |= (discs_opponent & 0X0000000040000000ULL) & (discs_player << 7);
	
	/* direction _W */
	flipped |= (discs_opponent & 0X0000004000000000ULL) & (discs_player >> 1);
		
	/* direction SE */
	my_discs = discs_player & 0X0000000000080402ULL;
	opp_discs = (discs_opponent & 0X0000000010080400ULL) | 0X0000001FEFF7F800ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000002000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000010080400ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000202020ULL;
	opp_discs = (discs_opponent & 0X0000000020202000ULL) | 0X0000001FDFDFC000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000002000000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000020202000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000F00000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000002000000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0408000000000000ULL;
	opp_discs = (discs_opponent & 0X0008100000000000ULL) | 0X03F7E00000000000ULL;
	r = (opp_discs + 0X0000100000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008100000000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X2020000000000000ULL;
	opp_discs = (discs_opponent & 0X0020200000000000ULL) | 0X1FDFC00000000000ULL;
	r = (opp_discs + 0X0000200000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020200000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000002000000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F4(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction _E */
	unsigned long long flipped = (discs_opponent & 0X0000000200000000ULL) & (discs_player << 1);
	
	/* direction NE */
	flipped |= (discs_opponent & 0X0000020000000000ULL) & (discs_player >> 7);
	
	/* direction SE */
	flipped |= (discs_opponent & 0X0000000002000000ULL) & (discs_player << 9);

	/* direction S */
	my_discs = discs_player & 0X0000000000040404ULL;
	opp_discs = (discs_opponent & 0X0000000004040400ULL) | 0X00000003FBFBF800ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000400000000ULL;
	if(r != 0) {
		flipped |= (0X0000000400000000ULL - (0X2ULL<<_bsrll(my_discs))) & 0X0000000004040400ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0404000000000000ULL;
	opp_discs = (discs_opponent & 0X0004040000000000ULL) | 0X03FBF80000000000ULL;
	r = (opp_discs + 0X0000040000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004040000000000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X2010000000000000ULL;
	opp_discs = (discs_opponent & 0X0010080000000000ULL) | 0X1FEFF00000000000ULL;
	r = (opp_discs + 0X0000080000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010080000000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000000102040ULL;
	opp_discs = (discs_opponent & 0X0000000008102000ULL) | 0X00000003F7EFC000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000000400000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000008102000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X000000F000000000ULL;
	r = (discs_opponent + 0X0000000800000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000F800000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000400000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G4(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _W */
	my_discs = discs_player & 0X000000F800000000ULL;
	r = (discs_opponent + 0X0000000400000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000FC00000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000000081020ULL;
	opp_discs = (discs_opponent & 0X0000000004081000ULL) | 0X00000001FBF7E000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000000200000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000004081000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X1008000000000000ULL;
	opp_discs = (discs_opponent & 0X0008040000000000ULL) | 0X0FF7F80000000000ULL;
	r = (opp_discs + 0X0000040000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008040000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000020202ULL;
	opp_discs = (discs_opponent & 0X0000000002020200ULL) | 0X00000001FDFDFC00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000200000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000002020200ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0202000000000000ULL;
	opp_discs = (discs_opponent & 0X0002020000000000ULL) | 0X01FDFC0000000000ULL;
	r = (opp_discs + 0X0000020000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002020000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000200000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H4(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _W */
	my_discs = discs_player & 0X000000FC00000000ULL;
	r = (discs_opponent + 0X0000000200000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000FE00000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000000040810ULL;
	opp_discs = (discs_opponent & 0X0000000002040800ULL) | 0X00000000FDFBF000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000000100000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000002040800ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0804000000000000ULL;
	opp_discs = (discs_opponent & 0X0004020000000000ULL) | 0X07FBFC0000000000ULL;
	r = (opp_discs + 0X0000020000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004020000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000010101ULL;
	opp_discs = (discs_opponent & 0X0000000001010100ULL) | 0X00000000FEFEFE00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000100000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000001010100ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0101000000000000ULL;
	opp_discs = (discs_opponent & 0X0001010000000000ULL) | 0X00FEFE0000000000ULL;
	r = (opp_discs + 0X0000010000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0001010000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000100000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A5(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _E */
	my_discs = discs_player & 0X000000003F000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000080000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0810200000000000ULL;
	opp_discs = (discs_opponent & 0X0010204000000000ULL) | 0X07EFDF8000000000ULL;
	r = (opp_discs + 0X0000004000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010204000000000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000000002010ULL;
	opp_discs = (discs_opponent & 0X0000000000402000ULL) | 0X000000007FBFC000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000000080000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000402000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X8080800000000000ULL;
	opp_discs = (discs_opponent & 0X0080808000000000ULL) | 0X7F7F7F0000000000ULL;
	r = (opp_discs + 0X0000008000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0080808000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000008080ULL;
	opp_discs = (discs_opponent & 0X0000000000808000ULL) | 0X000000007F7F0000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000080000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000808000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000080000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B5(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _E */
	my_discs = discs_player & 0X000000001F000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000040000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0408100000000000ULL;
	opp_discs = (discs_opponent & 0X0008102000000000ULL) | 0X03F7EFC000000000ULL;
	r = (opp_discs + 0X0000002000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008102000000000ULL;
	}

	/* direction SE */
	my_discs = discs_player & 0X0000000000001008ULL;
	opp_discs = (discs_opponent & 0X0000000000201000ULL) | 0X000000003FDFE000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000000040000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000201000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X4040400000000000ULL;
	opp_discs = (discs_opponent & 0X0040404000000000ULL) | 0X3FBFBF8000000000ULL;
	r = (opp_discs + 0X0000004000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040404000000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000004040ULL;
	opp_discs = (discs_opponent & 0X0000000000404000ULL) | 0X000000003FBF8000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000040000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000404000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000040000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C5(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NW */
	unsigned long long flipped = (discs_opponent & 0X0000004000000000ULL) & (discs_player >> 9);
	
	/* direction SW */
	flipped |= (discs_opponent & 0X0000000000400000ULL) & (discs_player << 7);
	
	/* direction _W */
	flipped |= (discs_opponent & 0X0000000040000000ULL) & (discs_player >> 1);
	
	/* direction SE */
	my_discs = discs_player & 0X0000000000000804ULL;
	opp_discs = (discs_opponent & 0X0000000000100800ULL) | 0X000000001FEFF000ULL;
	r = ((my_discs<<9) + opp_discs) & 0X0000000020000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000100800ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000002020ULL;
	opp_discs = (discs_opponent & 0X0000000000202000ULL) | 0X000000001FDFC000ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000020000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000202000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X000000000F000000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000020000000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0204080000000000ULL;
	opp_discs = (discs_opponent & 0X0004081000000000ULL) | 0X01FBF7E000000000ULL;
	r = (opp_discs + 0X0000001000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004081000000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X2020200000000000ULL;
	opp_discs = (discs_opponent & 0X0020202000000000ULL) | 0X1FDFDFC000000000ULL;
	r = (opp_discs + 0X0000002000000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020202000000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000020000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F5(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped = (discs_opponent & 0X0000000000020000ULL) & (discs_player << 9);
	
	/* direction _E */
	flipped |= (discs_opponent & 0X0000000002000000ULL) & (discs_player << 1);
	
	/* direction NE */
	flipped |= (discs_opponent & 0X0000000200000000ULL) & (discs_player >> 7);
	
	/* direction S */
	my_discs = discs_player & 0X0000000000000404ULL;
	opp_discs = (discs_opponent & 0X0000000000040400ULL) | 0X0000000003FBF800ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000004000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000040400ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0404040000000000ULL;
	opp_discs = (discs_opponent & 0X0004040400000000ULL) | 0X03FBFBF800000000ULL;
	r = (opp_discs + 0X0000000400000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004040400000000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X4020100000000000ULL;
	opp_discs = (discs_opponent & 0X0020100800000000ULL) | 0X3FDFEFF000000000ULL;
	r = (opp_discs + 0X0000000800000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020100800000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000000001020ULL;
	opp_discs = (discs_opponent & 0X0000000000081000ULL) | 0X0000000003F7E000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000000004000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000081000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X00000000F0000000ULL;
	r = (discs_opponent + 0X0000000008000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000F8000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000004000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G5(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _W */
	my_discs = discs_player & 0X00000000F8000000ULL;
	r = (discs_opponent + 0X0000000004000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000FC000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000000000810ULL;
	opp_discs = (discs_opponent & 0X0000000000040800ULL) | 0X0000000001FBF000ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000000002000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000040800ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X2010080000000000ULL;
	opp_discs = (discs_opponent & 0X0010080400000000ULL) | 0X1FEFF7F800000000ULL;
	r = (opp_discs + 0X0000000400000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010080400000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000000202ULL;
	opp_discs = (discs_opponent & 0X0000000000020200ULL) | 0X0000000001FDFC00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000002000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000020200ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0202020000000000ULL;
	opp_discs = (discs_opponent & 0X0002020200000000ULL) | 0X01FDFDFC00000000ULL;
	r = (opp_discs + 0X0000000200000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002020200000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000002000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H5(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction _W */
	my_discs = discs_player & 0X00000000FC000000ULL;
	r = (discs_opponent + 0X0000000002000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000FE000000ULL;
	}

	/* direction SW */
	my_discs = discs_player & 0X0000000000000408ULL;
	opp_discs = (discs_opponent & 0X0000000000020400ULL) | 0X0000000000FDF800ULL;
	r = ((my_discs<<7) + opp_discs) & 0X0000000001000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000020400ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X1008040000000000ULL;
	opp_discs = (discs_opponent & 0X0008040200000000ULL) | 0X0FF7FBFC00000000ULL;
	r = (opp_discs + 0X0000000200000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008040200000000ULL;
	}

	/* direction S */
	my_discs = discs_player & 0X0000000000000101ULL;
	opp_discs = (discs_opponent & 0X0000000000010100ULL) | 0X0000000000FEFE00ULL;
	r = ((my_discs<<8) + opp_discs) & 0X0000000001000000ULL;
	if(r != 0) {
		flipped |= (r - (0X2ULL<<_bsrll(my_discs))) & 0X0000000000010100ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0101010000000000ULL;
	opp_discs = (discs_opponent & 0X0001010100000000ULL) | 0X00FEFEFE00000000ULL;
	r = (opp_discs + 0X0000000100000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0001010100000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000001000000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped =(discs_opponent & 0X0000000000004000ULL) & (discs_player << 9);

	/* direction S */
	flipped |= (discs_opponent & 0X0000000000008000ULL) & (discs_player << 8);
	
	/* direction _E */
	my_discs = discs_player & 0X00000000003F0000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000800000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0408102000000000ULL;
	opp_discs = (discs_opponent & 0X0008102040000000ULL) | 0X03F7EFDF80000000ULL;
	r = (opp_discs + 0X0000000040000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008102040000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X8080808000000000ULL;
	opp_discs = (discs_opponent & 0X0080808080000000ULL) | 0X7F7F7F7F00000000ULL;
	r = (opp_discs + 0X0000000080000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0080808080000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000800000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction S */
	unsigned long long flipped = (discs_opponent & 0X0000000000004000ULL) & (discs_player << 8);
	
	/* direction SE */
	flipped |= (discs_opponent & 0X0000000000002000ULL) & (discs_player << 9);
	
	/* direction _E */
	my_discs = discs_player & 0X00000000001F0000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000400000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));		
	}

	/* direction NE */
	my_discs = discs_player & 0X0204081000000000ULL;
	opp_discs = (discs_opponent & 0X0004081020000000ULL) | 0X01FBF7EFC0000000ULL;
	r = (opp_discs + 0X0000000020000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004081020000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X4040404000000000ULL;
	opp_discs = (discs_opponent & 0X0040404040000000ULL) | 0X3FBFBFBF80000000ULL;
	r = (opp_discs + 0X0000000040000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040404040000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000400000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped = (discs_opponent & 0X0000000000001000ULL) & (discs_player << 9);

	/* direction S */
	flipped |= (discs_opponent & 0X0000000000002000ULL) & (discs_player << 8);
	
	/* direction NW */
	flipped |= (discs_opponent & 0X0000000040000000ULL) & (discs_player >> 9);
	
	/* direction SW */
	flipped |= (discs_opponent & 0X0000000000004000ULL) & (discs_player << 7);
	
	/* direction _W */
	flipped |= (discs_opponent & 0X0000000000400000ULL) & (discs_player >> 1);
	
	/* direction _E */
	my_discs = discs_player & 0X00000000000F0000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000200000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0102040800000000ULL;
	opp_discs = (discs_opponent & 0X0002040810000000ULL) | 0X00FDFBF7E0000000ULL;
	r = (opp_discs + 0X0000000010000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002040810000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X2020202000000000ULL;
	opp_discs = (discs_opponent & 0X0020202020000000ULL) | 0X1FDFDFDFC0000000ULL;
	r = (opp_discs + 0X0000000020000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020202020000000ULL;
	}
	

	if(flipped) {
		discs_player |= (flipped | 0X0000000000200000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_D6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped = (discs_opponent & 0X0000000000000800ULL) & (discs_player << 9);
	
	/* direction S */
	flipped |= (discs_opponent & 0X0000000000001000ULL) & (discs_player << 8);
	
	/* direction SW */
	flipped |= (discs_opponent & 0X0000000000002000ULL) & (discs_player << 7);
	
	/* direction _E */
	my_discs = discs_player & 0X0000000000070000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000100000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0001020400000000ULL;
	opp_discs = (discs_opponent & 0X0000020408000000ULL) | 0X0000FDFBF0000000ULL;
	r = (opp_discs + 0X0000000008000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000020408000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X1010101000000000ULL;
	opp_discs = (discs_opponent & 0X0010101010000000ULL) | 0X0FEFEFEFE0000000ULL;
	r = (opp_discs + 0X0000000010000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010101010000000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0000804000000000ULL;
	opp_discs = (discs_opponent & 0X0000004020000000ULL) | 0X00007FBFC0000000ULL;
	r = (opp_discs + 0X0000000020000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000004020000000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X0000000000C00000ULL;
	r = (discs_opponent + 0X0000000000200000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000E00000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000100000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_E6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped = (discs_opponent & 0X0000000000000400ULL) & (discs_player << 9);

	/* direction S */
	flipped |= (discs_opponent & 0X0000000000000800ULL) & (discs_player << 8);
	
	/* direction SW */
	flipped |= (discs_opponent & 0X0000000000001000ULL) & (discs_player << 7);
	
	/* direction _E */
	my_discs = discs_player & 0X0000000000030000ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000080000ULL;
	if(r != 0) {
		flipped |= 0X0000000000080000ULL - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction NE */
	my_discs = discs_player & 0X0000010200000000ULL;
	opp_discs = (discs_opponent & 0X0000000204000000ULL) | 0X000000FDF8000000ULL;
	r = (opp_discs + 0X0000000004000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000204000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0808080800000000ULL;
	opp_discs = (discs_opponent & 0X0008080808000000ULL) | 0X07F7F7F7F0000000ULL;
	r = (opp_discs + 0X0000000008000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008080808000000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0080402000000000ULL;
	opp_discs = (discs_opponent & 0X0000402010000000ULL) | 0X007FBFDFE0000000ULL;
	r = (opp_discs + 0X0000000010000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000402010000000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X0000000000E00000ULL;
	r = (discs_opponent + 0X0000000000100000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000F00000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000080000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SE */
	unsigned long long flipped = (discs_opponent & 0X0000000000000200ULL) & (discs_player << 9);

	/* direction S */
	flipped |= (discs_opponent & 0X0000000000000400ULL) & (discs_player << 8);

	/* direction _E */
	flipped |= (discs_opponent & 0X0000000000020000ULL) & (discs_player << 1);

	/* direction NE */
	flipped |= (discs_opponent & 0X0000000002000000ULL) & (discs_player >> 7);
	
	/* direction SW */
	flipped |= (discs_opponent & 0X0000000000000800ULL) & (discs_player << 7);
	
	/* direction N_ */
	my_discs = discs_player & 0X0404040400000000ULL;
	opp_discs = (discs_opponent & 0X0004040404000000ULL) | 0X03FBFBFBF8000000ULL;
	r = (opp_discs + 0X0000000004000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004040404000000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X8040201000000000ULL;
	opp_discs = (discs_opponent & 0X0040201008000000ULL) | 0X7FBFDFEFF0000000ULL;
	r = (opp_discs + 0X0000000008000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040201008000000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X0000000000F00000ULL;
	r = (discs_opponent + 0X0000000000080000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000F80000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000040000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SW */
	unsigned long long flipped = (discs_opponent & 0X0000000000000400ULL) & (discs_player << 7);
	
	/* direction S */
	flipped |= (discs_opponent & 0X0000000000000200ULL) & (discs_player << 8);
	
	/* direction _W */
	my_discs = discs_player & 0X0000000000F80000ULL;
	r = (discs_opponent + 0X0000000000040000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000FC0000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X4020100800000000ULL;
	opp_discs = (discs_opponent & 0X0020100804000000ULL) | 0X3FDFEFF7F8000000ULL;
	r = (opp_discs + 0X0000000004000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020100804000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0202020200000000ULL;
	opp_discs = (discs_opponent & 0X0002020202000000ULL) | 0X01FDFDFDFC000000ULL;
	r = (opp_discs + 0X0000000002000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002020202000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000020000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H6(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction SW */
	unsigned long long flipped = (discs_opponent & 0X0000000000000200ULL) & (discs_player << 7);
	
	/* direction S */
	flipped |= (discs_opponent & 0X0000000000000100ULL) & (discs_player << 8);
	
	/* direction _W */
	my_discs = discs_player & 0X0000000000FC0000ULL;
	r = (discs_opponent + 0X0000000000020000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000FE0000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X2010080400000000ULL;
	opp_discs = (discs_opponent & 0X0010080402000000ULL) | 0X1FEFF7FBFC000000ULL;
	r = (opp_discs + 0X0000000002000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010080402000000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0101010100000000ULL;
	opp_discs = (discs_opponent & 0X0001010101000000ULL) | 0X00FEFEFEFE000000ULL;
	r = (opp_discs + 0X0000000001000000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0001010101000000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000010000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NE */
	my_discs = discs_player & 0X0204081020000000ULL;
	opp_discs = (discs_opponent & 0X0004081020400000ULL) | 0X01FBF7EFDF800000ULL;
	r = (opp_discs + 0X0000000000400000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004081020400000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X8080808080000000ULL;
	opp_discs = (discs_opponent & 0X0080808080800000ULL) | 0X7F7F7F7F7F000000ULL;
	r = (opp_discs + 0X0000000000800000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0080808080800000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000003F00ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000008000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000008000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NE */
	my_discs = discs_player & 0X0102040810000000ULL;
	opp_discs = (discs_opponent & 0X0002040810200000ULL) | 0X00FDFBF7EFC00000ULL;
	r = (opp_discs + 0X0000000000200000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002040810200000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X4040404040000000ULL;
	opp_discs = (discs_opponent & 0X0040404040400000ULL) | 0X3FBFBFBFBF800000ULL;
	r = (opp_discs + 0X0000000000400000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040404040400000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000001F00ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000004000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000004000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NW */
	unsigned long long flipped = (discs_opponent & 0X0000000000400000ULL) & (discs_player >> 9);
	
	/* direction _W */
	flipped |= (discs_opponent & 0X0000000000004000ULL) & (discs_player >> 1);
	
	/* direction N_ */
	my_discs = discs_player & 0X2020202020000000ULL;
	opp_discs = (discs_opponent & 0X0020202020200000ULL) | 0X1FDFDFDFDFC00000ULL;
	r = (opp_discs + 0X0000000000200000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020202020200000ULL;
	}

	/* direction NE */
	my_discs = discs_player & 0X0001020408000000ULL;
	opp_discs = (discs_opponent & 0X0000020408100000ULL) | 0X0000FDFBF7E00000ULL;
	r = (opp_discs + 0X0000000000100000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000020408100000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000000F00ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000002000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000002000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_D7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction N_ */
	my_discs = discs_player & 0X1010101010000000ULL;
	opp_discs = (discs_opponent & 0X0010101010100000ULL) | 0X0FEFEFEFEFE00000ULL;
	r = (opp_discs + 0X0000000000100000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010101010100000ULL;
	}

	/* direction NE */
	my_discs = discs_player & 0X0000010204000000ULL;
	opp_discs = (discs_opponent & 0X0000000204080000ULL) | 0X000000FDFBF00000ULL;
	r = (opp_discs + 0X0000000000080000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000204080000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0000008040000000ULL;
	opp_discs = (discs_opponent & 0X0000000040200000ULL) | 0X0000007FBFC00000ULL;
	r = (opp_discs + 0X0000000000200000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000040200000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000000700ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000001000ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0X000000000000C000ULL;
	r = (discs_opponent + 0X0000000000002000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000000000E000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000001000ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_E7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction N_ */
	my_discs = discs_player & 0X0808080808000000ULL;
	opp_discs = (discs_opponent & 0X0008080808080000ULL) | 0X07F7F7F7F7F00000ULL;
	r = (opp_discs + 0X0000000000080000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008080808080000ULL;
	}

	/* direction NE */
	my_discs = discs_player & 0X0000000102000000ULL;
	opp_discs = (discs_opponent & 0X0000000002040000ULL) | 0X00000000FDF80000ULL;
	r = (opp_discs + 0X0000000000040000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000002040000ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0000804020000000ULL;
	opp_discs = (discs_opponent & 0X0000004020100000ULL) | 0X00007FBFDFE00000ULL;
	r = (opp_discs + 0X0000000000100000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000004020100000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000000300ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000000800ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0X000000000000E000ULL;
	r = (discs_opponent + 0X0000000000001000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000000000F000ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000800ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction _E */
	unsigned long long flipped = (discs_opponent & 0X0000000000000200ULL) & (discs_player << 1);
	
	/* direction N_ */
	my_discs = discs_player & 0X0404040404000000ULL;
	opp_discs = (discs_opponent & 0X0004040404040000ULL) | 0X03FBFBFBFBF80000ULL;
	r = (opp_discs + 0X0000000000040000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004040404040000ULL;
	}

	/* direction NE */
	flipped |= (discs_opponent & 0X0000000000020000ULL) & (discs_player >> 7);

	/* direction NW */
	my_discs = discs_player & 0X0080402010000000ULL;
	opp_discs = (discs_opponent & 0X0000402010080000ULL) | 0X007FBFDFEFF00000ULL;
	r = (opp_discs + 0X0000000000080000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000402010080000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X000000000000F000ULL;
	r = (discs_opponent + 0X0000000000000800ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000000000F800ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000400ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NW */
	my_discs = discs_player & 0X8040201008000000ULL;
	opp_discs = (discs_opponent & 0X0040201008040000ULL) | 0X7FBFDFEFF7F80000ULL;
	r = (opp_discs + 0X0000000000040000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040201008040000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0202020202000000ULL;
	opp_discs = (discs_opponent & 0X0002020202020000ULL) | 0X01FDFDFDFDFC0000ULL;
	r = (opp_discs + 0X0000000000020000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002020202020000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X000000000000F800ULL;
	r = (discs_opponent + 0X0000000000000400ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000000000FC00ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000200ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H7(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NW */
	my_discs = discs_player & 0X4020100804000000ULL;
	opp_discs = (discs_opponent & 0X0020100804020000ULL) | 0X3FDFEFF7FBFC0000ULL;
	r = (opp_discs + 0X0000000000020000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020100804020000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0101010101000000ULL;
	opp_discs = (discs_opponent & 0X0001010101010000ULL) | 0X00FEFEFEFEFE0000ULL;
	r = (opp_discs + 0X0000000000010000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0001010101010000ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X000000000000FC00ULL;
	r = (discs_opponent + 0X0000000000000200ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X000000000000FE00ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000100ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_A8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NE */
	my_discs = discs_player & 0X0102040810200000ULL;
	opp_discs = (discs_opponent & 0X0002040810204000ULL) | 0X00FDFBF7EFDF8000ULL;
	r = (opp_discs + 0X0000000000004000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002040810204000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X8080808080800000ULL;
	opp_discs = (discs_opponent & 0X0080808080808000ULL) | 0X7F7F7F7F7F7F0000ULL;
	r = (opp_discs + 0X0000000000008000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0080808080808000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X000000000000003FULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000000080ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000080ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_B8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NE */
	my_discs = discs_player & 0X0001020408100000ULL;
	opp_discs = (discs_opponent & 0X0000020408102000ULL) | 0X0000FDFBF7EFC000ULL;
	r = (opp_discs + 0X0000000000002000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000020408102000ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X4040404040400000ULL;
	opp_discs = (discs_opponent & 0X0040404040404000ULL) | 0X3FBFBFBFBFBF8000ULL;
	r = (opp_discs + 0X0000000000004000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040404040404000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X000000000000001FULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000000040ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000040ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_C8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NW */
	unsigned long long flipped = (discs_opponent & 0X0000000000004000ULL) & (discs_player >> 9);
	
	/* direction _W */
	flipped |= (discs_opponent & 0X0000000000000040ULL) & (discs_player >> 1);
	
	/* direction N_ */
	my_discs = discs_player & 0X2020202020200000ULL;
	opp_discs = (discs_opponent & 0X0020202020202000ULL) | 0X1FDFDFDFDFDFC000ULL;
	r = (opp_discs + 0X0000000000002000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0020202020202000ULL;
	}

	/* direction NE */
	my_discs = discs_player & 0X0000010204080000ULL;
	opp_discs = (discs_opponent & 0X0000000204081000ULL) | 0X000000FDFBF7E000ULL;
	r = (opp_discs + 0X0000000000001000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000204081000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X000000000000000FULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000000020ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000020ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_D8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction N_ */
	my_discs = discs_player & 0X1010101010100000ULL;
	opp_discs = (discs_opponent & 0X0010101010101000ULL) | 0X0FEFEFEFEFEFE000ULL;
	r = (opp_discs + 0X0000000000001000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0010101010101000ULL;
	}

	/* direction NE */
	my_discs = discs_player & 0X0000000102040000ULL;
	opp_discs = (discs_opponent & 0X0000000002040800ULL) | 0X00000000FDFBF000ULL;
	r = (opp_discs + 0X0000000000000800ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000002040800ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0000000080400000ULL;
	opp_discs = (discs_opponent & 0X0000000000402000ULL) | 0X000000007FBFC000ULL;
	r = (opp_discs + 0X0000000000002000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000402000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000000007ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000000010ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0X00000000000000C0ULL;
	r = (discs_opponent + 0X0000000000000020ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000000000E0ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000010ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_E8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction N_ */
	my_discs = discs_player & 0X0808080808080000ULL;
	opp_discs = (discs_opponent & 0X0008080808080800ULL) | 0X07F7F7F7F7F7F000ULL;
	r = (opp_discs + 0X0000000000000800ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0008080808080800ULL;
	}

	/* direction NE */
	my_discs = discs_player & 0X0000000001020000ULL;
	opp_discs = (discs_opponent & 0X0000000000020400ULL) | 0X0000000000FDF800ULL;
	r = (opp_discs + 0X0000000000000400ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000000020400ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0000008040200000ULL;
	opp_discs = (discs_opponent & 0X0000000040201000ULL) | 0X0000007FBFDFE000ULL;
	r = (opp_discs + 0X0000000000001000ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000000040201000ULL;
	}

	/* direction _E */
	my_discs = discs_player & 0X0000000000000003ULL;
	r = ((my_discs<<1) + discs_opponent) & 0X0000000000000008ULL;
	if(r != 0) {
		flipped |= r - (0X2ULL<<_bsrll(my_discs));
	}

	/* direction _W */
	my_discs = discs_player & 0X00000000000000E0ULL;
	r = (discs_opponent + 0X0000000000000010ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000000000F0ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000008ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_F8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r;
	
	/* direction NE */
	unsigned long long flipped = (discs_opponent & 0X0000000000000200ULL) & (discs_player >> 7);
	
	/* direction _E */
	flipped |= (discs_opponent & 0X0000000000000002ULL) & (discs_player << 1);
	
	/* direction N_ */
	my_discs = discs_player & 0X0404040404040000ULL;
	opp_discs = (discs_opponent & 0X0004040404040400ULL) | 0X03FBFBFBFBFBF800ULL;
	r = (opp_discs + 0X0000000000000400ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0004040404040400ULL;
	}

	/* direction NW */
	my_discs = discs_player & 0X0000804020100000ULL;
	opp_discs = (discs_opponent & 0X0000004020100800ULL) | 0X00007FBFDFEFF000ULL;
	r = (opp_discs + 0X0000000000000800ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000004020100800ULL;
	}
	

	/* direction _W */
	my_discs = discs_player & 0X00000000000000F0ULL;
	r = (discs_opponent + 0X0000000000000008ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000000000F8ULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000004ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_G8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NW */
	my_discs = discs_player & 0X0080402010080000ULL;
	opp_discs = (discs_opponent & 0X0000402010080400ULL) | 0X007FBFDFEFF7F800ULL;
	r = (opp_discs + 0X0000000000000400ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0000402010080400ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0202020202020000ULL;
	opp_discs = (discs_opponent & 0X0002020202020200ULL) | 0X01FDFDFDFDFDFC00ULL;
	r = (opp_discs + 0X0000000000000200ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0002020202020200ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X00000000000000F8ULL;
	r = (discs_opponent + 0X0000000000000004ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000000000FCULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000002ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}

bool RXBitBoard::do_flips_H8(unsigned long long& discs_player, unsigned long long& discs_opponent) {

	unsigned long long my_discs, opp_discs, r, flipped = 0ULL;
	
	/* direction NW */
	my_discs = discs_player & 0X8040201008040000ULL;
	opp_discs = (discs_opponent & 0X0040201008040200ULL) | 0X7FBFDFEFF7FBFC00ULL;
	r = (opp_discs + 0X0000000000000200ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0040201008040200ULL;
	}

	/* direction N_ */
	my_discs = discs_player & 0X0101010101010000ULL;
	opp_discs = (discs_opponent & 0X0001010101010100ULL) | 0X00FEFEFEFEFEFE00ULL;
	r = (opp_discs + 0X0000000000000100ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X0001010101010100ULL;
	}

	/* direction _W */
	my_discs = discs_player & 0X00000000000000FCULL;
	r = (discs_opponent + 0X0000000000000002ULL) & my_discs;
	if(r != 0) {
		flipped |= (r-1) & 0X00000000000000FEULL;
	}

	if(flipped) {
		discs_player |= (flipped | 0X0000000000000001ULL);
		discs_opponent ^= flipped;
		return true;
	}
	
	return false;

}



RXBitBoard::type_do_flips const RXBitBoard::do_flips[] = {
RXBitBoard::do_flips_H8, RXBitBoard::do_flips_G8, RXBitBoard::do_flips_F8, RXBitBoard::do_flips_E8, RXBitBoard::do_flips_D8, RXBitBoard::do_flips_C8, RXBitBoard::do_flips_B8, RXBitBoard::do_flips_A8,
RXBitBoard::do_flips_H7, RXBitBoard::do_flips_G7, RXBitBoard::do_flips_F7, RXBitBoard::do_flips_E7, RXBitBoard::do_flips_D7, RXBitBoard::do_flips_C7, RXBitBoard::do_flips_B7, RXBitBoard::do_flips_A7,
RXBitBoard::do_flips_H6, RXBitBoard::do_flips_G6, RXBitBoard::do_flips_F6, RXBitBoard::do_flips_E6, RXBitBoard::do_flips_D6, RXBitBoard::do_flips_C6, RXBitBoard::do_flips_B6, RXBitBoard::do_flips_A6,
RXBitBoard::do_flips_H5, RXBitBoard::do_flips_G5, RXBitBoard::do_flips_F5,                    NULL,                    NULL, RXBitBoard::do_flips_C5, RXBitBoard::do_flips_B5, RXBitBoard::do_flips_A5,
RXBitBoard::do_flips_H4, RXBitBoard::do_flips_G4, RXBitBoard::do_flips_F4,                    NULL,                    NULL, RXBitBoard::do_flips_C4, RXBitBoard::do_flips_B4, RXBitBoard::do_flips_A4,
RXBitBoard::do_flips_H3, RXBitBoard::do_flips_G3, RXBitBoard::do_flips_F3, RXBitBoard::do_flips_E3, RXBitBoard::do_flips_D3, RXBitBoard::do_flips_C3, RXBitBoard::do_flips_B3, RXBitBoard::do_flips_A3,
RXBitBoard::do_flips_H2, RXBitBoard::do_flips_G2, RXBitBoard::do_flips_F2, RXBitBoard::do_flips_E2, RXBitBoard::do_flips_D2, RXBitBoard::do_flips_C2, RXBitBoard::do_flips_B2, RXBitBoard::do_flips_A2,
RXBitBoard::do_flips_H1, RXBitBoard::do_flips_G1, RXBitBoard::do_flips_F1, RXBitBoard::do_flips_E1, RXBitBoard::do_flips_D1, RXBitBoard::do_flips_C1, RXBitBoard::do_flips_B1, RXBitBoard::do_flips_A1
};

#endif
