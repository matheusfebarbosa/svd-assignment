all: tp
	
tp: dataset.o svd.o main.o linalg.o
	g++ dataset.o svd.o main.o linalg.o -o tp.out -lm -Wall -Wextra -std=c++1z -pedantic

main.o: main.cpp
	g++ -c main.cpp -lm -Wall -Wextra -std=c++1z -pedantic

dataset.o: dataset.cpp dataset.hpp
	g++ -c dataset.cpp -lm -Wall -Wextra -std=c++1z -pedantic

svd.o: svd.cpp svd.hpp
	g++ -c svd.cpp -lm -Wall -Wextra -std=c++1z -pedantic

submission.o: submission.cpp
	g++ -c submission.cpp -lm -Wall -Wextra -std=c++1z -pedantic

linalg.o: linalg.cpp linalg.hpp
	g++ -c linalg.cpp -lm -Wall -Wextra -std=c++1z -pedantic

clean:
	$(RM) *.o tp.out