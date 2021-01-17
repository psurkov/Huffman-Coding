#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <exception>
#include <stdexcept>
#include <string_view>

namespace CLI {

    class CLI final {
        struct CLIException final : public std::runtime_error {
            CLIException (const std::string& msg) : std::runtime_error(msg) {}
        };

        std::unordered_map<std::string, std::vector<std::string> > args;
        bool compress;
        std::string input, output;

    public:
        CLI(int argc, const char **argv);
        bool isCompress() const noexcept;
        std::string getInputPath() const noexcept;
        std::string getOutputPath() const noexcept;
    };

} // namespace CLI