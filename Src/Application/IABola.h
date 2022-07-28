#pragma once 
#include "Componente.h"
#include "Estado.h"
enum States { Hunt, Wander, Stay, Die, p };
enum WanderState { Right, Left, Up, Down, RU,RD, LU,LD };
class IABola: public Componente
{
public: 
	IABola(Entidad* pEnt, std::string target, int x, int y, int z);
	~IABola(); 

	virtual void Update(float deltaTime, Mensaje const & mensaje);
	void wander();
	void stay();
	void hunt();
	void divide();
	void die();
private:
	void restaurapS();
	Ogre::SceneNode* node;
	Ogre::ParticleSystem* pS;
	int x, y, z;
	Ogre::SceneNode *target;
	States state;
	WanderState wState;
	int cont;
	int contDivide;
	int numDiv;
	int valueDiv, valueWait, velDir;
}; 
