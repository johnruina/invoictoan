#pragma once

#include <thread>
#include <string>
#include <random>

#include "App.h"

App::App() : wnd(1280,720,L"tbaltd")
{
	
}

int App::Go() {

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); 

		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
			
		DoFrame();
	}

	return 0;
}

long long int frame = 0;	

void App::DoFrame()
{
	frame++;
	float color = float(rand()%255) / 255.0f;
	wnd.gfx().ClearBuffer(color,color, 1.0f);
	if (frame == 5) {
		wnd.audio.PlayAudio(L"Agartha.wav");
	}

	wnd.gfx().RenderFrame();
}
