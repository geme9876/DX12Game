#include "pch.h"
#include "Device.h"

void Device::Init()
{

#ifdef _DEBUG
	//IID_PPV_ARGS ��ġ�� UID ����, ���������͸� ��ȯ����
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif

	HRESULT ret;
	ret = ::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));
	ret = ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}
