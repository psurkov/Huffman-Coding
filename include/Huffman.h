#pragma once

#include <unordered_map>
#include <memory>
#include <optional>
#include <set>
#include <cassert>

namespace HuffmanArchiver::HuffmanTree {

    class HuffmanTree final {
        struct HuffmanNode final {
            std::optional<uint8_t> c = std::nullopt;
            std::unique_ptr<HuffmanNode> l = nullptr, r = nullptr;
            explicit HuffmanNode(uint8_t c_) : c(c_) {}
            HuffmanNode(std::unique_ptr<HuffmanNode> l_, std::unique_ptr<HuffmanNode> r_) : l(std::move(l_)), r(std::move(r_)) {}
            [[nodiscard]] std::unordered_map<uint8_t, std::string> getCharsEncodingTable() const noexcept;
        };

        std::unique_ptr<HuffmanNode> root;

    public:
        HuffmanTree() = default;
        explicit HuffmanTree(std::unique_ptr<HuffmanNode> node) : root(std::move(node)) {}
        explicit HuffmanTree(uint8_t c) : root(std::make_unique<HuffmanNode>(c)) {}
        explicit HuffmanTree(std::unique_ptr<HuffmanTree> l, std::unique_ptr<HuffmanTree> r) : root(std::make_unique<HuffmanNode>(std::move(l->root), std::move(r->root))) {}
        [[nodiscard]] std::unordered_map<uint8_t, std::string> getCharsEncodingTable() const noexcept;
        static HuffmanTree buildFromFreqTable(const std::unordered_map<uint8_t, size_t> &freq);
    };

} // HuffmanArchiver::huffmanImpl::HuffmanTree