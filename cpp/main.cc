#include <iostream>
#include <map>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;

map<char, int> count_occurances(const string& str) {
    
    map<char, int> result = {}; 

    for (auto sym : str) {
        result[sym]++;
    }
    cout << "returning" << endl;
    return result;
}


map<char, int> count_occurances(int size, char* ptr) {
    
    map<char, int> result = {}; 

    for (size_t i = 0; i < size; i++) {
        auto sym = ptr[i];
        result[sym]++;
    }

    cout << "returning" << endl;
    return result;
}

void print_result(const map<char, int>& stats) {
    for (const auto& [sym, count] : stats) {
        cout << "(" << sym << "): " << count << endl;
    }

}


void c_str_test() {
    char symbols[4] = {'1','2','3','4'};
    auto size = 1000 * 1000 * 1000;

    auto chars = new char[size]; 
    
    cout << "generating string" << endl;
    for (size_t i = 0; i < size; i++) {
        chars[i] = symbols[rand() % 4];
    }


    cout << "counting" << endl;
    auto result = count_occurances(size, chars);

    cout << "result:" << endl;
    print_result(result);

    delete chars;
}

void cpp_str() {
    char symbols[4] = {'1','2','3','4'};


    auto size = 1000 * 1000 * 1000;
    
    string str;
    str.reserve(size);


    cout << "generating string" << endl;

    for (size_t i = 0; i < size; i++) {
        str += symbols[rand() % 4];
    }


    cout << "counting" << endl;
    auto result = count_occurances(str);

    cout << "result:" << endl;
    print_result(result);
}

int main() {
   cpp_str();
}