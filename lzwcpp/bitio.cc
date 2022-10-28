#include <string>
#include "bitio.hh"
#include <iostream>
#include <climits>

BitInput::BitInput(std::istream& is): input_stream(is), index(-1), buffer(0) {}

bool BitInput::input_bit(){

    if(index == -1){
        index = CHAR_BIT-1;
        char b;
        input_stream.get(b);
        buffer = int(b) ;
    }
    return (buffer>>index--) & 1;
}

int BitInput::read_n_bits(int n){
    int b = 0;
    for (int i= n-1; i >=0; i--){
        b |= (input_bit()<<i);
    }
    return b;
}

BitOutput::BitOutput(std::ostream& os): output_stream(os), index(CHAR_BIT-1), buffer(0) {}


BitOutput::~BitOutput(){
    if(output_stream){
        if(index<CHAR_BIT-1){
            output_stream.put(buffer);
        }
    }
 }

void BitOutput::output_bit(bool bit){

    if (index == CHAR_BIT-1){
        buffer = 0;
    }

    buffer |= (bit<<index--);

    if (index == -1){
        output_stream.put(buffer);
        index = CHAR_BIT-1;
    }
}

void BitOutput::output_n_bits(int bits, int n){
    for (int i = n-1; i>=0; --i){
        output_bit((bits>>i)&1);
    }
}
