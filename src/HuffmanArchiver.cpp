#include "HuffmanArchiver.h"

namespace HuffmanArchiver {

    std::unordered_map<uint8_t, size_t> HuffmanArchiver::calculateFrequencies() {
        std::unordered_map<uint8_t, size_t> freq(256);
        uint8_t byte;
        while (es.getByte(byte)) {
            freq[byte]++;
        }
        return freq;
    }

    void HuffmanArchiver::encode(const std::unordered_map<uint8_t, std::string> &encodingTable) {
        es.reset();
        uint8_t byte;
        uint32_t len = 0;
        while (es.getByte(byte)) {
            auto it = encodingTable.find(byte);
            assert(it != encodingTable.end());
            len += encodingTable.find(byte)->second.size();
        }

        es.writeLenToAdditional(len);
        es.reset();
        while (es.getByte(byte)) {
            auto it = encodingTable.find(byte);
            es.writeBitString(it->second);
        }
        es.flush();
    }

    HuffmanIO::SizeInfo HuffmanArchiver::compress() {
        if (!es.isInputEmpty()) {
            auto frequencies = calculateFrequencies();
            HuffmanTree::HuffmanTree huffmanTree = HuffmanTree::HuffmanTree::buildFromFreqTable(frequencies);
            auto encodingTable = huffmanTree.getCharsEncodingTable();
            es.writeEncodingTable(encodingTable);
            encode(encodingTable);
        }

        return es.getSizeInfo();
    }

    void HuffmanArchiver::decode(const std::unordered_map<std::string, uint8_t> &decodingTable) {
        std::string buf;
        size_t len = ds.readLenFromAdditional();
        for (size_t i = 0; i < len; i++) {
            bool bit;
            ds.getBit(bit);
            buf += '0' + bit;
            auto it = decodingTable.find(buf);
            if (it != decodingTable.end()) {
                ds.writeByte(it->second);
                buf.clear();
            }
        }
    }

    HuffmanIO::SizeInfo HuffmanArchiver::extract() {
        if (!ds.isInputEmpty()) {
            auto decodingTable = ds.readDecodingTable();
            decode(decodingTable);
        }
        return ds.getSizeInfo();
    }

} // namespace HuffmanArchiver