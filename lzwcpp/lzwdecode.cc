#include "lzw.hh"

#include <chrono>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr
        << "Please include the name of the file you would like to compress"
        << std::endl;
    return 1;
  }

  auto start_time = std::chrono::high_resolution_clock::now();
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
    std::cout << "File" << argv[1] << " is empty." << std::endl;
    return 1;
  }

  LZW decompressor;
  std::ofstream output;

  output.open(std::string(argv[1]) + ".decompressed.lzw", std::ios::binary);

  // decompress file
  decompressor.decode(input, output);
  auto end_time = std::chrono::high_resolution_clock::now();

  output.close();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
      end_time - start_time);

  // output statistics
  fs::path out = std::string(argv[1]) + ".decompressed.lzw";
  auto output_size = fs::file_size(out);
  std::cout << "Decompressed file size (bytes) : " << output_size << std::endl;
  std::cout << "Decompression Time (ms) : " << duration.count() << std::endl;
  return 0;
}
