#include "Variable.h"


Variable::Variable(void)
{
}


Variable::~Variable(void)
{
}

string Variable::getValue() {
	return value;
}

string Variable::getName() {
	return name;
}

void Variable::setName(string val) {
	name=val;
}

void Variable::setValue(string val) {
	value=val;
}

bool Variable::operator==(const Variable &other) const {
	return !(*this == other);
}

