#pragma once
#include "../bitio/bitio.hh"
#include <iostream>

class Four_To_One {
public:
  Four_To_One() = default;
  ~Four_To_One() = default;

  void encode(const char *input_file, uint64_t file_size, std::ostream &output);
  void decode(const char *input, std::ostream &output);
};
