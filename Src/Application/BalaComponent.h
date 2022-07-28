#pragma once 

#include "FComponent.h"
class BalaComponent: public Componente { 
public: 
	BalaComponent(Entidad* pEntidad, string type = " "); 
	~BalaComponent(); 

	virtual void Update(float deltaTime, Mensaje const & msj);//A rellenar con el cuenta steps
	virtual void destroy() {};


	float tiempoInicio;
	string tipo;

private:
	Ogre::ParticleSystem* particleSystem;
}; 
