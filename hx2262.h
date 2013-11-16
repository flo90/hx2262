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

#ifndef _HX2262_H_
#define _HX2262_H_
#include <inttypes.h>
#include <stdbool.h>

uint8_t hx2262_init(void (*penabletx) (void), void (*pdisabletx) (void), void (*pclk_periode) (uint8_t i));
uint8_t hx2262_send(char *code, uint8_t repeat);
uint8_t hx2262_elroab440_send(char *code, bool state, uint8_t repeat);
#endif
