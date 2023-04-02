#include "direct_mapped_dict.hh"
#include <cassert>
#include <iostream>
#include <string>

// tests for endcode dictionary
//
//

void test_code_of() {

  LZW_Encode_Dictionary f;

  // load strings into dictionary
  f.add_string("A", 1, 1);
  f.add_string("AC", 2, 2);
  f.add_string("ACT", 3, 3);

  // make sure codes of those strings are right
  assert(f.code_of("A", 1) == 1);
  assert(f.code_of("AC", 2) == 2);
  assert(f.code_of("ACT", 3) == 3);
}

void test_code_of_manual() {
  LZW_Encode_Dictionary f;
  // load strings into dictionary
  f.add_string("A", 1, 1);
  f.add_string("AC", 2, 2);
  f.add_string("ACT", 3, 3);

  // make sure codes of those strings are right
  assert(f.code_of_manual(1, INDEX_OF_A) == 1);
  assert(f.code_of_manual(2, (INDEX_OF_A << 2) + INDEX_OF_C) == 2);
  assert(f.code_of_manual(3, (INDEX_OF_A << 4) + (INDEX_OF_C << 2) +
                                 (INDEX_OF_T)) == 3);
}

void test_find_longest_in_dict() {

  // test string shorter than max string length with total string less than max
  // length

  {

    LZW_Encode_Dictionary f;

    // load three strings, each building on last
    f.add_string("A", 1, 1);
    f.add_string("AC", 2, 2);
    f.add_string("ACT", 3, 3);

    // for this input, "ACT" should be the longest run in the dict
    std::string input = "ACTG";

    Next_Longest_Run ret =
        f.find_longest_in_dict(input.c_str(), input.c_str() + input.length());
    std::string output = input.substr(0, ret.next_run_length);
    assert(output == "ACT");
    assert(ret.codeword_of_next_run == 3);
    assert(f.code_of(output.c_str(), 3) == 3);
  }

  // test string shorter than max string length with total string greater than
  // max length
  {

    LZW_Encode_Dictionary f;

    // load three strings, each building on last
    f.add_string("A", 1, 1);
    f.add_string("AC", 2, 2);
    f.add_string("ACT", 3, 3);

    // for this input, "ACT" should be the longest run in the dict
    std::string input = "ACTGACTGACTGACTGACTGACTG";

    Next_Longest_Run ret =
        f.find_longest_in_dict(input.c_str(), input.c_str() + input.length());
    std::string output = input.substr(0, ret.next_run_length);
    assert(output == "ACT");
    assert(ret.codeword_of_next_run == 3);
    assert(f.code_of(output.c_str(), 3) == 3);
  }

  // test string equal to max string length

  {
    LZW_Encode_Dictionary f;

    // build string
    char oneA = 'A';
    std::string input = "";
    for (unsigned i = 1; i <= MAX_STRING_LENGTH; i++) {
      input += oneA;
      f.add_string(input.c_str(), i, i);
    }

    Next_Longest_Run ret =
        f.find_longest_in_dict(input.c_str(), input.c_str() + input.length());
    std::string output = input.substr(0, ret.next_run_length);

    assert(output == input);
    assert(ret.codeword_of_next_run == MAX_STRING_LENGTH);
    assert(f.code_of(output.c_str(), input.length()) == MAX_STRING_LENGTH);
  }
}

void test_find_longest_looping_down() {

  LZW_Encode_Dictionary f;

  // load three strings, each building on last
  f.add_string("A", 1, 1);
  f.add_string("AC", 2, 2);

  // for this input, "ACT" should be the longest run in the dict
  std::string input = "ACTG";
  int index =
      (INDEX_OF_A << 6) + (INDEX_OF_C << 4) + (INDEX_OF_T << 2) + INDEX_OF_G;
  Next_Longest_Run ret = f.find_longest_looping_down(4, index);
  std::string output = input.substr(0, ret.next_run_length);
  assert(output == "AC");
  assert(ret.codeword_of_next_run == 2);
  assert(f.code_of(output.c_str(), 2) == 2);
}

void test_find_longest_looping_up() {

  LZW_Encode_Dictionary f;

  // load three strings, each building on last
  f.add_string("A", 1, 1);
  f.add_string("AC", 2, 2);
  f.add_string("ACT", 3, 3);

  // for this input, "ACT" should be the longest run in the dict
  std::string input = "ACTG";

  Next_Longest_Run ret = f.find_longest_looping_up_on_fly(
      input.c_str(), input.c_str() + input.length(), 0, 0);
  std::string output = input.substr(0, ret.next_run_length);
  assert(output == "ACT");
  assert(ret.codeword_of_next_run == 3);
  assert(f.code_of(output.c_str(), 3) == 3);
}

