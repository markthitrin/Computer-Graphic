#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) {
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height)) {
		return false;
	}
	if (!this->gfx.Initialize(this->render_window.GetHWND(), width, height)) {
		return false;
	}
	return true;
}

bool Engine::ProcessMessages() {
	return this->render_window.ProcessMessages();
}

void Engine::Update() {
	while (!keyboard.KeyBufferIsEmpty()) {
		unsigned char c = keyboard.ReadKey().GetKeyCode();
		std::string outmsg = "keycode : ";
		outmsg += c;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}
	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent me = mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
			std::string out = std::to_string(me.GetPosX()) + " " + std::to_string(me.GetPosY()) + "\n";
			OutputDebugStringA(out.c_str());
		}
	}
}

void Engine::RenderFrame() {
	gfx.RenderFrame();
}