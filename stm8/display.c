/* Copyright (C) 2015 Baruch Even
 *
 * This file is part of the B3603 alternative firmware.
 *
 *  B3603 alternative firmware is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  B3603 alternative firmware is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with B3603 alternative firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "display.h"
#include "stm8s.h"

#include <string.h>

// repeated integer calculations to char 
#define D4	10000
#define D3	1000
#define D2	100
#define D1	10



uint8_t display_idx;
uint8_t display_data[4];
uint8_t pending_display_data[4];
uint8_t pending_update;
uint16_t timer;

// a, b ,c ,d ,e ,f ,g, DP

// 0  0  1  1  1  0  1 	= o  0x1D
// 0  0  1  1  1  0  0  = u  0x1C
// 0  0	 0  1  1  1  1	= t  0x0F
// 0  0  1  0  0  0  0  = i  0x10
// 0  0  1  0  1  0  1  = n  0x25
// 0  0  0  0  1  1  0  = I  0x06
// 1  0  1  1  0  1  1  = S  0x5B
// 1  0  0  1  1  1  1  = E  0x4F
// 1  1  1  0  1  1  0 =  N	0x76
// 0  0  1  1  1  1  0 = W left part of W 	0x1E
// 0  1  1  1  1  0  0 = W right part of W    	0x3C
// 1  1  0  0  1  1  1 = P 0x67
// 1  1  1  1  1  1  0 = O 0x7E
              

//	  A	
//	F   B
//	  G
//	E   C
//	  D   DP
//

static const uint8_t display_number[10] = {
	0xFC, // '0'
	0x60, // '1'
	0xDA, // '2'
	0xF2, // '3'
	0x66, // '4'
	0xB6, // '5'
	0xBE, // '6'
	0xE0, // '7'
	0xFE, // '8'
	0xF6, // '9'
};

// http://luishernandezengineeringportfoli.weebly.com/electronics-final.html
// https://en.wikipedia.org/wiki/Seven-segment_display
// http://margaretshepherd.blogspot.nl/2013/05/114-7-segment-display.html
// http://www.josepino.com/microcontroller/7-segment-ascii
// https://github.com/dmadison/LED-Segment-ASCII
// http://easternstargeek.blogspot.nl/2011/09/ascii-to-seven-segment-table.html
/*
static const uint8_t display_char_lowercase[] = {
(uint8_t)(0x77<<1), / * A * /
(uint8_t)(0x1F<<1), / * B * /
(uint8_t)(0x6E<<1), / * C * /
(uint8_t)(0x3D<<1), / * D * /
(uint8_t)(0x4F<<1), / * E * /
(uint8_t)(0x47<<1), / * F * /
(uint8_t)(0x7B<<1), / * G * /
(uint8_t)(0x37<<1), / * H * /
	(uint8_t)(0x30<<1), / * I * /
	(uint8_t)(0x1E<<1), / * J * /
	(uint8_t)(0x75<<1), / * K * /
	(uint8_t)(0x38<<1), / * L * /
	(uint8_t)(0x15<<1), / * M * /
	(uint8_t)(0x37<<1), / * N * /
	(uint8_t)(0x3F<<1), / * O * /
	(uint8_t)(0x73<<1), / * P * /
	(uint8_t)(0x6B<<1), / * Q * /
	(uint8_t)(0x33<<1), / * R * /
	(uint8_t)(0x6D<<1), / * S * /
	(uint8_t)(0x78<<1), / * T * /
	(uint8_t)(0x3E<<1), / * U * /
	(uint8_t)(0x3E<<1), / * V * /
	(uint8_t)(0x2A<<1), / * W * /
	(uint8_t)(0x76<<1), / * X * /
	(uint8_t)(0x6E<<1), / * Y * /
	(uint8_t)(0x5B<<1)  / * Z * /	
	
};
*/
#define SET_DATA(bit) do { if (bit) { PD_ODR |= (1<<4); } else { PD_ODR &= ~(1<<4); }} while (0)
#define PULSE_CLOCK() do { PA_ODR |= (1<<1); PA_ODR &= ~(1<<1); } while (0)
#define SAVE_DATA() do { PA_ODR &= ~(1<<2); PA_ODR |= (1<<2); } while (0)




uint8_t uint16_to_digit(uint16_t value, uint16_t devider)
{
// return the value of the digit, no longer the char
return (value / devider) % 10;
}

// Displays the cfg_output_t uint16_t values
void display_show_uint16(uint16_t value)
{
uint8_t ch1;
uint8_t ch2;
uint8_t ch3;
uint8_t ch4;

    ch1 = uint16_to_digit(value, D4);

    ch2 = uint16_to_digit(value, D3);

    ch3 = uint16_to_digit(value, D2);
                
    ch4 = uint16_to_digit(value, D1);

    display_show_decimal_digits(ch1, 0, ch2, 1, ch3, 0, ch4, 0);
}






inline void display_word(uint16_t word)
{
	uint8_t i;

	for (i = 0; i < 16; i++) {
		uint8_t bit = word & 1;
		word >>= 1;
		SET_DATA(bit);
		PULSE_CLOCK();
	}
	SAVE_DATA();
}

void display_refresh(void)
{
	uint8_t i = display_idx++;
	uint8_t bit = 8+(i*2);
	uint16_t digit = 0xFF00 ^ (3<<bit);
	
	//uint16_t digit=(~(0x03<<(2*i)))<<8;
	//uint8_t digit[4]={0xFC,0xF3,0xCF,0x3F};

	if (timer > 0)
		timer--;
	
	if (pending_update && timer == 0) {
		memcpy(display_data, pending_display_data, sizeof(display_data));
		pending_update = 0;
		timer = 1500; // 1/2 of a second, approximately
	}
	

	display_word(digit | display_data[i]);

	if (display_idx == 4)
		display_idx = 0;
}

uint8_t display_char(uint8_t ch, uint8_t dot)
{
	if (dot)
		dot = 1;
	// display numbers
	if (ch >= 0 && ch <= 9)
		return display_number[ch] | dot;
	//display lowercase
/*	if (ch >= 'a' && ch <= 'z')
		return display_char_lowercase[ch-'a'] | dot;
*/	
	return dot;
}

void display_show_decimal_digits(uint8_t ch1, uint8_t dot1, uint8_t ch2, uint8_t dot2, uint8_t ch3, uint8_t dot3, uint8_t ch4, uint8_t dot4)
{
	pending_display_data[3] = display_char(ch1, dot1);
	pending_display_data[2] = display_char(ch2, dot2);
	pending_display_data[1] = display_char(ch3, dot3);
	pending_display_data[0] = display_char(ch4, dot4);
	pending_update = 1;
}

void display_show_raw_digits(uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4 )
{
	pending_display_data[3] = ch1;
	pending_display_data[2] = ch2;
	pending_display_data[1] = ch3;
	pending_display_data[0] = ch4;
	pending_update = 1;
}

