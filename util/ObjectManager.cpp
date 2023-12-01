#include "ObjectManager.h"

#include "../object/ObjectBase.h"
#include "../object/EnemyBase.h"
#include "../object/DeadPerson.h"
#include "../object/OrnamentBase.h"
#include "../object/Player.h"

#include "../gimmick/TransparentObject.h"
#include "../gimmick/Steelyard.h"
#include "../gimmick/CrankScaffold.h"
#include "../gimmick/Elevator.h"

#include "game.h"
#include "Util.h"

#include "InputState.h"
#include "ExternalFile.h"

#include <random>

namespace {
	const char* const player_Filename = "data/player/player16.mv1";
	//�����f���̃t�@�C���p�X
	const char* const switch_filename = "data/model/switch.mv1";
	const char* const steelyard_filename = "data/model/steelyard.mv1";
	const char* const transparent_obj_filename = "data/model/trans.mv1";
	const char* const elevator_obj_filename = "data/model/elevator.mv1";
	const char* const manualCrank_obj_filename = "data/model/manualCrank.mv1";

	//���ۂɎg���\��̃��f���p�X
	//�ł����r��
	const char* const big_buildingA_filepath = "data/model/city/model/BigBuildingA.mv1";
	const char* const big_buildingB_filepath = "data/model/city/model/BigBuildingB.mv1";
	const char* const big_buildingC_filepath = "data/model/city/model/BigBuildingC.mv1";
	const char* const big_buildingD_filepath = "data/model/city/model/BigBuildingD.mv1";
	//A�݂����Ȍ`�̃r��
	const char* const buildingA_filepath = "data/model/city/model/BuildingA.mv1";
	const char* const buildingB_filepath = "data/model/city/model/BuildingB.mv1";
	const char* const buildingC_filepath = "data/model/city/model/BuildingC.mv1";
	//�����`�̃r��
	const char* const buildingA_type2_filepath = "data/model/city/model/Building2A.mv1";
	const char* const buildingB_type2_filepath = "data/model/city/model/Building2B.mv1";
	const char* const buildingC_type2_filepath = "data/model/city/model/Building2C.mv1";
	//�C�O�Ō���悤�Ȉ��H�X
	const char* const storeA_filepath = "data/model/city/model/StoreA.mv1";
	const char* const storeB_filepath = "data/model/city/model/StoreB.mv1";
	const char* const storeC_filepath = "data/model/city/model/StoreC.mv1";
	//��
	const char* const street_filepath = "data/model/city/model/Street.mv1";
	const char* const T_street_filepath = "data/model/city/model/TStreet.mv1";
	const char* const Tile_filepath = "data/model/city/model/Tile.mv1";
	const char* const scaffold_filepath = "data/model/city/others/Scaffold.mv1";
	//����
	const char* const BlueContainer_filepath = "data/model/city/container/mv1/BlueContainer.mv1";
	const char* const RedContainer_filepath = "data/model/city/container/mv1/RedContainer.mv1";
	const char* const YellowContainer_filepath = "data/model/city/container/mv1/YellowContainer.mv1";
	const char* const OrangeContainer_filepath = "data/model/city/container/mv1/OrangeContainer.mv1";
	const char* const PurpleContainer_filepath = "data/model/city/container/mv1/PurpleContainer.mv1";
	const char* const GreenContainer_filepath = "data/model/city/container/mv1/GreenContainer.mv1";
}

