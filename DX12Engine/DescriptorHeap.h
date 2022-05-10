#pragma once
class SwapChain;
class DescriptorHeap // dx11까지는 view라는 이름이었음
{
public:
	void Init(ComPtr<ID3D12Device> device,std::shared_ptr<SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int32 idx) { return _rtvHandle[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE		GetBackBufferView();
private:
	ComPtr<ID3D12DescriptorHeap>		_rtvHeap;
	uint32								_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE			_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];
	std::shared_ptr<class SwapChain>	_swapChain;
};

