#include <iostream>
#include <variant>
#include <unordered_map>
using std::unordered_map;
#include "classes.hpp"

struct Value {
	int type;
	std::variant<int, float, string> value;

	Value() {

	}

	Value(int e) {
		this->value = e;
		this->type = Qualifiers::integer;
	}
	Value(float e) {
		this->value = e;
		this->type = Qualifiers::floating;
	}
	Value(string e) {
		this->value = e;
		this->type = Qualifiers::str;
	}

	Value(Node* e) {
		if (e->type == Qualifiers::integer) {
			this->value = std::stoi(e->value);
			this->type = Qualifiers::integer;
		}
		else if (e->type == Qualifiers::floating) {
			this->value = std::stof(e->value);
			this->type = Qualifiers::floating;
		}
		else if (e->type == Qualifiers::str) {
			this->value = e->value;
			this->type = Qualifiers::str;
		}
	}
	~Value() {

	}

	int getInt() {
		if (this->type != Qualifiers::integer)
			throw std::runtime_error("dats no int");
		return std::get<int>(this->value);
	}
	float getFloat() {
		if (this->type != Qualifiers::floating) {
			if (this->type == Qualifiers::integer)
				return std::get<int>(this->value);
			else
				throw std::runtime_error(this->type + " isnt a float.");
		}
		return std::get<float>(this->value);
	}
	string getString() {
		return std::get<string>(this->value);
	}

	Value operator+(Value& rh) {
		if (this->type == Qualifiers::integer && rh.type == Qualifiers::integer) {
			// return int
			return Value(this->getInt() + rh.getInt());
		}
		else if ((this->type == Qualifiers::integer && rh.type == Qualifiers::integer)
			|| (this->type == Qualifiers::integer && rh.type == Qualifiers::floating)
			|| (this->type == Qualifiers::floating && rh.type == Qualifiers::floating)) {
			return Value(this->getFloat() + rh.getFloat());
		}
		else if (this->type == Qualifiers::str && rh.type == Qualifiers::str) {
			return Value(this->getString() + rh.getString());
		}
		else if (this->type == Qualifiers::str && rh.type == Qualifiers::floating) {
			return Value(this->getString() + std::to_string(rh.getFloat()));
		}
		else if (this->type == Qualifiers::floating && rh.type == Qualifiers::str) {
			return Value(std::to_string(this->getFloat()) + rh.getString());
		}

		else if (this->type == Qualifiers::str && rh.type == Qualifiers::integer) {
			return Value(this->getString() + std::to_string(rh.getInt()));
		}
		else if (this->type == Qualifiers::integer && rh.type == Qualifiers::str) {
			return Value(std::to_string(this->getInt()) + rh.getString());
		}
	}
	Value operator-(Value& rh) {
		if (this->type == Qualifiers::integer && rh.type == Qualifiers::integer) {
			// return int
			return Value(this->getInt() - rh.getInt());
		}
		else if ((this->type == Qualifiers::integer && rh.type == Qualifiers::integer)
			|| (this->type == Qualifiers::integer && rh.type == Qualifiers::floating)
			|| (this->type == Qualifiers::floating && rh.type == Qualifiers::floating)) {
			return Value(this->getFloat() - rh.getFloat());
		}
	}
	Value operator*(Value& rh) {
		if (this->type == Qualifiers::integer && rh.type == Qualifiers::integer) {
			// return int
			return Value(this->getInt() * rh.getInt());
		}
		else if ((this->type == Qualifiers::integer && rh.type == Qualifiers::integer)
			|| (this->type == Qualifiers::integer && rh.type == Qualifiers::floating)
			|| (this->type == Qualifiers::floating && rh.type == Qualifiers::integer)
			|| (this->type == Qualifiers::floating && rh.type == Qualifiers::floating)) {
			return Value(this->getFloat() * rh.getFloat());
		}
	}
	Value operator/(Value& rh) {
		return Value(this->getFloat() / rh.getFloat());
	}
};

Value evalExpr(Node* root, unordered_map<string, Value>& memory) {
	if (root->type == Qualifiers::integer) {
		int e = std::stoi(root->value);
		return Value(e);
	}
	else if (root->type == Qualifiers::floating) {
		float e = std::stof(root->value);
		return Value(e);
	}
	else if (root->type == Qualifiers::str) {
		return Value(root->value);
	}
	else if (root->type == Qualifiers::iden) {
		return memory[root->value];
	}
	else if (root->value == "+" || root->value == "-" || root->value == "*" || root->value == "/") {
		auto* lh = root->children[0];
		auto* rh = root->children[1];

		Value v1, v2;

		if (lh->type == Qualifiers::oper)
			v1 = evalExpr(lh, memory);
		else if (lh->type == Qualifiers::iden)
			v1 = memory[lh->value];
		else
			v1 = Value(lh);

		if (rh->type == Qualifiers::oper)
			v2 = evalExpr(rh, memory);
		else if (rh->type == Qualifiers::iden)
			v2 = memory[rh->value];
		else
			v2 = Value(rh);

		if (root->value == "+")
			return v1 + v2;
		else if (root->value == "-")
			return v1 - v2;
		else if (root->value == "/")
			return v1 / v2;
		else
			return v1 * v2;

		throw std::runtime_error("Unsupported operation <+/-> between: " + lh->value + " and " + rh->value);
	}
	// Handle other cases or throw an error.
	throw std::runtime_error("Invalid expression");
}

void print(Value r) {
	if (r.type == Qualifiers::integer)
		std::cout << r.getInt() << std::endl;
	else if (r.type == Qualifiers::floating)
		std::cout << r.getFloat() << std::endl;
	else if (r.type == Qualifiers::str)
		std::cout << r.getString() << std::endl;
}
void interpret(Tree AST) {
	unordered_map<string, Value> memory;

	auto setMem = [&memory](string key, Value value) {
		return memory[key] = value;
		};

	for (Node* line : AST.root->children) {
		try {
			if (line->type == Qualifiers::functionCall) {
				if (line->children[0]->value == "print") {
					print(evalExpr(line->children[1], memory));
				}
			}

			if (line->type == Qualifiers::variableDeclaration) {
				setMem(line->children[1]->value, evalExpr(line->children[2], memory));
			}
		}
		catch (std::exception const& e) {
			cout << e.what();
			exit(-1);
		}
	}
}