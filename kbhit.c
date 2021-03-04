#include "kbhit.h"
#include <sys/select.h>
#include <stdlib.h> // para poder usar NULL
#include <stdio.h> // para poder usar getc, printf...
//#define DEBUG
#include "dprintf.h" // para poder usar DPRINTF

static char ch2=0;
  
int kbread(void)
{
	char ch=ch2;
	DPRINTF("kbread '%c'\n",ch2);
	ch2=0;
	return ch;
}

int kbhit(void)
{
  struct timeval tv;
  fd_set read_fd;
  if (ch2)
	  {
	  DPRINTF("kbhit buffer '%c'\n",ch2);
	  return ch2;
	  }

  system ("/bin/stty raw");

  /* Do not wait at all, not even a microsecond */
  tv.tv_sec=0;
  tv.tv_usec=0;

  /* Must be done first to initialize read_fd */
  FD_ZERO(&read_fd);

  /* Makes select() ask if input is ready:
   * 0 is the file descriptor for stdin    */
  FD_SET(0,&read_fd);

  /* The first parameter is the number of the
   * largest file descriptor to check + 1. */
  if(select(1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1)
    { 
    system ("/bin/stty cooked");
    return 0;  /* An error occured */ 
    }

  /*  read_fd now holds a bit map of files that are
   * readable. We test the entry for the standard
   * input (file 0). */
  
if(FD_ISSET(0,&read_fd))
    /* Character pending on stdin */
  {
  ch2=getc(stdin);
  system ("/bin/stty cooked");
  DPRINTF("kbhit '%c'\n",ch2);
//  ungetc(ch2,stdin);
  return ch2;
  }
  /* no characters were pending */
  system ("/bin/stty cooked");
  return 0;
  }

