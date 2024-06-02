#pragma once
#include <string>
#include <vector>

using std::string, std::vector, std::cout;

enum Qualifiers {
	uninitialized = -1,
	lParen,
	rParen,
	lineEnd,
	iden,
	integer,
	floating,
	oper,
	expr
};

class Node {
public:
	int type = Qualifiers::uninitialized;
	string value;
	vector<Node*> children;

	Node() {

	}
	Node(int type, string value) {
		this->type = type;
		this->value = value;
	}
	Node(string value) {
		this->type = Qualifiers::uninitialized;
		this->value = value;
	}
	~Node() {

	}

	void printSubTree(int indent) {
		cout << string(indent * 4, ' ') << this->type << ':' << this->value << "\n";

		for (int i = 0; i < children.size(); i++) {
			auto v = children[i];
			v->printSubTree(indent + 1);
		}
	}
};

class Tree {
public:
	Node* root;

	Tree() {

	}
	Tree(Node* root) {
		this->root = root;
	}
	~Tree() {

	}

	void print() {
		root->printSubTree(0);
	}
};

class Token {
public:
	int qual;
	string name;

	Token() {
		qual = Qualifiers::uninitialized;
		name = "";
	}

	Token(int qual, string name) {
		this->qual = qual;
		this->name = name;
	}
	~Token() {

	}

	void print() {
		cout << "(QUAL: " << this->qual << ", NAME: " << this->name << ")\n";
	}
};