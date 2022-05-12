#include "pch.h"
#include "DX12FrameWork.h"
#include "DX12Engine.h"

std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
std::shared_ptr<Shader> shader = std::make_shared<Shader>();

void DX12FrameWork::Init(const WindowInfo& info)
{
	DX12Engine::This().Init(info);

	std::vector<Vertex> vec(3);
	vec[0]._p = Vec3(0.f, 0.5f, 0.5f);
	vec[0]._c = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1]._p = Vec3(0.5f, -0.5f, 0.5f);
	vec[1]._c = Vec4(0.f, 1.0f, 0.f, 1.f);
	vec[2]._p = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2]._c = Vec4(0.f, 0.f, 1.f, 1.f);
	mesh->Init(vec);

	if (S_OK != shader->Init(L"..\\Resource\\Shader\\DrawTriangle.hlsli"))
	{
		//아 여기까지 오니 전부 예외처리 필요하네
		return;
	}

	DX12Engine::This().GetCmdQueue()->WaitSync();
}

void DX12FrameWork::Update()
{
	DX12Engine::This().RenderBegin();
	
	shader->Update();
	{
		Transform t;
		t._offset = Vec4(0.75f, 0.f, 0.f,0.f);
		mesh->SetTransform(t);
		mesh->Render();
	}

	{
		Transform t;
		t._offset = Vec4(0.f, 0.75f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->Render();
	}

	DX12Engine::This().RenderEnd();

}
