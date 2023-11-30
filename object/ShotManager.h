#pragma once
#include <DxLib.h>
#include <memory>
#include <list>

class Model;
class Shot;
class Player;

class ShotManager
{
public:
	ShotManager();
	virtual ~ShotManager();

	//�X�V
	void Update();
	//�`��
	void Draw();

	void Hit(Player& player);

	//�V���b�g�����
	void Fire(VECTOR framePos,VECTOR playerPos, float height);

private:

	int shotHandle_ = 0;

	std::list<std::shared_ptr<Shot>> shots_;		//���f���|�C���^�̃��X�g

};

