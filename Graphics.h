
//THE BEGINNING OF HELL
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <vector>

namespace WRL = Microsoft::WRL;

class Graphics {
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	~Graphics();

	void ClearBuffer(float red, float green, float blue) noexcept {
		const float color[] = { red,green,blue,1.0f };
		devicecontext->ClearRenderTargetView(backbuffer.Get(), color);
		devicecontext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	};

	void RenderFrame();
private:
	WRL::ComPtr<ID3D11RenderTargetView> backbuffer;
	WRL::ComPtr<ID3D11Device> device;
	WRL::ComPtr<ID3D11DeviceContext> devicecontext;
	WRL::ComPtr<IDXGISwapChain> swapchain;
	WRL::ComPtr<ID3D11DepthStencilView> dsv;
};