ObjectManager::ObjectManager()
{

	modelHandle_[ObjectType::enemy] = MV1LoadModel(player_Filename);
	modelHandle_[ObjectType::gimmickSwitch] = MV1LoadModel(switch_filename);
	modelHandle_[ObjectType::gimmickSteelyard] = MV1LoadModel(steelyard_filename);
	modelHandle_[ObjectType::trans] = MV1LoadModel(transparent_obj_filename);
	modelHandle_[ObjectType::elevator] = MV1LoadModel(elevator_obj_filename);
	modelHandle_[ObjectType::CrankScaffold] = MV1LoadModel(manualCrank_obj_filename);

	modelHandle_[ObjectType::BigBuildingA] = MV1LoadModel(big_buildingA_filepath);
	modelHandle_[ObjectType::BigBuildingB] = MV1LoadModel(big_buildingB_filepath);
	modelHandle_[ObjectType::BigBuildingC] = MV1LoadModel(big_buildingC_filepath);
	modelHandle_[ObjectType::BigBuildingD] = MV1LoadModel(big_buildingD_filepath);

	modelHandle_[ObjectType::BuildingAType1] = MV1LoadModel(buildingA_filepath);
	modelHandle_[ObjectType::BuildingBType1] = MV1LoadModel(buildingB_filepath);
	modelHandle_[ObjectType::BuildingCType1] = MV1LoadModel(buildingC_filepath);

	modelHandle_[ObjectType::BuildingAType2] = MV1LoadModel(buildingA_type2_filepath);
	modelHandle_[ObjectType::BuildingBType2] = MV1LoadModel(buildingB_type2_filepath);
	modelHandle_[ObjectType::BuildingCType2] = MV1LoadModel(buildingC_type2_filepath);

	modelHandle_[ObjectType::StoreA] = MV1LoadModel(storeA_filepath);
	modelHandle_[ObjectType::StoreB] = MV1LoadModel(storeB_filepath);
	modelHandle_[ObjectType::StoreC] = MV1LoadModel(storeC_filepath);

	modelHandle_[ObjectType::Street] = MV1LoadModel(street_filepath);
	modelHandle_[ObjectType::TStreet] = MV1LoadModel(T_street_filepath);
	modelHandle_[ObjectType::Tile] = MV1LoadModel(Tile_filepath);
	modelHandle_[ObjectType::Scaffold] = MV1LoadModel(scaffold_filepath);

	modelHandle_[ObjectType::BlueContainer] = MV1LoadModel(BlueContainer_filepath);
	modelHandle_[ObjectType::RedContainer] = MV1LoadModel(RedContainer_filepath);
	modelHandle_[ObjectType::YellowContainer] = MV1LoadModel(YellowContainer_filepath);
	modelHandle_[ObjectType::OrangeContainer] = MV1LoadModel(OrangeContainer_filepath);

}

ObjectManager::~ObjectManager()
{
	for (auto& type : modelHandle_) {
		MV1DeleteModel(type.second);
	}
}

void ObjectManager::ObjectGenerator()
{

	auto& loadData = ExternalFile::GetInstance();

	for (auto& objInfo : loadData.GetLoadObjectInfo()) {
		//�G���쐬
		if (objInfo.first == "BigBuildingA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingA, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingB") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingB, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingC, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingD") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingD, objSecond);
			}
		}
		else if (objInfo.first == "BuildingA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingAType1, objSecond);
			}
		}
		else if (objInfo.first == "BuildingB") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingBType1, objSecond);
			}
		}
		else if (objInfo.first == "BuildingC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingCType1, objSecond);
			}
		}
		else if (objInfo.first == "Building2A") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingAType2, objSecond);
			}
		}
		else if (objInfo.first == "Building2B") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingBType2, objSecond);
			}
		}
		else if (objInfo.first == "Building2C") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingCType2, objSecond);
			}
		}
		else if (objInfo.first == "StoreA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::StoreA, objSecond);
			}
		}
		else if (objInfo.first == "StoreB") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::StoreB, objSecond);
			}
		}
		else if (objInfo.first == "StoreC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::StoreC, objSecond);
			}
		}
		else if (objInfo.first == "Street") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Street, objSecond);
			}
		}
		else if (objInfo.first == "TStreet") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::TStreet, objSecond);
			}
		}
		else if (objInfo.first == "Tile") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Tile, objSecond);
			}
		}
		else if (objInfo.first == "Scaffold") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Scaffold, objSecond);
			}
		}
		else if (objInfo.first == "BlueContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BlueContainer, objSecond);
			}
		}
		else if (objInfo.first == "RedContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::RedContainer, objSecond);
			}
		}
		else if (objInfo.first == "YellowContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::YellowContainer, objSecond);
			}
		}
		else if (objInfo.first == "OrangeContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::OrangeContainer, objSecond);
			}
		}
	}

	for (auto& gimmick : loadData.GetGimmickInfo()) {
		//�M�~�b�N�X�C�b�`���쐬
		if (gimmick.first == "Trans") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::trans, objSecond);
			}
		}
		//�M�~�b�N�V�����쐬
		else if (gimmick.first == "Steelyard") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard, objSecond);
			}
		}
		else if (gimmick.first == "CrankScaffold") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::CrankScaffold, objSecond);
			}
		}
		else if (gimmick.first == "Elevator") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::elevator, objSecond);
			}
		}
	}
}

