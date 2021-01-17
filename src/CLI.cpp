#include "CLI.h"
#include <iostream>

namespace CLI {

    static const std::unordered_map<std::string, size_t> requiredValues = {{"c", 0}, {"u", 0}, {"f", 1}, {"o", 1}};

    CLI::CLI(int argc, const char **argv) {
        std::unordered_map<std::string, std::vector<std::string>> args;
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                throw CLIException("Arg value before first argument");
            }
            size_t len = strlen(argv[i]);
            if (len == 1) {
                throw CLIException("No" + std::to_string(i) + "arg");
            }
            std::string arg = std::string(argv[i]).substr(1 + static_cast<size_t>(len != 2));
            if (arg == "file") {
                arg = "f";
            }
            else if (arg == "output") {
                arg = "o";
            }

            if (args.count(arg)) {
                throw CLIException("Arg " + arg + " duplicated");
            }

            auto &values = args[arg];
            int valPos = i + 1;
            while (valPos < argc && argv[valPos][0] != '-') {
                values.emplace_back(std::string(argv[valPos++]));
            }
            i = valPos - 1;
        }

        for (const auto &[arg, values] : args) {
            static_cast<void>(values);
            if (!requiredValues.count(arg)) {
                throw CLIException("Unsupported arg " + arg);
            }
            if (requiredValues.find(arg)->second != values.size()) {
                throw CLIException("Wrong number of " + arg + " values");
            }
        }

        if (!args.count("f")) {
            throw CLIException("No arg file");
        }
        if (!args.count("o")) {
            throw CLIException("No arg output");
        }

        if (!args.count("c") && !args.count("u")) {
            throw CLIException("No arg c or u");
        }
        if (args.count("c") && args.count("u")) {
            throw CLIException("Incompatible args c and u");
        }

        compress = static_cast<bool>(args.count("c"));

        input = args["f"][0], output = args["o"][0];
        if (input == output) {
            throw CLIException("Input and output are equal");
        }
    }

    bool CLI::isCompress() const noexcept {
        return compress;
    }

    std::string CLI::getInputPath() const noexcept {
        return input;
    }

    std::string CLI::getOutputPath() const noexcept {
        return output;
    }

} // namespace CLI