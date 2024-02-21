#include "ObjectManager.h"

#include "Player.h"
#include "EnemyBase.h"
#include "ThrowEnemy.h"
#include "ObjectBase.h"
#include "Corpse.h"
#include "OrnamentBase.h"
#include "SignBoardEnemy.h"

#include "../gimmick/Elevator.h"
#include "../gimmick/HopStepJump.h"
#include "../gimmick/CrankScaffold.h"
#include "../gimmick/TransparentObject.h"
#include "../gimmick/PenetrationScaffld.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

#include <random>

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::MainStageObjectGenerator()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�u���̐���
	for (auto& objTable : file.GetLoadMainStageObjectInfo())
	{
		for (auto& objInfo : objTable.second)
		{
			for (auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}

	//�M�~�b�N�̐���
	for (auto& objTable : file.GetGimmickInfo())
	{
		for (auto& objInfo : objTable.second)
		{
			for (auto& data : objData_)
			{
				if (objTable.first == data.name)
				{
					GimmickObjectGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

void ObjectManager::OpeningStageObjectGenerator()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�u���̐���
	for (auto& objTable : file.GetLoadOpeningStageObjectInfo())
	{
		for (auto& objInfo : objTable.second)
		{
			for (auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

void ObjectManager::EndStageObjectGenerator()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�u���̐���
	for (auto& objTable : file.GetLoadMainStageObjectInfo()) {
		for (auto& objInfo : objTable.second)
		{
			for (auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

void ObjectManager::CorpseGenerator(const int handle, const  LoadObjectInfo objInfo, const  int animNo)
{
	//���̂����������
	objects_[ObjectType::Corpse].push_back(std::make_shared<Corpse>(handle, Material::Other, objInfo, animNo));

	//���̂�4�����������烊�^�[��
	if (objects_[ObjectType::Corpse].size() < 4) return;

	//���̂̃��X�g�̈�Ԑ擪(�Â�)���̂��폜����
	objects_[ObjectType::Corpse].remove(objects_[ObjectType::Corpse].front());
}

//�X�V
void ObjectManager::Update(Player& player, std::shared_ptr<ShotManager> shotManager)
{
	//objects_�̊e�v�f��isEnable���擾���A�����ɂȂ��Ă���ΊY���R���e�i�̍폜
	for (auto& list : objects_) {
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->GetIsEnabled(); });
	}

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//���̂Ƃ��̑��̃I�u�W�F�N�g�̏Փ˔�����s��
	for (auto& list : objects_) {
		for (auto& obj : list.second) {
			if (list.first == ObjectType::Corpse) {
				continue;
			}
			for (auto& deadperson : objects_[ObjectType::Corpse]) {
				distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
				if (distanceSize < 4000.0f) {
					obj->UpdateForCorpse(deadperson);
				}
			}
		}
	}

	//enemy��Shot
	for (auto& obj : objects_[ObjectType::Enemy]) {
		if (std::dynamic_pointer_cast<ThrowEnemy>(obj) != nullptr) {
			std::dynamic_pointer_cast<ThrowEnemy>(obj)->Shot(shotManager, player.GetStatus().pos, player.GetStatus().height);
		}
	}

	//�X�V
	for (auto& list : objects_) {
		for (auto& obj : list.second) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
			if (distanceSize < 4000) {
				obj->Update(player);
			}
		}
	}

	for (auto& enemy : ExternalFile::GetInstance().GetEnemyInfo(playerPos)) {
		if (!usedEnemyList_[enemy.name]) {
			EnemyGenerator(player.GetDeathCount(), enemy);
		}
	}

	for (auto& table : ExternalFile::GetInstance().GetGimmickInfo()) {
		if (table.first != "CorpseMountain") {
			continue;
		}
		for (auto& corpseMt : table.second) {
			if (!usedCorpseMtList_[corpseMt.name]) {
				GenerateCorpseMountain(player.GetDeathCount(), corpseMt);
			}
		}
	}
}

//�`��
void ObjectManager::Draw(const VECTOR PlayerPos)
{
	float distance = 0.0f;

	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			//�I�u�W�F�N�g����v���C���[�܂ł̋������Ƃ�T�C�Y�ϊ�����
			distance = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), PlayerPos);
			//�v���C���[���狗����8000.0f������������`�悷��
			if (distance < 8000.0f) {
				obj->Draw();
			}
		}
	}

	int size = static_cast<int>(objects_[ObjectType::Enemy].size());
}

const std::list<std::shared_ptr<Model>>& ObjectManager::GetAllCheckCollModel()
{

	for (auto& obj : objects_) {
		for (auto& model : obj.second) {
			if (model->GetIsCollCheck()) {
				checkCollList_.push_back(model->GetModelPointer());
			}
		}
	}

	return checkCollList_;
}

const std::list<std::shared_ptr<Model>>& ObjectManager::GetSpecificModel(const ObjectType type)
{

	std::list<std::shared_ptr<Model>> specificList;

	//�����Ŏw�肳�ꂽ�I�u�W�F�N�g�̃��f���|�C���^����L�Ő錾����
	//�z��ɑ������
	for (auto& obj : objects_) {
		//�����ƈႤ�^�C�v�������ꍇcontinue
		if (obj.first != type) {
			continue;
		}
		//�w�肳�ꂽ�^�C�v�������ꍇ
		//���̃I�u�W�F�N�g�̃��X�g���烂�f���|�C���^���擾��
		//specificList�ɑ������
		for (auto& objSecond : obj.second) {
			specificList.push_back(objSecond->GetModelPointer());
		}
	}

	//���X�g��Ԃ�
	return specificList;
}

const std::list<std::shared_ptr<ObjectBase>>& ObjectManager::GetSpecificObject(const ObjectType type)
{

	std::list<std::shared_ptr<ObjectBase>> obj = {};

	if (objects_.count(type) > 0) {
		obj = objects_[type];
	}

	return obj;
}

void ObjectManager::AddCheckCollModel()
{

	checkCollList_.clear();

	for (auto& obj : objects_) {
		for (auto& objSecond : obj.second) {
			if (objSecond->AddCollModel() != nullptr) {
				checkCollList_.push_back(objSecond->AddCollModel());
			}
		}
	}
}

void ObjectManager::RandomPositionGenerator(LoadObjectInfo& info, const VECTOR loadObjPos)
{
	float distance = 500.0f;

	std::random_device random;
	std::mt19937 value(random());

	std::uniform_int_distribution<> randomPosX(loadObjPos.x - distance, loadObjPos.x);
	std::uniform_int_distribution<> randomPosZ(loadObjPos.z, loadObjPos.z + distance);

	info.pos.x = static_cast<float>(randomPosX(value));
	info.pos.z = static_cast<float>(randomPosZ(value));
}

void ObjectManager::CircumferencePosition(const float angle, VECTOR& infoPos, const VECTOR playerPos)
{
	VECTOR pos = {};
	float radian = MathUtil::DegreeToRadian(angle);

	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	pos = VScale(pos,200.0f);

	pos.y = infoPos.y;

	pos = VAdd(playerPos, pos);

	pos.y = infoPos.y;

	infoPos = pos;
}

void ObjectManager::EnemyGenerator(const int deathCount, const  LoadObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//������̃T�C�Y���擾����
	int size = static_cast<int>(info.name.size());

	//�u.�v�ȍ~�̕�����ɂ����
	//�G�l�~�[�̏����p�^�[����ύX����
	if (size > 0) {
		std::string numStr = StrUtil::GetStringAfterSign(info.name, ".");
		numStr = StrUtil::GetStringBeforeSign(numStr, "-");

		std::string str = StrUtil::GetStringBeforeSign(info.name, ".");

		//�����񂪁uALL�v��������
		if (numStr == "ALL") {
			GeneratedForTheNumberOfTimesYouDie(deathCount, info);
		}
		else if (str == "SignBoardEnemy") {
			objects_[ObjectType::Enemy].push_back(std::make_shared<SignBoardEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::SignBoardPlayer)].name), Material::Other, info));
			usedEnemyList_[info.name] = true;
		}
		else {
			GeneratePredeterminedNumberOfTimes(deathCount, numStr, info);
		}
	}

	
}

void ObjectManager::EndEnemyGenerator(const int deathCount, LoadObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	float angle = 0.0f;
	for (int i = 0; i < deathCount; i++) {
		info.pos.x += angle;

		//�C���X�^���X��
		objects_[ObjectType::Enemy].push_back(std::make_shared<EnemyBase>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		angle -= 15.0f;

		usedEnemyList_[info.name] = true;
	}
}

void ObjectManager::GeneratedForTheNumberOfTimesYouDie(const int deathCount, LoadObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	float angle = 0.0f;
	for (int i = 0; i < deathCount; i++) {
		//���͈͂̒��Ń����_���ɃX�|�[��������
		RandomPositionGenerator(info, info.pos);

		//�v���C���[�𒆐S�ɉ~����ŃX�|�[��������
		//CircumferencePosition(angle, info.pos, info.pos);

		//�C���X�^���X��
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		angle -= 15.0f;

		usedEnemyList_[info.name] = true;
	}
}

void ObjectManager::GeneratePredeterminedNumberOfTimes(const int deathCount, const  std::string str, const  LoadObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//�Ō�̕������int�^�̐��l�ɕϊ�����
	int num = atoi(str.c_str());

	//������̍Ō�̐�����deathCount���������
	//�G�l�~�[����������
	if (num <= deathCount) {
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		usedEnemyList_[info.name] = true;
	}
}

void ObjectManager::GenerateCorpseMountain(const int deathCount, const  LoadObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	std::string str = StrUtil::GetStringAfterSign(info.name, ".");
	str = StrUtil::GetStringBeforeSign(str, "-");

	//�Ō�̕������int�^�̐��l�ɕϊ�����
	int num = atoi(str.c_str());

	//������̍Ō�̐�����deathCount���������
	//�G�l�~�[����������
	if (num <= deathCount) {
		objects_[ObjectType::CorpseMountain].push_back(std::make_shared<OrnamentBase>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::CorpseMountain)].name), Material::Other, info));
		usedCorpseMtList_[info.name] = true;
	}
}

//�u�������@
void ObjectManager::OrnamentGenerator(const std::string name, const ObjectType objType, const Material materialType, const  LoadObjectInfo objInfo)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(name), materialType, objInfo));
}

void ObjectManager::GimmickObjectGenerator(const std::string name, const ObjectType objType, const  Material materialType, const  LoadObjectInfo objInfo)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	switch (objType) {
	case ObjectType::TransScaffold:
		objects_[objType].push_front(std::make_shared<TransparentObject>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::Elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::CrankScaffold:
		objects_[objType].push_front(std::make_shared<CrankScaffold>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::PenetrationScaffld:
		objects_[objType].push_front(std::make_shared<PenetrationScaffld>(model.GetModelHandle("Train"), materialType, objInfo));
		break;
	case ObjectType::HopStepJump:
		objects_[objType].push_front(std::make_shared<HopStepJump>(model.GetModelHandle(name), materialType, objInfo));
		break;
	}
}
