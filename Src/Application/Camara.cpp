#pragma once 
#include "Camara.h"

using namespace Ogre;
Camara::Camara(Entidad * pEnt) : Componente(pEnt)
{
	cam = pEnt->getPEstado()->getScnManager()->createCamera("camera");
	cam->setPosition(0, 15, -70);
	cam->lookAt(0, 0, 0);
	cam->setNearClipDistance(5);
	node = pEnt->getPEstado()->getScnManager()->getSceneNode("GNodeAlaia")->createChildSceneNode("NodoCamera");
	node->attachObject(cam);

	vp = pEnt->getPEstado()->getWin()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0, 150, 150));

	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
	
	
}

Camara::~Camara()
{
	vp->clear();
}
void Camara::attach(Ogre::SceneNode* node) {
	node->attachObject(cam);
}
void Camara::Update(float deltaTime, Mensaje const & msj) {
	Mensaje msg = msj;

	if (msg.getTipo() == Input && msg.getReceptor() == pEntidad) {
		
		if (msg.getSubTipo() == SubTipo::OrientaCamara) {
			int pos = msg.getMsg().find("/");
			std::string xS = msg.getMsg().substr(0, pos);
			std::string subcad = msg.getMsg().substr(pos + 1);
			pos = subcad.find("/");
			std::string yS = subcad.substr(0, pos);
			std::string zS = subcad.substr(pos + 1);

			float x = std::stof(xS)* deltaTime*0.1;
			float z = deltaTime * std::stof(zS)*0.1;

			node->rotate(Ogre::Quaternion(Ogre::Degree(-x*2), Ogre::Vector3::UNIT_Y));
		}
	}
}