#include "three_stream_lzw.hh"
#include "../dictionary/direct_mapped_dict.hh"
#include <arpa/inet.h>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>

std::array<int, 1 << CHAR_BIT> encode_values;
std::array<int, 1 << CHAR_BIT> decode_values;
void LZW::encode(const char *input_file, uint64_t file_size,
                 std::ostream &codeword_output, std::ostream &char_output,
                 std::ostream &indicator_output) {

  encode_values['A'] = 0;
  encode_values['C'] = 1;
  encode_values['T'] = 2;
  encode_values['G'] = 3;

  // initialize starter dictionary
  LZW_Encode_Dictionary dictionary;
  dictionary.load_starting_dictionary();

  BitOutput char_bit_o(char_output);
  BitOutput indicator_bit_o(indicator_output);

  // track our codewords with a helper
  Codeword_Helper codeword_helper;
  codeword_type codeword = codeword_helper.get_next_codeword();

  char next_character;
  const char *end_of_input = input_file + file_size;

  Next_Longest_Run longest_run;
  while (true) {

    // find longest run of chars already in our dictionary
    longest_run = dictionary.find_longest_in_dict(input_file, end_of_input);

    // make sure we haven't passed EOF
    if ((input_file + longest_run.next_run_length) >= end_of_input) {
      break;
    }

    if (longest_run.next_run_length < 8) {

      if (input_file + 8 > end_of_input) {
        longest_run.next_run_length = 2;
        break;
      }
      indicator_bit_o.output_bit(false);
      for (unsigned i = 0; i < 8; i++) {

        // output next character
        next_character = input_file[i];
        char_bit_o.output_n_bits(encode_values[next_character], 2);
      }

      // add the run we saw + the new character to our dict
      dictionary.add_string(input_file, longest_run, codeword);

      input_file += 8;
    } else {
      indicator_bit_o.output_bit(true);

      // output codeword
      /* codeword_bit_o.output_n_bits(longest_run.codeword_of_next_run, */
      /*                              codeword_helper.bits_per_codeword); */
      /* std::cout << longest_run.codeword_of_next_run << std::endl; */
      codeword_type cw_to_write = htons(longest_run.codeword_of_next_run);
      codeword_output.write(reinterpret_cast<const char *>(&cw_to_write),
                            sizeof(codeword_type));

      /* << std::endl; */
      // output next character
      next_character = input_file[longest_run.next_run_length];
      char_bit_o.output_n_bits(encode_values[next_character], 2);

      // add the run we saw + the new character to our dict
      dictionary.add_string(input_file, longest_run, codeword);

      input_file += longest_run.next_run_length + 1;
    }
    codeword = codeword_helper.get_next_codeword();
    longest_run.next_run_length = 0;
  }
  indicator_bit_o.output_bit(true);
  // output special eof character
  /* std::cout << codeword_helper.EOF_CODEWORD << std::endl; */
  codeword_type codeword_to_write = htons(codeword_helper.EOF_CODEWORD);
  codeword_output.write(reinterpret_cast<const char *>(&codeword_to_write),
                        sizeof(codeword_type));

  // after we've encoded, we either have
  // no current block (case 0)
  // we have a current block that is a single character (case 1)
  // otherwise we have a current block > 1 byte (default)
  switch (longest_run.next_run_length) {
  case 0:
    char_bit_o.output_bit(false);
    char_bit_o.output_bit(false);
    break;
  case 1:
    char_bit_o.output_bit(false);
    char_bit_o.output_bit(true);
    char_bit_o.output_n_bits(encode_values[input_file[0]], 2);
    break;
  default:
    char_bit_o.output_bit(true);
    char_bit_o.output_bit(true);
    std::string rest = "";
    while (input_file < end_of_input) {
      rest += input_file[0];
      input_file++;
    }
    char_bit_o.output_n_bits(rest.length(), 3);

    for (unsigned i = 0; i < rest.length(); i++) {
      char_bit_o.output_n_bits(encode_values[rest[i]], 2);
    }
    break;
  }
}

void LZW::decode(const char *char_input, const char *codeword_input,
                 const char *indicator_input, std::ostream &output) {

  decode_values[0] = 'A';
  decode_values[1] = 'C';
  decode_values[2] = 'T';
  decode_values[3] = 'G';
  // load starting dictionary
  LZW_Decode_Dictionary dictionary;
  LZW_Encode_Dictionary encode_dictionary;
  dictionary.load_starting_dictionary();

  // track our codewords with a helper
  Codeword_Helper codeword_helper;
  codeword_type codeword = codeword_helper.get_next_codeword();

  codeword_type codeword_found;
  char next_byte;
  bool next_is_codeword = false;

  BitInput char_bit_i(char_input);
  BitInput indicator_bit_i(indicator_input);

  while (true) {

    next_is_codeword = indicator_bit_i.input_bit();

    if (next_is_codeword) {

      codeword_found =
          (uint8_t(codeword_input[0]) << 8) + (uint8_t(codeword_input[1]));
      /* std::cout << codeword_found << std::endl; */
      codeword_input += 2;
      if (codeword_found == codeword_helper.EOF_CODEWORD) {
        break;
      }
      // read the next character
      next_byte = char(decode_values[char_bit_i.read_n_bits(2)]);

      // look up the codeword in the dictionary and add on next character
      std::string decodedCodeword = dictionary.str_of(codeword_found);
      std::string new_string = std::string();
      new_string = decodedCodeword + next_byte;

      // output the new string
      output << new_string;

      // add this new string to our dictionary
      dictionary.add_string(new_string, codeword);
      encode_dictionary.add_string(new_string.c_str(), new_string.length(),
                                   codeword);
    } else {

      std::string full_string = std::string();
      for (int i = 0; i < 8; i++) {
        int next_index_value = char_bit_i.read_n_bits(2);
        next_byte = char(decode_values[next_index_value]);
        output << next_byte;
        full_string += next_byte;
      }
      Next_Longest_Run nlr = encode_dictionary.find_longest_in_dict(
          full_string.c_str(), full_string.c_str() + full_string.length());
      std::string str_to_add = full_string.substr(0, nlr.next_run_length + 2);
      dictionary.add_string(str_to_add, codeword);
      encode_dictionary.add_string(full_string.c_str(), nlr.next_run_length + 1,
                                   codeword);
    }
    codeword = codeword_helper.get_next_codeword();
  }

  // the two bits after eof are a code for the last portion of the encoded file
  // either there is an extra codeword(1), extra byte (3), or nothing left (0)
  int whats_left = char_bit_i.read_n_bits(2);
  switch (whats_left) {
  case 1:
    output << char(decode_values[char_bit_i.read_n_bits(2)]);
    break;
  case 3:

    int left = char_bit_i.read_n_bits(3);
    if (left == 0) {
      left = 8;
    }
    for (int i = 0; i < left; i++) {
      output << char(decode_values[char_bit_i.read_n_bits(2)]);
    }

    break;
  }
}
