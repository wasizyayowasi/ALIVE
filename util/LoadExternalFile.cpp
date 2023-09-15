#include "LoadExternalFile.h"
#include "DxLib.h"

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

LoadExternalFile::LoadExternalFile()
{
	loadInfo("player");
}

LoadExternalFile::~LoadExternalFile()
{
	rewriteInfo();
}

void LoadExternalFile::draw()
{
}

void LoadExternalFile::loadInfo(const char* filename)
{
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	ifstream ifs(path.c_str());
	assert(ifs.good());

	json json_;
	ifs >> json_;

	player.jumpPower = json_["jumpPower"];
	player.runningJumpPower = json_["runningJumpPower"];
	player.rotSpeed = json_["rotSpeed"];
	player.walkSpeed = json_["walkSpeed"];
	player.runningSpeed = json_["runningSpeed"];

	int i = 0;
	for (auto& animNo : json_["animNo"]) {
		player.animNo_[i] = animNo;
		i++;
	}

}

void LoadExternalFile::rewriteInfo()
{
	json json_ = {
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",3.0f},
	   {"runningSpeed",7.0f},
	   {"animNo", json::array({ 0,1,2,3,4,5,6,7,8,9,10 })},
	};

	string filename = json_["name"];
	string extension = ".json";
	filename += extension;

	ofstream writeing_file;
	writeing_file.open(filename, ios::out);
	writeing_file << json_.dump() << std::endl;
	writeing_file.close();

}
