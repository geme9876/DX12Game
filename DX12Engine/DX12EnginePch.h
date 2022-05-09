#pragma once
#include<windows.h>
#include<tchar.h>
#include<memory>
#include<string>
#include<vector>
#include<array>
#include<list>
#include<map>

#include "3rd/DirectX-Headers/include/directx/d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#pragma comment(lib,"d3d12")
#pragma comment(lib,"dxgi")
#pragma comment(lib,"dxguid")
#pragma comment(lib,"d3dcompiler")

using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= DirectX::XMFLOAT2;
using Vec3		= DirectX::XMFLOAT3;
using Vec4		= DirectX::XMFLOAT4;
using Matrix	= DirectX::XMMATRIX;

void HelloEngine();