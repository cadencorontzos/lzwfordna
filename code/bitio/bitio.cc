#include "bitio.hh"
#include <climits>
#include <iostream>
#include <string>

BitInput::BitInput(const char *input)
    : input_stream(input), is_index(0), index(-1), buffer(0) {}

/// reads a single bit from the input
///
bool BitInput::input_bit() {
  // we are outputting bits right to left, so if we've gone past the right side
  // of the byte, we've writtentthe whole byte output byte and reset index
  if (index == -1) {
    index = CHAR_BIT - 1;
    char b;
    b = input_stream[is_index];
    is_index += 1;
    buffer = int(b);
  }

  // now return the bit of the current index, then decrease index
  return (buffer >> index--) & 1;
}

/// returns an int which represents the next n bits
/// @param n number of bits to read
int BitInput::read_n_bits(int n) {
  int b = 0;
  for (int i = n - 1; i >= 0; i--) {
    b |= (input_bit() << i);
  }
  return b;
}

BitOutput::BitOutput(std::ostream &os)
    : output_stream(os), index(CHAR_BIT - 1), buffer(0) {}

BitOutput::~BitOutput() {
  // if there is anything in buffer, we need to output upon destruction
  if (output_stream) {
    if (index < CHAR_BIT - 1) {
      output_stream.put(buffer);
    }
  }
}

/// Writes a single bit to output
/// @param bit The bit to output
void BitOutput::output_bit(bool bit) {

  // we are going left to right, so save the bit then decrement the index
  buffer |= (bit << index--);

  // if we've been given a whole byte, output it then start a new buffer
  if (index == -1) {
    output_stream.put(buffer);
    index = CHAR_BIT - 1;
    buffer = 0;
  }
}

/// Outputs multiple bits to the output
/// @param bits a buffer which holds the bits to write
/// @param n number of bits to write from buffer
void BitOutput::output_n_bits(int bits, int n) {
  for (int i = n - 1; i >= 0; --i) {
    output_bit((bits >> i) & 1);
  }
}
