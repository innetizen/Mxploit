#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>

std::unordered_map<int,int> g_map;

void worker(int id, int key)
{
    // simulate read -> db -> emplace sequence
    auto it = g_map.find(key);
    if (it == g_map.end()) {
        // simulate DB read latency
        std::this_thread::sleep_for(std::chrono::milliseconds(10 * (id+1)));
        // concurrent emplace without synchronization
        g_map.emplace(key, id + 100);
        std::cout << "thread " << id << " emplaced key " << key << " -> " << (id+100) << "\n";
    }
    else {
        std::cout << "thread " << id << " found key " << key << " -> " << it->second << "\n";
    }
}

int main()
{
    std::cout << "Starting reproducer with unsynchronized unordered_map access\n";
    std::thread t1(worker, 1, 42);
    std::thread t2(worker, 2, 42);
    t1.join();
    t2.join();
    std::cout << "Final map size: " << g_map.size() << "\n";
    return 0;
}
