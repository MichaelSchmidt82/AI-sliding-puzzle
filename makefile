all: a-star random_board

a-star: a-star.cpp
	g++ -O2 --std=c++11 -o a-star a-star.cpp state.cpp

random_board: random_board.cpp
	g++ --std=c++11 -o random_board random_board.cpp
