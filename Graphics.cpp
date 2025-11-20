#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

//#define CHECK_HR(hr) if (FAILED(hr)) throw "GRAPHICS ERROR";
#define CHECK_HR(hr) hr;

struct VERTEX { FLOAT X, Y, Z; float R,G,B,A; };

Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferCount = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapchain, &device, nullptr, &devicecontext
	);

	WRL::ComPtr<ID3D11Resource> pbackbuffer;

	swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &pbackbuffer);
	device->CreateRenderTargetView(pbackbuffer.Get(), nullptr, &backbuffer);

	D3D11_DEPTH_STENCIL_DESC dsdesc = {};
	dsdesc.DepthEnable = TRUE;
	dsdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsdesc.DepthFunc = D3D11_COMPARISON_LESS;
	WRL::ComPtr<ID3D11DepthStencilState> dss;
	device->CreateDepthStencilState(&dsdesc, &dss);

	devicecontext->OMSetDepthStencilState(dss.Get(), 1u);

	WRL::ComPtr<ID3D11Texture2D> depthstencil;
	D3D11_TEXTURE2D_DESC descdepth = {};
	//1280, 720
	descdepth.Height = 720u;
	descdepth.Width = 1280u;
	descdepth.MipLevels = 1u;
	descdepth.ArraySize = 1u;
	descdepth.Format = DXGI_FORMAT_D32_FLOAT;
	descdepth.SampleDesc.Count = 1u;
	descdepth.SampleDesc.Quality = 0u;
	descdepth.Usage = D3D11_USAGE_DEFAULT;
	descdepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device->CreateTexture2D(&descdepth, nullptr, &depthstencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descdsv = {};
	descdsv.Format = DXGI_FORMAT_D32_FLOAT;
	descdsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descdsv.Texture2D.MipSlice = 0u;

	device->CreateDepthStencilView(depthstencil.Get(), &descdsv, &dsv);

	devicecontext->OMSetRenderTargets(1u, backbuffer.GetAddressOf(), dsv.Get());

}

Graphics::~Graphics()
{
    swapchain->SetFullscreenState(FALSE, NULL);
}

void Graphics::RenderFrame()
{
    swapchain->Present(1u, 0u);

    HRESULT hr;
    if (FAILED(hr = swapchain->Present(1u, 0u))) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw device->GetDeviceRemovedReason();
        }
        else {
            throw "smth went wrong";
        }
    }
}
