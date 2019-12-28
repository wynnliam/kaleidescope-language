CMP=g++
EXE=./bin/kaleid.out
FLG=-g -Wall -o

# Source files
SRC= ./src/*.cpp

.phony: clean

all: $(SRC_ALL)
	$(CMP) $(FLG) $(EXE) $(SRC)

clean:
	rm ./bin/*.out
