#include <string>
#include "bitio.hh"
#include <iostream>

const int CHARACTER_SIZE =8;
BitInput::BitInput(std::istream& is)
{
    input_stream = &is;
    index = 0;
    buffer = 0;
};


bool BitInput::input_bit(){

    if(index == CHARACTER_SIZE || index == 0){
        index = 0;
        (*input_stream).get(buffer);
    }

    return (buffer>>index++) & 1;
}


BitOutput::BitOutput(std::ostream& os)
{
    output_stream = &os;
    index = 0;
    buffer = 0;
};


BitOutput::~BitOutput(){
    if(output_stream){
        if(index>0){
            (*output_stream).put(buffer);
        }
    }
 };

// Output a single bit (buffered)
void BitOutput::output_bit(bool bit){

    if (index == 0){
        buffer = 0;
    }


    switch (bit){
        case true:{ 
            buffer |= (1<<index);
            [[fallthrough]];
        }
        case false: ++index;
    }

    // if a byte is complete, output it to the stream

    if (index == CHARACTER_SIZE){
        (*output_stream).put(buffer);
        index = 0;
    }

}

