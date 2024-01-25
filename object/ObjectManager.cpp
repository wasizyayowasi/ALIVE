#include "ObjectManager.h"

#include "../object/Player.h"
#include "../object/EnemyBase.h"
#include "../object/ObjectBase.h"
#include "../object/DeadPerson.h"
#include "../object/OrnamentBase.h"

#include "../gimmick/Elevator.h"
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

	auto& loadData = ExternalFile::GetInstance();

	for (auto& objInfo : loadData.GetLoadMainStageObjectInfo()) {
		if (objInfo.first == "BigBuildingA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BigBuildingA, objSecond);
			}
		}
		else if (objInfo.first == "SignBoard") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::SignBoardType1, objSecond);
			}
		}
		else if (objInfo.first == "BuildingC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BuildingCType1, objSecond);
			}
		}
		else if (objInfo.first == "Building2A") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BuildingAType2, objSecond);
			}
		}
		else if (objInfo.first == "StoreC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::StoreC, objSecond);
			}
		}
		else if (objInfo.first == "Train") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Train, objSecond);
			}
		}
		else if (objInfo.first == "Street") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Street, objSecond);
			}
		}
		else if (objInfo.first == "TStreet") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::TStreet, objSecond);
			}
		}
		else if (objInfo.first == "Tile") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Tile, objSecond);
			}
		}
		else if (objInfo.first == "Scaffold") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Scaffold, objSecond);
			}
		}
		else if (objInfo.first == "BlueContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BlueContainer, objSecond);
			}
		}
		else if (objInfo.first == "RedContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::RedContainer, objSecond);
			}
		}
		else if (objInfo.first == "YellowContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::YellowContainer, objSecond);
			}
		}
		else if (objInfo.first == "OrangeContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::OrangeContainer, objSecond);
			}
		}
	}

	for (auto& gimmick : loadData.GetGimmickInfo()) {
		if (gimmick.first == "Trans") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::Trans, objSecond);
			}
		}
		else if (gimmick.first == "CrankScaffold") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::CrankScaffold, objSecond);
			}
		}
		else if (gimmick.first == "Elevator") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::Elevator, objSecond);
			}
		}
		else if (gimmick.first == "PenetrationScaffld") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::PenetrationScaffld, objSecond);
			}
		}
	}
}

void ObjectManager::OpeningStageObjectGenerator()
{
	//�Z�k��
	auto& loadData = ExternalFile::GetInstance();

	for (auto& objInfo : loadData.GetLoadOpeningStageObjectInfo()) {
		if (objInfo.first == "Bed") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Bed, objSecond);
			}
		}
		else if (objInfo.first == "RoomWall") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::RoomWall, objSecond);
			}
		}
		else if (objInfo.first == "WoodFloor") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::WoodFloor, objSecond);
			}
		}
		else if (objInfo.first == "Window") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Window, objSecond);
			}
		}
		else if (objInfo.first == "Clock") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Clock, objSecond);
			}
		}
		else if (objInfo.first == "Chair") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Chair, objSecond);
			}
		}
		else if (objInfo.first == "Desk") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Desk, objSecond);
			}
		}
		else if (objInfo.first == "WoodenBox") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::WoodenBox, objSecond);
			}
		}
		else if (objInfo.first == "SignBoardType2") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::SignBoardType2, objSecond);
			}
		}
	}
}

void ObjectManager::DeadPersonGenerator(int handle, LoadObjectInfo objInfo, int animNo)
{
	//���̂����������
	objects_[ObjectType::DeadPerson].push_back(std::make_shared<DeadPerson>(handle, objInfo, animNo));

	//���̂�4�����������烊�^�[��
	if(objects_[ObjectType::DeadPerson].size() < 4) return;

	//���̂̃��X�g�̈�Ԑ擪(�Â�)���̂��폜����
	objects_[ObjectType::DeadPerson].remove(objects_[ObjectType::DeadPerson].front());

}

//�X�V
void ObjectManager::Update(Player& player,std::shared_ptr<ShotManager> shotManager)
{
	//objects_�̊e�v�f��isEnable���擾���A�����ɂȂ��Ă���ΊY���R���e�i�̍폜
	for (auto& list : objects_) {
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->IsEnabled(); });
	}

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//���̂Ƃ��̑��̃I�u�W�F�N�g�̏Փ˔�����s��
	for (auto& list : objects_) {
		for (auto& obj : list.second) {
			if (list.first == ObjectType::DeadPerson) {
				continue;
			}
			for (auto& deadperson : objects_[ObjectType::DeadPerson]) {
				distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
				if (distanceSize < 1000.0f) {
					obj->HitColl(deadperson);
				}
			}
		}
	}

	//enemy��Shot
	for (auto obj : objects_[ObjectType::Enemy]) {
		if (std::dynamic_pointer_cast<EnemyBase>(obj) != nullptr) {
			std::dynamic_pointer_cast<EnemyBase>(obj)->Shot(shotManager, player.GetStatus().pos, player.GetStatus().height);
		}
	}

	
	//�X�V
	for (auto list : objects_) {
		for (auto obj : list.second) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
			if (distanceSize < 1000) {
				obj->Update(player);
			}
		}
	}
	
	auto deleteBorderLineInfo = ExternalFile::GetInstance().GetDeleteObjInfo(playerPos, "DeleteBorderLine");

	if (playerPos.x > deleteBorderLineInfo.pos.x) {
		auto deletePointInfo = ExternalFile::GetInstance().GetDeleteObjInfo(deleteBorderLineInfo.pos, "DeletePoint");
		for (auto list : objects_) {
			for (auto obj : list.second) {
				if (obj->GetPos().x < deletePointInfo.pos.x) {
					obj->SetIsEnable(false);
				}
			}
		}
	}

	EnemyGenerator(player.GetDeathCount(),player.GetStatus().pos);
}

