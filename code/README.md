# Lempel-Ziv-Welch for DNA

A collection of compression tools, including several implementations of Lempel-Ziv-Welch, specifically tailored towards compressing DNA.

## Prerequisites

- `cmake` and `g++`
- if haven't installed the submodules for this repo, run `git submodules update --init --recursive`
- some code relies on little endianess
- all implementations uses `pext`, which requires an x86 instruction set. Computers that don't support `pext` may see varied performance results
- all implementations expect files with only "A", "C", "T", or "G" in ASCII encoding. No newlines at the end of files

## Usage

To compile, run

```
make all
```

If you get an error about `./entropy-encoding`, make sure you have submodules installed.

Make sure to check out the [documentation](https://cadencorontzos.github.io/lzwfordna/).

### LZW

To encode a file with LZW, 

```
./lzwencode <filename>
```

To decode

```
./lzwdecode <original-filename>.compressed.lzw
```

### Three Stream LZW

To encode a file with Three_Stream_LZW, 

```
./three_stream_lzwencode <filename>
```

Three stream will produce a bunch of output files. To decompress effectively, run

```
./three_stream_lzwdecode <original-filename>.compressed.lzw
```

### Four To One

To encode a file with Four_To_One encoding 

```
./four_to_one_encode <filename>
```

To decode

```
./four_to_one_decode <original-filename>.4t1
```

## Testing

Most of the modules have unit test files, which you can produce by running 

```
make test
```

You can also run integration tests on each individual compressor. See the `./scripts` directory. Here is an example for regular LZW, assuming you have a folder of DNA files called Corpus1

```
sh scripts/integration.sh Corpus1
```

## Benchmarking

One of the corpora that I used for testing is publicly available [here](https://tinyurl.com/DNAcorpus). The other is not publicly available anywhere and I'm not sure about the legality of providing it, but if you search the internet hard enough it is easy to find. 

To benchmark the implementations, you can use `scripts/run_lzw_on_corpora.py`, which will produce a `csv` and `md` for the run that you can easily load into data analysis.

If you want to compare these implementations or others to command line tools, you can easily use or slightly change the script at `scripts/benchmark.sh`.
