CC = g++
NAME = strawberry
INCLUDE = -I/opt/homebrew/Cellar/sfml/2.5.1_2/include
LIBS = -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-system -lsfml-window -lsfml-graphics
CXXFLAGS = -o $(NAME) -std=c++20 $(INCLUDE) $(LIBS) -Wall -w

make:
	$(CC) *.cpp $(CXXFLAGS)
