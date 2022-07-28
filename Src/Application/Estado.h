#pragma once
#include "Entidad.h"
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include "OgreSceneNode.h"
#include <OgreConfig.h>
#include "OgreConfigFile.h"

#include <OgreTextureManager.h>
#include <OgreWindowEventUtilities.h>
#include <OgreException.h>
#include "Fisic.h"
#include <fmod.hpp>
#include <fmod_studio.hpp>

#include "GUI.h"


class Juego;

#include "Factory.h"



class Estado
{
public:
		Estado(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow, FMOD::System* sys, Juego * game = nullptr);
	virtual ~Estado();


	void addViewPort();
	Ogre::SceneManager* getScnManager(){ return scnMgr; };
	Fisic* getFisicManager() { return fisicaManager; };

	virtual bool update(float delta);
	virtual void keyPressed(std::string key);
	virtual void keyReleased(std::string key);
	virtual void joystickMoved(float x, float y, int js = 0);
	Entidad* getEntidad(std::string s);

	virtual void cambiaEstado(std::string const  &estado){}
	void addEntidad(std::string &name,Entidad * ent) {
		entidades.insert(std::make_pair(name, ent));
	}

	virtual bool initCEGUI();
	virtual void destroy() { 
		m_gui.hideMouseCursor();
		m_gui.getRoot()->destroyChild(guiRoot); 
	};
	virtual void init() {};

	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

	void destroy(std::string b) { 
		int i = 0;
		while (i < borrar.size() && borrar[i] != b) i++;
		if (i >= borrar.size())
			borrar.push_back(b); 
	}


	//mensajes-------------------------------------
	bool addMsg(Mensaje & msg, bool needNow = false){ 
		if(!needNow)bufer.push(msg); 
		else mensajes.push(msg);
		return true; }
	void swapMsgBufer(){
		mensajes.swap(bufer);
	}


	//Ogre------------------------------------------
	Ogre::RenderWindow* getWin() { return mWin; }
	Factory* getFactory() { return factoria; }
	CEGUI::Window* getGUIRoot() {
		return guiRoot;
	}
protected:
	Factory * factoria;
	std::vector<std::string> borrar;
	std::map<std::string, Entidad*> entidades;
	Ogre::SceneManager * scnMgr;
	Ogre::Camera* cam;
	Ogre::Viewport* vp;
	Ogre::Light* light;
	Ogre::RenderWindow* mWin;
	GUI m_gui;
	CEGUI::Window *guiRoot;

	std::priority_queue <Mensaje> mensajes, bufer;
	Fisic* fisicaManager;
	FMOD::System* system;
	bool noInput; int  contInput;


	Juego * game_;


};
