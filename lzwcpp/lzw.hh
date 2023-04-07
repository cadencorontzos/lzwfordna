#pragma once
#include "bitio/bitio.hh"
#include <string>

class LZW {
public:
  LZW() = default;
  ~LZW() = default;

  void encode(const char *input_file, uint64_t file_size, std::ostream &output,
              std::ostream &char_output, std::ostream &output_runlengths);
  void decode(const char *input, std::ostream &output);
};
