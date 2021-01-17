#include <unordered_map>
#include <memory>
#include <sstream>
#include <iostream>
#include "doctest.h"
#include "HuffmanIO.h"

using namespace std;
using namespace HuffmanArchiver::HuffmanIO;

TEST_SUITE("EncodingIO") {

    TEST_CASE("Test get") {
        stringstream is({0, static_cast<char>(255), 100}), os;        
        EncodingIO es(is, os);
        uint8_t byte;
        CHECK(es.getByte(byte));
        CHECK(byte == 0);
        CHECK(es.getByte(byte));
        CHECK(byte == 255);
        CHECK(es.getByte(byte));
        CHECK(byte == 100);
        CHECK(!es.getByte(byte));
    }
    
    TEST_CASE("Write bit string") {
        stringstream is, os;        
        EncodingIO es(is, os);
        
        // 1111010110011001
        es.writeBitString("11");
        es.writeBitString("110");
        es.writeBitString("1");
        es.writeBitString("0");
        es.writeBitString("110");
        es.writeBitString("011");
        es.writeBitString("001");
        char c;
        os.get(c);
        CHECK(static_cast<uint8_t>(c) == (1 << 7) + (1 << 6) + (1 << 5) + (1 << 4) + (1 << 2) + 1);
        os.get(c);
        CHECK(static_cast<uint8_t>(c) == (1 << 7) + (1 << 4) + (1 << 3) + 1);
    }

    TEST_CASE("Flush") {
        stringstream is, os;
        {
            EncodingIO es(is, os);
            es.writeBitString("011");
        }
        char c;
        os.get(c);
        CHECK(static_cast<uint8_t>(c) == (1 << 6) + (1 << 5));
    }

    TEST_CASE("Write encoding") {
        stringstream ss;        
        EncodingIO es(ss, ss);
        const unordered_map<uint8_t, std::string> encoding = {{'a', "100000000000000000001"}};
        es.writeEncodingTable(encoding);
        char c;
        ss.get(c);
        CHECK(static_cast<uint8_t>(c) == 0);
        ss.get(c);
        CHECK(static_cast<uint8_t>(c) == 'a');
        ss.get(c);
        CHECK(static_cast<uint8_t>(c) == 21);
        CHECK(es.readUInt32_t() == (1 << 20) + 1);
    }
}

TEST_SUITE("DecodingIO") {

    TEST_CASE("Get bit") {
        string s1 = "11110101";
        string s2 = "10011001";
        string s = s1 + s2;

        stringstream is({static_cast<char>(0b11110101), static_cast<char>(0b10011001)}), os;        
        DecodingIO ds(is, os);
        bool b;
        for (size_t i = 0; i < 16; i++) {
            ds.getBit(b);
            CHECK(b == static_cast<bool>(s[i] - '0'));
        }
    }

    TEST_CASE("Test write byte") {
        stringstream is, os;        
        DecodingIO ds(is, os);
        ds.writeByte(100);
        ds.writeByte(0);
        ds.writeByte(255);
        char c;
        os.get(c);
        CHECK(static_cast<uint8_t>(c) == 100);
        os.get(c);
        CHECK(static_cast<uint8_t>(c) == 0);
        os.get(c);
        CHECK(static_cast<uint8_t>(c) == 255);
        CHECK(os.peek() == EOF);
    }

    TEST_CASE("Test decoding by end to end") {
        unordered_map<uint8_t, std::string> encoding;
        for (int i = 0; i < 20; i++) {
            encoding[i] = string(1 + i / 2, '0' + i % 2);
        }
        stringstream ss1, ss2, ss3; 
        EncodingIO es(ss1, ss2);
        es.writeEncodingTable(encoding);
        DecodingIO ds(ss2, ss3);
        unordered_map<std::string, uint8_t> decoding = ds.readDecodingTable();
        CHECK(encoding.size() == decoding.size());
        for (const auto &[code, byte] : decoding) {
            CHECK(encoding[byte] == code);
        }
    }
}