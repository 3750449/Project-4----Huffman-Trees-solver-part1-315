
#include "PriorityQueue.hpp"
#include <algorithm> // for sort() --


//Build initial queue from leaves; sort MIN is placed at the back
PriorityQueue::PriorityQueue(std::vector<HufNode*> nodes) : items_(nodes) {

    std::sort(items_.begin(), items_.end(),

    [](const HufNode* a, const HufNode* b){

        if (a->freq != b->freq) return a->freq > b->freq;

        return a->key_word < b->key_word;
    }

    );

}

std::size_t PriorityQueue::size() const noexcept {

    return items_.size();
}

bool PriorityQueue::empty() const noexcept {

    return items_.empty();
}

//find the minimum element resides at the back
HufNode* PriorityQueue::findMin() const noexcept {

    return items_.empty()? nullptr : items_.back();
}

//remove the minimum from the back while preserving queue
HufNode* PriorityQueue::extractMin() noexcept {

    if (items_.empty())
        return nullptr;

    auto* n=items_.back();
    items_.pop_back();

    return n;
}

//remove minimum value from queue
void PriorityQueue::deleteMin() noexcept {

    if (!items_.empty())
        items_.pop_back();

}

//compare to avoid duplicates
bool PriorityQueue::higherPriority(const HufNode* a, const HufNode* b) noexcept {

    if (a->freq != b->freq)
        return a->freq > b->freq;

    return a->key_word < b->key_word;
}


//insert to find the first position where `node` should appear to keep sorted
void PriorityQueue::insert(HufNode* node){

    std::size_t lo = 0;
    std::size_t hi = items_.size();

    while ( lo < hi ) {

        auto mid = ( lo + hi )/2;

        if (higherPriority(node, items_[mid]))
            hi = mid;

        else lo = mid + 1;

    }

    items_.insert(items_.begin() + lo, node);

}
