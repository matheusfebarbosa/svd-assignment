CC=g++
EXEC=$(PWD)/recommender
RUNARGS=$(PWD)/dataset/ratings.csv $(PWD)/dataset/targets.csv > submission.csv
TESTARGS=$(PWD)/dataset/ratings.csv 
BUILD=$(PWD)/build
SRC=$(PWD)/src
INCLUDE=$(PWD)/include
CFLAGS=-lm -Wall -Wextra -std=c++1z -pedantic
TARGETS=dataset.o svd.o main.o linalg.o submission.o evaluate.o
INCLUDES=-I/usr/local/include -Ia -Ib -Ic -I$(INCLUDE)

all: config tp

run: all
	$(EXEC) $(RUNARGS)

test: all
	$(EXEC) $(TESTARGS)
	
config:
	mkdir -p $(BUILD)

tp: $(TARGETS)
	$(CC) $(patsubst %,$(BUILD)/%,$(TARGETS)) -o $(EXEC) $(INCLUDES)
main.o: $(SRC)/main.cpp
	$(CC) -c $(SRC)/main.cpp -o  $(BUILD)/main.o $(CFLAGS) $(INCLUDES)
dataset.o: $(SRC)/dataset.*
	$(CC) -c $(SRC)/dataset.cpp -o $(BUILD)/dataset.o $(CFLAGS) $(INCLUDES)
svd.o: $(SRC)/svd.*
	$(CC) -c $(SRC)/svd.cpp -o $(BUILD)/svd.o $(CFLAGS) $(INCLUDES)
submission.o: $(SRC)/submission.*
	$(CC) -c $(SRC)/submission.cpp -o $(BUILD)/submission.o $(CFLAGS) $(INCLUDES)
linalg.o: $(SRC)/linalg.*
	$(CC) -c $(SRC)/linalg.cpp -o $(BUILD)/linalg.o $(CFLAGS) $(INCLUDES)
evaluate.o: $(SRC)/evaluate.*
	$(CC) -c $(SRC)/evaluate.cpp -o $(BUILD)/evaluate.o $(CFLAGS) $(INCLUDES)
clean:
	$(RM) -r $(BUILD) $(EXEC)


.PHONY: config clean run