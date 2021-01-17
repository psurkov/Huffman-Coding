#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "CLI.h"
#include "HuffmanArchiver.h"

int main(int argc, const char **argv) {
    CLI::CLI cli(argc, argv);

    std::ifstream input(cli.getInputPath(), std::ifstream::binary);
    std::ofstream output(cli.getOutputPath(), std::ofstream::binary);


    HuffmanArchiver::HuffmanArchiver huffmanArchiver(input, output);

    if (cli.isCompress()) {
        std::cout << huffmanArchiver.compress();
    }
    else {
        std::cout << huffmanArchiver.extract();
    }
}