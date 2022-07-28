#include "Estado.h"
#include "Juego.h"
#include "FactoryBalas.h"

class EstadoMenu;
Estado::Estado(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow, FMOD::System* sys, Juego* game){

	//Aqui montamos el mundo físico
	scnMgr = mng;
	mWin = mWindow;
	system = sys;
	game_ = game;
	fisicaManager = new Fisic();
}

bool Estado::initCEGUI() {
	m_gui.init("../Media/GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.loadScheme("GameMenu.scheme");
	m_gui.loadScheme("HUDDemo.scheme");
	m_gui.loadScheme("VanillaSkin.scheme");
	m_gui.loadScheme("AlfiskoSkin.scheme");
	m_gui.loadScheme("Generic.scheme");
	m_gui.loadScheme("OgreTray.scheme");
	m_gui.loadScheme("SampleBrowser.scheme");
	m_gui.loadScheme("VanillaCommonDialogs.scheme");
	m_gui.loadScheme("WindowsLook.scheme");
	m_gui.loadScheme("Generic.scheme");
	m_gui.setFont("DejaVuSans-10");



	return true;
}
Estado::~Estado(){
	for (std::pair<std::string,Entidad*> n : entidades) {
		delete n.second;
	}
	delete fisicaManager;
	delete factoria;
	
}

bool Estado::update(float delta){

	m_gui.draw();

	return true;
}

void Estado::joystickMoved(float x, float y, int js) {


}
void Estado::keyPressed(std::string s) {
	
}
void Estado::keyReleased(std::string s) {

}
Entidad* Estado::getEntidad(std::string s) {
	auto flag = entidades.find(s);
	if (flag != entidades.end()) return entidades.at(s);
	
	return nullptr;
}

bool Estado::mouseMoved(const OIS::MouseEvent& me) {
	
	m_gui.moveMouse(me.state.X.abs, me.state.Y.abs);
	return true;
}
bool Estado::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) {
	m_gui.downMouse(id);
	return true;
}
bool Estado::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) {
	m_gui.upMouse(id);
	return true;
}