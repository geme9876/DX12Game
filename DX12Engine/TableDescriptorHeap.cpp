#include "pch.h"
#include "TableDescriptorHeap.h"
#include "DX12Engine.h"


void TableDescriptorHeap::Init(uint32 count)
{
	//몇개의 GPU뷰를 만들건지
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	//GPU 뷰에 들어갈 상수버퍼 갯수
	// 뷰 2개 * 상수버퍼(b0~b4) 5개
	desc.NumDescriptors = count * REGISTER_COUNT;

	//GPU에서 사용할 뷰
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//GPU에 접근하여 GPU상에 힙을 할당한다
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * REGISTER_COUNT;
}

void TableDescriptorHeap::Clear()
{
	//스왑체인(이중버퍼링)처럼 몇번째 뷰(힙)를 사용중인지
	_currentGroupIndex = 0;
}

void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::CommitTable()
{
	//GPU 메모리 상에서 레지스터로 업로드한다
	//그냥 상수버퍼 값을 실제 GPU상에 전달하는 부분
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	CMDLIST->SetGraphicsRootDescriptorTable(0, handle);

	//이중버퍼링에서 다음 렌더타겟에 미리 그리는 것처럼...
	_currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	//b0~b4 의 CPU 뷰의 주소값
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint32 reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	handle.ptr += reg * _handleSize;
	return handle;
}