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

#include "elroab440.h"
#include "hx2262.h"

#include <inttypes.h>
#include <stdbool.h>

uint8_t elroab440_send(char *code, uint8_t repeat, bool state)
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