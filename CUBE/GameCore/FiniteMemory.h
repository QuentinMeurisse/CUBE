#ifndef CUBE_FINITEMEMORY_H
#define CUBE_FINITEMEMORY_H

#include <deque>
#include <algorithm>
#include <memory>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <iostream>

template<typename T>
class FiniteMemory {
public:
    FiniteMemory(){
        max_size = 100;
        max_duplicate = 5;
    };

    FiniteMemory(size_t maxSize, int maxDuplicate) : max_size(maxSize), max_duplicate(maxDuplicate) {}

    void insert(std::shared_ptr<T> x){
        if (q.size() == max_size) q.pop_front();// If our memory is full, we need to make some place
        q.push_back(x);
    }

    /**
     *
     * @param x
     * @return the item x is in q at least max_duplicate times
     */
    bool contain_too_much (std::shared_ptr<T> x){
        auto f = [&](std::shared_ptr<T> y){ return *x == *y;};
        int num = std::count_if(q.begin(), q.end(), f);
        return num >= max_duplicate;
    }

    friend std::ostream &operator<<(std::ostream &os, const FiniteMemory &memory) {
        os << "memory size: " << memory.q.size() <<  " max duplications: " << memory.max_duplicate <<std::endl;
        std::unordered_map<std::string, size_t> counts;
        for (auto const& x : memory.q) {
            std::stringstream ss;
            ss << *x;
            ++counts[ss.str()];
        }
        for (auto const&[x, y] : counts)
            os << "\t" << x << ": " << y << std::endl;
        return os;
    }

    void setMaxSize(size_t maxSize) {
        max_size = maxSize;
    }

    void setMaxDuplicate(int maxDuplicate) {
        max_duplicate = maxDuplicate;
    }


private:
    std::deque<std::shared_ptr<T>> q{};
    size_t max_size;
    int max_duplicate;

};


#endif //CUBE_FINITEMEMORY_H
