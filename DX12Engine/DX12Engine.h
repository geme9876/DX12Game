#pragma once
class DX12Engine : public Singleton<DX12Engine>
{
public:
	void Init(const WindowInfo& info);
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};
	
	std::shared_ptr<class Device>			_device;
	std::shared_ptr<class CommandQueue>		_commandQueue;
	std::shared_ptr<class SwapChain>		_swapChain;
	std::shared_ptr<class DescriptorHeap>	_descriptorHeap;

};



