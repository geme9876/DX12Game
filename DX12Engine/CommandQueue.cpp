#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DX12Engine.h"
CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, std::shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator
	// 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_cmdList->Close();

	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

}

void CommandQueue::WaitSync()
{

	++_fenceValue;

	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	
	_cmdAlloc->Reset(); //== vector.clear()
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),	// 그릴곳
		D3D12_RESOURCE_STATE_PRESENT,						// 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET);				// 외주 결과물

	CMDLIST->SetGraphicsRootSignature(ROOTSIGNATURE.Get());
	CONSTANTBUFFER->Clear();
	TABLEDESCHEAP->Clear();

	ID3D12DescriptorHeap* descHeap = TABLEDESCHEAP->GetDescriptorHeap().Get();
	//이 함수는 굉장히 무겁기 때문에 여러번 호출되면 안됨

	//CMDLIST->SetGraphicsRootDescriptorTable(0, handle) 해당 구문이 정상적으로 동작하려면 반드시 선행되어야한다.
	//GPU에 뷰가 셋팅이 되어야지, 뷰를 통해 접근할수있기네 너무 당연하다...
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	//텍스쳐 락 잡음
	_cmdList->ResourceBarrier(1, &barrier);
	
	//reset으로 초기화 되었던 설정 셋팅
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// 랜더타겟에 그린다
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	//백버퍼 교체해서 그린다
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	//백퍼버 교체 수행
	_swapChain->Present();

	// 큐 처리가 끝날 떄까지 대기
	WaitSync();

	_swapChain->SwapIndex();
}
