#pragma once
#include "Entidad.h"
#include "GComponent.h"
#include "Transform.h"
#include "Animation.h"
#include "Camara.h"
#include "FComponent.h"
#include <iostream>
#include <fstream>
#include "SoundManager.h"
#include "BalaComponent.h"
#include "IABola.h"
#include "LifeComponent.h"
#include "ManaComponent.h"
#include "ConsumibleComponent.h"
Entidad::Entidad(Estado* pEstado): pEstado(pEstado){
	cont++;
	activo = true;
	nombreNodo = " ";
}

Entidad::Entidad(Estado* pEstado, std::string prefab, std::string nombre) : pEstado(pEstado) {
	cont++;
	
	if (nombre == "" )
		if (pEstado->getEntidad(prefab) != nullptr)
			nombreNodo = prefab + to_string(cont);
		else nombreNodo = prefab;
	else
		nombreNodo = nombre;

	std::string path = "../Media/prefabs/";
	path += prefab + ".txt";
	std::ifstream fe(path);
	std::string type;
	fe >> type;
	activo = true;

	while (type != "---") {
		if (type == "Grafico") {
			std::string mesh; fe >> mesh;
			añadeComponenteGrafico(mesh, nombreNodo);
		}
		else if (type == "Logico") {
			std::string component; fe >> component;
			añadeComponenteLogico(component);
		}
		else if (type == "Animation") {
			std::string mesh; fe >> mesh;
			añadeAnimacion(mesh, fe);
		}
		else if (type == "Camara") {
			componentes.insert(std::make_pair("Camara", new Camara(this)));
		}
		else if (type == "CFisico") {
			float altoCaja; float anchoCaja; float profCaja; bool suelo; tipoFisica type; int masa;
			fe >> altoCaja >> anchoCaja >> profCaja >> suelo;
			int auxType; fe >> auxType; type = (tipoFisica)auxType;
			fe >> masa;
			añadeComponenteFisico(altoCaja, anchoCaja, profCaja, suelo, type, masa);
		}
		fe >> type;
	}
	fe.close();
	
}

Entidad::~Entidad()
{
	for (std::pair<std::string, Componente*> p : componentes) {
		delete p.second;
	}
	
}

bool Entidad::añadeAnimacion(std::string mesh, std::ifstream & fe) {
	if (fe.is_open()) {
		auto s = componentes.find("Grafico");
		if (s != componentes.end())
		{
			std::cout << "No puede tener componente grafico y animacion\n";
		}
		Animation *animation = new Animation(this, mesh);
		int numAnims; fe >> numAnims;
		for (int i = 0; i < numAnims; i++) {
			std::string name; fe >> name;
			animation->addAnimationState(name);
		}
		componentes.insert(std::make_pair("Anim", animation));
		return true;
	}
	else return false;
	
}
void Entidad::setAnim(std::string const &s, bool loop, bool enabled, bool idle) {
	auto flag = componentes.find("Anim");
	if (flag != componentes.end()) {
		static_cast<Animation*>(componentes.at("Anim"))->addAnimationState(s, loop, enabled, idle);
	}
}
bool Entidad::añadeAnimacion(std::string name, bool enabled, bool loop) {

	auto s = componentes.find("Grafico");
	if (s != componentes.end())
	{
		componentes.at("Grafico")->destroy();
		componentes.erase(s);
	}
	dynamic_cast<Animation*>(componentes.at("Anim"))->addAnimationState(name,enabled, loop);
	return true;
}

bool Entidad::añadeComponenteGrafico(std::string mesh) {
	nombreNodo = mesh + to_string(cont);
	componentes.insert(std::make_pair("Grafico", new GComponent(this, cont,mesh)));
	return true;
}

bool Entidad::añadeComponenteGrafico(std::string mesh, std::string ogreNodeName) {
	nombreNodo = ogreNodeName;
	componentes.insert(std::make_pair("Grafico", new GComponent(this, mesh,nombreNodo)));
	return true;
}

bool Entidad::añadeComponenteFisico(float altoCaja, float anchoCaja, float profCaja ,bool suelo, tipoFisica type, int masa) {
	
	componentes.insert(std::make_pair("Fisico", new FComponent(this, altoCaja, anchoCaja, profCaja, nombreNodo, suelo, type, masa)));
	return true;
}

bool Entidad::añadeComponenteLogico(std::string component) {
	if (component == "Transform") {
		componentes.insert(std::make_pair("Transform", new Transform(this, 0, 0, 0)));
	}
	if (component == "BalaComponent") {	
			componentes.insert(std::make_pair("BalaComponent", new BalaComponent(this)));
	}
	if (component == "BalaComponentSimple") {
		componentes.insert(std::make_pair("BalaComponent", new BalaComponent(this, "Simple")));

	}
	else if (component == "IABola") {
		componentes.insert(std::make_pair("IABola", new IABola(this,"Alaia", 0, 0, 0)));
	}
	else if (component == "LifeComponent") {
		componentes.insert(std::make_pair("LifeComponent", new LifeComponent(this)));
	}
	else if (component == "ManaComponent") {
		componentes.insert(std::make_pair("ManaComponent", new ManaComponent(this)));
	}
	else if (component == "ConsumibleComponent") {
		componentes.insert(std::make_pair("Consumible", new ConsumibleComponent(this)));
	}
	
	
	return true;
}


bool Entidad::añadeComponenteSM(std::string component, void* sys) {
	if (component == "SoundManager")
		componentes.insert(std::make_pair("SoundManager", new SoundManager(this, (FMOD::System*)sys)));
	return true;
}

void Entidad::Update(float deltaTime,  Mensaje & msj){
	Mensaje msg = msj;
	if (activo){
		
		if (msg.getReceptor() == this || msg.getReceptor() == nullptr) {
			for (std::pair<std::string, Componente*> c : componentes)
			{
				c.second->Update(deltaTime, msg);
			}
		}
	}
}

void Entidad::destruyeComponenteGrafico() {
		GComponent* g = dynamic_cast<GComponent*>  (componentes.at("Grafico"));
		if (g != nullptr)
			g->destroy();
}

void Entidad::Awake(){ activo = true; }
void Entidad::Sleep(){ activo = false; }
