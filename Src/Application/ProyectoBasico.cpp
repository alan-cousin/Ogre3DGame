#include <stdio.h>
#include "Juego.h"
//#include <vld.h>
#include <cstdlib>
#include <iostream>
#include <ctime>


#ifdef _DEBUG || !_WIN32
int main(){
#else
#include <Windows.h>
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	){
#endif
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
	std::srand(std::time(nullptr)); // use current time as seed for random generator

	Juego* game = new Juego();
	game->run();
	delete game;
	return 0;
}