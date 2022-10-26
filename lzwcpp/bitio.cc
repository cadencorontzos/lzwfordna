#include <string>
#include "bitio.hh"
#include <iostream>
#include <climits>

BitInput::BitInput(std::istream& is): input_stream(is), index(CHAR_BIT), buffer(0) {}

bool BitInput::input_bit(){

    if(index == CHAR_BIT){
        index = 0;
        char b;
        input_stream.get(b);
        buffer = int(b) ;
    }
    return (buffer>>index++) & 1;
}


BitOutput::BitOutput(std::ostream& os): output_stream(os), index(0), buffer(0) {}


BitOutput::~BitOutput(){
    if(output_stream){
        if(index>0){
            output_stream.put(buffer);
        }
    }
 }

void BitOutput::output_bit(bool bit){

    if (index == 0){
        buffer = 0;
    }

    buffer |= (bit<<index++);\
    
    if (index == CHAR_BIT){
        output_stream.put(buffer);
        index = 0;
    }
}

