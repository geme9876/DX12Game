#pragma once
class Mesh
{
public:

	void Init(std::vector<Vertex>& vertexs);
	void Render();

private:
	ComPtr<ID3D12Resource>	_vertexBuffer;

	//디스크립터 힙이 아니지만 동일한 역할 , 모델 뷰 구조인가? 뷰를 통해 리소스에 접근
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {}; 
	uint32 _vertexCount = 0;
};

