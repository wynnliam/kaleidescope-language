CMP=g++
EXE=./bin/kaleid.out
FLG=-g -Wall -o

# Source files
SRC_MAIN=./src/main.cpp

SRC_ALL=$(SRC_MAIN)

.phony: clean

all: $(SRC_ALL)
	$(CMP) $(FLG) $(EXE) $(SRC_ALL)

clean:
	rm ./bin/*.out
