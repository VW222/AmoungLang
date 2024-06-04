#include <iostream>
#include <string>
#include <vector>

#include "parser.hpp"
#include "interpreter.hpp"

using std::string, std::vector, std::cout;

int main()
{
    vector<char> splitChars = { '\n', ';', '#', ' ', '=', '+', '-', '(', ')', '*', '/', '"' };
    vector<string> operators = { "=", "+", "*", "-", "/" };
    vector<string> keywords = { "var" };
    string file = readFile("asd.cum");

    vector<Token> tokenStream = lex(file, splitChars, operators, keywords);
    Tree t = parseStream(tokenStream, operators);
    t.print();
    interpret(t);
}