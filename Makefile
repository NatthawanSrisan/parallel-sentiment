CXX = g++
CXXFLAGS = -std=c++11 -pthread

SRC = main.cpp queue.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = test

all: $(TARGET)

$(TARGET): $(OBJ)
 $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
 $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
 rm -f $(OBJ) $(TARGET)