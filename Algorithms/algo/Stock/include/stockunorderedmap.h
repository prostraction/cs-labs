#ifndef STOCKUNORDEREDMAP_H
#define STOCKUNORDEREDMAP_H

#include "bid.h"
#include "stock.h"
#include <string>
#include <unordered_map>

/*
    Реализация через unordered_map (хеш-таблица)
    Вставка                     в теории O(1), по факту сильно дольше (коллизия/размер)
    Удаление/Редактирование     O(1)
    Вывод                       O(n*log(n))
*/

template <typename T>
class StockUnorderedMap : Stock<T> {
    public:
        StockUnorderedMap() {
            /*stock_buy.reserve(10000000);
            stock_buy.max_load_factor(0.25);
            stock_sell.reserve(10000000);
            stock_sell.max_load_factor(0.25);*/
        }

        const uint64_t Add(const struct Bid<T>& bid) {
            if (bid.price < 0.01 || bid.count == 0) {
                return -1;
            }
            if (bid.type == Type::buy) {
                struct BidWithID<T> bid_uniqie {last_buy_id, bid.price, bid.count, bid.type};
                stock_buy[last_buy_id] = bid_uniqie;
                return last_buy_id++;
            } else if (bid.type == Type::sell) {
                struct BidWithID<T> bid_uniqie {last_sell_id, bid.price, bid.count, bid.type};
                stock_sell[last_sell_id] = bid_uniqie;
                return last_sell_id++;
            }
            return -1;
        }
        const bool Edit(const uint64_t &id, const struct Bid<T>& bid) {
            struct BidWithID<T> bid_uniqie {id, bid.price, bid.count, bid.type};
            if (bid.type == Type::buy) {
                stock_buy[id] = bid_uniqie;
                return true;
            } else if (bid.type == Type::sell) {
                stock_sell[id] = bid_uniqie;
                return true;
            }
            return false;
        }
        const bool Remove(const uint64_t &id, const Type& type) {
            bool found = false;
            if (type == Type::buy) {
                stock_buy.erase(id);
            } else if (type == Type::sell) {
                stock_sell.erase(id);
            }
            return found;
        }
        const std::string ShowTop() {
            auto comp = [&](const auto& m, const auto& n)-> bool {
                return (m.second.type == sell) ?  m.second.price < n.second.price : m.second.price > n.second.price;
            };

            std::string result {};
            std::cout << "\nTop 10 buy binds: \n";
            std::vector<std::pair<uint64_t, BidWithID<T>>> sorted_elements_buy(stock_buy.begin(), stock_buy.end());
            std::sort(sorted_elements_buy.begin(), sorted_elements_buy.end(), comp);
            result += printSortedVector(sorted_elements_buy);

            std::cout << "Top 10 sell binds: \n";
            std::vector<std::pair<uint64_t, BidWithID<T>>> sorted_elements_sell(stock_sell.begin(), stock_sell.end());
            std::sort(sorted_elements_sell.begin(), sorted_elements_sell.end(), comp);
            result += printSortedVector(sorted_elements_sell);
            return result;
        }

    private:
        std::string printSortedVector(const std::vector<std::pair<uint64_t, BidWithID<T>>> &vec) {
            std::string result {};
            if (vec.empty()) {
                return result;
            }

            // Для суммирования цен заявок в дальнейшем
            auto it = vec.begin();
            auto last = *it;

            // Если только одна заявка в стакане
            if (it == vec.end()) {
                std::string data = "\t" + std::to_string(last.second.price) + " " +  std::to_string(last.second.count) + " " + ((last.second.type == buy) ? "buy" : "sell") + "\n";
                std::cout << data;
                result += data;
                return result;
            }

            // Суммируем заявки с одинаковой ценой
            for (size_t i = 0; it != vec.end() && i < 10; ++it) {
                if (it->second.price != last.second.price) {
                    std::string data = "\t" + std::to_string(last.second.price) + " " +  std::to_string(last.second.count) + " " + ((last.second.type == buy) ? "buy" : "sell") + "\n";
                    std::cout << data;
                    result += data;
                    last = *it;
                    i++;
                } else {
                    last.second.count += it->second.count;
                    // Выводим последний элемент
                    if (vec.size() == 1) {
                        std::string data = "\t" + std::to_string(last.second.price) + " " +  std::to_string(last.second.count) + " " + ((last.second.type == buy) ? "buy" : "sell") + "\n";
                        std::cout << data;
                        result += data;
                    }
                }
            }
            return result;
        }

        std::unordered_map<uint64_t, BidWithID<T>> stock_buy;
        uint64_t last_buy_id {0};

        std::unordered_map<uint64_t, BidWithID<T>> stock_sell;
        uint64_t last_sell_id {0};
};

#endif