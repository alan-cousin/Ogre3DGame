#include "SoundManager.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <strsafe.h>
#include <comdef.h>
 
SoundManager::SoundManager(Entidad* pEnt, FMOD::System* sys) : Componente(pEnt){
	system = sys;
	cargaAudio("../Media/Reverb/Reverb.wav");
	cargarAssetsAudio();
} 
SoundManager::~SoundManager() { 

	FMOD_RESULT result;
	for (auto it : vfx) {
		it.second->release();
	}

	for (auto it : vmusic) {
		it.second->release();
	}
	for (auto it : cFx) {
		it->stop();
	}
	for (auto it : cMA) {
		it->stop();
	}
	
	mainGroup->release();
	result = reverbGroup->release();
	result = reverbUnit->release();
	result = reverbGroup->removeDSP(reverbUnit);
	result = reverbUnit->disconnectAll(true, true);
} 

void::SoundManager::cargarAssetsAudio() {
	FMOD_RESULT result;
	FMOD::Sound* sound;
	TCHAR szDir[MAX_PATH];
	TCHAR dir;
	char* cabecera = "..\\Media\\Sounds\\";
	ua_tcscpy(szDir, TEXT("..\\Media\\Sounds\\*"));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	DWORD dwError = 0;
	char* aux = " ";

	hFind = FindFirstFile(szDir, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		dwError = GetLastError();
		if (dwError != ERROR_FILE_NOT_FOUND) {
			do {
				_bstr_t b(FindFileData.cFileName);
				aux = b;
				if (aux[0] != '.') {
					std::string aux2;
					aux2.assign(cabecera); aux2.append(aux);
					result = system->createSound(aux2.c_str(), FMOD_3D, 0, &sound);
					sound->set3DMinMaxDistance(5.0f, 5000.0f);
					vfx.insert(std::pair<std::string, FMOD::Sound*>(b, sound));
				}

				FindNextFile(hFind, &FindFileData);
				dwError = GetLastError();
			} while (dwError != ERROR_NO_MORE_FILES);
		}
	}
	else {
		std::cout << "Handle invalido\n";
	}
	cabecera = "..\\Media\\Music\\";
	ua_tcscpy(szDir, TEXT("..\\Media\\Music\\*"));

	hFind = FindFirstFile(szDir, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		dwError = GetLastError();
		if (dwError != ERROR_FILE_NOT_FOUND) {
			do {
				_bstr_t b(FindFileData.cFileName);
				aux = b;
				if (aux[0] != '.') {
					std::string aux2;
					aux2.assign(cabecera); aux2.append(aux);
					result = system->createSound(aux2.c_str(), FMOD_3D, 0, &sound);
					sound->set3DMinMaxDistance(5.0f, 5000.0f);
					vmusic.insert(std::pair<std::string, FMOD::Sound*>(b, sound));
				}
				FindNextFile(hFind, &FindFileData);
				dwError = GetLastError();
			} while (dwError != ERROR_NO_MORE_FILES);
		}
	}
	else {
		std::cout << "Handle invalido\n";
	}
}

void SoundManager::cargaAudio(std::string irPath) {
	// Sistema de audio
	cFx.resize(12);
	cMA.resize(6);
	reverbConnectionAmbM.resize(6);
	reverbConnectionfx.resize(12);
	FMOD_RESULT result;
	result =system->createChannelGroup("reverb", &reverbGroup);
	result =system->createChannelGroup("main", &mainGroup);
	/*
	Creamos el recurso dcp y lo añadimos a la reverb
	*/

	system->createDSPByType(FMOD_DSP_TYPE_CONVOLUTIONREVERB, &reverbUnit);
	reverbGroup->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, reverbUnit);

	/*
	No vamos a usar el audio asi que lo abrimos solo para leer
	*/
	FMOD::Sound* irSound2;
	
	result = system->createSound("../Media/Reverb/Reverb.wav", FMOD_DEFAULT | FMOD_OPENONLY, NULL, &irSound2);

	/*
	Cogemos la informacion del archivo de audio
	*/
	FMOD_SOUND_FORMAT irSoundFormat;
	FMOD_SOUND_TYPE irSoundType;
	int irSoundBits, irSoundChannels;
	irSound2->getFormat(&irSoundType, &irSoundFormat, &irSoundChannels, &irSoundBits);
	unsigned int irSoundLength;
	irSound2->getLength(&irSoundLength, FMOD_TIMEUNIT_PCM);

	/*
	El formato del archivo de respuesta a impulso debe ser wav, PCM, 16 bits, 48 kh, el numero de canales no es importante
	*/
	unsigned int irDataLength = sizeof(short)* (irSoundLength * irSoundChannels + 1);
	short* irData = (short*)malloc(irDataLength);
	irData[0] = (short)irSoundChannels;
	unsigned int irDataRead;
	irSound2->readData(&irData[1], irDataLength - sizeof(short), &irDataRead);
	reverbUnit->setParameterData(FMOD_DSP_CONVOLUTION_REVERB_PARAM_IR, irData, irDataLength);

	/*
	Tenemos un canal en seco y otro con la reverb sola
	*/
	reverbUnit->setParameterFloat(FMOD_DSP_CONVOLUTION_REVERB_PARAM_DRY, -80.0f);

	/*
	Liberamos el recurso de respuesta a impulso
	*/
	free(irData);
	irSound2->release();

	system->set3DSettings(1.0f, 1.0f, 1.0f);
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR listenerpos = { 0.0f, 0.0f, -1.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

	mainGroup->set3DAttributes(&listenerpos, &vel, &up);
	system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
	system->update();
}

