#pragma once
#include "GComponent.h"
#include "Estado.h"


class Animation :
	public GComponent
{
public:
	Animation(Entidad* pEnt, std::string mesh, std::string anim = "");
	virtual ~Animation();
	virtual void Update(float deltaTime, Mensaje const & msj);


	void addAnimationState(std::string name, bool loop = false, bool enabled = true, bool idle = false);

private:
	std::map<std::string, Ogre::AnimationState*> animStates;
	float time;
};


