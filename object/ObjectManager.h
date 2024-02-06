#pragma once
#include "ObjectData.h"
#include <list>
#include <string>
#include <memory>
#include <DxLib.h>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;
class ShotManager;

class ObjectManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ObjectManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ObjectManager();

	/// <summary>
	/// ���C���X�e�[�W�̃I�u�W�F�N�g����
	/// </summary>
	void MainStageObjectGenerator();

	/// <summary>
	/// �I�[�v�j���O�V�[���̃I�u�W�F�N�g�𐶐�����
	/// </summary>
	void OpeningStageObjectGenerator();

	/// <summary>
	/// �G���f�B���O�V�[���̃I�u�W�F�N�g�𐶐�����
	/// </summary>
	void EndStageObjectGenerator();

	/// <summary>
	/// ���̐���
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^��</param>
	/// <param name="animNo">���������Ɏg�p����A�j���[�V�����ԍ�</param>
	void DeadPersonGenerator(int handle, LoadObjectInfo objInfo,int animNo);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	/// <param name="shotManager">�V���b�g�}�l�[�W���[�̃X�}�[�g�|�C���^</param>
	void Update(Player& player, std::shared_ptr<ShotManager> shotManager);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="pos">�v���C���[�|�W�V����</param>
	void Draw(VECTOR PlayerPos);

	/// <summary>
	/// �Փ˔���Ɏg�p���郂�f�����擾����
	/// </summary>
	/// <returns>�Փ˔�����s�����f���X�}�[�g�|�C���^�̃��X�g</returns>
	std::list<std::shared_ptr<Model>> GetAllCheckCollModel();

	/// <summary>
	/// baseType�ʂɃI�u�W�F�N�g�̐�����U�蕪����
	/// </summary>
	/// <param name="baseType">�I�u�W�F�N�g�̃x�[�X�^�C�v</param>
	/// <param name="objType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="materialType">�I�u�W�F�N�g�̃}�e���A���^�C�v</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^</param>
	void SortingObject(ObjectBaseType baseType, ObjectType objType,Material materialType, LoadObjectInfo objInfo);

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

	/// <summary>
	/// �Փ˔�����s�����f����ǉ�����
	/// </summary>
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

	/// <summary>
	/// �G����
	/// </summary>
	/// <param name="deathCount">���܂łɃv���C���[�����S������</param>
	/// <param name="info">�z�u�f�[�^</param>
	void EnemyGenerator(int deathCount, LoadObjectInfo info);
private:
	/// <summary>
	/// ���񂾉񐔕���������
	/// </summary>
	/// <param name="deathCount">���܂łɃv���C���[�����S������</param>
	/// <param name="info">�z�u�f�[�^</param>
	void GeneratedForTheNumberOfTimesYouDie(int deathCount, LoadObjectInfo info);

	/// <summary>
	/// ����̉񐔕���������
	/// </summary>
	/// <param name="deathCount">���܂łɃv���C���[�����S������</param>
	/// <param name="str">������</param>
	/// <param name="info">�z�u�f�[�^</param>
	void GeneratePredeterminedNumberOfTimes(int deathCount,std::string str, LoadObjectInfo info);

	/// <summary>
	/// �u�������@
	/// </summary>
	/// <param name="objType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="materialType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	void OrnamentGenerator(ObjectType objType, Material materialType, LoadObjectInfo objInfo);

	/// <summary>
	/// �M�~�b�N�����@
	/// </summary>
	/// <param name="objType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="materialType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	void GimmickObjectGenerator(ObjectType objType, Material materialType, LoadObjectInfo objInfo);
private:

	std::list<std::shared_ptr<Model>> checkCollList_;
	std::unordered_map<std::string, bool>  usedEnemyList_;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//�I�u�W�F�N�g��objectType���L�[��list�ł���
};

