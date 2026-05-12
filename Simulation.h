#pragma once
#include <vector>
#include <queue>
#include <set>
#include <iostream>

enum EventType {
    FINISH = 0,
    START = 1,
    WAIT = 2,
    READY = 3
};

struct Event {
    long long time;
    EventType type;
    int k, i, j, p;
    bool operator>(const Event& other) const {
        if (time != other.time) return time > other.time;
        if (type != other.type) return type > other.type;
        return k > other.k;
    }
};

struct Item {
    int k, type;
};

class Simulation {
private:
    int m;
    int n;
    std::vector<std::vector<int>> t_table;
    std::vector<std::queue<Item>> machine_queues;
    std::vector<long long> current_load;

    std::vector<bool> is_busy;
    std::set<std::pair<long long, int>> machine_loads;
    std::priority_queue<Event, std::vector<Event>, std::greater<>> events;

    void update_load(int mj, long long delta);
    void try_assign(int mj, long long cur_t);

public:
    Simulation(int m, int n, 
               const std::vector<std::vector<int>>& t_table,
               const std::vector<std::queue<Item>>& machine_queues,
               const std::vector<long long>& current_load);

    void run();
};