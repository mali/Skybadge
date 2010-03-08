/*
 - badge.h
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

#include "serial.h"

#define DEFAULT_PORT  "/dev/ttyS0"
#define DEFAULT_SPEED	3

#define STX 0x02
#define ETX 0x03

typedef struct badgecfg
{
  char * port;
  char * message;
  int speed;
}BadgeCfg;

int SendMessage(BadgeCfg *);

