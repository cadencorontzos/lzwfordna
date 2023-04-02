#include "dictionary.hh"
#include <climits>
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <x86intrin.h>

constexpr uint8_t MAX_STRING_LENGTH = 15;
typedef uint16_t codeword_type;
const codeword_type MAX_CODEWORD =
    static_cast<codeword_type>((1 << (sizeof(codeword_type) * CHAR_BIT)) - 1);
const int CODEWORD_SIZE = sizeof(codeword_type) * CHAR_BIT;

// Direct Mapped Dictionary:
//
// We will have dictionaries for every length 1 to MAX_STRING_LENGTH, for
// strings of that length
//
// We will map the 4 neucleotides into two bits, so any string of length n can
// be represented by 2n bits That 2n bit encoding is an index into the nth
// dictionary
//
// in the nth dictionary, there is space for every string of length n
//
// upon lookup, we look at the space in the nth dictionary corresponding to the
// 2n bit index given by our lookup string if there is a codeword there, we
// return it. If the codeword is 0, we know we haven't seen that string before
//
// for strings longer than length MAX_STRING_LENGTH, we have a unordered_map on
// top to take the rest
//
// when we run out of codewords, we simply don't allow any more strings to be
// added
//
class LZW_Encode_Dictionary : private LZWDictionary<codeword_type> {
private:
  using index_type = uint32_t;
  const int INDEX_BITS = CHAR_BIT * sizeof(index_type);
  std::array<int, 1 << CHAR_BIT> values;
  const int FIND_LONGEST_START = 7;
  const int NEXT_LONGEST_INDEX_SIZE = 16;
  // 00000110 00000110 00000110 00000110 00000110 00000110 00000110 00000110
  const uint64_t MASK = 434041037028460038;

  index_type map_str(const char *input, unsigned len) const {
    index_type result = 0;
    for (unsigned i = 0; i < len; i++) {
      result = (result << 2) + values[input[i]];
    }
    return result;
  }

  // we will map the strings directly to memory in this array
  std::array<std::unique_ptr<codeword_type[]>, MAX_STRING_LENGTH> dictionary;
  // track when we run out of codewords
  bool empty;

  // add a hashmap on top of our direct map for strings longer than our max
  std::unordered_map<std::string, codeword_type> longer_than_max;
  std::unordered_map<std::string, codeword_type>::const_iterator end;

public:
  LZW_Encode_Dictionary()
      : LZWDictionary<codeword_type>(), empty(false),
        end(longer_than_max.cend()) {
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
    values['A'] = 0;
    values['C'] = 1;
    values['T'] = 2;
    values['G'] = 3;
  }

  codeword_type code_of(const char *input, unsigned len) const override {
    codeword_type lookup = (dictionary[len - 1])[map_str(input, len)];
    return lookup;
  }

  // allow the caller to provide the string already mapped.
  // allows for the mapped string to be produced on the fly in some cases
  codeword_type code_of_manual(unsigned len, int mapped_string) const {
    codeword_type lookup = (dictionary[len - 1])[mapped_string];
    return lookup;
  }

  // We assume that the start is already in the dictionary, and loop up from
  // there
  int find_longest_looping_up_on_fly(const char *input,
                                     const char *end_of_input, int start,
                                     int index) {
    int length = start;
    int entry = 0;
    while (input + length < end_of_input && length < MAX_STRING_LENGTH) {
      index = (index << 2) + values[input[length]];
      entry = code_of_manual(length + 1, index);
      // if entry is non zero, it means we have seen that string before
      if (entry == 0) {
        return length;
      }
      length++;
    }
    return length;
  }

  // We assume that the start is already in the dictionary, and loop up from
  // there
  int find_longest_looping_up(const char *input, const char *end_of_input,
                              int start, index_type index) {
    int length = start;
    int entry = 0;
    while (input + length < end_of_input && length < MAX_STRING_LENGTH) {
      entry = code_of_manual(length + 1,
                             (index >> (INDEX_BITS - (length + 1) * 2)));
      // if entry is non zero, it means we have seen that string before
      if (entry == 0) {
        return length;
      }
      length++;
    }
    return length;
  }

  // Assumes we already have the starting characters in the dictionary
  // should never return 0
  int find_longest_looping_down(int start, index_type index) {
    int length = start;
    int entry;
    while (length > 0) {
      entry = code_of_manual(length, index);
      // if entry is non zero, it means we have seen that string before
      if (entry != 0) {
        return length;
      }
      length--;
      index = (index >> 2);
    }
    return length;
  }

  // find longest using binary search.
  // assumes that the longest run < max
  // assumes that all characters are in starting dictionary
  int find_longest_binary_search(const char *input, int left, int right,
                                 index_type full_index) {
    int middle = left + ((right - left) / 2);
    int entry;
    index_type index;
    while (left <= right) {
      index = full_index >> (INDEX_BITS - (middle)*2);
      entry = code_of_manual(middle, index);
      if (entry == 0) {
        right = middle - 1;
        middle = left + ((right - left) / 2);
      } else if (middle == MAX_STRING_LENGTH ||
                 code_of(input, middle + 1) == 0) {
        return middle;
      } else {
        left = middle + 1;
        middle = left + ((right - left) / 2);
      }
    }
    return 1;
  }

  int find_longest_in_dict(const char *input,
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

    /* // check the starting length */
    /* index_type index_for_fl_start = next_long_index >>
     * (INDEX_BITS-(FIND_LONGEST_START*2)); */
    /* int entry = code_of_manual(FIND_LONGEST_START, index_for_fl_start); */
    /* if(entry == 0){ */
    /* 	return find_longest_looping_down(FIND_LONGEST_START,
     * index_for_fl_start); */
    /* } */
    /* return find_longest_looping_up(input, end_of_input, FIND_LONGEST_START,
     * next_long_index); */

    // start at start and binary search
    //
    // if we don't have enough input left, loop up from 0
    /* if(input+MAX_STRING_LENGTH> end_of_input){ */
    /* 	return find_longest_looping_up(input, end_of_input, 0, 0); */
    /* } */
    // check the starting string
    int index = map_str(input, FIND_LONGEST_START);
    int entry = code_of_manual(FIND_LONGEST_START, index);
    if (entry == 0) {
      return find_longest_binary_search(input, 1, FIND_LONGEST_START - 1,
                                        next_long_index);
    }
    return find_longest_binary_search(input, FIND_LONGEST_START,
                                      MAX_STRING_LENGTH, next_long_index);

    // loop up from 0
    /* int length = 0; */
    /* int entry = 0; */
    /* while(input+length < end_of_input) { */
    /* 	length++; */
    /* 	entry = code_of(input, length); */
    /* 	// if entry is non zero, it means we have seen that string before */
    /* 	if (entry == 0){ */
    /* 		return length-1; */
    /* 	} */
    /* } */
    /* if(length == 0){ */
    /* 	return 0; */
    /* } */
    /* return length; */
  }

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

  void load_starting_dictionary() override {
    add_string("A", 1, 1);
    add_string("C", 1, 2);
    add_string("T", 1, 3);
    add_string("G", 1, 4);
  }
};

// Direct Decode
//
// we simply have an array of strings long enough to accomodate every codeword.
//
// codewords index into the dictionary, and adding and searching are trivial
// from that fact
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

// Codeword_Helper for Direct Map:
//
// Since we have fixed length codeword, we only worry about overflow and
// incrementing the codeword
//
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

class Next_Longest_Run : public Next_Run<codeword_type> {
private:
};
