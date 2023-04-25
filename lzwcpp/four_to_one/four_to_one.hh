#pragma once
#include "../bitio/bitio.hh"
#include <iostream>

/**
 * A compressor that converts DNA neucleotides to two bit encodings
 */
class Four_To_One {
public:
  Four_To_One() = default;
  ~Four_To_One() = default;

  /**
   * Encodes an input stream by converting chars to 2 bits
   * Assumes input only consists of A, C, T, or G
   * @param input_file pointer to input stream
   * @param file_size length of input stream
   * @param output where to put encoded chars
   */
  void encode(const char *input_file, uint64_t file_size, std::ostream &output);

  /**
   * Decodes an input stream using LZW
   * Assumes that input was comressed using this same class
   * @param input_file pointer to input stream
   * @param output where to put decoded chars
   */
  void decode(const char *input, std::ostream &output);
};
