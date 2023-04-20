#include <chrono>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "entropy-encoding/include/turborc.h"
#include "three_stream_lzw/three_stream_lzw.hh"
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr
        << "Please include the name of the file you would like to compress"
        << std::endl;
    return 1;
  }

  std::string indicator_input_filename = std::string(argv[1]) + ".indicator";
  // indicator
  std::string indicator_second_output_filename =
      std::string(argv[1]) + ".indicator.rc";
  {

    std::vector<char *> command2 = {
        "./turborc", "-d",
        const_cast<char *>(indicator_second_output_filename.c_str()),
        const_cast<char *>(indicator_input_filename.c_str())};
    entropy_encoder(4, command2.data(), 1, 1);
  }

  // cws
  std::string codeword_input_filename = std::string(argv[1]) + ".codeword";
  std::string codeword_second_output_filename =
      std::string(argv[1]) + ".codeword.rc";
  {
    std::vector<char *> command = {
        "./turborc", "-d",
        const_cast<char *>(codeword_second_output_filename.c_str()),
        const_cast<char *>(codeword_input_filename.c_str())};
    entropy_encoder(4, command.data(), 1, 1);
  }

  std::string chars_input_filename = std::string(argv[1]) + ".chars";
  int char_input_file =
      open(chars_input_filename.c_str(), O_RDONLY, (mode_t)0600);
  if (char_input_file == EOF) {
    std::cout << "Unable to open " << chars_input_filename << "." << std::endl;
    return 1;
  }

  struct stat char_fileinfo;
  fstat(char_input_file, &char_fileinfo);
  const char *char_input = static_cast<char *>(mmap(
      0, char_fileinfo.st_size, PROT_READ, MAP_SHARED, char_input_file, 0));

  if (char_input == MAP_FAILED) {
    std::cout << "MMap error with file " << chars_input_filename << "."
              << std::endl;
    return 1;
  }

  if (char_fileinfo.st_size == 0) {
    std::cout << "File" << argv[1] << " is empty." << std::endl;
    return 1;
  }

  int indicator_input_file =
      open(indicator_input_filename.c_str(), O_RDONLY, (mode_t)0600);
  if (indicator_input_file == EOF) {
    std::cout << "Unable to open " << indicator_input_filename << "."
              << std::endl;
    return 1;
  }

  struct stat indicator_fileinfo;
  fstat(indicator_input_file, &indicator_fileinfo);
  const char *indicator_input =
      static_cast<char *>(mmap(0, indicator_fileinfo.st_size, PROT_READ,
                               MAP_SHARED, indicator_input_file, 0));

  if (indicator_input == MAP_FAILED) {
    std::cout << "MMap error with file " << indicator_input_filename << "."
              << std::endl;
    return 1;
  }

  if (indicator_fileinfo.st_size == 0) {
    std::cout << "File" << argv[1] << " is empty." << std::endl;
    return 1;
  }

  // codewords
  //
  //
  int codeword_input_file =
      open(codeword_input_filename.c_str(), O_RDONLY, (mode_t)0600);
  if (codeword_input_file == EOF) {
    std::cout << "Unable to open " << codeword_input_filename << "."
              << std::endl;
    return 1;
  }

  struct stat codeword_fileinfo;
  fstat(codeword_input_file, &codeword_fileinfo);
  const char *codeword_input =
      static_cast<char *>(mmap(0, codeword_fileinfo.st_size, PROT_READ,
                               MAP_SHARED, codeword_input_file, 0));

  if (codeword_input == MAP_FAILED) {
    std::cout << "MMap error with file " << codeword_input_filename << "."
              << std::endl;
    return 1;
  }

  if (codeword_fileinfo.st_size == 0) {
    std::cout << "File" << argv[1] << " is empty." << std::endl;
    return 1;
  }

  LZW decompressor;
  std::ofstream output;

  output.open(std::string(argv[1]) + ".decompressed.lzw", std::ios::binary);

  // decompress file
  auto start_time = std::chrono::high_resolution_clock::now();
  decompressor.decode(char_input, codeword_input, indicator_input, output);
  auto end_time = std::chrono::high_resolution_clock::now();

  output.close();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  // output statistics
  fs::path out = std::string(argv[1]) + ".decompressed.lzw";
  auto output_size = fs::file_size(out);
  std::cout << "Decompressed file size (bytes) : " << output_size << std::endl;
  std::cout << "Decompression Time (ms) : " << duration.count() << std::endl;
  return 0;
}
