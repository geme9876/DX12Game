#include "pch.h"
#include "Shader.h"
#include "DX12Engine.h"

HRESULT Shader::Init(const std::wstring& path)
{
	//예외 던져서 처리해야할듯?

	HRESULT hr;
	hr = CreateVertexShader(path, "VS_Main", "vs_5_0");
	if (FAILED(hr))
	{
		return hr;
	}
	hr = CreatePixelShader(path, "PS_Main", "ps_5_0");
	if (FAILED(hr))
	{
		return hr;
	}

	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	_pipelineDesc.InputLayout = { desc, _countof(desc) };
	_pipelineDesc.pRootSignature = ROOTSIGNATURE.Get();

	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_pipelineDesc.DepthStencilState.DepthEnable = FALSE;
	_pipelineDesc.DepthStencilState.StencilEnable = FALSE;
	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;


	//TODO : try catch로 디바이스 생성 예외처리 해보자
	hr = DEVICE->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));

	return hr;
}

void Shader::Update()
{
	CMDLIST->SetPipelineState(_pipelineState.Get());
}

HRESULT Shader::CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = ::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, &blob, &_errBlob);
	if (FAILED(hr))
	{
		if (_errBlob)
		{
			OutputDebugStringA((char*)_errBlob->GetBufferPointer());
		}
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
		return hr;
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
	return hr;
}

HRESULT Shader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	return CreateShader(path, name, version, _vsBlob, _pipelineDesc.VS);
}

HRESULT Shader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	return CreateShader(path, name, version, _psBlob, _pipelineDesc.PS);
}