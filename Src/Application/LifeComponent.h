#pragma once 
#include "Componente.h"
#include "GUI.h"
#include "Estado.h"
class LifeComponent : public Componente{ 
public: 
	LifeComponent(Entidad* pEntidad);
	~LifeComponent(); 
	virtual void Update(float deltaTime, Mensaje const & msj);
private:
	CEGUI::ProgressBar * life;
	int maxLife;
	float time;
}; 
