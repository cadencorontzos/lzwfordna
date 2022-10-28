/*
 * Unit tests for class BitIO
 */

#include "bitio.hh"
#include <cassert>
#include <sstream>
#include <iostream>
#include <climits>

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
  // std::cout << "hered" ;
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

  // std::cout << "hereagain";
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

void test_many_zeros()
{
  std::stringstream bits;
  {
    BitOutput bito(bits);
    for (int i = 0; i < 100; ++i) {
      bito.output_bit(false);
    }
  }

  BitInput biti(bits);
  for (int i = 0; i < 100; ++i) {
    assert(biti.input_bit() == false);
  }
  assert(!biti.input_bit());
}

//////////////////////////////////////////////////////////////////////////////
// unit tests for output_n_bits
void test_output_char(){
  std::stringstream bits;

  {
    BitOutput bito(bits);

    // the ascii code for 'A' is 65
    bito.output_n_bits(65, CHAR_BIT);
  }

  // we expect to find 01000001 with no trailing zeros
  BitInput biti(bits);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  for (int i = 0; i < 5; ++i) {
    assert(biti.input_bit() == false);
  }
  assert(biti.input_bit() == true);

  assert(!biti.input_bit());
}

void test_output_int(){
  std::stringstream bits;

  { 
    BitOutput bito(bits);

    // say we have an integer that doesn't fit in a byte, like 420
    bito.output_n_bits(420, CHAR_BIT+1);
  }

  // we expect to find 110100100 with seven trailing zeros
  BitInput biti(bits);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == false);
  for (int i = 0; i < 7; ++i) {
    assert(biti.input_bit() == false);
  }
  assert(!biti.input_bit());
}

void test_output_char_and_int(){
  std::stringstream bits;
  
  {
    BitOutput bito(bits);

    // lets mix the previous two cases
    bito.output_n_bits(65, CHAR_BIT);
    bito.output_n_bits(420, CHAR_BIT+1);
    
  }

  // we expect to find 110100100, then 01000001  with seven trailing zeros
  BitInput biti(bits);

  //01000001
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  for (int i = 0; i < 5; ++i) {
    assert(biti.input_bit() == false);
  }
  assert(biti.input_bit() == true);

  //110100100
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == true);
  assert(biti.input_bit() == false);
  assert(biti.input_bit() == false);

  // 7 trailing 0s
  assert(!biti.input_bit());
  for (int i = 0; i < 7; ++i) {
    assert(biti.input_bit() == false);
  }
  assert(!biti.input_bit());
}

void test_output_n_bits(){
  test_output_char();
  test_output_int();
  test_output_char_and_int();
}

//////////////////////////////////////////////////////////////////////////////
// unit tests for read_n_bits

void test_read_byte(){

  std::stringstream bits;

  {
    BitOutput bito(bits);

    // the ascii code for 'A' is 65
    bito.output_n_bits(65, CHAR_BIT);
  }

  BitInput biti(bits);
  char byte_found = (char) biti.read_n_bits(CHAR_BIT);
  assert(byte_found =='A');
  assert(!biti.input_bit());
}

void test_read_int(){
  std::stringstream bits;

  {
    BitOutput bito(bits);

    // the ascii code for 'A' is 65
    bito.output_n_bits(420, CHAR_BIT+1);
  }

  BitInput biti(bits);
  int int_found = biti.read_n_bits(CHAR_BIT+1);
  assert(int_found == 420);
  assert(!biti.input_bit());
}

void test_read_byte_and_int(){
  std::stringstream bits;

  {
    BitOutput bito(bits);

    bito.output_n_bits(65, CHAR_BIT);
    bito.output_n_bits(420, CHAR_BIT+1);
  }

  BitInput biti(bits);

  char byte_found = (char) biti.read_n_bits(CHAR_BIT);
  assert(byte_found =='A');

  int int_found = biti.read_n_bits(CHAR_BIT+1);
  assert(int_found == 420);

  assert(!biti.input_bit());
}

void test_read_n_bits(){
  test_read_byte();
  test_read_int();
  test_read_byte_and_int();
}

//////////////////////////////////////////////////////////////////////////////

int main() {
  test_1_bit();
  test_4_bits();
  test_8_bits();
  test_9_bits();
  test_16_bits();
  test_100_bits();
  test_many_zeros();

  test_output_n_bits();
  test_read_n_bits();

  std::cout<< "tests passed !" << std::endl;
  return 0;
}
