#include <Windows.h>
#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int	nCmdShow) {

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to call CInitialize.");
		return -1;
	}
	Engine engine;
	if (engine.Initialize(hInstance, "Title", "MyWindowClass", 800, 600)) {
		while (engine.ProcessMessages()) {
			engine.Update();
			engine.RenderFrame();
		}
	}
	return 0;
}