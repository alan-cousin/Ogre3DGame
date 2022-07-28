#include "FComponent.h"  
#include <iostream>
 
FComponent::FComponent(Entidad* pEnt, float altoCaj, float anchoCaj, float profCaj, std::string nombreNodo, bool suelo, tipoFisica type, btScalar masa ):Componente(pEnt),mass(masa),altoCaja(altoCaj),anchoCaja(anchoCaj), profCaja(profCaj) {
	tipo = type;
	_suelo = suelo;
	eliminado = false; firstTime = true;
	
	timePas = 0;
	trigger = nullptr;
	//Si está vinculado a un componente gráfico
	if (nombreNodo != " ") {

		//Muestra la caja en Ogre
		
		pEntidad->getPEstado()->getScnManager()->getSceneNode(pEntidad->getNombreNodo())->_update(true, true);

		//Le pone al cuerpo coordenadas, orientación y volumen (sacado del componente gráfico)
		Ogre::Vector3 posN = pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode"+ pEntidad->getNombreNodo())->getPosition();
		Ogre::AxisAlignedBox bbox = pEntidad->getPEstado()->getScnManager()->getSceneNode(pEntidad->getNombreNodo())->_getWorldAABB();
		Ogre::Vector3  v = bbox.getSize();
		if (altoCaj == 0 && profCaj == 0 && anchoCaj == 0) {
			altoCaja = v.y;
			profCaja = v.z;
			anchoCaja = v.x;
		}
		pTransform.setIdentity();
		pTransform.setOrigin(btVector3(posN.x, posN.y, posN.z));
		
		
		Ogre::Quaternion quat = pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode" + pEntidad->getNombreNodo())->getOrientation();
		pTransform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));//Tener en cuenta que en ogre el primer valor es w, mientras que en bullet va último.
		initBody();

		//Vincula el nodo gráfico al físico
		if (trigger) {
			trigger->setUserPointer(pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode" + pEntidad->getNombreNodo()));
			body->setUserPointer(pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode"+ pEntidad->getNombreNodo()));
		}
		else
			body->setUserPointer(pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode"+ pEntidad->getNombreNodo()));

		//Para poder acceder desde Fisic a los rigidbodies
		if (tipo != tipoFisica::Trigger)
			pEntidad->getPEstado()->getFisicManager()->addBodyToMap(nombreNodo, body);
		
	}

	//Si no está vinculado a un componente gráfico se crea por defecto en el 0,0,0 se puede reposicionar con un mensaje
	else {
		tipo = tipoFisica::Estatico;
		pTransform.setIdentity();
		pTransform.setOrigin(btVector3(0.0, 0.0, 0.0));
		initBody();
	}
	actualizaNodo();
} 

FComponent::~FComponent() { 
	if(body != nullptr)
		pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->removeRigidBody(body);

	if (trigger != nullptr)
		pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->removeCollisionObject(static_cast<btCollisionObject*>(trigger));
	delete body;
	delete motionState;
	delete shape;
	delete trigger;
} 

void FComponent::initBody() {
	//Aquí ajustamos la masa
	if (tipo == tipoFisica::Estatico || tipo == tipoFisica::Trigger)
		mass = 0;

	//La inercia inicial siempre es 0
	btVector3 localInertia(0, 0, 0);

	//Aquí hacemos la forma de la caja
	motionState = new btDefaultMotionState(pTransform);

	if (!_suelo) {
		if (tipo != tipoFisica::Trigger && tipo != tipoFisica::TriggerDinamico)
			shape = new btBoxShape(btVector3(btScalar(anchoCaja / 2), btScalar(altoCaja / 2), btScalar(profCaja / 2)));
		else {

			trigger = new btGhostObject();
			shape = new btBoxShape(btVector3(btScalar(0 / 2), btScalar(3), btScalar(0 / 2)));
			trigger->setCollisionShape(shape);
		}
	}
	else {
		shape = new btBoxShape(btVector3(btScalar(anchoCaja / 2), btScalar((altoCaja / 2) + 1.2), btScalar(profCaja / 2)));

	}

	//Aquí se inicializa el cuerpo en base a sus parámetros anteriores
	shape->calculateLocalInertia(mass, localInertia);
	pEntidad->getPEstado()->getFisicManager()->getCollisionShapes().push_back(shape);
	btRigidBody::btRigidBodyConstructionInfo RBInfo(mass, motionState, shape, localInertia);

	body = new btRigidBody(RBInfo);

	//Elasticidad del material
	body->setRestitution(0);

	//Para que Alaia no rote
	if (tipo == tipoFisica::Kinematico || tipo == tipoFisica::Trigger) {
		body->setAngularFactor(btVector3(0, 1, 0));
	}

	pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->addRigidBody(body);
	if (trigger)
		pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->addCollisionObject(trigger);

}

void FComponent::Update(float deltaTime, Mensaje const & msj) {
	Mensaje msg = msj;
	Componente::Update(deltaTime, msj);
	
	if (msg.getTipo() == Tipo::Fisica) {
		if (msg.getSubTipo() == SubTipo::Reposicionar) {

			int pos = msg.getMsg().find("/");
			std::string xS = msg.getMsg().substr(0, pos);
			std::string subcad = msg.getMsg().substr(pos + 1);
			pos = subcad.find("/");
			std::string yS = subcad.substr(0, pos);
			std::string zS = subcad.substr(pos + 1);
			//Aquí le asignamos el transform que debería tener la entidad
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3(std::stof(xS), std::stof(yS), std::stof(zS)));
			if (tipo == tipoFisica::Trigger || tipo == tipoFisica::TriggerDinamico)
				trigger->setWorldTransform(t);

			body->setWorldTransform(t);
			pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->stepSimulation(1.0f / deltaTime);

		}

		if (!firstTime)
		{

			//Este sería el caso kinematico concreto de la niña
			//Para los enemigos habría que hacer otro
			if (tipo == Kinematico || tipo == TriggerDinamico) {
				if (msg.getTipo() == Tipo::Fisica) {
					if (msg.getSubTipo() == SubTipo::Mover) {
						body->activate(true);
						//Aquí lo movemos con la info procedente del input
						int pos = msg.getMsg().find("/");
						std::string xS = msg.getMsg().substr(0, pos);
						std::string subcad = msg.getMsg().substr(pos + 1);
						pos = subcad.find("/");
						std::string yS = subcad.substr(0, pos);
						std::string zS = subcad.substr(pos + 1);
						float xF = std::stof(xS) * deltaTime*0.1;
						float zF = std::stof(zS) * deltaTime*0.1;
						Ogre::Vector3 valores = { xF,0,zF };
						Ogre::Matrix3 matriz = pEntidad->getPEstado()->getScnManager()->getSceneNode("NodoCamera")->getLocalAxes();

						if (pEntidad->getNombreNodo() == "Alaia")
							valores = matriz * valores;

						btVector3 vel = body->getLinearVelocity();
						
						vel.setValue(valores.x * 30, vel.y(), valores.z * 30);
						body->setLinearVelocity(vel);

						timePas += deltaTime;
						if (pEntidad->getNombreNodo() == "Alaia" && timePas >= 100 && abs(vel.getY()) < 0.1) {
							timePas = 0;
							int r = rand() % 4 + 1;
							std::string paso = "Play/pasos" + std::to_string(r) + ".wav/" + "0/0/0";
							Mensaje msEfect(Tipo::Audio, paso, SubTipo::Effect);
							pEntidad->getPEstado()->addMsg(msEfect);
						}
					}
					else if (msg.getSubTipo() == SubTipo::Salto) {
						if ((int)body->getLinearVelocity().getY() == 0) {
							std::string pos = "0/0/0";
							Mensaje msEfect(Tipo::Audio, "Play/salto.wav/" + pos, SubTipo::Effect);
							pEntidad->getPEstado()->addMsg(msEfect);
							body->activate(true);
							body->applyCentralImpulse(btVector3(0, 2000, 0));
						}

					}
					else if (msg.getSubTipo() == SubTipo::Nulo) {
						if (tipo != tipoFisica::TriggerDinamico) {
							btVector3 vel = body->getLinearVelocity();
							vel = vel * btVector3(0, 1, 0);
							body->setLinearVelocity(vel);
						}
					}
				}
			}
			if ((tipo == tipoFisica::Trigger || tipo == tipoFisica::TriggerDinamico)) {
				if (!eliminado && (trigger != nullptr && trigger->getNumOverlappingObjects() > 0)) {
					std::string receptor = pEntidad->getPEstado()->getFisicManager()->getRigidBody((btRigidBody*)trigger->getOverlappingObject(0));
					if ( receptor == "Alaia") {
						std::string msgStr = "";
						Mensaje m(Tipo::Fisica, msgStr, SubTipo::Trigge);
						m.setMsgInfo(pEntidad, pEntidad->getPEstado()->getEntidad(receptor));
						std::string emisor = static_cast<Ogre::SceneNode*>(trigger->getUserPointer())->getName();
						std::string s = emisor.substr(5, 1);
						
						if (s == "C") {
							Mensaje ms(Tipo::Gui, "+25", SubTipo::CambiaMana);
							ms.setMsgInfo(pEntidad, pEntidad->getPEstado()->getEntidad("Alaia"));
							pEntidad->getPEstado()->addMsg(ms, true);
						}
						else if (s == "M") {
							Mensaje ms(Tipo::Gui, "+25", SubTipo::CambiaVida);
							ms.setMsgInfo(pEntidad, pEntidad->getPEstado()->getEntidad("Alaia"));
							pEntidad->getPEstado()->addMsg(ms);
						}


						pEntidad->getPEstado()->addMsg(m);
						pEntidad->getPEstado()->destroy(pEntidad->getNombreNodo());
						eliminado = true;
						
					}
				}

			}
			if (msg.getTipo() == Tipo::Fisica) {
				if (msg.getSubTipo() == SubTipo::Inicializado) {
					pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->stepSimulation(1.0f / deltaTime);
				}
			}
			if (msg.getTipo() == Tipo::Fisica) {
				if (!eliminado && msg.getSubTipo() == SubTipo::Colision) {
					pEntidad->getPEstado()->destroy(pEntidad->getNombreNodo());
					eliminado = true;
				}
			}
		}
		firstTime = false;
	}


	actualizaNodo();

	
}

//Actualiza el nodo gráfico en base a los cambios del físico
void FComponent:: actualizaNodo() {

	if (tipo == tipoFisica::Trigger || tipo == tipoFisica::TriggerDinamico) userPointer = trigger->getUserPointer();
	else userPointer = body->getUserPointer();
	btVector3 position; btQuaternion orientation;
	if (userPointer) {
		btQuaternion orientation = body->getOrientation();
		position = body->getWorldTransform().getOrigin();
		
		Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
		sceneNode->setPosition(Ogre::Vector3(position.getX() , position.getY(), position.getZ()));	
	}
}