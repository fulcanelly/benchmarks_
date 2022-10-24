#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <functional>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;


//changelog:
// * get rid of std rand
// * add threading 

//todo
// * redo threading

unsigned long myrand() {          
    static thread_local unsigned long x=123456789, y=362436069, z=521288629;

    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

  return z;
}


template<class T>
auto count_occurances(const vector<T>& list) -> map<T, int> {
    map<char, int> result = {}; 

    for (auto sym : list) {
        result[sym]++;
    }

    return result;
}

template<class T>
void print_result(const map<T, int> stats) {
    cout << "{" << endl;
    for (const auto& [sym, count] : stats) {
        cout << "(" << (int)sym << "): " << count << endl;
    }
    cout << "}" << endl;

}



void fill_chunk(
    vector<char>& array, 
    std::pair<int, int> range,
    const std::function<char()>& gen
) {
    cout << "starting " << range.first << " to " << range.second << endl; 
    
    for (size_t i = range.first; i < range.second; i++) {
        array[i] = gen();
        
    }
    
    cout << "ednging " << range.first << " to " << range.second << endl; 

}

static auto avaliable_threads = std::thread::hardware_concurrency();

auto fill_in_parts(
    vector<char>& array, 
    const std::function<char()>& gen
) -> void {

    auto chunk_size = array.size() / avaliable_threads; 

    vector<std::thread> threads;
    
    for (size_t i = 0; i < array.size(); i += chunk_size) {
        threads.push_back(
            std::thread{ 
                fill_chunk,
                std::ref(array), 
                std::pair<int, int>{ i, i + chunk_size }, 
                gen 
            }
        );
    }

    for (auto&& thr : threads) {
        thr.join();
    }
    
    
}


template<class T, class C>
auto merge_count_maps(
    map<T, C> one, 
    map<T, C> another
) -> map<T, C> {
    map<T, C> result = one;

    for (const auto &[key, value] : another) {
        result[key] += value;
    }

    return result;
}

auto count_chunk_occurances(
    vector<char>& array,
    std::pair<int, int> range,
    map<char, int>& result
) {
    for (size_t i = range.first; i < range.second; i++) {
        result[array[i]]++;
    }
}

auto count_in_parts(
    vector<char>& array
) -> map<char, int> {
    vector< map<char, int> > stats_lists = {};
    vector< std::thread > threads;

    auto chunk_size = array.size() / avaliable_threads; 
     

    //this line is super important
    //if remove it everything will break
    // why ? relocations on push back...
    stats_lists.reserve(avaliable_threads);

    cout << "Init parts.." << endl;
    
    for (size_t i = 0; i < array.size(); i += chunk_size) {
        stats_lists.push_back({});

        threads.push_back(
            std::thread(
                count_chunk_occurances,
                std::ref(array),
                std::pair<int, int>{ i, i + chunk_size }, 
                std::ref(stats_lists.back())
            )
        );
    }
    
    cout << "Waiting..." << endl;
    for (auto&& thr : threads) {
        thr.join();
    }
    

    cout << "Merging ..." << endl;
    map<char, int> result;

    for (const auto stat : stats_lists) {
        result = merge_count_maps(
            result, stat
        );
    }
    
    return result;
}

void run() {
    const char lib[4] = {32, 12, 77, 1};

    auto size = 1000 * 1000 * 1000;
    
    vector<char> list = vector<char>(); 
    list.resize(size);

    cout << "Generating list..." << endl;

    auto gen = [&] {
        return lib[myrand() % 4];
    };

    fill_in_parts(list, gen);


    cout << "Counting..." << endl;

    auto result = count_in_parts(list);

    cout << "Result:" << endl;
    print_result(result);
}


int main() {
    run();
}