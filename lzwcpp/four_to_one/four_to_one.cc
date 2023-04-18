#include "four_to_one.hh"
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <x86intrin.h>

const uint64_t MASK = 434041037028460038;
void Four_To_One::encode(const char *input_file, uint64_t file_size,
                         std::ostream &output) {

  /* const char *end_of_input = input_file + file_size; */
  output.write(reinterpret_cast<const char *>(&file_size), sizeof(file_size));

  const uint64_t *input_ints = reinterpret_cast<const uint64_t *>(input_file);
  const uint64_t *end_of_input =
      input_ints + static_cast<uint64_t>(std::ceil(file_size / 8));
  while (input_ints + 1 <= end_of_input) {
    uint64_t first_half = _pext_u64(input_ints[0], MASK);
    output << uint8_t(first_half) << std::endl;
    input_ints += 1;
  }
}

void Four_To_One::decode(const char *input, std::ostream &output) {

  uint64_t original_file_size = 0;
  for (int i = 0; i < 8; i++) {
    original_file_size += (input[i] << (8 * i));
  }
  std::cout << "ogfs :" << original_file_size << std::endl;
  output << input[0];
  // the two bits after eof are a code for the last portion of the encoded file
  // either there is an extra codeword(1), extra byte (3), or nothing left (0)
  /* int whats_left = bit_input.read_n_bits(2); */
  /* switch (whats_left) { */
  /* case 1: */
  /*   output << char(decode_values[bit_input.read_n_bits(2)]); */
  /*   break; */
  /* case 3: */
  /*   int last_codeword = */
  /*       bit_input.read_n_bits(codeword_helper.bits_per_codeword); */
  /*   auto decoded_codeword = dictionary.str_of(last_codeword); */
  /*   output << decoded_codeword; */
  /*   break; */
  /* } */
}
