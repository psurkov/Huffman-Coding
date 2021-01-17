#include "Huffman.h"

namespace HuffmanArchiver::HuffmanTree {

    std::unordered_map<uint8_t, std::string> HuffmanTree::HuffmanNode::getCharsEncodingTable() const noexcept {
        if (c) {
            return {{*c, ""}};
        }

        auto lEncoding = l->getCharsEncodingTable();
        for (auto &i : lEncoding) {
            i.second = "0" + i.second;
        }

        auto rEncoding = r->getCharsEncodingTable();
        for (auto &i : rEncoding) {
            i.second = "1" + i.second;
        }

        auto result = std::move(lEncoding);
        result.insert(rEncoding.begin(), rEncoding.end());
        return result;
    }

    std::unordered_map<uint8_t, std::string> HuffmanTree::getCharsEncodingTable() const noexcept{
        if (!root) {
            return {};
        }
        if (root->c) {
            return {{*root->c, "0"}};
        }
        return root->getCharsEncodingTable();
    }

    HuffmanTree HuffmanTree::buildFromFreqTable(const std::unordered_map<uint8_t, size_t> &freq) {
        if (freq.empty()) {
            return HuffmanTree();
        }
        using pair = std::pair<size_t, std::unique_ptr<HuffmanTree>>;
        std::set<pair> q;

        for (const auto &p : freq) {
            q.insert(pair{p.second, std::make_unique<HuffmanTree>(p.first)});
        }

        while (q.size() > 1) {
            pair p1 = std::move(q.extract(q.begin()).value());
            pair p2 = std::move(q.extract(q.begin()).value());
            q.insert(pair{p1.first + p2.first,
                          std::make_unique<HuffmanTree>(std::move(p1.second), std::move(p2.second))});
        }
        assert(q.size() == 1);
        pair p = std::move(q.extract(q.begin()).value());

        return HuffmanTree(std::move(p.second->root));
    }

} // namespace HuffmanArchiver::HuffmanTree