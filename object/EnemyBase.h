#pragma once
#include "CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:
	//�t�@�C���p�X�Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(const char* fileName, LoadObjectInfo objInfo);
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~EnemyBase() {};

	void Update(Player& player) override;
	void StalkingUpdate();

private:


};

