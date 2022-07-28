#include <string>
#include "Estado.h"
#pragma once

enum Tipo{ Render, Fisica, Logica, Input, AnimationM, Audio, IA, Gui, Mana,Consumible };
enum SubTipo { Musica, Rotar, Escalar, Salto, Mover, InitFis, Reposicionar, Orientar, OrientaCamara, Effect, Trigge, Dispara, Nulo, 
	InitGui, CambiaVida, CambiaMana, DoPower,GetPower, Inicializado, Colision,Candy,Muffin};


class Mensaje{
public:
	bool operator<(const Mensaje &o) const
	{
		return prioridad < o.prioridad;
	}
	Mensaje(Tipo tip, std::string msg, SubTipo subtipo = SubTipo::Nulo, int prio = 0) { 
		tipo = tip; mensaje = msg; 
		subTipo = subtipo;

		if (prio == 0){
			switch (tipo)
			{
			case Render:
				prio = 1;
				break;
			case AnimationM:
				prio = 1;
				break;
			case Fisica:
				prio = 2;
				break;
			case Input:
				prio = 3;
				break;
			default:
				prio = 0;
				break;
			}
		}
		prioridad = prio; 
		emisor = nullptr;
		receptor = nullptr;
	}
	void setMsgInfo(Entidad* em, Entidad* re = nullptr) {
		emisor = em;
		receptor = re;
	}
	int getPriority() { return prioridad; }
	Entidad* getEmisor() { return emisor; }
	Entidad* getReceptor() { return receptor; };
	std::string getMsg() { return mensaje; }
	int getTipo() { return tipo; }
	int getSubTipo() { return subTipo; }

private:
	std::string mensaje;
	Entidad * emisor;
	Entidad * receptor;
	int prioridad;
	Tipo tipo;
	SubTipo subTipo;
};