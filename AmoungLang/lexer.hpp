#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "classes.hpp"


string readFile(string path) {
    std::ifstream ifs(path);
    return string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

bool in(char c, vector<char> arr) {
    for (char v : arr)
        if (c == v)
            return true;
    return false;
}

bool in(string c, vector<string> arr) {
    for (string v : arr)
        if (c == v)
            return true;
    return false;
}

vector<string> split(string file, vector<char>& splitChars) {
    vector<string> result;
    bool inString = false;
    string running = "";
    bool wasString = false;

    for (char c : file) {
        if (c == '"') {
            if (!inString) {
                running += '"';
                inString = true;
            }
            else {
                result.push_back(running);
                //result.push_back(string(1, c));
                running = "";
                inString = false;
            }
        }
        else if (!inString && in(c, splitChars)) {
            if (running.length() != 0) {
                result.push_back(running);
                result.push_back(string(1, c));
                running = "";
            }
            else
                result.push_back(string(1, c));
        }
        else
            running += c;
    }

    if (inString)
        throw std::runtime_error("string not closed");

    if (running.length() != 0)
        result.push_back(running);

    size_t t = result.size();
    for (int i = 0; i < t; i++) {
        if (result[i][0] == ' ') {
            result.erase(result.begin() + i);
            t--;
        }
    }

    return result;
}

vector<Token> lex(string& file, vector<char> splitChars, vector<string> operators, vector<string> keywords) {
    vector<Token> result;
    vector<string> stuff = split(file, splitChars);

    // i used chatgpt for these. sorry not sorry.
    std::regex renum("-?[0-9]+");
    std::regex refloat("[+-]?([0-9]*[.])?[0-9]+([eE][-+]?\\d+)?");
    //std::regex reop("(=)");

    for (string s : stuff) {
        int id = Qualifiers::iden;
        string name = s;

        if (s[0] == '\n')
        {
            id = Qualifiers::lineEnd;
            name = "\\n";
        }
        else if (s[0] == '(')
            id = Qualifiers::lParen;
        else if (s[0] == ')')
            id = Qualifiers::rParen;
        else if (s[0] == '"') {
            id = Qualifiers::str;
            name = name.erase(0, 1);
        }
        else if (in(s, operators))
            id = Qualifiers::oper;
        else if (std::regex_match(s, renum))
            id = Qualifiers::integer;
        else if (std::regex_match(s, refloat))
            id = Qualifiers::floating;
        else {
            if (in(name, keywords))
                id = Qualifiers::keyword;
        }
        result.push_back(Token(id, name));
    }

    return result;
}

void clean(vector<string>& vec) {
    int t = vec.size();
    for (int i = 0; i < t; i++) {
        string a = vec[i];
        if (a[0] == '#') {
            do {
                vec.erase(vec.begin() + i);
                t--;
            } while (vec[i - 1][0] != '\n' && i < t);
        }
        if (a[0] == ' ') {
            vec.erase(vec.begin() + i);
            t--;
        }
    }
}