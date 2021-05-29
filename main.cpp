#include <iostream>
#include <regex>
using namespace std;
int main() {
    regex test_regex("\\w+\\s*=\\s*\"\\w+\"");
    cout << regex_search("lol=\"lol\"", test_regex);
    return 0;
}