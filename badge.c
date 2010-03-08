/*
 - badge.c
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
#include <string.h> 
#include <stdio.h> 
#include <ctype.h> 
#include "badge.h"

void strupr(char *s)
{
  int i=strlen(s), j=0;
  char * tmp1=s, * tmp2=s;
  while(j++ < i)
    *(tmp2++) = toupper(*(s++));
  s=tmp1;
  return;
}

int SendMessage(BadgeCfg * pbcfg)
{
  char trame[150];
  memset (trame, 0, sizeof(trame));

  //Only Upper Case message
  strupr(pbcfg->message);

  sprintf(trame,"%c%i%s%c", STX, pbcfg->speed, pbcfg->message, ETX);

  if(OpenSerial(pbcfg->port) == 0)
  {
    SetRTSDTR();
    WriteSerial(trame);
//    printf("\n--Appuyez sur Entrée lorsque votre message s'affiche sur le badge --\n");
//    getchar(); 
    UnsetRTSDTR();  
    CloseSerial();
  }
  return(0);
}

