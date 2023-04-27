# LZW for DNA

A collection of compression tools, including several implemenations of LZW, specifically tailored towards compresing DNA.

### Prerequisites

- `cmake` and `g++`
- if haven't installed the submodules for this repo, run `git submodules update --init --recursive`
- some code relies on little endianess
- some code uses `pext`, which requires an x86 instruction set. Computers that don't support `pext` may see varied performance results

### Usage

See this [github site](https://cadencorontzos.github.io/lzwfordna/) for code documentation.
