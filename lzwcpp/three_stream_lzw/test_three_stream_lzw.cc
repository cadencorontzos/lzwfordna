#include "../bitio/bitio.hh"
#include "../dictionary/direct_mapped_dict.hh"
#include "three_stream_lzw.hh"
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

const int A_CODE = 0;
const int C_CODE = 1;
const int T_CODE = 2;
const int G_CODE = 3;

std::string to_bits(std::string str) {
  std::string res = "";

  for (unsigned index = 0; index < str.length(); index++) {
    res += std::bitset<8>(str[index]).to_string();
  }
  return res;
}

void detailed_assert(std::string expected, std::string output,
                     std::string test_name) {
  if (expected != output) {
    std::cout << "Test Failed: " << test_name << std::endl;
    std::cout << "Expected: " << std::endl;
    std::cout << "    literal string: " << expected << std::endl;
    std::cout << "    bits " << to_bits(expected) << std::endl;
    std::cout << "Found: " << std::endl;
    std::cout << "    literal string: " << expected << std::endl;
    std::cout << "    bits " << to_bits(output) << std::endl;
  }
  assert(expected == output);
}

// empty
std::string empty_decoded = "";
// we expect
// | eof              | 0| trailing
int empty_encoded_length;
std::array<std::string, 3> empty_expected_output() {
  Codeword_Helper codeword_helper;
  std::stringstream codeword_bits;
  std::stringstream indicator_bits;
  std::stringstream char_bits;
  {
    BitOutput cw_bito(codeword_bits);
    BitOutput char_bito(char_bits);
    BitOutput indicator_bito(indicator_bits);

    // eof
    indicator_bito.output_bit(true);
    cw_bito.output_n_bits(codeword_helper.EOF_CODEWORD,
                          codeword_helper.bits_per_codeword);
    // 00
    char_bito.output_bit(0);
    char_bito.output_bit(0);

  } // trailing
  const std::string &cw = codeword_bits.str();
  const std::string &chrs = char_bits.str();
  const std::string &idc = indicator_bits.str();
  return {cw, chrs, idc};
}

// 01100110
std::string single_char_decoded = "A";
// we expect
// eof       		 | 1| A       | trailing
std::array<std::string, 3> single_char_expected_output() {

  Codeword_Helper codeword_helper;
  std::stringstream codeword_bits;
  std::stringstream indicator_bits;
  std::stringstream char_bits;
  {
    BitOutput cw_bito(codeword_bits);
    BitOutput char_bito(char_bits);
    BitOutput indicator_bito(indicator_bits);

    // eof

    indicator_bito.output_bit(true);
    cw_bito.output_n_bits(codeword_helper.EOF_CODEWORD,
                          codeword_helper.bits_per_codeword);
    // 01
    char_bito.output_bit(0);
    char_bito.output_bit(1);

    // A
    char_bito.output_n_bits(A_CODE, 2);

  } // trailing
  const std::string &cw = codeword_bits.str();
  const std::string &chrs = char_bits.str();
  const std::string &idc = indicator_bits.str();
  return {cw, chrs, idc};
}

// 01100001 01100010
std::string two_chars_decoded = "AG";
// | cw for A         | G       | eof              | 0| trailing
std::array<std::string, 3> two_chars_expected_output() {

  Codeword_Helper codeword_helper;
  std::stringstream codeword_bits;
  std::stringstream indicator_bits;
  std::stringstream char_bits;
  {
    BitOutput cw_bito(codeword_bits);
    BitOutput char_bito(char_bits);
    BitOutput indicator_bito(indicator_bits);

    indicator_bito.output_bit(true);
    // eof
    cw_bito.output_n_bits(codeword_helper.EOF_CODEWORD,
                          codeword_helper.bits_per_codeword);

    char_bito.output_bit(1);
    char_bito.output_bit(1);
    char_bito.output_n_bits(2, 3);
    // A
    char_bito.output_n_bits(A_CODE, 2);
    // G
    char_bito.output_n_bits(G_CODE, 2);

  } // trailing
  const std::string &cw = codeword_bits.str();
  const std::string &chrs = char_bits.str();
  const std::string &idc = indicator_bits.str();
  return {cw, chrs, idc};
}

// 01100001 01100010 01100110
std::string three_chars_decoded = "AGAG";
// | cw for A         | G       | eof              | 11| cw for AG        |
// trailing
std::array<std::string, 3> three_chars_expected_output() {

  Codeword_Helper codeword_helper;
  std::stringstream codeword_bits;
  std::stringstream indicator_bits;
  std::stringstream char_bits;
  {
    BitOutput cw_bito(codeword_bits);
    BitOutput char_bito(char_bits);
    BitOutput indicator_bito(indicator_bits);

    indicator_bito.output_bit(true);
    // eof
    cw_bito.output_n_bits(codeword_helper.EOF_CODEWORD,
                          codeword_helper.bits_per_codeword);

    // 11
    char_bito.output_bit(1);
    char_bito.output_bit(1);
    char_bito.output_n_bits(4, 3);
    // A
    char_bito.output_n_bits(A_CODE, 2);
    // G
    char_bito.output_n_bits(G_CODE, 2);
    // A
    char_bito.output_n_bits(A_CODE, 2);
    // G
    char_bito.output_n_bits(G_CODE, 2);

  } // trailing
  const std::string &cw = codeword_bits.str();
  const std::string &chrs = char_bits.str();
  const std::string &idc = indicator_bits.str();
  return {cw, chrs, idc};
}

