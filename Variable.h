#include <iostream>

#pragma once

using namespace std;


class Variable
{
private:
	string name;
	string value;
public:
	string getName();
	string getValue();
	void setName(string val);
	void setValue(string val);
	Variable(void);
	~Variable(void);
};

