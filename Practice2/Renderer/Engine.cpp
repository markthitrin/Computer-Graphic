#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) {
	timer.Start();
	
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
	float dt = timer.GetMillisecondElapsed();
	timer.Restart();
	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}
	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown()) {
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
				this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01, 0);
			}
		}
	}
	
	const float cameraSpeed = 0.006;
	if (keyboard.KeyIsPressed('W')) {
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('S')) {
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('A')) {
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('D')) {
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed(VK_SPACE)) {
		this->gfx.camera.AdjustPosition(0, cameraSpeed * dt, 0);
	}
	if (keyboard.KeyIsPressed('Z')) {
		this->gfx.camera.AdjustPosition(0, -cameraSpeed * dt, 0);
	}
}

void Engine::RenderFrame() {
	gfx.RenderFrame();
}