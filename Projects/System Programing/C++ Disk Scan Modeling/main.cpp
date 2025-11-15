#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <map>

// Константы моделирования
constexpr int TRACKS_PER_HEAD = 125;      // Дорожек на головку (500/4)
constexpr int TOTAL_TRACKS = 500;         // Всего дорожек
constexpr int SECTORS_PER_TRACK = 16;     // Секторов на дорожке
constexpr int HEADS_COUNT = 4;            // Количество головок
constexpr double SEEK_TIME_PER_TRACK = 0.5; // Время перехода между дорожками (мс)
constexpr double RPM = 10000.0;           // Оборотов в минуту
constexpr double ROTATION_TIME = 60000.0 / RPM; // Время одного оборота (мс)
constexpr double SECTOR_READ_TIME = ROTATION_TIME / SECTORS_PER_TRACK; // Время чтения сектора
constexpr double MODELING_TIME = 300000.0; // Время моделирования 5 минут (мс)

// Типы операций
enum class OperationType { READ, WRITE };

// Структура для описания запроса
struct DiskRequest {
    int cylinder;           // Номер цилиндра (0-499)
    int head;               // Номер головки (0-3)
    int sector;             // Номер сектора (0-15)
    OperationType type;     // Тип операции
    double arrival_time;    // Время поступления запроса
    double service_time;    // Время обслуживания запроса
    bool processed;         // Флаг обработки запроса
    
    DiskRequest(int cyl, int hd, int sec, OperationType t, double arrival)
        : cylinder(cyl), head(hd), sector(sec), type(t), 
          arrival_time(arrival), service_time(0.0), processed(false) {}
};

// Структура для описания головки диска
struct DiskHead {
    int current_cylinder;   // Текущая позиция головки
    int current_sector;     // Текущий сектор
    bool direction;         // Направление движения (true - к большим номерам)
    
    DiskHead() : current_cylinder(0), current_sector(0), direction(true) {}
    
    // Расчет времени поиска дорожки
    double calculateSeekTime(int target_cylinder) const {
        return std::abs(target_cylinder - current_cylinder) * SEEK_TIME_PER_TRACK;
    }
    
    // Расчет времени ожидания сектора
    double calculateRotationalLatency(int target_sector) const {
        int sector_diff = (target_sector - current_sector + SECTORS_PER_TRACK) % SECTORS_PER_TRACK;
        return (sector_diff * ROTATION_TIME) / SECTORS_PER_TRACK;
    }
    
    // Расчет времени операции
    double calculateOperationTime(const DiskRequest& request) const {
        double seek_time = calculateSeekTime(request.cylinder);
        double latency = calculateRotationalLatency(request.sector);
        double operation_time = (request.type == OperationType::READ) ? 
            SECTOR_READ_TIME : 2 * SECTOR_READ_TIME; // Запись требует верификации
        
        return seek_time + latency + operation_time;
    }
    
    // Обновление позиции после обслуживания запроса
    void updatePosition(const DiskRequest& request) {
        current_cylinder = request.cylinder;
        current_sector = (request.sector + 1) % SECTORS_PER_TRACK;
    }
};

// Класс для генерации случайных запросов
class RequestGenerator {
private:
    std::mt19937 generator;
    std::uniform_real_distribution<double> time_dist;
    std::uniform_int_distribution<int> cylinder_dist;
    std::uniform_int_distribution<int> head_dist;
    std::uniform_int_distribution<int> sector_dist;
    std::bernoulli_distribution type_dist;
    
public:
    RequestGenerator(double max_arrival_time)
        : generator(std::chrono::steady_clock::now().time_since_epoch().count()),
          time_dist(0.0, max_arrival_time),
          cylinder_dist(0, TOTAL_TRACKS - 1),
          head_dist(0, HEADS_COUNT - 1),
          sector_dist(0, SECTORS_PER_TRACK - 1),
          type_dist(0.5) {} // Равная вероятность чтения/записи
    
    // Генерация случайного запроса
    std::unique_ptr<DiskRequest> generateRequest() {
        double arrival_time = time_dist(generator);
        int cylinder = cylinder_dist(generator);
        int head = head_dist(generator);
        int sector = sector_dist(generator);
        OperationType type = type_dist(generator) ? OperationType::READ : OperationType::WRITE;
        
        return std::make_unique<DiskRequest>(cylinder, head, sector, type, arrival_time);
    }
};

// Базовый класс для стратегий планирования
class SchedulingStrategy {
protected:
    std::vector<DiskHead> heads;
    double total_idle_time;
    int max_queue_length;
    
public:
    SchedulingStrategy() : heads(HEADS_COUNT), total_idle_time(0.0), max_queue_length(0) {}
    virtual ~SchedulingStrategy() = default;
    
