#include "BalaComponent.h"  
#include <ctime>
#include <iostream>
#include "OgreParticleSystem.h"
 
BalaComponent::BalaComponent(Entidad* pEntidad, string type) : Componente(pEntidad) {
	tipo = type;
	if (type == "Simple") {

		Ogre::Vector3 valores = { 1,0,1 };
		Ogre::Matrix3 matriz = pEntidad->getPEstado()->getScnManager()->getSceneNode("Alaia")->getLocalAxes();
		valores = matriz * valores;
		btVector3 pos1 = { valores.x ,0, valores.z };
		btVector3 posAux = pos1.rotate(btVector3(0, 1, 0), -3.141596 / 4);
		btVector3 posEnt = pEntidad->getPEstado()->getFisicManager()->getRigidBody("Alaia")->getWorldTransform().getOrigin();
		string posOgro = to_string(posAux.getX() * 3.666 + posEnt.getX()) + "/" +
			to_string(posEnt.getY()+2) + "/" +
			to_string(posAux.getZ() * 3.666 + posEnt.getZ());
		Mensaje ms(Tipo::Fisica, posOgro, SubTipo::Reposicionar);
		Mensaje ms1(Tipo::Fisica, "10", SubTipo::Dispara);
		ms.setMsgInfo(pEntidad, pEntidad);
		ms1.setMsgInfo(pEntidad, pEntidad);
		pEntidad->getPEstado()->addMsg(ms);
		pEntidad->getPEstado()->addMsg(ms1);
	}
	else {
		Ogre::Vector3 valores = { 0,1,0 };
		Ogre::Matrix3 matriz = pEntidad->getPEstado()->getScnManager()->getSceneNode("Alaia")->getLocalAxes();
		valores = matriz * valores;
		btVector3 pos1 = { 0 ,valores.y, 0 };
		
	}


	tiempoInicio = std::clock()* 1000;

} 
BalaComponent::~BalaComponent() { 

	particleSystem->removeAllEmitters();
	particleSystem->clear();


	
}

void BalaComponent::Update(float deltaTime, Mensaje const & msj)
{
	Mensaje msg = msj;
	Componente::Update(deltaTime, msj);

	if (msg.getTipo() == Tipo::Fisica) {
		if (msg.getSubTipo() == SubTipo::Dispara) {
			if (tipo == "Simple") {
				Ogre::Real escala = std::stof(msg.getMsg());

			btRigidBody* alaia = pEntidad->getPEstado()->getFisicManager()->getRigidBody("Alaia");
			btVector3 start = alaia->getWorldTransform().getOrigin();

			Ogre::Vector3 valores = { escala,0,escala };
			Ogre::Matrix3 matriz = pEntidad->getPEstado()->getScnManager()->getSceneNode("Alaia")->getLocalAxes();

				valores = matriz * valores;

				btVector3 vel = { valores.x ,0, valores.z };
				btVector3 velAux = vel.rotate(btVector3(0, 1, 0), -3.141596 / 4);

				btVector3 auxx(velAux.getX(), velAux.getY(), velAux.getZ() + 1000);

				btRigidBody * bala = pEntidad->getPEstado()->getFisicManager()->getRigidBody(pEntidad->getNombreNodo());
				bala->setCollisionFlags(bala->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

				btCollisionWorld::ClosestRayResultCallback RayCallback(start, auxx);
				pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->rayTest(start, auxx, RayCallback);

				if (RayCallback.hasHit()) {
					auxx = RayCallback.m_hitPointWorld;
				}

				pEntidad->getPEstado()->getFisicManager()->getRigidBody(pEntidad->getNombreNodo())->setLinearFactor({ 1, 0, 1 });
				pEntidad->getPEstado()->getFisicManager()->getRigidBody(pEntidad->getNombreNodo())->applyImpulse(velAux * 10, start);
				particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem(pEntidad->getNombreNodo() + "PFX", "Spiral");
				pEntidad->getPEstado()->getScnManager()->getSceneNode(pEntidad->getNombreNodo())->attachObject(particleSystem);
				particleSystem->setEmitting(true);
			}
			else {

				Ogre::Real escala = std::stof(msg.getMsg());

				btRigidBody* alaia = pEntidad->getPEstado()->getFisicManager()->getRigidBody("Alaia");
				btVector3 start = alaia->getWorldTransform().getOrigin();

				Ogre::Vector3 valores = { 0,escala,0 };
				Ogre::Matrix3 matriz = pEntidad->getPEstado()->getScnManager()->getSceneNode("Alaia")->getLocalAxes();

				valores = matriz * valores;

				btRigidBody * bala = pEntidad->getPEstado()->getFisicManager()->getRigidBody(pEntidad->getNombreNodo());
				bala->setCollisionFlags(bala->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
				btVector3 vel = { 0 ,-valores.y, 0 };
				pEntidad->getPEstado()->getFisicManager()->getRigidBody(pEntidad->getNombreNodo())->applyImpulse(vel * 10, start);

				particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem(pEntidad->getNombreNodo() + "PFX", "LLuvia");
				pEntidad->getPEstado()->getScnManager()->getSceneNode(pEntidad->getNombreNodo())->attachObject(particleSystem);
				particleSystem->setEmitting(true);
			}

			
		}

		if (tiempoInicio  + 5000000 /*1000000*/  < std::clock() * 1000) {
			pEntidad->Sleep();
			pEntidad->getPEstado()->destroy(pEntidad->getNombreNodo());
		}
	}

}
