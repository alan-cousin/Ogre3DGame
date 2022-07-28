#pragma once
#include <map>
class Entidad;
class Mensaje;

enum tipoFisica { Dinamico, Estatico, Kinematico, Trigger, TriggerDinamico };

class Componente {

public:
	Componente(Entidad* pEnt) { pEntidad = pEnt; activo = true; };
	virtual ~Componente() {}
	virtual void Update(float deltaTime, Mensaje const & msj) { if (!activo) return; };
	virtual void Awake() { activo = true; };
	virtual void Sleep() { activo = false; };
	virtual void destroy() {};

protected:
	bool activo;
	Entidad* pEntidad;
};