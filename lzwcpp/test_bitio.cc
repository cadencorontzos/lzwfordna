/*
 * Unit tests for class BitIO
 */

#include "bitio.hh"
#include <cassert>
#include <sstream>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////
void
test_1_bit()
{
  std::stringstream bits;

  {
    BitOutput bito(bits);
    bito.output_bit(1);
  } // Need to call bito's destructor here

  BitInput biti(bits);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);  // Should just be a trailing zero
}


//////////////////////////////////////////////////////////////////////////////
void
test_4_bits()
{
  std::stringstream bits;

  {
    BitOutput bito(bits);
    bito.output_bit(0);
    bito.output_bit(1);
    bito.output_bit(0);
    bito.output_bit(1);
  }

  BitInput biti(bits);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);  // Should just be a trailing zero
}

//////////////////////////////////////////////////////////////////////////////
void
test_8_bits()
{
  std::stringstream bits0, bits1;

  {
    BitOutput bito0(bits0);
    BitOutput bito1(bits1);
    for (int i = 0; i < 8; ++i) {
      bito0.output_bit(0);
    }
    for (int i = 0; i < 8; ++i) {
      bito1.output_bit(1);
    }
  }

  BitInput biti0(bits0);
  BitInput biti1(bits1);
  for (int i = 0; i < 8; ++i) {
    assert(!biti0.input_bit());
  }
  biti0.input_bit();
  assert(bits0.eof()); // Should be no trailing zeros this time!

  for (int i = 0; i < 8; ++i) {
    assert(biti1.input_bit());
  }
}

//////////////////////////////////////////////////////////////////////////////
void
test_9_bits()
{
  std::stringstream bits0, bits1;

  {
    BitOutput bito1(bits1);
    for (int i = 0; i < 9; ++i) {
      bito1.output_bit(1);
    }
  }

  BitInput biti1(bits1);
  for (int i = 0; i < 9; ++i) {
    assert(biti1.input_bit());
  }
  assert(!biti1.input_bit());
}

//////////////////////////////////////////////////////////////////////////////
void
test_16_bits()
{
  std::stringstream bits;

  {
    BitOutput bito(bits);
    for (int i = 0; i < 16; ++i) {
      bito.output_bit(i % 2);
    }
  }

  BitInput biti(bits);
  for (int i = 0; i < 16; ++i) {
    assert(biti.input_bit() == i % 2);
  }

  biti.input_bit();
  assert(bits.eof()); // Should be no trailing zeros this time!
}

//////////////////////////////////////////////////////////////////////////////
void
test_100_bits()
{
  std::stringstream bits;
  {
    BitOutput bito(bits);
    for (int i = 0; i < 100; ++i) {
      bito.output_bit(!(i % 2));
    }
  }

  BitInput biti(bits);
  for (int i = 0; i < 100; ++i) {
    assert(biti.input_bit() != i % 2);
  }
  assert(!biti.input_bit());
}


//////////////////////////////////////////////////////////////////////////////
int
main() {
  test_1_bit();
  test_4_bits();
  test_8_bits();
  test_9_bits();
  test_16_bits();
  test_100_bits();
  std::cout<< "tests passed !" << std::endl;
  return 0;
}
