#include <cassert>
#include <iostream>
#include <string>

template <typename codeword_type> class Next_Run {
public:
  unsigned next_run_length;
  codeword_type codeword_of_next_run;
  Next_Run(unsigned length, codeword_type cw)
      : next_run_length(length), codeword_of_next_run(cw){};
};

// Parent class
// A dictionary class to be used for LZW algorithm
// functions are allowed to be written to take char* or std::strings, depending
// on what makes sense for each implementation Children should define
// LZW_Encode_Dictionary and LZW_Decode_Dictionary
template <typename codeword_type, typename next_run_type = int>
class LZWDictionary {

public:
  LZWDictionary() = default;
  ~LZWDictionary() = default;

  // takes the input stream, pointer into stream, and input_size
  // returns the longest string that is present in the dicitonary, starting from
  // the pointer. Subject to size constraints
  virtual next_run_type find_longest_in_dict(const char *, int, int) {
    assert(0);
  }
  virtual next_run_type find_longest_in_dict(const char *, const char *) {
    assert(0);
  }

  // add a string codeword pair to the dictionary
  virtual void add_string(std::string, codeword_type) { assert(0); }
  virtual void add_string(const char *, unsigned, codeword_type) { assert(0); }
  virtual void add_string(const char *, next_run_type, codeword_type) {
    assert(0);
  }

  // fetch the codeword associated with a string of chars
  virtual codeword_type code_of(std::string, unsigned) const { assert(0); }
  virtual codeword_type code_of(const char *, unsigned) const { assert(0); }

  // fetch string associated with codeword
  virtual std::string str_of(codeword_type) const { assert(0); }

  // initilize the dictionary with starter values
  virtual void load_starting_dictionary() = 0;
};

// Keeps track of constants associated with codewords
// makes sure we don't go over our max codeword
// prevents LZW algorithm from having to track variable length codeword outputs
template <typename codeword_type> class CW_Tracker {
public:
  codeword_type current_codeword;
  const int EOF_CODEWORD;
  int bits_per_codeword;
  CW_Tracker(unsigned starting_codeword, unsigned eof,
             unsigned starting_codeword_size)
      : current_codeword(starting_codeword), EOF_CODEWORD(eof),
        bits_per_codeword(starting_codeword_size){};
  ~CW_Tracker() = default;
  virtual codeword_type get_next_codeword() = 0;
};
