CC = gcc
CFLAGS = -g
LDFLAGS = -lpthread -lrt
IFOLDER = -I ./include
LFOLDER = -L ./lib
OUTFILE = arkanopi
OBJS = arkanoPi.o commonLib.o arkanoPiLib.o pongLib.o fsm.o kbhit.o ledDisplay.o pseudoWiringPi.o teclado_TL04.o tmr.o
SRCS = arkanoPi.c commonLib.c arkanoPiLib.c pongLib.c fsm.c kbhit.c ledDisplay.c pseudoWiringPi.c teclado_TL04.c tmr.c

$(OUTFILE): $(OBJS)
	$(CC) $(CFLAGS) $(IFOLDER) $(LFOLDER) -o $(OUTFILE) $(OBJS) $(LDFLAGS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(IFOLDER) $(LFOLDER) -c $(SRCS) $(LDFLAGS)


clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY : all
all : $(OUTFILE)
