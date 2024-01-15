#pragma once
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;
class InputState;
class ShotManager;

class ObjectManager
{
public:

	ObjectManager();
	virtual ~ObjectManager();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="baseType">�ǂ�base�N���X�����Ƃɐ������邩</param>
	/// <param name="objType">�I�u�W�F�N�g�̎��</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^��</param>
	void ObjectGenerator();

	/// <summary>
	/// ���̐���
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^��</param>
	/// <param name="animNo">���������Ɏg�p����A�j���[�V�����ԍ�</param>
	void DeadPersonGenerator(int handle, LoadObjectInfo objInfo,int animNo);

	//�X�V
	void Update(Player& player, const InputState& input, std::shared_ptr<ShotManager> shotManager);

	//�`��
	void Draw(VECTOR pos);

	/// <summary>
	/// �Փ˔���Ɏg�p���郂�f�����擾����
	/// </summary>
	/// <returns>�Փ˔�����s�����f���X�}�[�g�|�C���^�̃��X�g</returns>
	std::list<std::shared_ptr<Model>> GetAllCheckCollModel();

	void SortingObject(ObjectBaseType baseType, ObjectType objType, LoadObjectInfo objInfo);

	/// <summary>
	/// ����̃��f���|�C���^���擾����
	/// </summary>
	/// <param name="type">�擾�������I�u�W�F�N�g�^�C�v</param>
	/// <returns>����̃��f���|�C���^���X�g</returns>
	std::list<std::shared_ptr<Model>> GetSpecificModel(ObjectType type);

	/// <summary>
	/// ����̃I�u�W�F�N�g�x�[�X�|�C���^���擾����
	/// </summary>
	/// <param name="type">�擾�����I�u�W�F�N�g�^�C�v</param>
	/// <returns>����̃I�u�W�F�N�g�x�[�X�|�C���^</returns>
	std::list<std::shared_ptr<ObjectBase>> GetSpecificObject(ObjectType type);

	//�Փ˔�����s�����f����ǉ�����
	void AddCheckCollModel();

	/// <summary>
	/// �����_���Ƀ|�W�V�����𐶐�����
	/// </summary>
	/// <param name="info">�I�u�W�F�N�g���</param>
	void RandomPositionGenerator(LoadObjectInfo& info,VECTOR loadObjPos);

	/// <summary>
	/// �~����̃|�W�V�������擾����
	/// </summary>
	/// <param name="info"></param>
	/// <param name="loadObjPos"></param>
	void CircumferencePosition(float angle,VECTOR& infoPos, VECTOR playerPos);

private:
	void EnemyGenerator(int deathCount,VECTOR playerPos);
	//�u�������@
	void OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//�M�~�b�N�����@
	void GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
private:

	std::list<std::shared_ptr<Model>> checkCollList_;

	std::unordered_map<ObjectType,int> modelHandle_;
	std::unordered_map<ObjectType,int> duplicateModelHandle_;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//�I�u�W�F�N�g��objectType���L�[��list�ł���
};
