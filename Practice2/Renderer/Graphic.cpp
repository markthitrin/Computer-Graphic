#include "Graphic.h"

bool Graphics::Initialize(HWND hwnd, int width, int height) {
	if (!InitializeDirectX(hwnd, width, height))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	return true;
}

bool Graphics::InitializeShaders() {

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE) {
#ifdef  _DEBUG // Debug
	#ifdef _WIN64
			shaderfolder = L"";
	#else
			shaderfolder = L"D:/Programming/Computer_Graphics/Practice2/Debug/";
	#endif // _WIN64
#else // Release
	#ifdef _WIN64
			shaderfolder = L"";
	#else
			shaderfolder = L"";
	#endif // _WIN64
#endif 
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!this->vertexshader.Initialize(this->device, shaderfolder + L"VertexShader.cso", layout, numElements)) {
		return false;
	}

	if (!this->pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso")) {
		return false;
	}

	return true;
}

bool Graphics::InitializeScene() {
	Vertex v[] = {
		Vertex(0.0f, -0.1f),
		Vertex(-0.1f, 0.0f),
		Vertex(0.1f, 0.0f),
		Vertex(0.0f, 0.1f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height) {
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	// result Adapter : [0]AMD , [1]NVIDIA3050, [2]Microsoft basic reder driver
	// So I am going to use the nvidia one here.
	constexpr int adapter_id = 1;
	// This should be change depending on device;

	if (adapters.size() < 1) {
		ErrorLogger::Log("NO DXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(	
		adapters[adapter_id].pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		this->swapchain.GetAddressOf(),
		this->device.GetAddressOf(),
		NULL,
		this->deviceContext.GetAddressOf());

	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create device ans swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "GetBuffer Failed,");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	this->deviceContext->RSSetViewports(1, &viewport);

	return true;
}

void Graphics::RenderFrame() {
	float bgcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	this->deviceContext->Draw(3, 0);

	this->swapchain->Present(1, NULL);
}