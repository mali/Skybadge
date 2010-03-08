/*
 - serial.c
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
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"


int InitSerialCfg ();
int RestoreSerialCfg ();

/************************************************/
int OpenSerial (char * port)
{
  if((fd = open(port, O_RDWR|O_NOCTTY)) == -1)	// tester avec O_NDELAY
    {
      perror("open failed");
      return (-1);
    }
  return (InitSerialCfg());
}

/************************************************/
int CloseSerial ()
{
  /* Wait a second */
  sleep(1);

  /* Restauration cfg */
  RestoreSerialCfg();
  if(close(fd) == -1)
    {
      perror("close failed");
      return (-1);
    }
  return (0);
}

/************************************************/
int WriteSerial(char * trame)
{
 /* on vide le buffers kernel (INPUT)*/
  if( tcflush(fd, TCIFLUSH) == -1)
    {
      perror("tcflush failed");
      return(-1);
    }
    
  if( write(fd, trame , strlen(trame)) == -1)
    {
      perror("write failed");
      return (-1);
    }
  return (0);
}


/************************************************/
int SetRTSDTR()
{
  int status=0;

  ioctl(fd, TIOCMGET, &status);
  status |= TIOCM_RTS;
  ioctl(fd, TIOCMSET, &status);  
  status |= TIOCM_DTR;
  ioctl(fd, TIOCMSET, &status);    
  return(0);
}

/************************************************/
int UnsetRTSDTR()
{
  int status=0;

  ioctl(fd, TIOCMGET, &status);
  status &= ~TIOCM_RTS;
  ioctl(fd, TIOCMSET, &status);  
  status &= ~TIOCM_DTR;
  ioctl(fd, TIOCMSET, &status);    
  return(0);  
}

/************************************************/
int InitSerialCfg ()
{
  /* lecture/Sauvegarde config */
  if( tcgetattr(fd, &oldcfg) == -1)
    {
      perror("tcgetattr failed");
      return (-1);
    }

  memcpy (&currentcfg, &oldcfg, sizeof(oldcfg));

  /* le mode raw convient très bien */
  cfmakeraw(&currentcfg);

  /* output/input sped */
  cfsetospeed(&currentcfg, B1200);
  cfsetispeed(&currentcfg, B1200);

  /* data bits = 8 */
  currentcfg.c_cflag |= CS8;

  /* parity = NONE */
  currentcfg.c_cflag &= ~PARENB;
  currentcfg.c_cflag &= ~PARODD;  

  /* stop bits = 1 */
  currentcfg.c_cflag &= ~CSTOPB;

  /* activation cfg */
  if( tcsetattr (fd, TCSANOW, &currentcfg) == -1)
    {
     perror("init tcsetattr failed");
     return (-1);      
    }

  /* on vide les buffers kernel */
  if( tcflush(fd, TCIOFLUSH) == -1)
    {
      perror("tcflush failed");
      return(-1);
    }
  return (0);
}

/************************************************/
int RestoreSerialCfg ()
{
  if( tcsetattr (fd, TCSANOW, &oldcfg) == -1)
    {
      perror("restore tcsetattr failed");
      return (-1);      
    }
  return (0);  
}