    // Основной метод выполнения стратегии
    virtual void processRequests(std::vector<std::unique_ptr<DiskRequest>>& requests, 
                               std::vector<double>& service_times) = 0;
    
    // Получение статистики
    virtual void getStatistics(double& idle_time, int& max_queue) const {
        idle_time = total_idle_time;
        max_queue = max_queue_length;
    }
    
    // Сброс состояния головок
    virtual void resetHeads() {
        for (auto& head : heads) {
            head = DiskHead();
        }
    }
};

// Стратегия FIFO (First-In-First-Out)
class FIFOStrategy : public SchedulingStrategy {
public:
    void processRequests(std::vector<std::unique_ptr<DiskRequest>>& requests,
                        std::vector<double>& service_times) override {
        double current_time = 0.0;
        int current_queue_length = 0;
        max_queue_length = 0;
        total_idle_time = 0.0;
        
        // Обработка запросов в порядке поступления
        for (auto& request : requests) {
            if (request->arrival_time > current_time) {
                // Простой системы
                total_idle_time += (request->arrival_time - current_time);
                current_time = request->arrival_time;
                current_queue_length = 0;
            } else {
                current_queue_length++;
                max_queue_length = std::max(max_queue_length, current_queue_length);
            }
            
            DiskHead& head = heads[request->head];
            double operation_time = head.calculateOperationTime(*request);
            request->service_time = current_time + operation_time - request->arrival_time;
            service_times.push_back(request->service_time);
            
            head.updatePosition(*request);
            current_time += operation_time;
            request->processed = true;
        }
    }
};

// Стратегия SCAN
class SCANStrategy : public SchedulingStrategy {
public:
    void processRequests(std::vector<std::unique_ptr<DiskRequest>>& requests,
                        std::vector<double>& service_times) override {
        double current_time = 0.0;
        max_queue_length = 0;
        total_idle_time = 0.0;
        
        // Обработка для каждой головки отдельно
        for (int head_id = 0; head_id < HEADS_COUNT; ++head_id) {
            // Фильтрация запросов для текущей головки
            std::vector<DiskRequest*> head_requests;
            for (auto& req : requests) {
                if (req->head == head_id && !req->processed) {
                    head_requests.push_back(req.get());
                }
            }
            
            // Сортировка запросов по цилиндрам
            std::sort(head_requests.begin(), head_requests.end(),
                     [](const DiskRequest* a, const DiskRequest* b) {
                         return a->cylinder < b->cylinder;
                     });
            
            processHeadRequests(head_id, head_requests, current_time, service_times);
        }
    }

private:
    void processHeadRequests(int head_id, std::vector<DiskRequest*>& head_requests,
                            double& current_time, std::vector<double>& service_times) {
        if (head_requests.empty()) return;
        
        DiskHead& head = heads[head_id];
        int current_idx = 0;
        int queue_length = 0;
        
        // Поиск ближайшего запроса к текущей позиции
        auto findNearestRequest = [&](bool direction) -> int {
            int best_idx = -1;
            int min_distance = TOTAL_TRACKS;
            
            for (int i = 0; i < head_requests.size(); ++i) {
                if (head_requests[i]->processed) continue;
                
                int distance = head_requests[i]->cylinder - head.current_cylinder;
                if ((direction && distance >= 0) || (!direction && distance <= 0)) {
                    int abs_distance = std::abs(distance);
                    if (abs_distance < min_distance) {
                        min_distance = abs_distance;
                        best_idx = i;
                    }
                }
            }
            
            return best_idx;
        };
        
        while (true) {
            int next_idx = findNearestRequest(head.direction);
            
            // Если в текущем направлении нет запросов, меняем направление
            if (next_idx == -1) {
                head.direction = !head.direction;
                next_idx = findNearestRequest(head.direction);
                
                // Если и в обратном направлении нет запросов - завершаем
                if (next_idx == -1) break;
            }
            
            DiskRequest* request = head_requests[next_idx];
            
            // Учет времени поступления запроса
            if (request->arrival_time > current_time) {
                total_idle_time += (request->arrival_time - current_time);
                current_time = request->arrival_time;
                queue_length = 0;
            } else {
                queue_length++;
                max_queue_length = std::max(max_queue_length, queue_length);
            }
            
            // Обслуживание запроса
            double operation_time = head.calculateOperationTime(*request);
            request->service_time = current_time + operation_time - request->arrival_time;
            service_times.push_back(request->service_time);
            
            head.updatePosition(*request);
            current_time += operation_time;
            request->processed = true;
        }
    }
};

