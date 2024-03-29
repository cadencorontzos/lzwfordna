#include "dictionary.hh"
#include <climits>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <unordered_map>

/**
 * A collection of classes for LZW using the std::unordered_map
 */
namespace Std_Dictionary {

typedef uint32_t codeword_type;
const codeword_type MAX_CODEWORD = std::numeric_limits<codeword_type>::max();
const int CODEWORD_SIZE = std::numeric_limits<codeword_type>::digits;

class Next_Longest_Run : public Next_Run<codeword_type> {
public:
  std::string next_longest_run_string;
  Next_Longest_Run(unsigned length, codeword_type cw, std::string str)
      : Next_Run<codeword_type>(length, cw), next_longest_run_string(str){};
  Next_Longest_Run()
      : Next_Run<codeword_type>(0, 0), next_longest_run_string(""){};
};

class LZW_Encode_Dictionary
    : private LZWDictionary<codeword_type, Next_Longest_Run> {
private:
  std::unordered_map<std::string, codeword_type> dictionary;
  std::unordered_map<std::string, codeword_type>::const_iterator end;

  /// track when we run out of codewords
  bool empty;

public:
  LZW_Encode_Dictionary()
      : LZWDictionary<codeword_type, Next_Longest_Run>(),
        end(dictionary.cend()), empty(false) {}

  codeword_type code_of(const char *input, unsigned len) const override {
    std::string str(input, len);
    auto lookup = dictionary.find(str);
    if (lookup == end) {
      return 0;
    }
    return lookup->second;
  }

  codeword_type code_of_manual(std::string str) const {
    auto lookup = dictionary.find(str);
    if (lookup == end) {
      return 0;
    }
    return lookup->second;
  }

  Next_Longest_Run find_longest_in_dict(const char *input,
                                        const char *end_of_input) override {
    int length = 0;
    int entry = 0;
    int last_entry = 0;
    std::string run_string = "";
    while (input + length < end_of_input) {
      length++;
      last_entry = entry;
      run_string = std::string(input, length);
      entry = code_of_manual(run_string);
      // if entry is non zero, it means we have seen that string before
      if (entry == 0) {
        run_string.pop_back();
        return Next_Longest_Run(length - 1, last_entry, run_string);
      }
    }

    return Next_Longest_Run(length, entry, run_string);
  }

  void add_string(const char *input, unsigned len,
                  codeword_type codeword) override {
    // we want to prevent overflow, so when we reached our limit,
    // allow this last addition then declare the dict empty.
    if (empty) {
      return;
    }
    if (codeword == MAX_CODEWORD) {
      empty = true;
    }
    std::string str(input, len);
    dictionary[str] = codeword;
  }

  void add_string(const char *input, Next_Longest_Run next_longest,
                  codeword_type codeword) override {
    // we want to prevent overflow, so when we reached our limit,
    // allow this last addition then declare the dict empty.
    if (empty) {
      return;
    }
    if (codeword == MAX_CODEWORD) {
      empty = true;
    }
    dictionary[next_longest.next_longest_run_string +
               input[next_longest.next_run_length]] = codeword;
  }

  void load_starting_dictionary() override {
    add_string("A", 1, 1);
    add_string("T", 1, 2);
    add_string("C", 1, 3);
    add_string("G", 1, 4);
  }
};

/**
 * Std Decode just uses a the same unordered_map
 */
class LZW_Decode_Dictionary : private LZWDictionary<codeword_type> {
private:
  // dictionary and end of dictionary
  std::unordered_map<codeword_type, std::string> dictionary;
  std::unordered_map<codeword_type, std::string>::const_iterator end;
  bool empty;

public:
  LZW_Decode_Dictionary()
      : LZWDictionary<codeword_type>(), end(dictionary.cend()), empty(false) {}

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
    return dictionary.find(codeword)->second;
  }

  void load_starting_dictionary() override {
    add_string("A", 1);
    add_string("T", 2);
    add_string("C", 3);
    add_string("G", 4);
  }
};

/**
 * Tracks codewords and allows for growing codewords
 */
class Codeword_Helper : public CW_Tracker<codeword_type> {
private:
  unsigned int current_max_cw;

public:
  Codeword_Helper()
      : CW_Tracker<codeword_type>(6, 5, 3),
        current_max_cw(1 << bits_per_codeword){};
  codeword_type get_next_codeword() {
    if (current_codeword == MAX_CODEWORD) {
      return MAX_CODEWORD;
    }
    if (current_codeword == current_max_cw) {
      bits_per_codeword++;
      current_max_cw = (1 << bits_per_codeword);
    }
    return current_codeword++;
  }
};
} // namespace Std_Dictionary

using namespace Std_Dictionary;
