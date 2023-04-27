#include "dictionary.hh"
#include <climits>
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <x86intrin.h>

/**
 * A collection of classes for LZW using the Direct Map Dictionary
 * Only tested on little endian machines
 * See encode class description for a more through explanation
 */
namespace Direct_Map_Dictionary {

// indexes for starting strings of length 1
const int INDEX_OF_A = 0;
const int INDEX_OF_C = 1;
const int INDEX_OF_T = 2;
const int INDEX_OF_G = 3;

/// Whether or not find_longest uses bs
const bool BINARY_SEARCH = false;
/// The size limit of our dictionaries
constexpr uint8_t MAX_STRING_LENGTH = 15;
/// where to search from when finding longest run
const int FIND_LONGEST_START = 7;
typedef uint16_t codeword_type;
const int CODEWORD_SIZE = sizeof(codeword_type) * CHAR_BIT;
const codeword_type MAX_CODEWORD =
    static_cast<codeword_type>((1 << (sizeof(codeword_type) * CHAR_BIT)) - 1);
using index_type = uint32_t;

/**
 * A class to track next longest run for Direct Map Dictionary
 * tracks the indexes and the codewords of the run
 */
class Next_Longest_Run : public Next_Run<codeword_type> {
public:
  index_type longest_run_index;

  /**
   * @param length length of next run
   * @param cw codeword of next run
   * @param index index of next run (the 2 bit encoding of the run string)
   */
  Next_Longest_Run(unsigned length, codeword_type cw, index_type index)
      : Next_Run<codeword_type>(length, cw), longest_run_index(index){};
  Next_Longest_Run() : Next_Run<codeword_type>(0, 0), longest_run_index(0){};
};

/**
 * Direct Mapped Encode Dictionary:
 *
 * We will have dictionaries for every length 1 to MAX_STRING_LENGTH, for
 * strings of that length
 *
 * We will map the 4 neucleotides into two bits, so any string of length n can
 * be represented by 2n bits That 2n bit encoding is an index into the nth
 * dictionary
 *
 * in the nth dictionary, there is space for every string of length n
 *
 * upon lookup, we look at the space in the nth dictionary corresponding to the
 * 2n bit index given by our lookup string if there is a codeword there, we
 * return it. If the codeword is 0, we know we haven't seen that string before
 *
 * for strings longer than length MAX_STRING_LENGTH, we have a unordered_map on
 * top to take the rest
 *
 * when we run out of codewords, we simply don't allow any more strings to be
 * added
 *
 * May only work on little endian systems due to use of pext
 */
class LZW_Encode_Dictionary
    : private LZWDictionary<codeword_type, Next_Longest_Run> {
private:
  const int INDEX_BITS = CHAR_BIT * sizeof(index_type);
  std::array<int, 1 << CHAR_BIT> values;
  const int NEXT_LONGEST_INDEX_SIZE = 16;
  /// 00000110 00000110 00000110 00000110 00000110 00000110 00000110 00000110
  const uint64_t MASK = 434041037028460038;

  /**
   * Returns the two bit encoding of the input string
   * @param input pointer to input stream
   * @param len length to map from input
   */
  index_type map_str(const char *input, unsigned len) const {
    index_type result = 0;
    for (unsigned i = 0; i < len; i++) {
      result = (result << 2) + values[input[i]];
    }
    return result;
  }

  std::array<std::unique_ptr<codeword_type[]>, MAX_STRING_LENGTH>
      dictionary; /// The indexed array of our codewords
  bool empty;     /// to track when we run out of codewords

public:
  LZW_Encode_Dictionary()
      : LZWDictionary<codeword_type, Next_Longest_Run>(), empty(false) {
    // we have an array for each possible string length < our max
    for (int i = 0; i < MAX_STRING_LENGTH; i++) {
      // for strings of length n, they can be represented by 2n bits
      // so 4^(i+1) is the number of possible strings of length i+1
      int size = pow(4, i + 1);
      dictionary[i] = std::unique_ptr<codeword_type[]>(
          (codeword_type *)calloc(size, sizeof(codeword_type)));
      if (!dictionary[i]) {
        std::cout << "Memory Failure. Exiting...." << std::endl;
        abort();
      }
    }

    // load the code values for our neucleotides
    values['A'] = INDEX_OF_A;
    values['C'] = INDEX_OF_C;
    values['T'] = INDEX_OF_T;
    values['G'] = INDEX_OF_G;
  }

  codeword_type code_of(const char *input, unsigned len) const override {
    codeword_type lookup = (dictionary[len - 1])[map_str(input, len)];
    return lookup;
  }

  /**
   * Returns the codeword of the string
   * requires index to be precomputed
   * @param len lenght of original string
   * @param mapped_string index of string
   */
  codeword_type code_of_manual(unsigned len, index_type mapped_string) const {
    codeword_type lookup = (dictionary[len - 1])[mapped_string];
    return lookup;
  }

  /**
   * Loops up from start, looking for longest run
   * computes index "on the fly", adding onto the starting index given
   * @param input pointer to input string
   * @param end_of_input pointer to end of input stream
   * @param start where to start searching from
   * @param index index of the string up to start
   */
  Next_Longest_Run find_longest_looping_up_on_fly(const char *input,
                                                  const char *end_of_input,
                                                  int start, index_type index) {
    int length = start;
    codeword_type entry = 0;
    codeword_type last_entry = 0;
    while (input + length < end_of_input && length < MAX_STRING_LENGTH) {
      index = (index << 2) + values[input[length]];
      last_entry = entry;
      entry = code_of_manual(length + 1, index);
      // if entry is non zero, it means we have seen that string before
      if (entry == 0) {
        return Next_Longest_Run(length, last_entry, index >> 2);
      }
      length++;
    }
    return Next_Longest_Run(length, entry, index);
  }

  /**
   * Loops up from start, looking for longest run
   * Assumes index given is for a string of MAX_STRING_LENGTH or greater, or til
   * end of input
   * @param input pointer to input string
   * @param end_of_input pointer to end of input stream
   * @param start where to start searching from
   * @param index index of the input up to
   */
  Next_Longest_Run find_longest_looping_up(const char *input,
                                           const char *end_of_input, int start,
                                           index_type index) {
    int length = start;
    codeword_type entry = 0;
    codeword_type last_entry = 0;
    index_type next_index = 0;
    while (input + length < end_of_input && length < MAX_STRING_LENGTH) {
      next_index = (index >> (INDEX_BITS - (length + 1) * 2));
      last_entry = entry;
      entry = code_of_manual(length + 1, next_index);
      // if entry is non zero, it means we have seen that string before
      if (entry == 0) {
        return Next_Longest_Run(length, last_entry, next_index >> 2);
      }
      length++;
    }
    return Next_Longest_Run(length, entry, next_index);
  }

  /**
   * Loops down from start, looking for longest run
   * Assumes index given is for a string of start or greater
   * @param start where to start searching from
   * @param index index of the input up to start
   */
  Next_Longest_Run find_longest_looping_down(int start, index_type index) {
    int length = start;
    codeword_type entry;
    while (length > 0) {
      entry = code_of_manual(length, index);
      // if entry is non zero, it means we have seen that string before
      if (entry != 0) {
        return Next_Longest_Run(length, entry, index);
      }
      length--;
      index = (index >> 2);
    }
    return Next_Longest_Run(length, entry, index);
  }

  /**
   * Binary searches within bounds of left and right for the longest run
   * Assumes that such a run exists
   * @param left left bound
   * @param right right bound
   * @param full_index index that covers strings within starting bounds
   */
  Next_Longest_Run find_longest_binary_search(const char *input, int left,
                                              int right,
                                              index_type full_index) {
    int middle = left + ((right - left) / 2);
    codeword_type entry;
    index_type index;
    index_type next_index;
    while (left <= right) {
      next_index = full_index >> (INDEX_BITS - (middle + 1) * 2);
      index = next_index >> 2;
      entry = code_of_manual(middle, index);
      if (entry == 0) {
        right = middle - 1;
        middle = left + ((right - left) / 2);
      } else if (middle == MAX_STRING_LENGTH ||
                 code_of_manual(middle + 1, next_index) == 0) {
        return Next_Longest_Run(middle, entry, index);
      } else {
        left = middle + 1;
        middle = left + ((right - left) / 2);
      }
    }

    index = values[input[0]];
    entry = code_of_manual(1, index);
    return Next_Longest_Run(1, entry, index);
  }

  Next_Longest_Run find_longest_in_dict(const char *input,
                                        const char *end_of_input) override {

    // start at start and loop up or down
    if (input + 16 > end_of_input) {
      return find_longest_looping_up_on_fly(input, end_of_input, 0, 0);
    }

    const uint64_t *input_ints = reinterpret_cast<const uint64_t *>(input);
    uint64_t swapped = _bswap64(input_ints[0]);
    uint64_t swapped2 = _bswap64(input_ints[1]);
    uint64_t first_half = _pext_u64(swapped, MASK);
    uint64_t second_half = _pext_u64(swapped2, MASK);
    index_type next_long_index =
        (first_half << NEXT_LONGEST_INDEX_SIZE) + second_half;

    if (!BINARY_SEARCH) {
      // check the starting length
      index_type index_for_fl_start =
          next_long_index >> (INDEX_BITS - (FIND_LONGEST_START * 2));
      int entry = code_of_manual(FIND_LONGEST_START, index_for_fl_start);
      if (entry == 0) {
        return find_longest_looping_down(FIND_LONGEST_START,
                                         index_for_fl_start);
      }
      return find_longest_looping_up(input, end_of_input,
                                     FIND_LONGEST_START - 1, next_long_index);

    } else {

      // start at start and binary search
      // check the starting string
      int index = next_long_index >> (INDEX_BITS - FIND_LONGEST_START * 2);
      int entry = code_of_manual(FIND_LONGEST_START, index);
      if (entry == 0) {
        return find_longest_binary_search(input, 1, FIND_LONGEST_START - 1,
                                          next_long_index);
      }
      return find_longest_binary_search(input, FIND_LONGEST_START,
                                        MAX_STRING_LENGTH, next_long_index);
    }
  }

  /**
   * Adds a string to the dictionary
   * @param input pointer to start of input
   * @param len length to read
   * @param codeword codeword to assign to input string
   */
  void add_string(const char *input, unsigned len,
                  codeword_type codeword) override {
    // we want to prevent collisions, so when we reached our limit,
    // allow this last addition then declare the dict empty.
    if (empty) {
      return;
    }
    if (codeword == MAX_CODEWORD) {
      empty = true;
    }

    // we just don't add anything longer to the dict
    if (len > MAX_STRING_LENGTH) {
      return;
    }
    (dictionary[len - 1])[map_str(input, len)] = codeword;
  }

  /**
   * Adds a string to the dictionary
   * builds on index in the next_longest argument
   * @param input pointer to start of input
   * @param next_longest information about the next longest run
   * @param codeword codeword to assign to input string
   */
  void add_string(const char *input, Next_Longest_Run next_longest,
                  codeword_type codeword) override {
    if (empty) {
      return;
    }
    if (codeword == MAX_CODEWORD) {
      empty = true;
    }
    // we just don't add anything longer to the dict
    if (next_longest.next_run_length + 1 > MAX_STRING_LENGTH) {
      return;
    }
    index_type index = (next_longest.longest_run_index << 2) +
                       values[input[next_longest.next_run_length]];
    (dictionary[next_longest.next_run_length])[index] = codeword;
  }

  void load_starting_dictionary() override {
    add_string("A", 1, 1);
    add_string("C", 1, 2);
    add_string("T", 1, 3);
    add_string("G", 1, 4);
  }
};

/**
 * Direct Decode
 *
 * we simply have an array of strings long enough to accomodate every codeword.
 *
 * codewords index into the dictionary, and adding and searching are trivial
 */
class LZW_Decode_Dictionary : private LZWDictionary<codeword_type> {
private:
  static constexpr int CODEWORD_SIZE = CHAR_BIT * sizeof(codeword_type);
  bool empty;

public:
  std::array<std::string, MAX_CODEWORD + 1> dictionary;
  LZW_Decode_Dictionary() : LZWDictionary<codeword_type>(), empty(false) {}

  void add_string(std::string str, codeword_type codeword) override {
    if (empty) {
      return;
    }
    if (codeword == MAX_CODEWORD) {
      empty = true;
    }
    dictionary[codeword] = str;
  }

  std::string str_of(codeword_type codeword) const override {
    return dictionary[codeword];
  }

  void load_starting_dictionary() override {
    add_string("A", 1);
    add_string("C", 2);
    add_string("T", 3);
    add_string("G", 4);
  }
};

/**
 * Codeword_Helper for Direct Map:
 *
 * Since we have fixed length codeword, we only worry about overflow and
 * incrementing the codeword
 *
 */
class Codeword_Helper : public CW_Tracker<codeword_type> {

public:
  Codeword_Helper()
      : CW_Tracker<codeword_type>(6, 5, sizeof(codeword_type) * CHAR_BIT){};
  codeword_type get_next_codeword() {
    if (current_codeword == MAX_CODEWORD) {
      return MAX_CODEWORD;
    }
    return current_codeword++;
  }
};
} // namespace Direct_Map_Dictionary

using namespace Direct_Map_Dictionary;
