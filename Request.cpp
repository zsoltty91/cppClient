#include "Request.h"

Request::Request(void)
{
}


Request::~Request(void)
{
}

void Request::addVariable(Variable variable) {
	values.push_back(variable);
}

void Request::removeVariable(Variable variable) {
	values.remove(variable);
}

Variable Request::getFirstVariable() {
	return values.front();
}

void Request::removeFirstVariable() {
	values.pop_front();
}

list<Variable> Request::getVariables() {
	return values;
}

void Request::setName(string nam)
{
	name = nam;
}

string Request::getName() 
{
	return name;
}
	
void Request::setType(string typ)
{
	type=typ;
}
	
string getType();
{
	return type;
}

bool Request::operator==(const Request &other) const {
	return !(*this==other);
}