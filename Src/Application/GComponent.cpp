
#include "GComponent.h"
using namespace Ogre;
GComponent::GComponent(Entidad* pEnt, int cont, std::string name) : Componente (pEnt){	
	std::string mesh = name + ".mesh";
	ent = pEnt->getPEstado()->getScnManager()->createEntity(mesh);
	groupNode = pEnt->getPEstado()->getScnManager()->getRootSceneNode()->createChildSceneNode("GNode"+name);
	node = groupNode->createChildSceneNode(name );
	node->attachObject(ent);
	firstMsg = false;
	nodeCh = nullptr;
}

GComponent::GComponent(Entidad* pEnt, std::string name, std::string ogreNodeName) : Componente(pEnt) {
	std::string mesh = name + ".mesh";
	ent = pEnt->getPEstado()->getScnManager()->createEntity(mesh);
	groupNode = pEnt->getPEstado()->getScnManager()->getRootSceneNode()->createChildSceneNode("GNode" +ogreNodeName);
	node = groupNode->createChildSceneNode(ogreNodeName);
	node->attachObject(ent);
	firstMsg = false;
	nodeCh = nullptr;
}

void GComponent::Update(float deltaTime,  Mensaje const & msj) { 
	if (!firstMsg) {
		Mensaje msg = msj;
		Componente::Update(deltaTime, msj);
	
		if (msg.getTipo() == Tipo::Render) {

			if (msg.getSubTipo() == SubTipo::Orientar) {
				int pos = msg.getMsg().find("/");
				std::string xS = msg.getMsg().substr(0, pos);
				std::string subcad = msg.getMsg().substr(pos + 1);
				pos = subcad.find("/");
				std::string yS = subcad.substr(0, pos);
				std::string zS = subcad.substr(pos + 1);

				Vector3 aux = Ogre::Vector3(groupNode->getPosition().x + std::stof(xS) * 100000, groupNode->getPosition().y, groupNode->getPosition().z + std::stof(zS) * 100000);
				node->setOrientation(groupNode->getChild("NodoCamera")->getOrientation());
			
				node->lookAt(aux, Node::TS_LOCAL, Vector3::UNIT_Z);
			

			}

			else if (msg.getSubTipo() == SubTipo::Rotar) {
				int angle = std::stof(msg.getMsg());
				rota(angle, { 0,1,0 });

			}
			
		}
	}
	else { 
		firstMsg = true; 
	}
}

//No sé si esto funka bien del todo, además lo llamo desde la entidad y le hago un casting guarro
//Debería mandarlo como mensaje.
//El juego peta al salir
void GComponent::destroy() {
	pEntidad->getPEstado()->getScnManager()->destroySceneNode("GNode" + pEntidad->getNombreNodo());
	
}
GComponent::~GComponent() {
	pEntidad->getPEstado()->getScnManager()->destroySceneNode("GNode" + pEntidad->getNombreNodo());

}
void GComponent::rota(float angle, Vector3 eje){
	groupNode->rotate(Quaternion(Degree(angle), eje));	
	
}
void GComponent::translate(float x, float y, float z) {
	
	groupNode->translate(groupNode->getChild("NodoCamera")->getLocalAxes(), Vector3(x, y, z));
}
void GComponent::scale(float x, float y, float z) {
	node->scale(x, y, z);
}