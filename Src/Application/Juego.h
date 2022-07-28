#pragma once
#include <stdio.h>

#include <string>
#include <stack>
#include <iostream>
#include "Estado.h"
#include "InputManager.h"



class Juego: public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener
{
public:
	Juego();
	~Juego();
	
	bool run();
	std::stack<Estado*>pEstados;
	std::stack<Estado*> borrar;
	void restart() {
		restart_ = true;
		exit = true;
	}
	void exitGame() {

	exit = true;
	}

	void quitaEstado() {
		Estado * aux = pEstados.top();
		borrar.push(aux);
		pEstados.pop();
	}
	void cambiaEstado(Estado* state, bool sobrescribe = false);
private:

	bool init();
	bool initOgre();
	bool initOis();
	bool initFmod();
	bool restart_;

	std::string recursos, plugins;
	Ogre::Root *root;
	Ogre::ConfigFile cf;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager * scnMgr;

	//Fmod------------------

	FMOD::System     *system;
	FMOD_RESULT       result;

	//Ois-------------------
	InputManager * mInputMgr;
		 



	void handleInput();
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	
	
	virtual bool axisMoved(const OIS::JoyStickEvent & arg, int axis);
	virtual bool buttonPressed(const OIS::JoyStickEvent & arg, int button);
	virtual bool buttonReleased(const OIS::JoyStickEvent & arg, int buton);

	virtual bool sliderMoved(const OIS::JoyStickEvent & arg, int index) { return true; }
	virtual bool vector3Moved(const OIS::JoyStickEvent & arg, int index) { return true; }
	virtual bool povMoved(const OIS::JoyStickEvent & arg, int index);
	
	bool exitGame(const CEGUI::EventArgs &e);


	int contJoystick;
	bool exit = false;
	bool firstTime;
};

