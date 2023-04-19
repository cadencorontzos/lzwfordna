#include "four_to_one.hh"
#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <x86intrin.h>

std::array<int, 1 << CHAR_BIT> encode_values;
const uint64_t MASK = 434041037028460038;
void Four_To_One::encode(const char *input_file, uint64_t file_size,
                         std::ostream &output) {

  encode_values['A'] = 0;
  encode_values['C'] = 1;
  encode_values['T'] = 2;
  encode_values['G'] = 3;
  // write out the size of the file
  output.write(reinterpret_cast<const char *>(&file_size), sizeof(file_size));
  const char *end_of_input = input_file + file_size;
  // calcualate end of file
  const uint64_t *input_ints = reinterpret_cast<const uint64_t *>(input_file);
  const uint64_t *end_of_input_64 =
      input_ints + static_cast<uint64_t>(std::floor(file_size / 8));

  unsigned output_num = 0;
  // output the file in 2 byte chunks using pext
  while (input_ints < end_of_input_64 && output_num + 8 <= file_size) {
    uint64_t first_half = _pext_u64(input_ints[0], MASK);
    output.write(reinterpret_cast<const char *>(&first_half), 2);
    input_ints += 1;
    input_file += 8;
    output_num += 8;
  }

  const uint32_t *input_32 = reinterpret_cast<const uint32_t *>(input_file);
  const uint32_t *end_of_input_32 =
      input_32 + static_cast<uint32_t>(std::floor(file_size / 4));
  if (input_32 + 1 < end_of_input_32 && output_num + 4 <= file_size) {
    uint32_t next_chunk = _pext_u32(input_32[0], (MASK >> 32));
    output.write(reinterpret_cast<const char *>(&next_chunk), 1);
    input_32 += 1;
    input_file += 4;
    output_num += 4;
  }

  BitOutput bit_output(output);
  while (input_file < end_of_input) {
    bit_output.output_n_bits(encode_values[input_file[0]], 2);
    input_file++;
    output_num++;
  }
}

std::array<int, 1 << CHAR_BIT> decode_values;
std::string index_to_string(uint64_t index) {

  decode_values[0] = 'A';
  decode_values[1] = 'C';
  decode_values[2] = 'T';
  decode_values[3] = 'G';
  std::string result = "";
  for (unsigned i = 0; i <= (sizeof(index) * CHAR_BIT - 2); i = i + 2) {
    result += char(decode_values[(index >> i) & 3]);
  }
  return result;
}

void Four_To_One::decode(const char *input, std::ostream &output) {

  decode_values[0] = 'A';
  decode_values[1] = 'C';
  decode_values[2] = 'T';
  decode_values[3] = 'G';

  // extract original file size
  uint64_t original_file_size = 0;
  for (int i = 0; i < 8; i++) {
    original_file_size += (uint8_t(input[i]) << (8 * i));
  }
  input += 8;

  uint64_t bytes_output = 0;
  const char *end_of_input = input + original_file_size;
  const uint32_t *input_file = reinterpret_cast<const uint32_t *>(input);
  const uint32_t *end_of_input_32 =
      input_file + int(std::ceil(original_file_size / 16));

  while (input_file < end_of_input_32 and
         bytes_output + 16 <= original_file_size) {
    output << index_to_string(input_file[0]).substr(0, 16);
    input_file++;
    input += 4;
    bytes_output += 16;
  }

  while (input < end_of_input and bytes_output + 4 <= original_file_size) {
    output << index_to_string(input[0]).substr(0, 4);
    input++;
    bytes_output += 4;
  }

  // output the last few characters
  BitInput bit_input(input);
  while (bytes_output < original_file_size) {
    char f = char(decode_values[bit_input.read_n_bits(2)]);
    output << f;
    bytes_output++;
  }
}
