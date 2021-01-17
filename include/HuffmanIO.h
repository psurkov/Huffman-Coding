#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <cassert>

namespace HuffmanArchiver::HuffmanIO {

    struct SizeInfo {
        size_t before = 0, after = 0, additional = 0;
    };
    std::ostream &operator<<(std::ostream &os, const SizeInfo &sizeInfo);


    class HuffmanAbstractIO {
    protected:
        std::istream &is;
        std::ostream &os;
        SizeInfo sizeInfo;
    public:
        HuffmanAbstractIO(std::istream &is_, std::ostream &os_) : is(is_), os(os_) {
            is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            os.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        }
        void reset();
        uint32_t readUInt32_t();
        void writeUInt32_t(uint32_t x);
        [[nodiscard]] bool isInputEmpty() const noexcept;
        [[nodiscard]] SizeInfo getSizeInfo() const noexcept;
    };


    class EncodingIO final : public HuffmanAbstractIO {
        std::queue<bool> writeBuffer;
    public:
        EncodingIO(std::istream &is_, std::ostream &os_) : HuffmanAbstractIO(is_, os_) {}
        bool getByte(uint8_t &byte);
        void writeBitString(const std::string &str);
        void flush();
        void writeLenToAdditional(uint32_t len);
        void writeEncodingTable(const std::unordered_map<uint8_t, std::string> &encoding);
        ~EncodingIO();
    };


    class DecodingIO final : public HuffmanAbstractIO {
        std::queue<bool> readBuffer;
    public:
        DecodingIO(std::istream &is_, std::ostream &os_) : HuffmanAbstractIO(is_, os_) {}
        void getBit(bool &bit);
        void writeByte(uint8_t byte);
        uint32_t readLenFromAdditional();
        std::unordered_map<std::string, uint8_t> readDecodingTable();
    };

} // HuffmanArchiver::HuffmanIO