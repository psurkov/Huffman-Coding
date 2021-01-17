#include <unordered_map>
#include <memory>
#include "doctest.h"
#include "Huffman.h"

using namespace std;
using namespace HuffmanArchiver::HuffmanTree;

TEST_SUITE("huffman.cpp") {
    using FreqTable = unordered_map<uint8_t, size_t>;
    using EncTable = unordered_map<uint8_t, string>;

    TEST_CASE("Empty freq") {
        HuffmanTree tree = HuffmanTree::buildFromFreqTable(FreqTable{});
        CHECK(tree.getCharsEncodingTable() == EncTable{});
    }

    TEST_CASE("One char") {
        HuffmanTree tree = HuffmanTree::buildFromFreqTable(unordered_map<uint8_t, size_t>{{0, 566}});
        CHECK(tree.getCharsEncodingTable() == EncTable{{0, "0"}});
    }

    TEST_CASE("Two chars") {
        HuffmanTree tree = HuffmanTree::buildFromFreqTable(FreqTable{{0, 1337}, {10, 5}});
        auto enc = tree.getCharsEncodingTable();
        CHECK(enc.size() == 2);
        CHECK(enc[0].size() == 1);
        CHECK(enc[10].size() == 1);
        CHECK(enc[0] != enc[10]);
    }

    TEST_CASE("Three chars") {
        HuffmanTree tree = HuffmanTree::buildFromFreqTable(FreqTable{{0, 2}, {10, 100}, {1, 1}});
        auto enc = tree.getCharsEncodingTable();
        CHECK(enc.size() == 3);
        CHECK(enc[0].size() == 2);
        CHECK(enc[10].size() == 1);
        CHECK(enc[1].size() == 2);
    }

    TEST_CASE("Five chars") {
        HuffmanTree tree = HuffmanTree::buildFromFreqTable(FreqTable{{'a', 5}, {'b', 2}, {'c', 1}, {'d', 1}, {'r', 3}});
        auto enc = tree.getCharsEncodingTable();
        CHECK(enc.size() == 5);
        CHECK(enc['a'].size() == 1);
        CHECK(enc['r'].size() == 2);
        CHECK(enc['b'].size() == 3);
        CHECK(enc['c'].size() == 4);
        CHECK(enc['d'].size() == 4);
    }

    TEST_CASE("Exact codes") {
        HuffmanTree tree = HuffmanTree::buildFromFreqTable(FreqTable{{'a', 10}, {'b', 8}, {'c', 2}, {'d', 1}});
        auto enc = tree.getCharsEncodingTable();
        CHECK(enc.size() == 4);
        CHECK(enc['a'] == "0");
        CHECK(enc['b'] == "11");
        CHECK(enc['c'] == "101");
        CHECK(enc['d'] == "100");
    }
}