void test_find_longest_binary_search() {

  int index = (0 << 6) + (1 << 4) + (2 << 2) + 3;
  uint32_t long_index = (index << 24) + (index << 16) + (index << 8) + index;

  // test len < pivot
  {
    LZW_Encode_Dictionary f;

    // load three strings, each building on last
    f.add_string("A", 1, 1);
    f.add_string("AC", 2, 2);
    f.add_string("ACT", 3, 3);

    // for this input, "ACT" should be the longest run in the dict
    std::string input = "ACTGACTGACTGACTGACTG";

    Next_Longest_Run ret = f.find_longest_binary_search(
        input.c_str(), 1, MAX_STRING_LENGTH, long_index);
    std::string output = input.substr(0, ret.next_run_length);
    assert(output == "ACT");
    assert(ret.codeword_of_next_run == 3);
    assert(f.code_of(output.c_str(), 3) == 3);
  }

  // test len = pivot
  {
    LZW_Encode_Dictionary f;

    // for this input, "ACT" should be the longest run in the dict
    std::string input = "ACTGACTGACTGACTGACTG";
    unsigned pivot = FIND_LONGEST_START;
    for (unsigned i = 1; i <= pivot; i++) {
      f.add_string(input.substr(0, i).c_str(), i, i);
    }
    Next_Longest_Run ret = f.find_longest_binary_search(
        input.c_str(), 1, MAX_STRING_LENGTH, long_index);
    std::string output = input.substr(0, ret.next_run_length);
    assert(output == input.substr(0, pivot));
    assert(ret.codeword_of_next_run == pivot);
    assert(f.code_of(output.c_str(), pivot) == pivot);
  }

  // test len > pivot
  {
    LZW_Encode_Dictionary f;

    // for this input, "ACT" should be the longest run in the dict
    std::string input = "ACTGACTGACTGACTGACTG";
    unsigned pivot = FIND_LONGEST_START + 2;
    for (unsigned i = 1; i <= pivot; i++) {
      f.add_string(input.substr(0, i).c_str(), i, i);
    }
    Next_Longest_Run ret = f.find_longest_binary_search(
        input.c_str(), 1, MAX_STRING_LENGTH, long_index);
    std::string output = input.substr(0, ret.next_run_length);
    assert(output == input.substr(0, pivot));
    assert(ret.codeword_of_next_run == pivot);
    assert(f.code_of(output.c_str(), pivot) == pivot);
  }
}

void test_add_string_encode() {
  LZW_Encode_Dictionary f;

  f.add_string("ACTGC", 5, 1);

  assert(f.code_of("ACTGC", 5) == 1);
}

void test_add_string_with_NLR_encode() {
  LZW_Encode_Dictionary f;

  index_type index = (INDEX_OF_A << 8) + (INDEX_OF_C << 6) + (INDEX_OF_T << 4) +
                     (INDEX_OF_G << 2) + INDEX_OF_C;
  // we say the next longest run is the run of ACTGC
  Next_Longest_Run nlr(5, 1, index);

  // then if we add_string, we should be adding ACTGCT
  f.add_string("ACTGCT", nlr, 1);

  assert(f.code_of("ACTGCT", 6) == 1);
  assert(f.code_of_manual(6, (index << 2) + INDEX_OF_T));
}

void test_encode() {
  test_add_string_encode();
  test_add_string_with_NLR_encode();
  test_code_of();
  test_code_of_manual();
  test_find_longest_in_dict();
  test_find_longest_looping_down();
  test_find_longest_looping_up();
  test_find_longest_binary_search();
}

// tests for decode dictionary
//
//

void test_add_string_decode() {
  LZW_Decode_Dictionary f;
  f.add_string("ACTGC", 1);
  assert(f.str_of(1) == "ACTGC");
}

void test_str_of() {

  LZW_Decode_Dictionary f;

  // add strings into decode dict
  f.add_string("A", 1);
  f.add_string("AT", 2);
  f.add_string("ACT", 3);

  // make sure lookups are correct
  assert(f.str_of(1) == "A");
  assert(f.str_of(2) == "AT");
  assert(f.str_of(3) == "ACT");
}

void test_decode() {
  test_add_string_decode();
  test_str_of();
}

// test for codeword_helper
//
//

void test_codeword_helper() {

  Codeword_Helper codeword_helper;

  // check that it is set up correctly
  assert(codeword_helper.EOF_CODEWORD == 5);
  assert(codeword_helper.current_codeword == 6);

  // check that helper increments correctly
  for (int i = codeword_helper.current_codeword;
       i < (1 << codeword_helper.bits_per_codeword); i++) {
    assert(i == codeword_helper.get_next_codeword());
  }
  // make sure we didn't go over max
  assert(codeword_helper.get_next_codeword() ==
         (1 << codeword_helper.bits_per_codeword) - 1);
}

int main() {
  test_encode();
  test_decode();
  test_codeword_helper();
  return 0;
}
