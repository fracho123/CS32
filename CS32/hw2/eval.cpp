#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

const int CHAR = 1;
const int OPER = 2;
const int OPEN = 3;
const int CLOS = 4;
const int MISC = 5;

int priority(const char c);
int identify(const char c);
void convert(const string infix, string& postfix);
bool check(const string infix);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	if (!check(infix))
		return 1;
	convert(infix, postfix);
	for (unsigned int i = 0; i < infix.length(); i++)
		if (islower(infix[i]))
			if (!values.contains(infix[i]))
				return 2;

	//evaluate the postfix expression
	char ch;
	int val, res;
	stack<int> OpStack;
	for (unsigned int i = 0; i < postfix.length(); i++) {
		ch = postfix[i];
		if (identify(ch) == CHAR) {
			values.get(ch, val);
			OpStack.push(val);
		}
		else {
			int op2 = OpStack.top();
			OpStack.pop();
			int op1 = OpStack.top();
			OpStack.pop();
			switch (ch) {
			case '*':	res = op1 * op2;	break;
			case '/':	if (op2 == 0) return 3;	res = op1 / op2;	break;
			case '+':	res = op1 + op2;	break;
			case '-':	res = op1 - op2;	break;
			}
			OpStack.push(res);
		}
	}
	result = OpStack.top();
	return 0;
}

int priority(const char c) {
	if (c == '+' || c == '-')
		return 2;
	if (c == '*' || c == '/')
		return 1;
	return 0;
}

int identify(const char c) {
	if (c == '+' || c == '-' || c == '*' || c == '/')
		return OPER;
	if (c == '(')
		return OPEN;
	if (c == ')')
		return CLOS;
	if (islower(c))
		return CHAR;
	return MISC;
}

void convert(const string infix, string& postfix) {
	postfix = "";
	stack<char> OpStack;
	int ch;
	for (unsigned int i = 0; i < infix.length(); i++) {
		ch = identify(infix[i]);
		switch (ch) {
		case CHAR:
			postfix += infix[i];
			break;
		case OPEN: {
			OpStack.push(infix[i]);
			break;
		}
		case CLOS:
			while (OpStack.top() != '(') //most problematic line
			{
				postfix += OpStack.top();
				OpStack.pop();
			}
			OpStack.pop();
			break;
		case OPER: {
			while (!OpStack.empty() && OpStack.top() != '(' && priority(infix[i]) > priority(OpStack.top()))
			{
				postfix += OpStack.top();
				OpStack.pop();
			}
			OpStack.push(infix[i]);
			break;
		}
		}
	}
	while (!OpStack.empty()) {
		postfix += OpStack.top();
		OpStack.pop();
	}
}



bool check(const string infix) {
	string noSpace = "";
	//turn string into string without spaces and validates it only has parenthesis, operators, and lowercase
	for (unsigned int i = 0; i < infix.length(); i++) {
		if (infix[i] != ' ') {
			if (identify(infix[i]) == MISC)
				return false;
			noSpace += infix[i];
		}
	}
	//check the first character
	if (identify(noSpace[0]) != CHAR && identify(noSpace[0]) != OPEN)
		return false;

	//go through string looking for syntax errors
	int nOpen = 0;
	if (identify(noSpace[0]) == OPEN)
		nOpen++;
	for (unsigned int i = 1; i < noSpace.length(); i++) {
		//basic syntax and ordering
		//for prev
		if (identify(noSpace[i]) == OPEN && (identify(noSpace[i - 1]) != OPER && identify(noSpace[i - 1]) != OPEN))
			return false;
		if (identify(noSpace[i]) == CHAR && (identify(noSpace[i - 1]) != OPER && identify(noSpace[i - 1]) != OPEN))
			return false;
		if (identify(noSpace[i]) == OPER && (identify(noSpace[i - 1]) != CHAR && identify(noSpace[i - 1]) != CLOS))
			return false;
		if (identify(noSpace[i]) == CLOS && (identify(noSpace[i - 1]) != CHAR && identify(noSpace[i - 1]) != CLOS))
			return false;
		//for next
		//some include null byte because those are characters that can end an equation
		if (identify(noSpace[i]) == OPEN && (identify(noSpace[i + 1]) != CHAR && identify(noSpace[i + 1]) != OPEN))
			return false;
		if (identify(noSpace[i]) == CHAR && (identify(noSpace[i + 1]) != OPER && identify(noSpace[i + 1]) != CLOS && noSpace[i + 1] != '\0'))
			return false;
		if (identify(noSpace[i]) == OPER && (identify(noSpace[i + 1]) != CHAR && identify(noSpace[i + 1]) != OPEN))
			return false;
		if (identify(noSpace[i]) == CLOS && (identify(noSpace[i + 1]) != OPER && identify(noSpace[i + 1]) != CLOS && noSpace[i + 1] != '\0'))
			return false;

		//check that there is sufficient space between parenthesis. It's suspiscious if they're too close too each other
		if (identify(noSpace[i]) == OPEN)
			nOpen++;
		if (identify(noSpace[i]) == CLOS)
			nOpen--;
	}
	//if there are too many of an open or close parenthesis
	if (nOpen != 0)
		return false;

	return true;
}

int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+" && answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()o", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(*o)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*" && answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+" && answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/" && answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	cout << "Passed all tests" << endl;
}