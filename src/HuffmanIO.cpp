#include "HuffmanIO.h"

namespace HuffmanArchiver::HuffmanIO {

    std::ostream &operator<<(std::ostream &os, const SizeInfo &sizeInfo) {
        return os << sizeInfo.before << '\n' << sizeInfo.after << '\n' << sizeInfo.additional << std::flush;

    }

    void HuffmanAbstractIO::reset() {
        is.seekg(0, is.beg);
        sizeInfo.before = 0;
    }

    uint32_t HuffmanAbstractIO::readUInt32_t() {
        uint32_t x = 0;
        unsigned char buf[4];
        is.read(reinterpret_cast<char*>(buf), 4);
        for (size_t i = 0; i < 4; i++) {
            x |= buf[i] << 8 * i;
        }
        return x;
    }

    void HuffmanAbstractIO::writeUInt32_t(uint32_t x) {
        unsigned char buf[4];
        for (size_t i = 0; i < 4; i++) {
            buf[i] = (x >> 8 * i) & 0xFF;
        }
        os.write(reinterpret_cast<char*>(buf), 4);
    }

    bool HuffmanAbstractIO::isInputEmpty() const noexcept {
        return is.peek() == EOF;
    }

    SizeInfo HuffmanAbstractIO::getSizeInfo() const noexcept {
        return sizeInfo;
    }


    bool EncodingIO::getByte(uint8_t &byte) {
        if (is.peek() == EOF) {
            return false;
        }
        char c;
        is.get(c);
        byte = static_cast<uint8_t>(c);
        sizeInfo.before++;
        return true;
    }

    void EncodingIO::writeBitString(const std::string &str) {
        for (char c : str) {
            assert(c == '0' || c == '1');
            writeBuffer.push(c == '1');
        }
        while (writeBuffer.size() >= 8) {
            uint8_t byte = 0;
            for (size_t i = 0; i < 8; i++) {
                byte = byte * 2 + writeBuffer.front();
                writeBuffer.pop();
            }
            os.put(byte);
            sizeInfo.after++;
        }
    }


    void EncodingIO::flush() {
        if (!writeBuffer.empty()) {
            writeBitString(std::string(8 - writeBuffer.size(), '0'));
        }
    }

    void EncodingIO::writeLenToAdditional(uint32_t len) {
        writeUInt32_t(len);
        sizeInfo.additional += 4;
    }

    void EncodingIO::writeEncodingTable(const std::unordered_map<uint8_t, std::string> &encodingTable) {
        os.put(static_cast<uint8_t>(encodingTable.size() - 1));
        sizeInfo.additional = 1;
        for (const auto &[byte, codeStr] : encodingTable) {
            os.put(byte);
            os.put(static_cast<uint8_t>(codeStr.size()));
            uint32_t code = 0;
            for (char c : codeStr) {
                assert(c == '0' || c == '1');
                code = code * 2 + static_cast<int>(c == '1');
            }
            writeUInt32_t(code);
            sizeInfo.additional += 6;
        }
    }

    EncodingIO::~EncodingIO() {
        flush();
    }


    void DecodingIO::getBit(bool &bit) {
        if (readBuffer.empty()) {
            char c;
            is.get(c);
            auto byte = static_cast<uint8_t>(c);
            for (size_t i = 0; i < 8; i++) {
                readBuffer.push((byte >> (7 - i)) & 1);
            }
            sizeInfo.before++;
        }
        bit = readBuffer.front();
        readBuffer.pop();
    }

    void DecodingIO::writeByte(uint8_t byte) {
        os.put(byte);
        sizeInfo.after++;
    }

    uint32_t DecodingIO::readLenFromAdditional() {
        sizeInfo.additional += 4;
        return readUInt32_t();
    }

    std::unordered_map<std::string, uint8_t> DecodingIO::readDecodingTable() {
        char c;
        is.get(c);
        auto n = static_cast<size_t>(static_cast<uint8_t>(c) + 1);
        sizeInfo.additional = 1;
        std::unordered_map<std::string, uint8_t> decodingTable(n);
        for (size_t i = 0; i < n; i++) {
            is.get(c);
            auto byte = static_cast<uint8_t>(c);
            is.get(c);
            auto len = static_cast<uint8_t>(c);
            uint32_t code = readUInt32_t();
            std::string str;
            for (size_t i = 0; i < len; i++) {
                str.push_back('0' + ((code >> (len - i - 1)) & 1));
            }
            decodingTable[str] = byte;
            sizeInfo.additional += 6;
        }

        return decodingTable;
    }

} // HuffmanArchiver::HuffmanIO