#pragma once
#include "ObjectData.h"
#include "DxLib.h"
#include <memory>
#include <list>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;

class ObjectManager
{
public:

	virtual ~ObjectManager();

	static ObjectManager& GetInstance() {
		static ObjectManager instance;
		return instance;
	}

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="baseType">�ǂ�base�N���X�����Ƃɐ������邩</param>
	/// <param name="objType">�I�u�W�F�N�g�̎��</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^��</param>
	void ObjectGenerator(ObjectBaseType baseType,ObjectType objType,LoadObjectInfo objInfo);

	/// <summary>
	/// ���̐���
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^��</param>
	/// <param name="animNo">���������Ɏg�p����A�j���[�V�����ԍ�</param>
	void DeadPersonGenerator(int handle, LoadObjectInfo objInfo,int animNo);

	//�X�V
	void Update(Player& player);

	//�`��
	void Draw();

	/// <summary>
	/// �Փ˔���Ɏg�p���郂�f�����擾����
	/// </summary>
	/// <returns>�Փ˔�����s�����f���X�}�[�g�|�C���^�̃��X�g</returns>
	std::list<std::shared_ptr<Model>> GetAllCheckCollModel();

	/// <summary>
	/// ����̃��f���|�C���^���擾����
	/// </summary>
	/// <param name="type">�擾�������I�u�W�F�N�g�^�C�v</param>
	/// <returns>����̃��f���|�C���^���X�g</returns>
	std::list<std::shared_ptr<Model>> GetSpecificModel(ObjectType type);

private:

	ObjectManager();

	ObjectManager(const ObjectManager&) = delete;
	void operator = (const ObjectManager) = delete;

private:
	//�L�����N�^�[�����@
	void CharacterGenerator(ObjectType objType, LoadObjectInfo objInfo);

	void EnemyGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//�u�������@
	void OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//�^�ׂ�u�������@
	void CarryObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//�M�~�b�N�����@
	void GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
private:

	int playerHandle_ = -1;
	int fieldHandle_ = -1;				//���t�B�[���h�̃��f���n���h��
	int boxHandle_ = -1;				//���̔��I�u�W�F�N�g�p�n���h��
	int switchHandle_ = -1;
	int steelyardHandle_ = -1;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//�I�u�W�F�N�g��objectType���L�[��list�ł���
};

