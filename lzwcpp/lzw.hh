#pragma once
#include "bitio/bitio.hh"
#include <string>

/**
 * A compressor that uses LZW encoding
 */
class LZW {
public:
  LZW() = default;
  ~LZW() = default;

  /**
   * Encodes an input stream using LZW
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
