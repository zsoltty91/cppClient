#include <iostream>
#include <list>

#include "Variable.h"

using namespace std;

#pragma once

class Request
{
private:
	list<Variable> values; 

public:
	void addVariable(Variable variable);
	void removeVariable(Variable variable);
	Variable getFirstVariable();
	void removeFirstVariable();
	list<Variable> getVariables();
	Request(void);
	~Request(void);
	bool operator==(const Request &other) const;
};