void SoundManager::reproduceFx(std::string fx, float x, float y, float z, float wet) {


	FMOD_VECTOR pos = { x *0.01f, y*0.01f, z*0.1f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	FMOD_RESULT result;

	vfx.at(fx)->setMusicSpeed(((float)(rand() % 60 - 30) / 100.0) + 1.0);
	FMOD::Sound* s = vfx[fx];

	bool cOcupied = false;
	cFx[0]->isPlaying(&cOcupied);
	if (!cOcupied) {
		system->playSound(s, mainGroup, false, &cFx[0]);
		result = cFx[0]->set3DAttributes(&pos, &vel);
		cFx[0]->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channelHead);
		reverbUnit->addInput(channelHead, &reverbConnectionfx[0], FMOD_DSPCONNECTION_TYPE_SEND);
		reverbConnectionfx[0]->setMix(wet);
	}
	else {
		int i = 1;
		while (i < 12 && cOcupied)
		{
			cFx[i]->isPlaying(&cOcupied);
			if (!cOcupied) {
				system->playSound(s, mainGroup, false, &cFx[i]);
				result = cFx[i]->set3DAttributes(&pos, &vel);
				cFx[i]->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channelHead);
				reverbUnit->addInput(channelHead, &reverbConnectionfx[i], FMOD_DSPCONNECTION_TYPE_SEND);
				reverbConnectionfx[i]->setMix(wet);
				
			}
			else if (i == cFx.size() - 1) {
				system->playSound(s, mainGroup, false, &cFx[i]);
				result = cFx[i]->set3DAttributes(&pos, &vel);
				cFx[i]->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channelHead);
				reverbUnit->addInput(channelHead, &reverbConnectionfx[i], FMOD_DSPCONNECTION_TYPE_SEND);
				reverbConnectionfx[i]->setMix(wet);
				i++;
			}
			else
			{
				i++;
			}
		}
	}

	system->update();

}

void SoundManager::reproduceAmbM(std::string amb, float wet, bool fade) {
	bool cOcupied = false;
	int i = 0;
	cMA[0]->isPlaying(&cOcupied);
	while (cOcupied && i < cMA.size()) {
		i++;
		cMA[i]->isPlaying(&cOcupied);

	}
	if (!cOcupied){
		vmusic.at(amb)->setMode(FMOD_LOOP_NORMAL);
		system->playSound(vmusic.at(amb), mainGroup, false, &cMA[i]);
		if (fade) {
			unsigned long long parentclock;
			cMA[i]->getDSPClock(NULL, &parentclock);
			cMA[i]->addFadePoint(parentclock, 0.0f);
			cMA[i]->addFadePoint(parentclock + 4096, 1.0f);
			cMA[i]->setVolume(0.24);
		}
		cMA[i]->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channelHead);
		reverbUnit->addInput(channelHead, &reverbConnectionAmbM[i], FMOD_DSPCONNECTION_TYPE_SEND);
		reverbConnectionAmbM[i]->setMix(wet);
	}

}
void SoundManager::Update(float deltaTime, Mensaje const & msj) {
	Componente::Update(deltaTime, msj);
	Mensaje msg = msj;
	if (msg.getTipo() == Tipo::Audio) {
		if (msg.getSubTipo() == SubTipo::Musica) {
			int pos = msg.getMsg().find("/");
			std::string accion = msg.getMsg().substr(0, pos);
			std::string cancion = msg.getMsg().substr(pos + 1);
			if (accion == "Play") {
				reproduceAmbM(cancion, 0.0, true);
			}
			else if (accion == "Stop") {
				if (cancion != "1")
					paraAmb(0, true);
				else paraAmb(1, true);

			}
		}
		if (msg.getSubTipo() == SubTipo::Effect) {
			int pos = msg.getMsg().find("/");
			std::string accion = msg.getMsg().substr(0, pos);
			std::string subcad = msg.getMsg().substr(pos + 1);
			pos = subcad.find("/");
			std::string fx = subcad.substr(0, pos);
			subcad = subcad.substr(pos + 1);
			pos = subcad.find("/");
			std::string xS = subcad.substr(0, pos);
			subcad = subcad.substr(pos + 1);
			pos = subcad.find("/");
			std::string yS = subcad.substr(0, pos);
			subcad = subcad.substr(pos + 1);
			pos = subcad.find("/");
			std::string zS = subcad.substr(0, pos);

			if (accion == "Play") {
				reproduceFx(fx, std::stof(xS), std::stof(yS),std::stof(zS),0);
			}
		}
	}
}
void SoundManager::paraAmb(int ch, bool fade) {
	if (fade) {
		unsigned long long parentclock;
		cMA[ch]->getDSPClock(NULL, &parentclock);
		cMA[ch]->addFadePoint(parentclock, 1.0f);
		cMA[ch]->addFadePoint(parentclock + 4096, 0.0f);
	}
	cMA[ch]->stop();
}