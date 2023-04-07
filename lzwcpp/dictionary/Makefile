CXX=g++
CXXFLAGS= -g -mbmi2 -Wall -Wextra -pedantic -std=c++2a -O3  -Werror
EXEC= test_std_dict test_direct_map test_mult_std_dict

all: $(EXEC)

# executables
test_std_dict: test_std_dict.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_direct_map: test_direct_map.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_mult_std_dict: test_mult_std_dict.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# objects 
test_std_dict.o: test_std_dict.cc std_dict.hh dictionary.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test_direct_map.o: test_direct_map.cc direct_mapped_dict.hh dictionary.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test_mult_std_dict.o: test_mult_std_dict.cc mult_std_dict.hh dictionary.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<


%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
