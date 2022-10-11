#include <iostream>
#include <map>
#include <string>
#include "lzw.hh"
#include "bitio.hh"

using namespace std;


LZW::LZW() = default;


LZW::~LZW() = default;

CompressionStatistics* LZW::encode(std::string filename, std::string output_filename){
    CompressionStatistics* stats = new CompressionStatistics {true, 0.0, 0.0, 1, 1} ;
    return stats;
}

    // Decode an input stream, and write to output stream
CompressionStatistics* LZW::decode(std::string filename, std::string output_filename){
    CompressionStatistics* stats = new CompressionStatistics {true, 0.0, 0.0, 1, 1} ;
    return stats;
}

int main(){

   
   map<string, float> bakery;
   bakery["Choclatecake"] = 585.5;
   bakery["Strawberrycake"] = 579;
   bakery["Mangocake"] = 345.6;
   bakery["Vanillacake"] = 650;
   bakery["MixedFruitcake"] = 539.9;

    map <string,float> :: iterator iter;

   cout<<"keys"<<"  &  "<<"values"<<endl;

   for (iter = bakery.begin(); iter != bakery.end(); iter++)
   {
     cout<<(*iter).first<<"    "<<(*iter).second<<"\n";
   }
    for(auto i = 0; i < 256; i++){
        std::cout << std::to_string(i) << std::endl;
    }

    return 0;
}