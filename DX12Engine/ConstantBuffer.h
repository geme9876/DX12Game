#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	//버퍼의 상자
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE* _mappedBuffer = nullptr;

	//버퍼 안에 들어있는 물품의 크기와 갯수
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;


	ComPtr<ID3D12DescriptorHeap> _cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE	_cpuHandleBegin = {}; //시작 핸들 주소와 간격
	uint32	_handleIncrementSize = 0;

	uint32					_currentIndex = 0;
};

