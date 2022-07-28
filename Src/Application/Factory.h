#pragma once 
#include <string>
#include <string.h>
using namespace std;
class Factory { 
public:
	virtual string create(string name) = 0;
	virtual int getLastCont() = 0;
};
