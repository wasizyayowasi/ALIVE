#pragma once
#include "ObjectType.h"
#include <memory>
#include <list>
#include <unordered_map>

class ObjectBase;

class ObjectManager
{
public:

	ObjectManager();
	virtual ~ObjectManager();

	void objectGenerator(ObjectBaseType baseType,ObjectType objType,const char* const filename);

	void update();
	void draw();

private:
	//キャラクター生成機
	void characterGenerator(ObjectType objType, const char* const filename);
	//敵生成機
	void enemyGenerator(ObjectType objType, const char* const filename);
	//置物生成機
	void ornamentGenerator(ObjectType objType, const char* const filename);
	//運べる置物生成機
	void carryObjectGenerator(ObjectType objType, const char* const filename);
	//ギミック生成機
	void gimmickObjectGenerator(ObjectType objType, const char* const filename);
private:

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;

};

