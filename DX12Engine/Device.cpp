#include "pch.h"
#include "Device.h"

void Device::Init()
{

#ifdef _DEBUG
	//IID_PPV_ARGS 장치의 UID 값과, 이중포인터를 반환해줌
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif

	HRESULT ret;
	ret = ::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));
	ret = ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}
