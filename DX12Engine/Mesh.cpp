#include "pch.h"
#include "Mesh.h"
#include "DX12Engine.h"

void Mesh::Init(std::vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	//업로드 타입으로 생성 , 본래라면 DEFAUT를 별개로 생성하여 업로드는 복사 용도로만 사용 해야함
	//보통은 normal 타입으로 생성하고, 업로드만 전담할때는 업로드 타입이 효율적
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//GPU에 리소스 버퍼 할당
	//디바이스를 통해 즉시 전달 됨
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

	//버퍼 주소
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	// 정점 1개 크기
	_vertexBufferView.StrideInBytes = sizeof(Vertex); 
	// 버퍼의 크기
	_vertexBufferView.SizeInBytes = bufferSize; 	
}

void Mesh::Render()
{
	CMDLIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMDLIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	// 1) 버퍼에 데이터 셋팅
	// 2) 버퍼의 주소를 레지스터에 전송
	// 장치를 통한 전달과는 다르게 레지스터에 지연 전달 됨
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

	//커맨드큐에 커맨드 밀어넣음 (커맨드큐의 RenderEnd에서 실행됨)
}