void ObjectManager::DeadPersonGenerator(int handle, LoadObjectInfo objInfo, int animNo)
{
	//���̂����������
	objects_[ObjectType::deadPerson].push_back(std::make_shared<DeadPerson>(handle, objInfo, animNo));

	//���̂�4�����������烊�^�[��
	if(objects_[ObjectType::deadPerson].size() < 4) return;

	//���̂̃��X�g�̈�Ԑ擪(�Â�)���̂��폜����
	objects_[ObjectType::deadPerson].remove(objects_[ObjectType::deadPerson].front());

}

//�X�V
void ObjectManager::Update(Player& player, const InputState& input, std::shared_ptr<ShotManager> shotManager)
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
			if (list.first == ObjectType::deadPerson) {
				continue;
			}
			for (auto& deadperson : objects_[ObjectType::deadPerson]) {
				distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
				if (distanceSize < 1000.0f) {
					obj->HitColl(deadperson);
				}
			}
		}
	}

	//enemy��Shot
	for (auto obj : objects_[ObjectType::enemy]) {
		if (std::dynamic_pointer_cast<EnemyBase>(obj) != nullptr) {
			std::dynamic_pointer_cast<EnemyBase>(obj)->Shot(shotManager, player.GetStatus().pos, player.GetStatus().height);
		}
	}

	
	//�X�V
	for (auto list : objects_) {
		for (auto obj : list.second) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
			if (distanceSize < 1000.0f) {
				obj->Update(player, input);
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
	case ObjectBaseType::ornamentBase:
		OrnamentGenerator(objType, objInfo);
		break;
		//�M�~�b�N�𐶐�
	case ObjectBaseType::gimmickBase:
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

	info.pos.x = randomPosX(value);
	info.pos.z = randomPosZ(value);

}

//�G�l�~�[������������|�W�V�������p�x�ɂ���Ď擾����
void ObjectManager::CircumferencePosition(float angle, VECTOR& infoPos, VECTOR playerPos)
{
	VECTOR pos = {};
	float radian = angle * DX_PI_F / 180.0f;

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
	
	//������̃T�C�Y���擾����
	int size = loadInfo.name.size();

	//�u.�v�ȍ~�̕�����ɂ����
	//�G�l�~�[�̏����p�^�[����ύX����
	if (size > 0) {
		std::string str = StrUtil::GetStringAfterSign(loadInfo.name, ".");

		//�����񂪁uALL�v��������
		if (str == "ALL") {
			float angle = 0.0f;
			LoadObjectInfo info;
			info = loadInfo;
			for (int i = 0; i < deathCount; i++) {
				//���͈͂̒��Ń����_���ɃX�|�[��������
				//RandomPositionGenerator(info, loadInfo.pos);
				//�v���C���[�𒆐S�ɉ~����ŃX�|�[��������
				CircumferencePosition(angle, info.pos, playerPos);
				//�C���X�^���X��
				objects_[ObjectType::enemy].push_back(std::make_shared<EnemyBase>(modelHandle_[ObjectType::enemy], info));
				angle -= 15.0f;
			}
		}
		else {
			//�Ō�̕������int�^�̐��l�ɕϊ�����
			int num = atoi(str.c_str());

			//������̍Ō�̐�����deathCount���������
			//�G�l�~�[����������
			if (num <= deathCount) {
				objects_[ObjectType::enemy].push_back(std::make_shared<EnemyBase>(modelHandle_[ObjectType::enemy], loadInfo));
			}
		}
	}
}

//�u�������@
void ObjectManager::OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	objects_[objType].push_front(std::make_shared<OrnamentBase>(modelHandle_[objType], objInfo));
}

void ObjectManager::GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::trans:
		objects_[objType].push_front(std::make_shared<TransparentObject>(modelHandle_[objType], objInfo));
		break;
	case ObjectType::gimmickSteelyard:
		objects_[objType].push_front(std::make_shared<Steelyard>(modelHandle_[objType], objInfo));
		break;
	case ObjectType::elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(modelHandle_[objType], objInfo));
		break;
	case ObjectType::CrankScaffold:
		objects_[objType].push_front(std::make_shared<CrankScaffold>(modelHandle_[objType], objInfo));
		break;
	}
}
