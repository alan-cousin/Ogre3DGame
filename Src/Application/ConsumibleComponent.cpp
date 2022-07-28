#include "ConsumibleComponent.h"  
#include "Estado.h"

ConsumibleComponent::ConsumibleComponent(Entidad* pEnt) : Componente(pEnt) {



}
ConsumibleComponent::~ConsumibleComponent() {
}
void ConsumibleComponent::Update(float deltaTime, Mensaje const & msj)
{
	Mensaje msg = msj;
	if (msg.getTipo() == Tipo::Consumible) {

		if (msg.getSubTipo() == SubTipo::Candy)
		{
			Entidad *aux4 = new Entidad(pEntidad->getPEstado());
			string auxcandy = pEntidad->getPEstado()->getFactory()->create("Candy");
			aux4->setNombreNodo(auxcandy);
			aux4->añadeComponenteGrafico("Candy", auxcandy);
			aux4->añadeComponenteFisico(2, 0, 2, false, tipoFisica::Trigger, 1);
			pEntidad->getPEstado()->addEntidad(auxcandy, aux4);
			Mensaje ms4(Tipo::Fisica, msg.getMsg(), SubTipo::Reposicionar, 8);
			ms4.setMsgInfo(aux4, aux4);
			aux4->getPEstado()->addMsg(ms4,true);
		}
		else if (msg.getSubTipo() == SubTipo::Muffin)
		{
			Entidad *aux4 = new Entidad(pEntidad->getPEstado());
			string auxMuffin = pEntidad->getPEstado()->getFactory()->create("Muffin");
			aux4->setNombreNodo(auxMuffin);
			aux4->añadeComponenteGrafico("Muffin", auxMuffin);
			aux4->añadeComponenteFisico(2, 0, 2, false, tipoFisica::Trigger, 1);
			pEntidad->getPEstado()->addEntidad(auxMuffin, aux4);
			Mensaje ms4(Tipo::Fisica, msg.getMsg(), SubTipo::Reposicionar, 8);
			ms4.setMsgInfo(aux4, aux4);
			aux4->getPEstado()->addMsg(ms4,true);
		}
	}





}
