#ifndef STOCKDEQUE_H
#define STOCKDEQUE_H

#include <cstdint>
#include <iostream>
#include <queue>
#include <algorithm>

#include <string>
#include "bid.h"
#include "stock.h"

/*
    Наивная реализация через очередь и сортировку
    Вставка                     O(1)
    Удаление/Редактирование     O(n) или O(ln(n)) в худшем случае
    Вывод (сортировка)          O(n*log(n))
*/

template <typename T>
class StockDeque : Stock<T> {
    public:
        // Добавление в один из двух deque в зависимости от типа заявки и возвращение uid
        const uint64_t Add(const struct Bid<T>& bid) override {
            if (bid.price < 0.01 || bid.count == 0) {
                return -1;
            }
            if (bid.type == buy) {
                struct BidWithID<T> bid_uniqie {last_buy_id, bid.price, bid.count, bid.type};
                stock_buy.push_back(bid_uniqie);
                return last_buy_id++;
            } else if (bid.type == sell) {
                struct BidWithID<T> bid_uniqie {last_sell_id, bid.price, bid.count, bid.type};
                stock_sell.push_back(bid_uniqie);
                return last_sell_id++;
            }
            return -1;
        }

        // Редактирование одной из двух deque на основе uid и bid.type
        const bool Edit(const uint64_t &id, const struct Bid<T>& bid) override {
            bool edited = false;
            if (bid.type == buy) {
                edited = editElement(id, stock_buy, bid);
            } else if (bid.type == sell) {
                edited = editElement(id, stock_sell, bid);
            }
            return edited;
        }

        // Удаление элемента одной из двух deque на основе uid и type
        const bool Remove(const uint64_t &id, const Type& type) override {
            bool removed = false;
            if (type == buy) {
                stock_buy = removeElement(id, stock_buy, removed);
            } else if (type == sell) {
                stock_sell = removeElement(id, stock_sell, removed);
            }
            return removed;
        }

        // Вызов функции печати топ10 заявок для deque покупки и deque продажи
        const std::string ShowTop() override {
            std::string result {};
            std::cout << "\nTop 10 buy binds: \n";
            std::sort(std::begin(this->stock_buy), std::end(this->stock_buy));
            result += printDeque(this->stock_buy);

            std::cout << "Top 10 sell binds: \n";
            std::sort(std::begin(this->stock_sell), std::end(this->stock_sell));
            result += printDeque(this->stock_sell);
            return result;
        }
    private:
        // Печать топ10 заявок с суммированием одинаковых заявок
        std::string printDeque(const std::deque<BidWithID<T>> &deq) {
            std::string result {};
            if (deq.empty()) {
                return result;
            }

            // Для суммирования цен заявок в дальнейшем
            auto it = deq.begin();
            auto last = *it;

            // Если только одна заявка в стакане
            if (it == deq.end()) {
                std::string data = "\t" + std::to_string(last.price) + " " + std::to_string(last.count) + " " + ((last.type == buy) ? "buy" : "sell") + "\n";
                std::cout << data;
                result += data;
                return result;
            }

            // Суммируем заявки с одинаковой ценой
            for (size_t i = 0; it != deq.end() && i < 10; ++it) {
                if (it->price != last.price) {
                    std::string data = "\t" + std::to_string(last.price) + " " + std::to_string(last.count) + " " + ((last.type == buy) ? "buy" : "sell") + "\n";
                    std::cout << data;
                    result += data;
                    last = *it;
                    i++;
                } else {
                    last.count += it->count;
                    // Выводим последний элемент
                    if (deq.size() == 1) {
                        std::string data = "\t" + std::to_string(last.price) + " " + std::to_string(last.count) + " " + ((last.type == buy) ? "buy" : "sell") + "\n";
                        std::cout << data;
                        result += data;
                    }
                }
            }
            return result;
        }

        // Удаление элемента в переданной deque с ее возвратом
        std::deque<BidWithID<T>> removeElement(const uint64_t &id, std::deque<BidWithID<T>> deq, bool &removed) {
            auto it = std::find_if(deq.begin(), deq.end(), [id](const BidWithID<T>& bid) { return bid.id == id; });
            if (it != deq.end()) {
                deq.erase(it);
                removed = true;
            } else {
                removed = false;
            }
            return deq;
        }

        // Редактирование deque через [] и iterator
        bool editElement(const uint64_t &id, std::deque<BidWithID<T>> &deq, const struct Bid<T>& bid) {
            bool found = false;
            int i = 0;
            for (auto it = deq.begin(); it != deq.end() && !found; ++it, i++) {
                if (it->id == id) {
                    BidWithID<T> newBidWithID(id, bid.price, bid.count, bid.type);
                    deq[i] = newBidWithID;
                    found = true;
                }
            }
            
            return found;
        }

        std::deque<BidWithID<T>> stock_buy;
        uint64_t last_buy_id {0};

        std::deque<BidWithID<T>> stock_sell;
        uint64_t last_sell_id {0};
};

#endif