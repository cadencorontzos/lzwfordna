#pragma once
#include "../bitio/bitio.hh"
#include <string>

class Three_Stream_LZW {
public:
  Three_Stream_LZW() = default;
  ~Three_Stream_LZW() = default;

  void encode(const char *input_file, uint64_t file_size,
              std::ostream &codeword_output, std::ostream &char_output,
              std::ostream &indicator_output);
  void decode(const char *char_input, const char *codeword_input,
              const char *indicator_input, std::ostream &output);
};
