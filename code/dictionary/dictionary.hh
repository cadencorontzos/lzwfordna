#include <cassert>
#include <iostream>
#include <string>

/**
 * Class to track runs of characters
 * to be used as template argument for LZWDictionary
 */
template <typename codeword_type> class Next_Run {
public:
  unsigned next_run_length;
  codeword_type codeword_of_next_run;
  Next_Run(unsigned length, codeword_type cw)
      : next_run_length(length), codeword_of_next_run(cw){};
};

/**
 * Parent class
 *
 * A dictionary class to be used for LZW algorithm
 *
 * functions are allowed to be written to take char* or std::strings, depending
 * on what makes sense for each implementation
 *
 * Children should define LZW_Encode_Dictionary and LZW_Decode_Dictionary
 *
 * Templated by codeword type and next_run_type
 */
template <typename codeword_type, typename next_run_type = int>
class LZWDictionary {

public:
  LZWDictionary() = default;
  ~LZWDictionary() = default;

  virtual next_run_type find_longest_in_dict(const char *, int, int) {
    assert(0);
  }

  /**
   * Finds next longest run in input
   * @param input pointer to start of input
   * @param end_of_input pointer to end of input
   */
  virtual next_run_type find_longest_in_dict(const char *, const char *) {
    assert(0);
  }

  /**
   * Adds string to dictionary
   * @param str string to add
   * @param codeword codeword to assign to string
   */
  virtual void add_string(std::string, codeword_type) { assert(0); }

  /**
   * Adds string to dictionary
   * @param input pointer to input stream
   * @param len length to read
   * @param codeword codeword to assign to string
   */
  virtual void add_string(const char *, unsigned, codeword_type) { assert(0); }

  virtual void add_string(const char *, next_run_type, codeword_type) {
    assert(0);
  }

  /**
   * Returns the codeword of the string
   * @param str string to read from
   * @param len length to read
   */
  virtual codeword_type code_of(std::string, unsigned) const { assert(0); }

  /**
   * Returns the codeword of the string
   * @param input pointer to input stream
   * @param len length to read
   */
  virtual codeword_type code_of(const char *, unsigned) const { assert(0); }

  /**
   * Returns the codeword of the string
   * @param input pointer to input stream
   * @param len length to read
   */
  virtual std::string str_of(codeword_type) const { assert(0); }

  /**
   * Loads dictionary with starting vaules
   */
  virtual void load_starting_dictionary() = 0;
};

/**
 * Keeps track of constants associated with codewords
 * makes sure we don't go over our max codeword
 * prevents LZW algorithm from having to track variable length codeword outputs
 */
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
  ///
  /// returns next codeword, checking for overflow and codword growing
  virtual codeword_type get_next_codeword() = 0;
};
