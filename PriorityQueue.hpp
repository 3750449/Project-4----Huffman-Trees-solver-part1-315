#ifndef PROJECT3_PART1_STUDENT_FILES_PRIORITYQUEUE_H
#define PROJECT3_PART1_STUDENT_FILES_PRIORITYQUEUE_H

#include <vector>
#include <string>

// for huffman tree structure
struct HufNode {

    std::string word;
    std::string key_word;

    int freq{0};

    HufNode* left{nullptr};
    HufNode* right{nullptr};

    HufNode(std::string w, int f) : word(std::move(w)), key_word(word), freq(f) {}
    HufNode(int f, std::string key, HufNode* l, HufNode* r) : key_word(std::move(key)), freq(f), left(l), right(r) {}

    bool isLeaf() const noexcept {

        return !left && !right;
    }

};

class PriorityQueue {

    public:

    explicit PriorityQueue(std::vector<HufNode*> nodes);
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    HufNode* findMin() const noexcept;
    HufNode* extractMin() noexcept;
    void deleteMin() noexcept;
    void insert(HufNode* node);

private:

    std::vector<HufNode*> items_;
    static bool higherPriority(const HufNode* a, const HufNode* b) noexcept;

};

#endif //PROJECT3_PART1_STUDENT_FILES_PRIORITYQUEUE_H