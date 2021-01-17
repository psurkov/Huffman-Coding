# Huffman coding

The program is a console archiver which uses [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding). The application is developed in OOP style. The classes «tree», «vertex tree» and «archiver» (the class incapsulating
compression and decompression) are clearly distinguished. The tests are implemented using [doctest](https://github.com/onqtam/doctest).


## Install

```bash
git clone https://github.com/psurkov/Huffman-Coding.git
cd Huffman-Coding
cmake . && make
```

## Use
Type `./huffman` to run the program or `./test_huffman` to run the tests. The program should be sent flags.

* The program supports the following flags:
  * `-c`: compression
  * `-u`: decompression
  * `-f`, `--file <path>`: input filename
  * `-o`, `--output <path>`: output filename
* The arguments can follow in any order.

The program outputs the statistics of compression/decompression: the size of the input file, the size of the output file
and the size of additional data in the output file. All sizes are shown in bytes.
