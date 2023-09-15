#pragma once
#include <vector>

struct ReadPlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int animNo_[11];
};

class LoadExternalFile
{
public:

	~LoadExternalFile();

	static LoadExternalFile& getInstance() {
		static LoadExternalFile instance;
		return instance;
	}

	void draw();

	ReadPlayerInfo getPlayerInfo() { return player; }

private:

	void loadInfo(const char* filename);
	void rewriteInfo();

	LoadExternalFile();

	ReadPlayerInfo player;

};

