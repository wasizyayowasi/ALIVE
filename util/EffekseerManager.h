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

	void Init();
	void End();

	void Update(const char* name);

	void Draw(const char* name);

private:

	int time = 0;

	std::unordered_map<std::string, int> nameAndHandleTable_;

	int  LoadEffekseerFile(const char* fileName);

	EffekseerManager();

	EffekseerManager(const EffekseerManager&) = delete;
	void operator = (const EffekseerManager&) = delete;



};

