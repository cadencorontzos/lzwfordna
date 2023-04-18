#include "four_to_one.hh"
#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <x86intrin.h>

const uint64_t MASK = 434041037028460038;
void Four_To_One::encode(const char *input_file, uint64_t file_size,
                         std::ostream &output) {

  // write out the size of the file
  output.write(reinterpret_cast<const char *>(&file_size), sizeof(file_size));

  // calcualate end of file
  const uint64_t *input_ints = reinterpret_cast<const uint64_t *>(input_file);
  const uint64_t *end_of_input =
      input_ints + static_cast<uint64_t>(std::ceil(file_size / 8));

  // output the file in 2 byte chunks using pext
  while (input_ints + 1 <= end_of_input) {
    uint64_t first_half = _pext_u64(input_ints[0], MASK);
    output.write(reinterpret_cast<const char *>(&first_half), 2);
    input_ints += 1;
    input_file += 8;
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

  // extract original file size
  uint64_t original_file_size = 0;
  for (int i = 0; i < 8; i++) {
    original_file_size += (uint8_t(input[i]) << (8 * i));
  }
  input += 8;

  const uint64_t *input_file = reinterpret_cast<const uint64_t *>(input);
  const uint64_t *end_of_input =
      input_file + int(std::ceil(original_file_size / 32));
  /* uint64_t chars_seen = 0; */
  std::cout << index_to_string(input_file[0]) << std::endl;
  std::cout << index_to_string(2) << std::endl;

  while (input_file + 1 < end_of_input) {
    output << index_to_string(input_file[0]);
    input_file++;
    input += 1;
  }
}
