#include "pch.h"
#include "Mesh.h"
#include "DX12Engine.h"

void Mesh::Init(std::vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	//���ε� Ÿ������ ���� , ������� DEFAUT�� ������ �����Ͽ� ���ε�� ���� �뵵�θ� ��� �ؾ���
	//������ normal Ÿ������ �����ϰ�, ���ε常 �����Ҷ��� ���ε� Ÿ���� ȿ����
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//GPU�� ���ҽ� ���� �Ҵ�
	//����̽��� ���� ��� ���� ��
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	//���� �ּ�
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	// ���� 1�� ũ��
	_vertexBufferView.StrideInBytes = sizeof(Vertex); 
	// ������ ũ��
	_vertexBufferView.SizeInBytes = bufferSize; 	
}

void Mesh::Render()
{
	CMDLIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMDLIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	// 1) ���ۿ� ������ ����
	// 2) ������ �ּҸ� �������Ϳ� ����
	// ��ġ�� ���� ���ް��� �ٸ��� �������Ϳ� ���� ���� ��
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = CONSTANTBUFFER->PushData(0, &_transform, sizeof(_transform));
		TABLEDESCHEAP->SetCBV(handle, CBV_REGISTER::b0);
	}

	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = CONSTANTBUFFER->PushData(0, &_transform, sizeof(_transform));
		TABLEDESCHEAP->SetCBV(handle, CBV_REGISTER::b1);
	}
	
	TABLEDESCHEAP->CommitTable();

	CMDLIST->DrawInstanced(_vertexCount, 1, 0, 0);

	//Ŀ�ǵ�ť�� Ŀ�ǵ� �о���� (Ŀ�ǵ�ť�� RenderEnd���� �����)
}