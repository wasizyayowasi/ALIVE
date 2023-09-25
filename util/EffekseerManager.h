#pragma once
#include <string>
#include <unordered_map>

class EffekseerManager
{
public:

	~EffekseerManager();

	static EffekseerManager& getInstance() {
		static EffekseerManager instance;
		return instance;
	}

	void update(const char* name);

	void draw(const char* name);

private:

	int time = 0;

	std::unordered_map<std::string, int> nameAndHandleTable_;

	int  loadEffekseerFile(const char* fileName);

	EffekseerManager();

	EffekseerManager(const EffekseerManager&) = delete;
	void operator = (const EffekseerManager&) = delete;



};

