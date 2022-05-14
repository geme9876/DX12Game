#include "pch.h"
#include "TableDescriptorHeap.h"
#include "DX12Engine.h"


void TableDescriptorHeap::Init(uint32 count)
{
	//��� GPU�並 �������
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	//GPU �信 �� ������� ����
	// �� 2�� * �������(b0~b4) 5��
	desc.NumDescriptors = count * REGISTER_COUNT;

	//GPU���� ����� ��
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//GPU�� �����Ͽ� GPU�� ���� �Ҵ��Ѵ�
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * REGISTER_COUNT;
}

void TableDescriptorHeap::Clear()
{
	//����ü��(���߹��۸�)ó�� ���° ��(��)�� ���������
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
	//GPU �޸� �󿡼� �������ͷ� ���ε��Ѵ�
	//�׳� ������� ���� ���� GPU�� �����ϴ� �κ�
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	CMDLIST->SetGraphicsRootDescriptorTable(0, handle);

	//���߹��۸����� ���� ����Ÿ�ٿ� �̸� �׸��� ��ó��...
	_currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	//b0~b4 �� CPU ���� �ּҰ�
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint32 reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	handle.ptr += reg * _handleSize;
	return handle;
}