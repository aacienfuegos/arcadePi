CC = gcc
CFLAGS = -g
LDFLAGS = -lpthread -lrt -lwiringPi
OUTFILE = arcadepi-bin
SRC_DIR = src
OBJ_DIR = obj
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(OUTFILE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)
$(OBJ): obj/%.o : src/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(LDFLAGS) -c $< -o $@

cross:
	make -f Makefile_cross

clean:
	rm -rf $(OBJ_DIR) $(OUTFILE)

.PHONY : all
all : $(OUTFILE)
