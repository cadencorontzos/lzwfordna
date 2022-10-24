#include <string>
#include "bitio.hh"
#include <iostream>
#include <climits>

const int CHARACTER_SIZE =8;
BitInput::BitInput(std::istream& is)
: input_stream(is), index(CHARACTER_SIZE), buffer(0)
{
}


// bool BitInput::eof(){
//     return (*input_stream).eof();
// }

bool BitInput::input_bit(){
    // if((*this).eof()){
    //     throw 100;
    // }
    if(index == CHARACTER_SIZE){
        index = 0;
        char b;
        input_stream.get(b);
        // std::cout <<"b:" <<b << std::endl;
        buffer = int(b) ;
    }
    // std::cout << std::endl << "buffer : " << buffer << std::endl;
    // std::cout << "index: " << index << std::endl;
    // std::cout << "buffer << index: " << (buffer>>index) << std::endl;

    // std::cout << "bit returned :" << ((buffer>>index) &1) << std::endl;
    
    return (buffer>>index++) & 1;
}


BitOutput::BitOutput(std::ostream& os)
: output_stream(os), index(0), buffer(0)
{}


BitOutput::~BitOutput(){
    if(output_stream){
        if(index>0){
            output_stream.put(buffer);
        }
    }
 };


// Output a single bit (buffered)
void BitOutput::output_bit(bool bit){

    if (index == 0){
        buffer = 0;
    }


    buffer |= (bit<<index++);

    // if a byte is complete, output it to the stream

    if (index == CHARACTER_SIZE){
        output_stream.put(buffer);
        index = 0;
    }
}

