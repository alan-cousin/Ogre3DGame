#pragma once 
#include "Componente.h"
#include "Estado.h"

class Camara: public Componente
{
public:
	Camara(Entidad* pEnt);
	virtual ~Camara();
	virtual void Update(float deltaTime, Mensaje const & msj);

	void attach(Ogre::SceneNode* node);

private:
	Ogre::Camera* cam;
	Ogre::Viewport* vp;
	Ogre::SceneNode* node;
};