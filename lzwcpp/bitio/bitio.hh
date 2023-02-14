/*
 * A pair of simple classes tu perform stream I/O on individual bits.
 */
#pragma once
#include <iostream>


// BitInput: Read a single bit at a time from an input stream.
// Before reading any bits, ensure your input stream still has valid inputs.
class BitInput {
 public:
  // Construct with an input stream
  BitInput(const char* input);

  BitInput(const BitInput&) = default;
  BitInput(BitInput&&) = default;

  // bool eof();
  // Read a single bit (or trailing zero)
  // Allowed to crash or throw an exception if called past end-of-file.
  bool input_bit();

  int read_n_bits(int n);
  private:
    const char* input_stream;
	int is_index;
    int index;
    int buffer;
 
};

// BitOutput: Write a single bit at a time to an output stream
// Make sure all bits are written out by the time the destructor is done.
class BitOutput {
 public:
  // Construct with an input stream
  BitOutput(std::ostream& os);

  // Flushes out any remaining output bits and trailing zeros, if any:
  ~BitOutput();

  BitOutput(const BitOutput&) = default;
  BitOutput(BitOutput&&) = default;

  // Output a single bit (buffered)
  void output_bit(bool bit);

  void output_n_bits(int bits, int n);

  private:
     std::ostream& output_stream;
     int index;
     char buffer;
};

