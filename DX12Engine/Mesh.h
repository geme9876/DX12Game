#pragma once
class Mesh
{
public:

	void Init(std::vector<Vertex>& vertexs);
	void Render();

private:
	ComPtr<ID3D12Resource>	_vertexBuffer;

	//��ũ���� ���� �ƴ����� ������ ���� , �� �� �����ΰ�? �並 ���� ���ҽ��� ����
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {}; 
	uint32 _vertexCount = 0;
};

