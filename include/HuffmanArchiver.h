#pragma once

#include <fstream>
#include <unordered_map>
#include <cassert>
#include "HuffmanIO.h"
#include "Huffman.h"

namespace HuffmanArchiver {

    class HuffmanArchiver final {
        HuffmanIO::EncodingIO es;
        HuffmanIO::DecodingIO ds;

        std::unordered_map<uint8_t, size_t> calculateFrequencies();
        void encode(const std::unordered_map<uint8_t, std::string> &encodingTable);

        void decode(const std::unordered_map<std::string, uint8_t> &decodingTable);
    public:
        HuffmanArchiver(std::istream &is, std::ostream &os) : es(is, os), ds(is, os) {}

        HuffmanIO::SizeInfo compress();
        HuffmanIO::SizeInfo extract();
    };

} // namespace HuffmanArchiver