//�`��
void ObjectManager::Draw(VECTOR PlayerPos)
{
	float distance = 0.0f;

	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			//�I�u�W�F�N�g����v���C���[�܂ł̋������Ƃ�T�C�Y�ϊ�����
			distance = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), PlayerPos);
			//�v���C���[���狗����5000.0f������������`�悷��
			if (distance < 5000.0f) {
				obj->Draw();
			}
		}
	}

	int size = static_cast<int>(objects_[ObjectType::Enemy].size());
}

std::list<std::shared_ptr<Model>> ObjectManager::GetAllCheckCollModel()
{

	for (auto& obj : objects_) {
		for (auto& model : obj.second) {
			if (model->IsCollCheck()) {
				checkCollList_.push_back(model->GetModelPointer());
			}
		}
	}

	return checkCollList_;
}

void ObjectManager::SortingObject(ObjectBaseType baseType, ObjectType objType, LoadObjectInfo objInfo)
{
	//objectBaseType�����ɃC���X�^���X������N���X�����߂�
	switch (baseType) {
		//�u���𐶐�
	case ObjectBaseType::OrnamentBase:
		OrnamentGenerator(objType, objInfo);
		break;
		//�M�~�b�N�𐶐�
	case ObjectBaseType::GimmickBase:
		GimmickObjectGenerator(objType, objInfo);
		break;
	}
}

std::list<std::shared_ptr<Model>> ObjectManager::GetSpecificModel(ObjectType type)
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

std::list<std::shared_ptr<ObjectBase>> ObjectManager::GetSpecificObject(ObjectType type)
{

	std::list<std::shared_ptr<ObjectBase>> obj;

	if (objects_.count(type) > 0) {
		obj = objects_[type];
	}

	return obj;
}

void ObjectManager::AddCheckCollModel()
{

	checkCollList_.clear();

	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			if (objSecond->AddCollModel() != nullptr) {
				checkCollList_.push_back(objSecond->AddCollModel());
			}
		}
	}
}

//���͈͓��Ń����_���ɏ����|�W�V�������擾����
void ObjectManager::RandomPositionGenerator(LoadObjectInfo& info, VECTOR loadObjPos)
{

	float distance = 500.0f;

	std::random_device random;
	std::mt19937 value(random());

	std::uniform_int_distribution<> randomPosX(loadObjPos.x - distance, loadObjPos.x);
	std::uniform_int_distribution<> randomPosZ(loadObjPos.z - distance, loadObjPos.z + distance);

	info.pos.x = static_cast<float>(randomPosX(value));
	info.pos.z = static_cast<float>(randomPosZ(value));

}

//�G�l�~�[������������|�W�V�������p�x�ɂ���Ď擾����
void ObjectManager::CircumferencePosition(float angle, VECTOR& infoPos, VECTOR playerPos)
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

//�G�����@
void ObjectManager::EnemyGenerator(int deathCount,VECTOR playerPos)
{
	//�O���t�@�C������ǂݍ��񂾁u�G�l�~�[�I�u�W�F�N�g�v�̏����擾����
	auto loadInfo = ExternalFile::GetInstance().GetEnemyInfo(playerPos);

	//�Z�k��
	auto& model = ModelManager::GetInstance();
	
	//������̃T�C�Y���擾����
	int size = static_cast<int>(loadInfo.name.size());

	for (auto& enemy : objects_[ObjectType::Enemy]) {
		if (enemy.get()->GetName() == loadInfo.name) {
			return;
		}
	}

	//�u.�v�ȍ~�̕�����ɂ����
	//�G�l�~�[�̏����p�^�[����ύX����
	if (size > 0) {
		std::string str = StrUtil::GetStringAfterSign(loadInfo.name, ".");

		//�����񂪁uALL�v��������
		if (str == "ALL") {
			float angle = 0.0f;
			for (int i = 0; i < deathCount; i++) {
				//���͈͂̒��Ń����_���ɃX�|�[��������
				//RandomPositionGenerator(info, loadInfo.pos);
				
				//�v���C���[�𒆐S�ɉ~����ŃX�|�[��������
				//CircumferencePosition(angle, loadInfo.pos, playerPos);
				CircumferencePosition(angle, loadInfo.pos, loadInfo.pos);

				//�C���X�^���X��
				objects_[ObjectType::Enemy].push_back(std::make_shared<EnemyBase>(model.GetModelHandle(ObjectType::Enemy), loadInfo));
				angle -= 15.0f;
			}
		}
		else {
			//�Ō�̕������int�^�̐��l�ɕϊ�����
			int num = atoi(str.c_str());

			//������̍Ō�̐�����deathCount���������
			//�G�l�~�[����������
			if (num <= deathCount) {
				objects_[ObjectType::Enemy].push_back(std::make_shared<EnemyBase>(model.GetModelHandle(ObjectType::Enemy), loadInfo));
			}
		}
	}
}

//�u�������@
void ObjectManager::OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(objType), objInfo));
}

void ObjectManager::GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	switch (objType) {
	case ObjectType::Trans:
		objects_[objType].push_front(std::make_shared<TransparentObject>(model.GetModelHandle(objType), objInfo));
		break;
	case ObjectType::Elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(model.GetModelHandle(objType), objInfo));
		break;
	case ObjectType::CrankScaffold:
		objects_[objType].push_front(std::make_shared<CrankScaffold>(model.GetModelHandle(objType), objInfo));
		break;
	case ObjectType::PenetrationScaffld:
		objects_[objType].push_front(std::make_shared<PenetrationScaffld>(model.GetModelHandle(ObjectType::BlueContainer), objInfo));
		break;
	}
}
