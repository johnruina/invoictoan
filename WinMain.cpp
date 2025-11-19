
#include <Windows.h>
#include <exception>

#include "App.h"

int CALLBACK	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	try {
		return App{}.Go();
	}
	catch (const std::exception e) {
		MessageBoxA(nullptr, e.what(), "standard exception", MB_OK | MB_ICONERROR);
	} 
	catch (...) {
		MessageBoxA(nullptr, "no details available", "unknown exception", MB_OK | MB_ICONERROR);
	}
	return -1;

}