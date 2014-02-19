/*
   
   Copyright (C) 2014 Florian Menne (florianmenne@t-online.de)

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

/*
  Compile this with: gcc -lwiringPi -o <name> hx2262.c rpi_hx2262app.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>

#include <wiringPi.h>

#include "hx2262.h"

#define RPI_PIN 17

uint8_t export_pin(uint8_t pin, bool export)
{
  FILE *fd;
  char path[50];
  
  if(export)
  {
    //open gpio export
    if( (fd = fopen("/sys/class/gpio/export", "r+") ) == NULL )
    {
      printf("Unable to open File: /sys/class/gpio/export\n");
      return 1;
    }
    
    //write pin to export and close
    fprintf(fd, "%d\n", pin);
    fclose(fd);
    
    //generate path depending on pin number
    sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
    
    //wait until the path is available
    while( (fd = fopen(path, "r+") ) == NULL );
    
    //set to out
    fprintf(fd, "out");
    fclose(fd);
  }
  
  else
  {
    //just the same as above with unexport
    if( (fd = fopen("/sys/class/gpio/unexport", "r+") ) == NULL )
    {
      printf("Unable to open File: /sys/class/gpio/unexport\n");
      return 1;
    }
    
    fprintf(fd, "%d\n", pin);
    fclose(fd);
  }
  
  return 0;
}

void clk(uint8_t i)
{
  //one clock periode takes 400us
  usleep(400*i);
}

void enableout(void)
{
  digitalWrite(RPI_PIN,1);
}

void disableout(void)
{
  digitalWrite(RPI_PIN,0);
}

int main(int argc, char **argv)
{
  uint8_t i;
  
  //check input for errors
  if(argc < 4)
  {
    printf("Too few arguments!\n");
    return -1;
  }
  
  if(argc > 4)
  {
    printf("Too many arguments\n");
    return -1;
  }
  
  for(i=0; i<10; i++)
  {
    if( (argv[1][i] != '0') && (argv[1][i] != '1') )
    {
      printf("%c\n", argv[1][i]);
      printf("Illegal character in code!\n");
      return -1;
    }
  }
  
  //export
  export_pin(RPI_PIN, true);
  
  wiringPiSetupSys();
  
  //init hx2262 lib
  hx2262_init(&enableout, &disableout, &clk);
  
  //just send
  hx2262_elroab440_send(argv[1], atoi(argv[2]), atoi(argv[3]));
  
  //unexport
  export_pin(RPI_PIN, false);
  
  return 0;
}