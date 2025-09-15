CXX = g++
CXXFLAGS = -std=c++14 -Wextra -O2

SOURCES = $(wildcard ch*.cpp item*.cpp)
BINS = $(SOURCES:.cpp=)

all: $(BINS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(BINS)
