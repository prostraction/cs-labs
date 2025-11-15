#ifndef STOCKMULTISET_H
#define STOCKMULTISET_H

#include "bid.h"
#include "stock.h"
#include <string>
#include <set>

/*
    Реализация через multiset (дерево)
    Вставка                     O(ln(n))
    Удаление/Редактирование     O(n) или O(ln(n)) -- используется id для поиска элемента, а не компаратор
    Вывод                       O(m заявок с одинаковой ценой ~= min O(10) ~= O(1))
*/

template <typename T>
class StockMultiset : Stock<T> {
    public:
        const uint64_t Add(const struct Bid<T>& bid) {
            if (bid.price < 0.01 || bid.count == 0) {
                return -1;
            }
            if (bid.type == Type::buy) {
                struct BidWithID<T> bid_uniqie {last_buy_id, bid.price, bid.count, bid.type};
                stock_buy.insert(bid_uniqie);
                return last_buy_id++;
            } else if (bid.type == Type::sell) {
                struct BidWithID<T> bid_uniqie {last_sell_id, bid.price, bid.count, bid.type};
                stock_sell.insert(bid_uniqie);
                return last_sell_id++;
            }
            return -1;
        }
        const bool Edit(const uint64_t &id, const struct Bid<T>& bid) {
            auto find_predicate = [id](const BidWithID<T>& bid) { return bid.id == id; };
            struct BidWithID<T> bid_uniqie {id, bid.price, bid.count, bid.type};
            if (bid.type == Type::buy) {
                auto it = std::find_if(stock_buy.begin(), stock_buy.end(), find_predicate);
                if (it != stock_buy.end()) {
                    stock_buy.erase(it);
                    stock_buy.insert(bid_uniqie);
                    return true;
                }
            } else if (bid.type == Type::sell) {
                auto it = std::find_if(stock_sell.begin(), stock_sell.end(), find_predicate);
                if (it != stock_sell.end()) {
                    stock_sell.erase(it);
                    stock_sell.insert(bid_uniqie);
                    return true;
                }
            }
            return false;
        }
        const bool Remove(const uint64_t &id, const Type& type) {
            auto find_predicate = [id](const BidWithID<T>& bid) { return bid.id == id; };
            if (type == Type::buy) {
                auto it = std::find_if(stock_buy.begin(), stock_buy.end(), find_predicate);
                if (it != stock_buy.end()) {
                    stock_buy.erase(it);
                    return true;
                }
            } else if (type == Type::sell) {
                auto it = std::find_if(stock_sell.begin(), stock_sell.end(), find_predicate);
                if (it != stock_sell.end()) {
                    stock_sell.erase(it);
                    return true;
                }
            }
            return false;
        }
        const std::string ShowTop() {
            std::string result {};
            std::cout << "\nTop 10 buy binds: \n";
            result += printMultiset(this->stock_buy);

            std::cout << "Top 10 sell binds: \n";
            result += printMultiset(this->stock_sell);
            return result;
        }

    private:
        std::string printMultiset(const std::multiset<BidWithID<T>> &set) {
            std::string result {};
            if (set.empty()) {
                return result;
            }

            // Для суммирования цен заявок в дальнейшем
            auto it = set.begin();
            auto last = *it;

            // Если только одна заявка в стакане
            if (it == set.end()) {
                std::string data = "\t" + std::to_string(last.price) + " " + std::to_string(last.count) + " " + ((last.type == buy) ? "buy" : "sell") + "\n";
                std::cout << data;
                result += data;
                return result;
            }

            // Суммируем заявки с одинаковой ценой
            for (size_t i = 0; it != set.end() && i < 10; ++it) {
                if (it->price != last.price) {
                    std::string data = "\t" + std::to_string(last.price) + " " + std::to_string(last.count) + " " + ((last.type == buy) ? "buy" : "sell") + "\n";
                    std::cout << data;
                    result += data;
                    last = *it;
                    i++;
                } else {
                    last.count += it->count;
                    // Выводим последний элемент
                    if (set.size() == 1) {
                        std::string data = "\t" + std::to_string(last.price) + " " + std::to_string(last.count) + " " + ((last.type == buy) ? "buy" : "sell") + "\n";
                        std::cout << data;
                        result += data;
                    }
                }
            }
            return result;
        }

        BidWithID<T> searchMultiset(const std::multiset<BidWithID<T>> &multiset, const uint64_t &id, bool &found) {
            auto it = multiset.begin();
            for (; it != multiset.end(); ++it) {
                if (it->id == id) {
                    found = true;
                    break;
                }
            }
            return *it;
        }
        std::multiset<BidWithID<T>> stock_buy;
        uint64_t last_buy_id {0};

        std::multiset<BidWithID<T>> stock_sell;
        uint64_t last_sell_id {0};
};

#endif