#pragma once
class Vertex
{
public:
	Vertex():_p(0.f,0.f,0.f),_c(0.f,0.f,0.f,0.f) {};
	~Vertex() {};
public:
	Vec3 _p;
	//Vec3 _n;
	Vec4 _c;
	//Vec2 _t;
};

