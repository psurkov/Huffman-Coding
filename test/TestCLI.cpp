#include <unordered_map>
#include <memory>
#include <sstream>
#include <iostream>
#include "doctest.h"
#include "CLI.h"

using namespace std;

TEST_SUITE("CLI") {
    TEST_CASE("Test correct input") {
        SUBCASE("Short keys") {
            const char *argv[] = {"", "-c", "-f", "a.txt", "-o", "b.bin"};
            CLI::CLI cli(6, argv);
            CHECK(cli.isCompress());
            CHECK(cli.getInputPath() == "a.txt");
            CHECK(cli.getOutputPath() == "b.bin");
        }

        SUBCASE("Long key") {
            const char *argv[] = {"", "--file", "a.txt", "-u", "-o", "b.bin"};
            CLI::CLI cli(6, argv);
            CHECK(!cli.isCompress());
            CHECK(cli.getInputPath() == "a.txt");
            CHECK(cli.getOutputPath() == "b.bin");
        }
        SUBCASE("Long keys") {
            const char *argv[] = {"", "--output", "b.bin", "--file", "a.txt", "-c"};
            CLI::CLI cli(6, argv);
            CHECK(cli.isCompress());
            CHECK(cli.getInputPath() == "a.txt");
            CHECK(cli.getOutputPath() == "b.bin");
        }
    }

    TEST_CASE("Test no required arg") {
        SUBCASE("No arg c or u") {
            const char *argv[] = {"", "-f", "a.txt", "-o", "b.bin"};
            CHECK_THROWS_WITH(CLI::CLI(5, argv), "No arg c or u");
        }
        SUBCASE("No arg file") {
            const char *argv[] = {"", "-u", "-o", "b.bin"};
            CHECK_THROWS_WITH(CLI::CLI(4, argv), "No arg file");
        }
        SUBCASE("No arg output") {
            const char *argv[] = {"", "-f", "a.txt", "-c"};
            CHECK_THROWS_WITH(CLI::CLI(4, argv), "No arg output");
        }
    }

    TEST_CASE("Dublicated args") {
        SUBCASE("Arg c duplicated") {
            const char *argv[] = {"", "-c", "-f", "a.txt", "-o", "b.bin", "-c"};
            CHECK_THROWS_WITH(CLI::CLI(7, argv), "Arg c duplicated");
        }
        SUBCASE("Arg f duplicate arg file") {
            const char *argv[] = {"", "-c", "-f", "a.txt", "-o", "b.bin", "--file", "a.txt"};
            CHECK_THROWS_WITH(CLI::CLI(8, argv), "Arg f duplicated");
        }
    }

    TEST_CASE("Incompatible args") {
        {
            const char *argv[] = {"", "-c", "-f", "a.txt", "-o", "b.bin", "-u"};
            CHECK_THROWS_WITH(CLI::CLI(7, argv), "Incompatible args c and u");
        }
    }

    TEST_CASE("No files") {
        {
            const char *argv[] = {"", "-c", "-f", "-o", "b.bin"};
            CHECK_THROWS_WITH(CLI::CLI(5, argv), "Wrong number of f values");
        }
    }

    TEST_CASE("Many files") {
        {
            const char *argv[] = {"", "-c", "-f", "a.txt", "-o", "b.bin", "c.bin"};
            CHECK_THROWS_WITH(CLI::CLI(7, argv), "Wrong number of o values");
        }
    }

    TEST_CASE("Equal input and output") {
        {
            const char *argv[] = {"", "-c", "-f", "a.txt", "-o", "a.txt"};
            CHECK_THROWS_WITH(CLI::CLI(6, argv), "Input and output are equal");
        }
    }
}