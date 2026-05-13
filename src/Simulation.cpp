#include "Simulation.h"

Simulation::Simulation(int m, int n,
                       const std::vector<std::vector<int>>& t_table,
                       const std::vector<std::queue<Item>>& machine_queues,
                       const std::vector<long long>& current_load)
    : m(m), n(n),
      t_table(t_table),
      machine_queues(machine_queues),
      current_load(current_load),
      is_busy(n, false) {
}

void Simulation::update_load(int mj, long long delta) {
    machine_loads.erase({current_load[mj], -mj});
    current_load[mj] += delta;
    machine_loads.insert({current_load[mj], -mj});
}

void Simulation::try_assign(int mj, long long cur_t) {
    if (!is_busy[mj] && !machine_queues[mj].empty()) {
        auto [k, type] = machine_queues[mj].front();
        machine_queues[mj].pop();
        is_busy[mj] = true;
        update_load(mj, -t_table[type][mj]);
        events.push({cur_t, START, k, type, mj, 0});
    }
}

void Simulation::run() {
    for (int j = 0; j < n; ++j) {
        machine_loads.insert({current_load[j], -j});
    }

    for (int j = 0; j < n; ++j) {
        try_assign(j, 0);
    }

    long long last_time = 0;

    while (!events.empty()) {
        auto [time, type, k, i, j, p] = events.top();
        events.pop();
        last_time = time;

        if (type == START) {
            std::cout << "start " << time << " " << k << " " << i << " " << j << "\n";
            events.push({time + t_table[i][j], FINISH, k, i, j, 0});
        } else if (type == FINISH) {
            std::cout << "finish " << time << " " << k << " " << i << " " << j << "\n";
            is_busy[j] = false;

            if (i + 1 == m - 1) {
                events.push({time, READY, k, 0, j, 0});
            } else {
                auto best = machine_loads.begin();
                int bj = -best->second;
                events.push({time, WAIT, k, i + 1, bj,
                             static_cast<int>(machine_queues[bj].size())});
                machine_queues[bj].push({k, i + 1});
                update_load(bj, t_table[i + 1][bj]);
                try_assign(bj, time);
            }
            try_assign(j, time);
        } else if (type == WAIT) {
            std::cout << "wait " << time << " " << k << " " << i << " " << j
                << " " << p << "\n";
        } else if (type == READY) {
            std::cout << "ready " << time << " " << k << " " << j << "\n";
        }
    }

    std::cout << "stop " << last_time << std::endl;
}