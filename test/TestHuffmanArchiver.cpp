#include <random>
#include <algorithm>
#include <sstream>
#include "doctest.h"
#include "HuffmanArchiver.h"

using namespace std;

TEST_SUITE("Test huffman archiver") {


    TEST_CASE("Empty file compress") {
        stringstream is, os;
        HuffmanArchiver::HuffmanArchiver huffmanArchiver(is, os);
        huffmanArchiver.compress();
        CHECK(os.str() == "");
    }

    TEST_CASE("Empty file extract") {
        stringstream is, os;
        HuffmanArchiver::HuffmanArchiver huffmanArchiver(is, os);
        huffmanArchiver.extract();
        CHECK(os.str() == "");
    }

    TEST_CASE("End to end test") {
        uniform_int_distribution<>dis(0, 256);
        mt19937 gen(0);
        string data;
        generate_n(back_inserter(data), 1337, [&]{return dis(gen);});

        stringstream ss1(data), ss2, ss3;
        HuffmanArchiver::HuffmanArchiver huffmanArchiver1(ss1, ss2);
        auto info1 = huffmanArchiver1.compress();
        
        CHECK(ss2.str().size() == info1.after + info1.additional);
        
        HuffmanArchiver::HuffmanArchiver huffmanArchiver2(ss2, ss3);
        auto info2 = huffmanArchiver2.extract();

        CHECK(info1.before == info2.after);
        CHECK(info1.after == info2.before);
        CHECK(info1.additional == info2.additional);
    }
}