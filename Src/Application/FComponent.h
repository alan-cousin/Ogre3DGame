#pragma once 

#include "Componente.h"
#include "Estado.h"

class FComponent  : public Componente { 
public: 
	FComponent(Entidad* pEnt, float altoCaja = 0, float anchoCaja=0, float profCaja=0, std::string nombreNodo=" ", bool suelo= false, tipoFisica type = tipoFisica::Estatico, btScalar masa = 0/*inamovible*/);
	~FComponent(); 
	virtual void Update(float deltaTime, Mensaje const & msj);//A rellenar con el cuenta steps
	virtual void destroy() {};
	void initBody();
	void actualizaNodo();
	btRigidBody* getRigidBody() { return body; };

protected:
	float altoCaja, anchoCaja, profCaja;
	btTransform pTransform;
	tipoFisica tipo;
	btScalar mass;
	btCollisionShape* shape;
	btDefaultMotionState *motionState;
	btRigidBody* body;
	btTransform trans;
	btGhostObject* trigger;
	void *userPointer;
	bool _suelo;
	bool eliminado, firstTime;
	float timePas;
}; 
