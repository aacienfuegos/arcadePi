CC = arm-linux-gnueabihf-gcc
CFLAGS = -g
LDFLAGS = -lpthread -lrt -lwiringPi -lbcm2835
IFOLDER = -I ./include
LFOLDER = -L ./lib
OUTFILE = arkanopi
OBJS = arkanoPi.o arkanoPiLib.o fsm.o kbhit.o ledDisplay.o teclado_TL04.o tmr.o
SRCS = arkanoPi.c arkanoPiLib.c fsm.c kbhit.c ledDisplay.c teclado_TL04.c tmr.c

$(OUTFILE): $(OBJS)
	$(CC) $(CFLAGS) $(IFOLDER) $(LFOLDER) -o $(OUTFILE) $(OBJS) $(LDFLAGS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(IFOLDER) $(LFOLDER) -c $(SRCS) $(LDFLAGS)


clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY : all
all : $(OUTFILE)
