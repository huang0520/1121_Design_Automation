CXX = g++
CXXFLAGS = -std=c++17 -O3 -DNDEBUG

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = ../bin/project1

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
