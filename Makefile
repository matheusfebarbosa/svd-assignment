tp: dataset.o svd.o main.o
	g++ dataset.o svd.o main.o -o tp -Wall -Wextra -std=c++1z -pedantic

main.o: main.cpp
	g++ -c main.cpp -Wall -Wextra -std=c++1z -pedantic

dataset.o: dataset.cpp dataset.hpp
	g++ -c dataset.cpp -Wall -Wextra -std=c++1z -pedantic

svd.o: svd.cpp
	g++ -c svd.cpp -Wall -Wextra -std=c++1z -pedantic

submission.o: submission.cpp
	g++ -c submission.cpp -Wall -Wextra -std=c++1z -pedantic

clean:
	$(RM) *.o tp