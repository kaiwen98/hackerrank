#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <regex>
#include <stack>
using namespace std;

typedef struct {
    string parent_tag_name;
    string tag_name;
    map<string, void*> *children = new map<string, void*>;
    map<string, string> *attributes = new map<string, string>;
} Tag;

void populate_tag(Tag* tag, string input) {
    // Parses an attribute string: "att = value"
    regex attr_regex("\\S+\\s*=\\s*\"\\S+\"");
    // Parse different if no attributes exist
    if (!regex_search(input, attr_regex)) {
        string name = (input.substr(
            input.find("<") + 1, 
            input.find(">") - 1));

        tag->tag_name = name;
        return;
    }

    tag->tag_name = input.substr(
        input.find("<") + 1, 
        input.find(' ') - 1);
    //cout << tag->tag_name << endl;
    auto words_begin = sregex_iterator(input.begin(), input.end(), attr_regex);
    auto words_end = sregex_iterator();
    string attribute;
    string value;
    string substring;
    for (auto i = words_begin; i != words_end; ++i) {
        attribute = i->str().substr(0, i->str().find("="));
        attribute.erase(attribute.find_last_not_of(" \n\r\t")+1);

        substring = i->str().substr(i->str().find("= ") + 2);
        substring = substring.erase(substring.find_last_not_of(" \n\r\t")+1);

        value = substring.substr(substring.find('"') + 1, substring.find('"', 1) - 1);
        value.erase(value.find_last_not_of(" \n\r\t")+1);

        tag->attributes->insert(pair<string, string>(attribute, value));
    }
}

bool check_is_close_tag(string input) {
    regex close_tag_regex("</\\w+>");
    return regex_search(input, close_tag_regex);
}

bool handle_input(string input, stack<Tag*> *tag_recursive) {
    Tag* new_tag = new Tag();
    if (input[0] ==  '<') {
        if (check_is_close_tag(input)) {
            tag_recursive->pop();
            return true;
        }
        populate_tag(new_tag, input);
        tag_recursive->top()->children->insert(pair<string, void*>(new_tag->tag_name, (void*)new_tag));
        tag_recursive->push(new_tag);

        return true;
    }
    return false;
}

bool handle_query(Tag * root, string input, string &result) {
    int initial_pos = 0;
    int end_pos = 0;
    int length = 0;
    string temp = input;
    string tag;
    Tag *curr_tag = root;
    string curr_attribute;
    int count = 0;

    while (1) {
        initial_pos = 0;
        end_pos = temp.find(".");

        // Proceed to access attribute if valid
        if (end_pos == string::npos) {
            end_pos = temp.find("~");

            tag = temp.substr(initial_pos, end_pos);
            try {
                curr_tag = (Tag*) curr_tag->children->at(tag);
            } catch(...) {
                cout << "Not Found!" << endl;
                return false;
            }

            curr_attribute = temp.substr(end_pos + 1);

            if (!tag.compare(curr_tag->tag_name)) {
                try {
                    curr_tag->attributes->at(curr_attribute);
                } catch(...) {
                    cout << "Not Found!" << endl;
                    return false;
                }
                result = curr_tag->attributes->at(temp.substr(end_pos + 1));
                cout << result << endl;
            }
            return true;
        } else {
            // Proceed to child tag if valid
            
            tag = temp.substr(initial_pos, end_pos);
            temp = temp.substr(end_pos + 1);
            try {
                curr_tag = (Tag*) curr_tag->children->at(tag);            
            } catch(...) {
                cout << "Not Found!" << endl;
                return false;
            }
            count++;
        }
    }
    return true;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    Tag *root = new Tag();
    root->tag_name = "root";
    Tag *parent;
    int num_lines;
    int num_queries;
    string input;
    string result;
    // Need to keep record of stack frames due to recursive call of a tag from a parent tag.
    stack<Tag*> *tag_recursive = new stack<Tag*>();
    tag_recursive->push(root);
    cin >> num_lines >> num_queries;

    while (num_lines) {
        getline(cin, input);
        if(handle_input(input, tag_recursive)) {
            num_lines--;
        }
    }

    vector<string> key;

    while (num_queries) {
        getline(cin, input);
        handle_query(root, input, result);
        
        num_queries--;
    }
    return 0;
}
