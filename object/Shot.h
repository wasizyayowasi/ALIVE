#pragma once
#include <DxLib.h>
#include <memory>

class Model;

class Shot
{
public:

	Shot(int handle, VECTOR initPos, VECTOR moveVec);
	virtual ~Shot();

	//�X�V
	void Update();
	//�`��
	void Draw();

	//���݂��Ă��邩�ǂ����擾����
	bool IsEnabled() { return isEnable_; }

private:

	bool isEnable_ = false;						//���݂��Ă��邩

	VECTOR initialisePos_ = {};					//���������̃|�W�V����
	VECTOR pos_ = {};							//�|�W�V����
	VECTOR moveVec_ = {};						//�ړ��x�N�g��

	std::shared_ptr<Model> model_;				//���f���|�C���^�̃��X�g

};

