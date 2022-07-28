#pragma once 
#include "Componente.h"
class ConsumibleComponent : public Componente
{
public:
	ConsumibleComponent(Entidad* pEnt);
	~ConsumibleComponent();
	virtual void Update(float deltaTime, Mensaje const & msj);

};