// 67 As
std::string chars_til_codeword_decoded =
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
// | cw for A         | G       | eof              | 11| cw for AG        |
// trailing
std::array<std::string, 3> chars_til_codeword_expected_output() {

  Codeword_Helper codeword_helper;
  std::stringstream codeword_bits;
  std::stringstream indicator_bits;
  std::stringstream char_bits;
  {
    BitOutput cw_bito(codeword_bits);
    BitOutput char_bito(char_bits);
    BitOutput indicator_bito(indicator_bits);

    for (int i = 0; i < 7; i++) {
      indicator_bito.output_bit(false);
      char_bito.output_n_bits(0, 16);
    }
    // on the 8th, we should
    // have cw of length 8
    indicator_bito.output_bit(true);
    codeword_type codeword = codeword_helper.get_next_codeword();
    cw_bito.output_n_bits(codeword + 6, codeword_helper.bits_per_codeword);
    // A
    char_bito.output_n_bits(A_CODE, 2);

    indicator_bito.output_bit(true);
    cw_bito.output_n_bits(codeword_helper.EOF_CODEWORD,
                          codeword_helper.bits_per_codeword);
    // 11
    char_bito.output_bit(1);
    char_bito.output_bit(1);
    // run of one A left
    char_bito.output_n_bits(2, 3);
    // A
    char_bito.output_n_bits(A_CODE, 2);
    // A
    char_bito.output_n_bits(A_CODE, 2);

  } // trailing
  const std::string &cw = codeword_bits.str();
  const std::string &chrs = char_bits.str();
  const std::string &idc = indicator_bits.str();
  return {cw, chrs, idc};
}
////////////////////////////////////////////////////////////
// encode unit tests

void test_encode(std::string decoded_string,
                 std::array<std::string, 3> expected_output) {
  std::cout << "Testing string :" << decoded_string << std::endl; // for debug
  std::stringstream char_output;
  std::stringstream cw_output;
  std::stringstream indicator_output;
  const char *input_file = decoded_string.c_str();
  int file_size = decoded_string.length();
  {
    Three_Stream_LZW lzw;
    lzw.encode(input_file, file_size, cw_output, char_output, indicator_output);
  }

  std::string char_output_string = char_output.str();
  std::string indicator_output_string = indicator_output.str();
  std::string cw_output_string = cw_output.str();
  detailed_assert(expected_output[0], cw_output_string, "codword stream");
  std::cout << "cw stream passed" << std::endl;
  detailed_assert(expected_output[1], char_output_string, "char stream");
  std::cout << "char stream passed" << std::endl;
  detailed_assert(expected_output[2], indicator_output_string,
                  "indicator stream");
  std::cout << "indicator stream passed" << std::endl;
}

void encode_tests() {
  test_encode(empty_decoded, empty_expected_output());
  test_encode(single_char_decoded, single_char_expected_output());
  test_encode(two_chars_decoded, two_chars_expected_output());
  test_encode(three_chars_decoded, three_chars_expected_output());
  test_encode(chars_til_codeword_decoded, chars_til_codeword_expected_output());
}

////////////////////////////////////////////////////////////
// decode unit tests

void test_decode(std::array<std::string, 3> encoded_string,
                 std::string decoded_string) {
  std::cout << "Testing decode " << decoded_string << std::endl;
  std::stringstream output;

  {
    Three_Stream_LZW lzw;
    lzw.decode(encoded_string[1].c_str(), encoded_string[0].c_str(),
               encoded_string[2].c_str(), output);
  }

  std::string expected_output(decoded_string);
  std::string output_string = output.str();
  detailed_assert(expected_output, output_string, "decode " + decoded_string);
}

void decode_tests() {
  test_decode(empty_expected_output(), empty_decoded);
  test_decode(single_char_expected_output(), single_char_decoded);
  test_decode(two_chars_expected_output(), two_chars_decoded);
  test_decode(three_chars_expected_output(), three_chars_decoded);
  test_decode(chars_til_codeword_expected_output(), chars_til_codeword_decoded);
}

////////////////////////////////////////////////////////////

int main() {
  std::cout << "Testing LZW for max codeword size of  " << CODEWORD_SIZE
            << std::endl;
  encode_tests();
  decode_tests();
}
