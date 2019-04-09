all: tp
	
tp: dataset.o svd.o main.o linalg.o
	g++ dataset.o svd.o main.o linalg.o -o tp.out -Wall -Wextra -std=c++1z -pedantic

main.o: main.cpp
	g++ -c main.cpp -Wall -Wextra -std=c++1z -pedantic

dataset.o: dataset.cpp dataset.hpp
	g++ -c dataset.cpp -Wall -Wextra -std=c++1z -pedantic

svd.o: svd.cpp svd.hpp
	g++ -c svd.cpp -Wall -Wextra -std=c++1z -pedantic

submission.o: submission.cpp
	g++ -c submission.cpp -Wall -Wextra -std=c++1z -pedantic

linalg.o: linalg.cpp linalg.hpp
	g++ -c linalg.cpp -Wall -Wextra -std=c++1z -pedantic

clean:
	$(RM) *.o tp.out