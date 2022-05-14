#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"

class DX12Engine : public Singleton<DX12Engine>
{
public:
	void Init(const WindowInfo& info);
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

	std::shared_ptr<Device>			GetDevice()			{ return _device; };
	std::shared_ptr<CommandQueue>	GetCmdQueue()		{ return _commandQueue; };
	std::shared_ptr<SwapChain>		GetSwapChain()		{ return _swapChain; };
	std::shared_ptr<RootSignature>	GetRootSignature()	{ return _rootSignature; };
	std::shared_ptr<ConstantBuffer>	GetConstantBuffer()	{ return _constantBuffer; };
	std::shared_ptr<TableDescriptorHeap>	GetTableDescHeap() { return _tableDescHeap; };


private:
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};
	
	std::shared_ptr<Device>			_device;
	std::shared_ptr<CommandQueue>	_commandQueue;
	std::shared_ptr<SwapChain>		_swapChain;
	std::shared_ptr<RootSignature>	_rootSignature;
	std::shared_ptr<ConstantBuffer>	_constantBuffer;
	std::shared_ptr<TableDescriptorHeap>	_tableDescHeap;
};

#define DEVICE			DX12Engine::This().GetDevice()->GetDevice()
#define CMDLIST			DX12Engine::This().GetCmdQueue()->GetCmdList()
#define ROOTSIGNATURE	DX12Engine::This().GetRootSignature()->GetSignature()
#define CONSTANTBUFFER	DX12Engine::This().GetConstantBuffer()
#define TABLEDESCHEAP	DX12Engine::This().GetTableDescHeap()




