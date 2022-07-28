#include "Animation.h"



Animation::Animation(Entidad * pEnt, std::string mesh, std::string anim): GComponent (pEnt, pEnt->cont,mesh)
{
	if (anim != "") addAnimationState(anim);
	time = 0.017f;
}


Animation::~Animation()
{

}

void Animation::Update(float deltaTime, Mensaje const & msj) {

	Mensaje msg = msj;
	GComponent::Update(deltaTime, msj);
	std::vector<std::string> borrar;
	if (msg.getTipo() == Tipo::AnimationM) {
		for (std::pair<std::string, Ogre::AnimationState*> pS : animStates) {
			pS.second->addTime(time);
			if (pS.second->hasEnded() && !pS.second->getLoop()) {
				borrar.push_back(pS.first);
				pS.second->setEnabled(false);
			}
		}

		for (std::string s : borrar)
			animStates.erase(s);
	}
}
void Animation:: addAnimationState(std::string name, bool loop, bool enabled, bool idle) {
	
	if (idle) {
		if (animStates.find("Run") != animStates.end()) {
			animStates.at("Run")->setEnabled(false);
			animStates.erase("Run");
		}
		
	}
	if (idle && animStates.size() != 0) 
		return;
	if (name == "Run" ) time = 0.005f;
	else time = 0.005f;
	if (animStates.find(name) == animStates.end()) {
		Ogre::AnimationState* newState;
		
		newState = ent->getAnimationState(name);
		
		newState->setTimePosition(0);
		newState->setEnabled(enabled);
		newState->setLoop(loop);
		
		animStates.insert(std::make_pair(name, newState));
	}
}