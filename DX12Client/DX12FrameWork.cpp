#include "pch.h"
#include "DX12FrameWork.h"
#include "DX12Engine.h"

void DX12FrameWork::Init(const WindowInfo& info)
{
	DX12Engine::This().Init(info);
}

void DX12FrameWork::Update()
{
	DX12Engine::This().Render();
}
