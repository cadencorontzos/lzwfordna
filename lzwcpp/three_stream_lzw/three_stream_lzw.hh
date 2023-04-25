#pragma once
#include "../bitio/bitio.hh"
#include <string>

/**
 * A compressor that uses modified LZW encoding
 *
 * The compressor compresses into three streams:
 * a character stream,
 * a codewords stream,
 * and an indicator bit stream.
 */
class Three_Stream_LZW {
public:
  Three_Stream_LZW() = default;
  ~Three_Stream_LZW() = default;

  /**
   * Encodes an input stream using LZW
   *
   * The indicator bits tell the deoder whether to
   * - read a codeword and decode it
   * - read the next 8 characters
   *
   * @param input_file pointer to input stream
   * @param file_size length of input stream
   * @param codeword_output where to put codewords
   * @param char_output where to put encoded characters
   * @param indicator_output where to put indicator bits
   */
  void encode(const char *input_file, uint64_t file_size,
              std::ostream &codeword_output, std::ostream &char_output,
              std::ostream &indicator_output);

  /**
   * Decodes an input stream using LZW
   * Assumes that inputs were comressed using this same class
   * @param char_input pointer to encoded char input stream
   * @param codeword_input pointer to codeword stream
   * @param indicator_input pointer to indicator input stream
   * @param output where to put decoded chars
   */
  void decode(const char *char_input, const char *codeword_input,
              const char *indicator_input, std::ostream &output);
};
