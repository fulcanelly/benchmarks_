#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <functional>
#include <unistd.h>
#include <random>

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;


//changelog:
// * get rid of std rand
// * add threading 
// * again using std rand but mt19937
//todo
// * redo threading


unsigned long myrand() {          
    thread_local std::random_device seeder;
    thread_local std::mt19937 engine(seeder());
    thread_local std::uniform_int_distribution<int> dist(0, 100);

    return dist(engine);
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
    cout << "starting F " << range.first << " to " << range.second << endl; 
    
    for (size_t i = range.first; i < range.second; i++) {
        array[i] = gen();
        
    }
    
    cout << "ednging " << range.first << " to " << range.second << endl; 
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
    cout << "starting C " << range.first << " to " << range.second << endl; 

    for (size_t i = range.first; i < range.second; i++) {
        result[array[i]]++;
    }
    cout << "ednging " << range.first << " to " << range.second << endl; 

}

static auto avaliable_threads = std::thread::hardware_concurrency();


auto do_job(
    vector<char>& array,
    const std::function<char()>& gen
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
                [] (
                    vector<char>& array, 
                    std::pair<int, int> range,  
                    const std::function<char()>& gen,
                    map<char, int>& result
                ) {
                    fill_chunk(array, range, gen);
                    count_chunk_occurances(array, range, result);
                },
                std::ref(array),
                std::pair<int, int>{ i, i + chunk_size }, 
                gen,
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

    auto result = do_job(list, gen);

    cout << "Result:" << endl;
    print_result(result);
}


int main() {
    run();
}