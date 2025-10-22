#ifndef STOCK_H
#define STOCK_H

#include "bid.h"

template <typename T>
class Stock {
    protected:
        // Функция Add возвращает уникальный номер заявки
        virtual const uint64_t Add(const struct Bid<T>& bid) = 0;
        virtual const bool Edit(const uint64_t &id, const struct Bid<T>& bid) = 0;
        virtual const bool Remove(const uint64_t &id, const Type& type) = 0;
        // Для тестов вывод также передается в std::string
        virtual const std::string ShowTop() = 0; 
        Stock() {}
};

#endif