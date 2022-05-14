#include "pch.h"
#include "DX12Engine.h"


void DX12Engine::Init(const WindowInfo& info)
{
	_window = info;
	ResizeWindow(info.width, info.height);

	_viewport = { 0,0,static_cast<FLOAT>(info.width),static_cast<FLOAT>(info.height), 0.f, 1.f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);


	_device				= std::make_shared<Device>();
	_commandQueue		= std::make_shared<CommandQueue>();
	_swapChain			= std::make_shared<SwapChain>();
	_rootSignature		= std::make_shared<RootSignature>();
	_constantBuffer		= std::make_shared<ConstantBuffer>();
	_tableDescHeap = std::make_shared<TableDescriptorHeap>();



	_device->Init();
	_commandQueue->Init(_device->GetDevice(),_swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _commandQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_constantBuffer->Init(sizeof(Transform), 256);
	_tableDescHeap->Init(256);

}

void DX12Engine::Render()
{
	RenderBegin();

	RenderEnd();
}

void DX12Engine::RenderBegin()
{
	_commandQueue->RenderBegin(&_viewport,&_scissorRect);
}

void DX12Engine::RenderEnd()
{
	_commandQueue->RenderEnd();
}

void DX12Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0,0,width,height };
	::AdjustWindowRect(&rect, WS_EX_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}
