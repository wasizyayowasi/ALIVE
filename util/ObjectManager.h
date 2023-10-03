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
	//�L�����N�^�[�����@
	void characterGenerator(ObjectType objType, const char* const filename);
	//�G�����@
	void enemyGenerator(ObjectType objType, const char* const filename);
	//�u�������@
	void ornamentGenerator(ObjectType objType, const char* const filename);
	//�^�ׂ�u�������@
	void carryObjectGenerator(ObjectType objType, const char* const filename);
	//�M�~�b�N�����@
	void gimmickObjectGenerator(ObjectType objType, const char* const filename);
private:

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;

};

