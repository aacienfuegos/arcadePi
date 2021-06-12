CC = gcc
CFLAGS = -g
LDFLAGS = -lpthread -lrt -lwiringPi
OUTFILE = arcadepi-bin
OBJS = arkanoPi.o controller.o commonLib.o arkanoPiLib.o pongLib.o fsm.o kbhit.o ledDisplay.o ledDisplayAux.o teclado_TL04.o tmr.o
SRCS = arkanoPi.c controller.c commonLib.c arkanoPiLib.c pongLib.c fsm.c kbhit.c ledDisplay.c ledDisplayAux.c teclado_TL04.c tmr.c

$(OUTFILE): $(OBJS)
	$(CC) -o $(OUTFILE) $(OBJS) $(LDFLAGS)
$(OBJS): $(SRCS)
	$(CC) -c $(SRCS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY : all
all : $(OUTFILE)
