all: a-star random_board

a-star:
	g++ -O2 --std=c++11 -o a_star a-star.cpp state.cpp

random_board: random_board.cpp
	g++ --std=c++11 -o random_board random_board.cpp
