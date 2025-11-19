#pragma once

#include <Windows.h>
#include <optional>

#include "Graphics.h"
#include "Audio.h"
#include "Mouse.h"
#include "Keyboard.h"

class Window {

public:
	class WindowClass {
	public:
		static LPCWSTR GetName();
		static HINSTANCE GetInstance();
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static LPCWSTR wndClassName;
		static WindowClass wndclass;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, LPCWSTR ClassName);
	~Window();
	Window(const Window*) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages();
	void SetWindowTitle(LPCSTR name);
public:
	Audio audio;
	Mouse mouse;
	Keyboard kbd;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
private:
	HWND hwnd;
	UINT height;
	UINT width;

};