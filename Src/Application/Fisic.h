#pragma once 
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include <vector>
#include <map>
#include "LinearMath\btIDebugDraw.h"


//Esta clase tendrá una instancia en el estado
//Regulará todo lo relacionado con el mundo físico
class Fisic { 
public: 
	Fisic(); 
	~Fisic(); 
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };
	//Devuelve el cuerpo con el nombre del nodo que se le pasa
	btRigidBody* getRigidBody(std::string nombre) { return physicsAccessors.at(nombre); };
	std::string getRigidBody(btRigidBody * rg) { 
		auto it = physicsAccessors.begin();
		std::string salida = "";
		while (it != physicsAccessors.end() && salida == "") {
			if (it->second == rg) salida = it->first;
			it++;
		}
		return salida;
	};
	void addBodyToMap(std::string n, btRigidBody* b) { physicsAccessors.insert(std::pair<std::string, btRigidBody*>(n, b)); };

private:
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	

	std::vector<btCollisionShape*> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
	btGhostPairCallback* ghost;
}; 
