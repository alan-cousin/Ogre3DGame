#pragma once 
#include "Estado.h"
#include "EstadoJuego.h"
class EstadoMenu : public Estado
{
public:
	EstadoMenu(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow, FMOD::System* sys, Juego* game = nullptr, std::string type = "ppal");
	virtual ~EstadoMenu();

	virtual bool initCEGUI();

	void opciones();
	void estadoAnt();
	void level1();
	void salir();
	void restart();
	void creditos();
	virtual bool update(float delta);
	virtual void joystickMoved(float x, float y, int js = 0);
	virtual void keyPressed(std::string key);
	virtual void keyReleased(std::string key);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

private:

	bool callback = false;
	std::string type_;
	void initMenuPause();
	void initCreditos();
	void initOpciones();
	void initPpal();
	void initGameOver();
	std::vector<CEGUI::PushButton*> botones;
	
};
