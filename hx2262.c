/*
   
   Copyright (C) 2013 Florian Menne (florianmenne@t-online.de)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see [http://www.gnu.org/licenses/].
*/

#include "hx2262.h"
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

void (*enabletx) (void) = NULL;
void (*disabletx) (void) = NULL;
void (*clk_periode) (uint8_t i) = NULL;

uint8_t hx2262_init(void (*penabletx) (void), void (*pdisabletx) (void), void (*pclk_periode) (uint8_t i))
{
  if(!penabletx || !pdisabletx || !pclk_periode)
  {
    return 1;
  }
  
  enabletx = penabletx;
  disabletx = pdisabletx;
  clk_periode = pclk_periode;
  
  disabletx();
  
  return 0;
}

static inline void toggletx(void)
{
  static bool txstate = false;
  
  if(txstate)
  {
    disabletx();
    txstate = false;
  }
  
  else
  {
    enabletx();
    txstate = true;
  }
}

uint8_t hx2262_send(char *code, uint8_t repeat)
{
  uint8_t i, j = 0;
  
  if(!enabletx || !disabletx || !clk_periode)
  {
    return 1;
  }
  
  //this array holds the generated signal
  uint8_t signal[50];
  
  //firstly generate the signal
  //the numbers are the periodes length
  for(i=0; i<12; i++)
  {
    switch(code[i])
    {
      //signal periodes devided by 4 to reduce clk_periode call overhead
      //0 is represented by 4 periodes on 12 periodes off and so on...
      case '0':
	signal[j++]=4/4;
	signal[j++]=12/4;
	signal[j++]=4/4;
	signal[j++]=12/4;
	break;
	
      //1 is represented by 12 periodes on 4 periodes off and so on...
      case '1':
	signal[j++]=12/4;
	signal[j++]=4/4;
	signal[j++]=12/4;
	signal[j++]=4/4;
	break;
      
      //f is represented by 4 periodes on 12 off 12 on 4 off
      case 'f':
	signal[j++]=4/4;
	signal[j++]=12/4;
	signal[j++]=12/4;
	signal[j++]=4/4;
	break;
	
      default:
	return 1;
    }
  }
  
  signal[48] = 4/4;
  signal[49] = 124/4;
    
  while(repeat--)
  {
    for(i=0; i<50; i++)
    {
      
      toggletx();
      clk_periode(signal[i]);
    }
  } 
  
  return 0;
}

uint8_t hx2262_elroab440_send(char *code, bool state, uint8_t repeat)
{
  uint8_t i;
  char buf[12];
  
  for(i = 0; i < 10; i++)
  {
    if(code[i] == '0')
    {
      buf[i] = 'f';
    }
    
    else if(code[i] == '1')
    {
      buf[i] = '0';
    }
    
    else
    {
      return 1;
    }
  }
  
  if(state)
  {
    buf[10] = '0';
    buf[11] = '0';
  }
  
  else
  {
    buf[10] = '0';
    buf[11] = 'f';
  }
  
  return hx2262_send(buf, repeat);
}