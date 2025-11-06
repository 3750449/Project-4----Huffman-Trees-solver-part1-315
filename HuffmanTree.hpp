#ifndef PROJECT3_PART1_STUDENT_FILES_HUFFMANTREE_H
#define PROJECT3_PART1_STUDENT_FILES_HUFFMANTREE_H

#include <vector>
#include <string>
#include <ostream>
#include "PriorityQueue.hpp"
#include <istream>
#include "utils.hpp"

class HuffmanTree {

    public:

    HuffmanTree() = default;
    ~HuffmanTree();

    static HuffmanTree buildFromCounts(const std::vector<std::pair<std::string,int>>& counts);

    void assignCodes(std::vector<std::pair<std::string, std::string>>& out) const;

    error_type writeHeader(std::ostream& outStream) const;
    error_type encode(const std::vector<std::string>& tokens, std::ostream& os_bits, int wrap_cols = 80) const;

    unsigned height() const noexcept;

    error_type buildFromHeader(const std::vector<std::pair<std::string,std::string>>& pairs);
    error_type decode(std::istream& code_stream, std::ostream& out_tokens) const;

private:

    HufNode* root_{ nullptr };

    static void destroy(HufNode* n) noexcept;
    static void assignCodesDFS(const HufNode* n, std::string& prefix, std::vector<std::pair<std::string,std::string>>& out);
    static void writeHeaderPreorder(const HufNode* n, std::string& prefix, std::ostream& outStream);
    static unsigned heightHelper(const HufNode* n) noexcept;

};

#endif //PROJECT3_PART1_STUDENT_FILES_HUFFMANTREE_H