
CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++2a -O3 -Werror
EXEC= test_std_dict 

all: test_std_dict 

test_std_dict: test_std_dict.cc std_dict.hh dictionary.hh
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
