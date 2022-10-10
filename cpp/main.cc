#include <iostream>
#include <map>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;

template<class T>
auto count_occurances(const vector<T>& list) -> map<T, int> {
    map<char, int> result = {}; 

    for (auto sym : list) {
        result[sym]++;
    }

    return result;
}


void print_result(const map<char, int>& stats) {
    for (const auto& [sym, count] : stats) {
        cout << "(" << sym << "): " << count << endl;
    }
}


void run() {
    const char lib[4] = {'1','2','3','4'};

    auto size = 1000 * 1000 * 1000;
    
    vector<char> list;
    list.reserve(size);

    cout << "Generating list..." << endl;

    for (size_t i = 0; i < size; i++) {
        list.push_back(lib[rand() % 4]);
    }

    cout << "Counting..." << endl;
    auto result = count_occurances(list);

    cout << "Result:" << endl;
    print_result(result);
}

int main() {
   run();
}