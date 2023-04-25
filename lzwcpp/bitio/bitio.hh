/*
 * A pair of simple classes tu perform stream I/O on individual bits.
 */
#pragma once
#include <iostream>

/**
 * BitInput: Read a single bit at a time from an input stream.
 * Before reading any bits, ensure your input stream still has valid inputs.
 */
class BitInput {
public:
  BitInput(const char *input);
  BitInput(const BitInput &) = default;
  BitInput(BitInput &&) = default;
  bool input_bit();
  int read_n_bits(int n);

private:
  const char *input_stream;
  int is_index;
  int index;
  int buffer;
};

/**
 * BitOutput: Write a single bit at a time to an output stream.
 * Makes sure all bits are written out by the time the destructor is done.
 */
class BitOutput {
public:
  BitOutput(std::ostream &os);
  ~BitOutput();
  BitOutput(const BitOutput &) = default;
  BitOutput(BitOutput &&) = default;
  void output_bit(bool bit);
  void output_n_bits(int bits, int n);

private:
  std::ostream &output_stream;
  int index;
  char buffer;
};
