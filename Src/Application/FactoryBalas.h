#pragma once

#include "Factory.h"

class FactoryBalas : public Factory {
public:
	FactoryBalas() : Factory() {};
	string create(string name) {
		cont++;
		if (name == "stone") {
			contEnem++;
			return name + to_string(contEnem);
		}
		return (name + to_string(cont));
	}
	int getLastCont() { return contEnem; }
private:
	int cont = 0;
	int contEnem = 0;
};