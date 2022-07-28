#pragma once 
#include "Componente.h"
#include "Estado.h"

class ManaComponent: public Componente 
{
public: 
	ManaComponent(Entidad* pEntidad);
	~ManaComponent(); 
	virtual void Update(float deltaTime, Mensaje const & msj);
private:
	CEGUI::ProgressBar * mana = nullptr;
	Ogre::ParticleSystem* particleSystem;
	int maxMana;
	void restaPower();
	void charge();
	void finCharge();
	void restaurapS();
	bool playinFX;

}; 
