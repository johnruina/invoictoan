#pragma once

#include "Timer.h"

#include "Window.h"

class App {
public:
	App();
	~App() = default;
	int Go();
private:
	Window wnd;
	Timer timer;
	void DoFrame();
};