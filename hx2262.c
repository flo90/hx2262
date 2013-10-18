#include "hx2262.h"
#include <stdbool.h>
#include <inttypes.h>

bool txstate = false;

uint8_t send_element(uint8_t code);

static inline void clk_periode(uint8_t i)
{
  //implement a i times clock periode here
}

static inline void enabletx(void)
{
  //enable transmitter here
}

static inline void disabletx(void)
{
  //disable transmitter here
}

static inline void toggletx(void)
{
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
  uint8_t i,j;
  
  j=0;
  
  //this array holds the generated signal
  uint8_t signal[50];
  
  //firstly generate the signal
  //the numbers are the periodes length
  for(i=0; i<12; i++)
  {
    switch(code[i])
    {
      //0 is represented by 4 periodes on 12 periodes off and so on...
      case '0':
	signal[j++]=4;
	signal[j++]=12;
	signal[j++]=4;
	signal[j++]=12;
	break;
	
      //1 is represented by 12 periodes on 4 periodes off and so on...
      case '1':
	signal[j++]=12;
	signal[j++]=4;
	signal[j++]=12;
	signal[j++]=4;
	break;
      
      //f is represented by 4 periodes on 12 off 12 on 4 off
      case 'f':
	signal[j++]=4;
	signal[j++]=12;
	signal[j++]=12;
	signal[j++]=4;
	break;
	
      default:
	return 1;
    }
  }
  
  signal[48] = 4;
  signal[49] = 124;
    
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