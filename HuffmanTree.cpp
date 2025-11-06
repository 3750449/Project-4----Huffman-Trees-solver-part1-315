
#include "HuffmanTree.hpp"
#include <unordered_map>
#include "PriorityQueue.hpp"
#include <vector>

HuffmanTree::~HuffmanTree(){ destroy(root_); }

void HuffmanTree::destroy(HufNode* n) noexcept {

    if(!n)
        return;

    destroy(n->left);
    destroy(n->right);
    delete n;

}

HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string,int>>& counts) {

    HuffmanTree huffTree;

    if (counts.empty())
        return huffTree;

    std::vector<HufNode*> leaves;

    leaves.reserve(counts.size());

    for (const auto& kv : counts)
        leaves.push_back(new HufNode(kv.first, kv.second));

    PriorityQueue pq(std::move(leaves));

    if (pq.size() == 1) {

        huffTree.root_ = pq.extractMin();

        return huffTree;
    }

    while (pq.size() >= 2){

        auto* a=pq.extractMin();
        auto* b=pq.extractMin();

        std::string key = (a->key_word < b->key_word ? a->key_word : b->key_word);

        pq.insert(new HufNode(a->freq + b->freq, key, a, b));

    }

    huffTree.root_=pq.extractMin();

    return huffTree;
}

void HuffmanTree::assignCodesDFS(const HufNode* n, std::string& p, std::vector<std::pair<std::string,std::string>>& out) {

    if(!n)
        return;

    if(n->isLeaf()) {

        out.emplace_back(n->word, p.empty()? "0" : p);

        return;
    }

    p.push_back('0');

    assignCodesDFS(n->left, p, out);
    p.back()='1';

    assignCodesDFS(n->right, p, out);
    p.pop_back();

}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string,std::string>>& out) const {

    out.clear();

    std::string p;

    assignCodesDFS(root_, p, out);

}

void HuffmanTree::writeHeaderPreorder(const HufNode* n, std::string& p, std::ostream& outStream) {

    if(!n)
        return;

    if(n->isLeaf()) {

        outStream << n->word << ' ' << (p.empty()? "0":p) << '\n';

        return;
    }

    p.push_back('0');
    writeHeaderPreorder(n->left, p, outStream);
    p.back()='1';
    writeHeaderPreorder(n->right, p, outStream);
    p.pop_back();

}

error_type HuffmanTree::writeHeader(std::ostream& outStream) const {

    if(!outStream.good())
        return FAILED_TO_WRITE_FILE;

    // empty with single newline
        if (!root_) {

            outStream.put('\n');

            return outStream.good() ? NO_ERROR : FAILED_TO_WRITE_FILE;
            }

        std::string p;

    writeHeaderPreorder(root_, p, outStream);

    return outStream.good() ? NO_ERROR : FAILED_TO_WRITE_FILE;
}

error_type HuffmanTree::encode(const std::vector<std::string>& tokens, std::ostream& out, int wrap_cols) const {

    std::vector<std::pair<std::string,std::string>> codes;

    assignCodes(codes);

    std::unordered_map<std::string,std::string> m;

    for(auto& kv: codes) {

        m.emplace(kv.first, kv.second);

    }

        //newline when tokens zero
        if (tokens.empty()) {

            out.put('\n');

            if (!out.good())
                return FAILED_TO_WRITE_FILE;

            return NO_ERROR;
            }

    int col = 0;

    for (const auto& t : tokens) {

        auto it = m.find(t);

        //if( it==m.end() )
        //return FAILED_TO_WRITE_FILE;
        if( it==m.end() )
            return ERR_TYPE_NOT_FOUND;

        for (char ch : it->second) {

            out.put(ch);

            if(!out.good())
                return FAILED_TO_WRITE_FILE;

            if( ++col == wrap_cols) {

                out.put('\n');

                if(!out.good())
                    return FAILED_TO_WRITE_FILE;

                col = 0;

            }

        }

    }

    if( col != 0) {

        out.put('\n');

        if(!out.good())
            return FAILED_TO_WRITE_FILE;

    }

    return NO_ERROR;
}

unsigned HuffmanTree::heightHelper(const HufNode* n) noexcept {

    if(!n)
        return 0;

    unsigned lh = heightHelper(n->left), rh = heightHelper(n->right);

    return 1+(lh > rh ? lh : rh);
}

unsigned HuffmanTree::height() const noexcept
{

    return heightHelper(root_);
}

error_type HuffmanTree::buildFromHeader( const std::vector<std::pair<std::string,std::string>>& pairs) {

    // reset any existing tree
    destroy(root_);
    root_ = nullptr;

    // Empty header, empty tree check
    if (pairs.empty())
        return NO_ERROR;

    // Create an empty internal root
    root_ = new HufNode(0, /*key*/"", nullptr, nullptr);

    for (const auto& kv : pairs) {

        const std::string& word = kv.first;
        const std::string& code = kv.second;

        if (word.empty() || code.empty())
            return ERR_TYPE_NOT_FOUND;

        HufNode* cur = root_;

        for (char b : code) {

            if (b != '0' && b != '1')
                return ERR_TYPE_NOT_FOUND;

            if (b == '0') {

                if (!cur->left)  cur->left  = new HufNode(0, "", nullptr, nullptr);
                cur = cur->left;

            } else {

                if (!cur->right) cur->right = new HufNode(0, "", nullptr, nullptr);
                cur = cur->right;

            }
        }

        // If this leaf already has a word, header had duplicate code
        if (!cur->word.empty())
            return ERR_TYPE_NOT_FOUND;

        cur->word     = word;   // leaf carries the word
        cur->key_word = word;   // tie-break representative (not used in decode)

    }

    return NO_ERROR;
}

error_type HuffmanTree::decode(std::istream& code_stream, std::ostream& out_tokens) const {

    // If stream is already bad, fail
    if (!code_stream.good() && !code_stream.eof())
        return UNABLE_TO_OPEN_FILE;

    // Empty tree, decoded output is a single newline
    if (!root_) {

        out_tokens.put('\n');

        return out_tokens.good() ? NO_ERROR : FAILED_TO_WRITE_FILE;
    }

    HufNode* cur = root_;
    bool wrote_any = false;

    for (;;) {

        int ci = code_stream.get();
        if (ci == EOF) break;

        char ch = static_cast<char>(ci);

        if (ch == '0' || ch == '1') {

            // step left/right
            cur = (ch == '0') ? cur->left : cur->right;

            if (!cur)
                return ERR_TYPE_NOT_FOUND; // invalid path

            // If we hit a leaf, emit a word line and reset to root
            if (cur->left == nullptr && cur->right == nullptr) {

                out_tokens << cur->word << '\n';

                if (!out_tokens)
                    return FAILED_TO_WRITE_FILE;

                wrote_any = true;
                cur = root_;

            }

        } else if (ch == '\n' || ch == '\r') {

            // ignore line breaks in the bitstream
            continue;

        } else {

            // unexpected character in .code
            return ERR_TYPE_NOT_FOUND;
        }

    }

    // If file ended mid-symbol, it's invalid
    if (cur != root_)
        return ERR_TYPE_NOT_FOUND;

    // If we decoded nothing at all, still write a single newline to match .tokens
    if (!wrote_any) {

        out_tokens.put('\n');

        if (!out_tokens)
            return FAILED_TO_WRITE_FILE;

    }

    return NO_ERROR;
}
