#pragma once
#include <DxLib.h>
#include <map>
#include <list>
#include <string>
#include <memory>

class Effect2D;

class EffectManager
{
private:

	struct EffectInfo {
		int numX;			//���̕�����
		int numY;			//�c�̕�����
		int chipSizeX;		//���̃`�b�v�T�C�Y
		int chipSizeY;		//�c�̃`�b�v�T�C�Y
		float size;			//�`��T�C�Y
	};

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EffectManager();

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	/// <returns></returns>
	static EffectManager& GetInstance() {
		static EffectManager instance;
		return instance;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �G�t�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="pos">���W</param>
	void AddEffect(std::string name, VECTOR pos);

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EffectManager();

	EffectManager(const EffectManager&) = delete;
	void operator = (const EffectManager&) = delete;
private:
	std::map<std::string,std::string> filepath_ = {};		//�t�@�C���p�X
	std::map<std::string,EffectInfo> handleInfo_ = {};		//effect���
	std::list<std::shared_ptr<Effect2D>> effect2D_ = {};	//effect
};

