#include "lzw.hh"
#include "../dictionary/direct_mapped_dict.hh"
#include <climits>
#include <fstream>
#include <iostream>
#include <string>

std::array<int, 1 << CHAR_BIT> encode_values;
std::array<int, 1 << CHAR_BIT> decode_values;
void LZW::encode(const char *input_file, uint64_t file_size,
                 std::ostream &output) {

  encode_values['A'] = 0;
  encode_values['C'] = 1;
  encode_values['T'] = 2;
  encode_values['G'] = 3;

  // initialize starter dictionary
  LZW_Encode_Dictionary dictionary;
  dictionary.load_starting_dictionary();

  BitOutput bit_output(output);

  // track our codewords with a helper
  Codeword_Helper codeword_helper;
  codeword_type codeword = codeword_helper.get_next_codeword();

  char next_character;
  codeword_type codeword_to_output;
  const char *end_of_input = input_file + file_size;

  Next_Longest_Run longest_run;
  while (true) {

    // find longest run of chars already in our dictionary
    longest_run = dictionary.find_longest_in_dict(input_file, end_of_input);

    // make sure we haven't passed EOF
    if ((input_file + longest_run.next_run_length) >= end_of_input) {
      break;
    }

    // output codeword
    bit_output.output_n_bits(longest_run.codeword_of_next_run,
                             codeword_helper.bits_per_codeword);

    // output next character
    next_character = input_file[longest_run.next_run_length];
    bit_output.output_n_bits(encode_values[next_character], 2);

    // add the run we saw + the new character to our dict
    dictionary.add_string(input_file, longest_run, codeword);

    codeword = codeword_helper.get_next_codeword();
    input_file += longest_run.next_run_length + 1;
    longest_run.next_run_length = 0;
  }
  // output special eof character
  bit_output.output_n_bits(codeword_helper.EOF_CODEWORD,
                           codeword_helper.bits_per_codeword);

  // after we've encoded, we either have
  // no current block (case 0)
  // we have a current block that is a single character (case 1)
  // otherwise we have a current block > 1 byte (default)
  switch (longest_run.next_run_length) {
  case 0:
    bit_output.output_bit(false);
    bit_output.output_bit(false);
    break;
  case 1:
    bit_output.output_bit(false);
    bit_output.output_bit(true);
    bit_output.output_n_bits(encode_values[input_file[0]], 2);
    break;
  default:
    bit_output.output_bit(true);
    bit_output.output_bit(true);

    codeword_to_output =
        dictionary.code_of(input_file, longest_run.next_run_length);
    bit_output.output_n_bits(codeword_to_output,
                             codeword_helper.bits_per_codeword);
    break;
  }
}

void LZW::decode(const char *input, std::ostream &output) {

  decode_values[0] = 'A';
  decode_values[1] = 'C';
  decode_values[2] = 'T';
  decode_values[3] = 'G';
  // load starting dictionary
  LZW_Decode_Dictionary dictionary;
  dictionary.load_starting_dictionary();

  // track our codewords with a helper
  Codeword_Helper codeword_helper;
  codeword_type codeword = codeword_helper.get_next_codeword();

  int codeword_found;
  char next_byte;
  BitInput bit_input(input);
  // assume the file isn't empty
  // read a codeword
  codeword_found = bit_input.read_n_bits(codeword_helper.bits_per_codeword);
  while (codeword_found != codeword_helper.EOF_CODEWORD) {

    // read the next character
    next_byte = char(decode_values[bit_input.read_n_bits(2)]);

    // look up the codeword in the dictionary and add on next character
    std::string decodedCodeword = dictionary.str_of(codeword_found);
    std::string new_string = decodedCodeword + next_byte;

    // output the new string
    output << new_string;

    // add this new string to our dictionary
    dictionary.add_string(new_string, codeword);
    codeword = codeword_helper.get_next_codeword();
    codeword_found = bit_input.read_n_bits(codeword_helper.bits_per_codeword);
  }

  // the two bits after eof are a code for the last portion of the encoded file
  // either there is an extra codeword(1), extra byte (3), or nothing left (0)
  int whats_left = bit_input.read_n_bits(2);
  switch (whats_left) {
  case 1:
    output << char(decode_values[bit_input.read_n_bits(2)]);
    break;
  case 3:
    int last_codeword =
        bit_input.read_n_bits(codeword_helper.bits_per_codeword);
    auto decoded_codeword = dictionary.str_of(last_codeword);
    output << decoded_codeword;
    break;
  }
}
