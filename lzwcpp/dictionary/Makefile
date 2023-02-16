
CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++2a -O3 
EXEC= test_std_dict test_direct_map

all: test_direct_map

test_std_dict: test_std_dict.cc std_dict.hh dictionary.hh
	$(CXX) $(CXXFLAGS) -o $@ $^

test_direct_map: test_direct_map.cc direct_mapped_dict.hh dictionary.hh
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
