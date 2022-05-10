#pragma once
template<typename T>
class Singleton
{
protected:
	Singleton() {}
public:
	Singleton(const Singleton& s) = delete;
	void operator=(const Singleton& s) = delete;
	static T& This()
	{
		static T instance;
		return instance;
	}
};