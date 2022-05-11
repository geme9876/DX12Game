#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();
	void Init(ComPtr<ID3D12Device> device, std::shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }

private:
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	//GPU µø±‚»≠ ∞¥√º
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	std::shared_ptr<SwapChain>			_swapChain;
	std::shared_ptr<DescriptorHeap>		_descHeap;
};

