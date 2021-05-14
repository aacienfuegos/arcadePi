CC = ./../tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
CFLAGS = -g
LDFLAGS = -lpthread -lrt -lwiringPi -lbcm2835
IFOLDER = -I ./include
LFOLDER = -L ./lib
OUTFILE = arkanopi
OBJS = arkanoPi.o commonLib.o arkanoPiLib.o pongLib.o fsm.o kbhit.o ledDisplay.o teclado_TL04.o tmr.o led_text.o
SRCS = arkanoPi.c commonLib.c arkanoPiLib.c pongLib.c fsm.c kbhit.c ledDisplay.c teclado_TL04.c tmr.c led_text.c

$(OUTFILE): $(OBJS)
	$(CC) $(CFLAGS) $(IFOLDER) $(LFOLDER) -o $(OUTFILE) $(OBJS) $(LDFLAGS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(IFOLDER) $(LFOLDER) -c $(SRCS) $(LDFLAGS)


clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY : all
all : $(OUTFILE)
