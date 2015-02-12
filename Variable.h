#include <iostream>

using namespace std;

#pragma once

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
	bool operator==(const Variable &other) const;
};