// Класс для анализа результатов
class ResultAnalyzer {
public:
    static void analyzeResults(const std::vector<double>& service_times,
                             double idle_time, int max_queue,
                             const std::string& strategy_name) {
        if (service_times.empty()) {
            std::cout << "No data to analyze for " << strategy_name << std::endl;
            return;
        }
        
        // Базовая статистика
        double min_time = *std::min_element(service_times.begin(), service_times.end());
        double max_time = *std::max_element(service_times.begin(), service_times.end());
        double sum = std::accumulate(service_times.begin(), service_times.end(), 0.0);
        double mean = sum / service_times.size();
        
        // Стандартное отклонение
        double variance = 0.0;
        for (double time : service_times) {
            variance += (time - mean) * (time - mean);
        }
        variance /= service_times.size();
        double std_dev = std::sqrt(variance);
        
        // Гистограмма
        std::map<int, int> histogram;
        int bins = 10;
        double bin_width = (max_time - min_time) / bins;
        
        for (double time : service_times) {
            int bin = static_cast<int>((time - min_time) / bin_width);
            if (bin == bins) bin--; // Последний bin включает max_time
            histogram[bin]++;
        }
        
        // Вывод результатов
        std::cout << "\n=== " << strategy_name << " Results ===" << std::endl;
        std::cout << "Min service time: " << min_time << " ms" << std::endl;
        std::cout << "Max service time: " << max_time << " ms" << std::endl;
        std::cout << "Mean service time: " << mean << " ms" << std::endl;
        std::cout << "Standard deviation: " << std_dev << " ms" << std::endl;
        std::cout << "Max queue length: " << max_queue << std::endl;
        std::cout << "Total idle time: " << idle_time << " ms" << std::endl;
        
        std::cout << "\nHistogram:" << std::endl;
        for (const auto& [bin, count] : histogram) {
            double lower = min_time + bin * bin_width;
            double upper = min_time + (bin + 1) * bin_width;
            std::cout << "[" << lower << " - " << upper << "): " << count << std::endl;
        }
    }
};

// Основной класс моделирования
class DiskSubsystemModel {
private:
    std::vector<std::unique_ptr<DiskRequest>> requests;
    RequestGenerator generator;
    
public:
    DiskSubsystemModel(double max_arrival_time) : generator(max_arrival_time) {}
    
    // Генерация запросов в течение времени моделирования
    void generateRequests() {
        requests.clear();
        double current_time = 0.0;
        
        while (current_time < MODELING_TIME) {
            auto request = generator.generateRequest();
            current_time = request->arrival_time;
            if (current_time < MODELING_TIME) {
                requests.push_back(std::move(request));
            }
        }
        
        std::cout << "Generated " << requests.size() << " requests" << std::endl;
    }
    
    // Запуск моделирования для обеих стратегий
    void runSimulation() {
        // Тестирование для разных значений t_max
        std::vector<double> t_max_values = {5000.0, 500.0, 50.0}; // t_max, t_max/10, t_max/100
        
        for (double t_max : t_max_values) {
            std::cout << "\n\n=== Testing with t_max = " << t_max << " ms ===" << std::endl;
            
            // Перегенерация запросов с новым t_max
            generator = RequestGenerator(t_max);
            generateRequests();
            
            // Копирование запросов для каждой стратегии
            auto requests_fifo = copyRequests();
            auto requests_scan = copyRequests();
            
            // FIFO стратегия
            {
                FIFOStrategy fifo;
                std::vector<double> service_times;
                fifo.processRequests(requests_fifo, service_times);
                
                double idle_time;
                int max_queue;
                fifo.getStatistics(idle_time, max_queue);
                
                ResultAnalyzer::analyzeResults(service_times, idle_time, max_queue, "FIFO");
            }
            
            // SCAN стратегия
            {
                SCANStrategy scan;
                std::vector<double> service_times;
                scan.processRequests(requests_scan, service_times);
                
                double idle_time;
                int max_queue;
                scan.getStatistics(idle_time, max_queue);
                
                ResultAnalyzer::analyzeResults(service_times, idle_time, max_queue, "SCAN");
            }
        }
    }

private:
    // Создание копии запросов для независимого тестирования стратегий
    std::vector<std::unique_ptr<DiskRequest>> copyRequests() const {
        std::vector<std::unique_ptr<DiskRequest>> copy;
        for (const auto& req : requests) {
            copy.push_back(std::make_unique<DiskRequest>(*req));
        }
        return copy;
    }
};

int main() {
    try {
        // Инициализация генератора случайных чисел
        std::srand(std::time(nullptr));
        
        // Запуск моделирования с начальным t_max = 5000 мс
        DiskSubsystemModel model(5000.0);
        model.runSimulation();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}