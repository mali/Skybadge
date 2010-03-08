/*
 - skybadge.c
 - by Laurent Navet 'Mali' <laurent dot navet (at) gmail dot com>

This software is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; version 2 dated June, 1991.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "badge.h"

#define OPT_LIST "p:v:m:"

BadgeCfg  Badge;

int TestOptions(int argc, char **argv);


int main (int argc, char **argv)
{
  int exit_code=0;
  if(!TestOptions(argc, argv))
  {
      printf("\n-- Port : %s\n-- Vitesse : %i\n-- Message : %s\n", Badge.port, Badge.speed, Badge.message);

      if(Badge.message != NULL)
	{
	 if(strlen(Badge.message) >= 120)
	  {
	    Badge.message[120]='\0';
	  }
	 SendMessage(&Badge);	 
	}
      else
	{
	 printf("\nNo message to send...\n");
	 exit_code=-1;
	}
  }
  else
  {
    printf("\nError while parsing options...\n");
    exit_code=-1;
  }
  if(exit_code == -1)
    {
      printf("Options :\n");
      printf(" -p port (/dev/ttyS0)\n");
      printf(" -v vitesse (0-4)\n");    
      printf(" -m message\n");    
    }
  return 0;
}



int TestOptions(int argc, char **argv)
{
  int opt = 0;
  int ex_code = 0;

  Badge.port = DEFAULT_PORT;
  Badge.speed = DEFAULT_SPEED;

  while( (opt=getopt(argc, argv, OPT_LIST)) != -1)
  {
    switch(opt)
    {
      case 'p':
      {
	Badge.port = optarg;
	break;				
      }
      case 'v':
      {
	Badge.speed=atoi(optarg);
	break;				
      }
      case 'm':
      {
	Badge.message=optarg;
	break;				
      }
      default:
      {
	ex_code = 1;
	break;
      }	
    }
  }
  return(ex_code);
}
