#pragma once
#include <stack>
#include <queue>
#include "preprocessor.hpp"
#include "lexer.hpp"

int precedence(string op) {
	if (op == "+" || op == "-")
		return 1;
	if (op == "*" || op == "/")
		return 2;
	return 0;
}

vector<Node*> shuntingYard(vector<Node*> stream) {
	std::queue<Node*> output;
	std::stack<Node*> operators;

	for (auto o1 : stream) {
		if (o1->type == Qualifiers::integer)
			output.push(o1);
		else if (o1->type == Qualifiers::oper) {
			while (!operators.empty()) {
				auto o2 = operators.top();
				if (o2->value == "(" || precedence(o2->value) < precedence(o1->value))
					break;
				output.push(o2);
				operators.pop();
			}
			operators.push(o1);
		}
		else if (o1->type == Qualifiers::lParen) {
			operators.push(o1);
		}
		else if (o1->type == Qualifiers::rParen) {
			while (!operators.empty() && operators.top()->type != Qualifiers::lParen) {
				output.push(operators.top());
				operators.pop();
			}
			if (!operators.empty() && operators.top()->type == Qualifiers::lParen)
				operators.pop(); // Pop the left parenthesis
		}
	}

	while (!operators.empty()) {
		output.push(operators.top());
		operators.pop();
	}

	vector<Node*> res;
	while (!output.empty()) {
		res.push_back(output.front());
		output.pop();
	}

	return res;
}


Tree parseStream(vector<Token> tokenStream, vector<string> operators) {
	auto root = new Node(Qualifiers::expr, "/");

	vector<Token> line;
	auto pushLine = [&root, &line]() {
		auto n = new Node(Qualifiers::expr, "line");

		for (Token v : line) {
			n->children.push_back(new Node(v.qual, v.name));
		}
		root->children.push_back(n);

		line.clear();
		};

	for (int i = 0; i < tokenStream.size(); i++) {
		Token t = tokenStream[i];
		if (t.qual != Qualifiers::lineEnd)
			line.push_back(t);
		else if (line.size() > 0) {
			pushLine();
		}
	}

	if (line.size() > 0)
		pushLine();

	for (Node* expr : root->children) {
		if (expr->children[1]->value == "(") {
			Node* func = expr->children[0];

			expr->children.pop_back();
			times(2, i)
				expr->children.erase(expr->children.begin());

			auto e = shuntingYard(expr->children);
			//for (string v : e) {
			//	cout << v << " ";
			//}
			//cout << "\n";

			Node* exprSub = new Node(Qualifiers::expr, "expr");
			std::stack<Node *> nodes;
			for (auto v : e) {
				if (v->type == Qualifiers::integer) {
					nodes.push(v);
				}
				else { // if its an operator
					Node* left = nodes.top();
					nodes.pop();
					Node* right = nodes.top();
					nodes.pop();
					Node* op = new Node(Qualifiers::oper, v->value);
					op->children.push_back(left);
					op->children.push_back(right);

					nodes.push(op);
				}
			}

			expr->children.clear();
			expr->children.push_back(func);
			expr->children.push_back(nodes.top());
		}
	}

	return Tree(root);
}
//auto exprNode = new Node(Qualifiers::expr, "expr");
			//for (int i = 2; i < expr->children.size() - 1; i++) {
			//	exprNode->children.push_back(expr->children[i]);
			//}
			//
			//int c = expr->children.size();
			//for (int i = 0; i < c - 1; i++) {
			//	expr->children.pop_back();
			//}
			//
			//expr->children.push_back(exprNode);