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

bool Variable::operator==(const Variable &other) const {
	return !(*this == other);
}

