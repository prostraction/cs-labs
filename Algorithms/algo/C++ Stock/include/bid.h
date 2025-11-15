#ifndef BID_H
#define BID_H

#include <cstdint>

enum Type {
    buy = 0,
    sell = 1
};

// Структура заявки, которая подается на вход стакану
template <typename T>
struct Bid {
    T price;
    uint64_t count;
    Type type;
};

// Структура заявки, которая находится в стакане. ID присваивается стаканом
template <typename T>
struct BidWithID : Bid<T> {
    uint64_t id;
    BidWithID() {} // Необходим для operator[] unordered_map
    BidWithID(const uint64_t &id, const T &price, const uint64_t &count, const Type &type) {
        this->id = id; 
        this->price = price; 
        this->count =count; 
        this->type = type;
    }
    bool operator<(BidWithID<T> const &other) const { return (this->type == sell) ?  this->price < other.price : this->price > other.price; }
};

#endif