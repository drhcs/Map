#ifndef MAP
#define MAP

#include <vector>
#include <list>

template<class Key, class Val>
class Map {

    const float max_load_;
    int max_size_;
    int size_;

    std::vector<std::list<std::pair<Key, Val>>> hash_table_;

    int Hash(Key key) {
        return std::hash<Key>{}(key) % max_size_;
    }

    float LoadFactor() {
        return ((float) size_) / ((float) max_size_);
    }

    void Resize() {
        max_size_ *= 2;
        std::vector<std::list<std::pair<Key, Val>>> new_hash_table(max_size_, std::list<std::pair<Key, Val>>());

        for (auto & chain : hash_table_)
        {
            while (!chain.empty())
            {
                Key key = chain.front().first;
                Val val = chain.front().second;
                chain.pop_front();

                int new_hash_code = Hash(key);
                new_hash_table[new_hash_code].push_back({key, val});
            }
        }

        hash_table_ = new_hash_table;
    }

    void IncreaseSize() {
        size_++;

        if (LoadFactor() >= max_load_)
        {
            Resize();
        }
    }

public:

    explicit Map(const float max_load = 0.75, int max_size = 10) : max_load_(max_load), max_size_(max_size) {
        hash_table_ = std::vector<std::list<std::pair<Key, Val>>>(max_size, std::list<std::pair<Key, Val>>());
        size_ = 0;
    }

    ~Map() {
        Clear();
    }

    bool Contains(Key key) {
        int hash_code = Hash(key);
        std::list<std::pair<Key, Val>> & chain = hash_table_[hash_code];
        for (const auto & node : chain)
        {
            Key node_key = node.first;
            Val node_val = node.second;

            if (node_key == key)
            {
                return true;
            }
        }
        return false;
    }

    Val At(Key key) {
        int hash_code = Hash(key);
        std::list<std::pair<Key, Val>> & chain = hash_table_[hash_code];
        for (const auto & node : chain)
        {
            Key node_key = node.first;
            Val node_val = node.second;

            if (node_key == key)
            {
                return node_val;
            }
        }
        return Val();
    }

    void Insert(Key key, Val val) {
        int hash_code = Hash(key);
        std::list<std::pair<Key, Val>> & chain = hash_table_[hash_code];
        for (auto & node : chain)
        {
            Key node_key = node.first;
            Val node_val = node.second;

            if (node_key == key)
            {
                node.second = val;
                return;
            }
        }
        hash_table_[hash_code].push_back({key, val});
        IncreaseSize();
    }

    void Remove(Key key) {
        if (Contains(key))
        {
            auto KeyExists = [&key](const std::pair<Key, Val> & node){
                return key == node.first;
            };

            int hash_code = Hash(key);
            hash_table_[hash_code].remove_if(KeyExists);
            size_--;
        }
    }

    int Size() {
        return size_;
    }

    bool Empty() {
        return size_ == 0;
    }

    void Clear() {
        for (auto & chain : hash_table_)
        {
            chain.clear();
        }
        hash_table_.clear();
        size_ = 0;
    }
};


#endif
