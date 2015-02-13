#include <iostream>
#include <list>

#include "Variable.h"

using namespace std;

#pragma once

class Request
{
private:
	list<Variable> values;
	string name;
	string type;

public:
	void addVariable(Variable variable);
	void removeVariable(Variable variable);
	Variable getFirstVariable();
	void removeFirstVariable();
	list<Variable> getVariables();
	void setName(string nam);
	string getName();
	void setType(string typ);
	string getType();
	Request(void);
	~Request(void);
	bool operator==(const Request &other) const;
};

