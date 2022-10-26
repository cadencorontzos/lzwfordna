#pragma once
#include <string>
#include "bitio.hh"

class LZW {

  private:

    void output_n_bits(int bits, int n , BitOutput& bit_output);
    int read_n_bits(int n, BitInput& bit_input);

  public:

    LZW();
    ~LZW();

    void encode(std::ifstream& input, std::ofstream& output);
    void decode(std::ifstream& input, std::ofstream& output);

};
