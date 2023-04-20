#include <cassert>
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::filesystem;
#include "three_stream_lzw.hh"

#include "entropy-encoding/include/turborc.h"
#include <string>
#include <vector>

float compressionRatio(uint64_t output_size, uint64_t input_size) {
  assert(output_size != 0);
  return (float)input_size / output_size;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr
        << "Please include the name of the file you would like to compress\n";
    return 1;
  }

  int input_file = open(argv[1], O_RDONLY, (mode_t)0600);
  if (input_file == EOF) {
    std::cout << "Unable to open " << argv[1] << "." << std::endl;
    return 1;
  }

  struct stat fileInfo;
  fstat(input_file, &fileInfo);
  const char *input = static_cast<char *>(
      mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, input_file, 0));

  if (input == MAP_FAILED) {
    std::cout << "MMap error with file " << argv[1] << "." << std::endl;
    return 1;
  }

  if (fileInfo.st_size == 0) {
    std::cout << "File" << argv[1]
              << " is empty. That's about as small as it will get."
              << std::endl;
    return 1;
  }

  std::cout << "Original File size (bytes) : " << fileInfo.st_size << std::endl;

  LZW compressor;
  std::ofstream output;
  std::string codeword_output_filename =
      std::string(argv[1]) + ".tmp.compressed.lzw.codeword";
  output.open(codeword_output_filename, std::ios::binary);

  std::ofstream rl_output;
  std::string indicator_output_filename =
      std::string(argv[1]) + ".tmp.compressed.lzw.indicator";
  rl_output.open(indicator_output_filename, std::ios::binary);
  std::ofstream char_output;
  std::string char_output_filename =
      std::string(argv[1]) + ".compressed.lzw.chars";
  char_output.open(char_output_filename, std::ios::binary);
  // compress file
  auto start_time = std::chrono::high_resolution_clock::now();
  compressor.encode(input, fileInfo.st_size, output, char_output, rl_output);
  auto end_time = std::chrono::high_resolution_clock::now();

  output.close();
  char_output.close();
  rl_output.close();

  // compress the indicator bits with entropy
  std::string indicator_second_output_filename =
      std::string(argv[1]) + ".compressed.lzw.indicator";
  {
    std::vector<char *> command2 = {
        "./turborc", "-1", "-f",
        const_cast<char *>(indicator_output_filename.c_str()),
        const_cast<char *>(indicator_second_output_filename.c_str())};
    entropy_encoder(5, command2.data(), 10, 0);
  }

  // compress codewords with entropy
  std::string codeword_second_output_filename =
      std::string(argv[1]) + ".compressed.lzw.codeword";
  {
    std::vector<char *> command = {
        "./turborc", "-20", "-Fs",
        const_cast<char *>(codeword_output_filename.c_str()),
        const_cast<char *>(codeword_second_output_filename.c_str())};
    entropy_encoder(5, command.data(), 20, 0);
  }

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  // output statistics
  auto output_size = fs::file_size(codeword_second_output_filename + ".rc") +
                     fs::file_size(indicator_second_output_filename + ".rc") +
                     fs::file_size(char_output_filename);
  std::cout << "Compressed file size (bytes) : " << output_size << std::endl;
  std::cout << "Compression Ratio : "
            << compressionRatio(output_size, fileInfo.st_size) << std::endl;
  std::cout << "Compression Time (ms) : " << duration.count() << std::endl;
  return 0;
}
