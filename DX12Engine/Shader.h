#pragma once
class Shader
{
public:
	HRESULT Init(const std::wstring& path);
	void Update();

private:
	HRESULT CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	HRESULT CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	HRESULT CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

