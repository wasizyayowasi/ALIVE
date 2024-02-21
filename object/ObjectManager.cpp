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
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//置物の生成
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

	//ギミックの生成
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
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//置物の生成
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
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//置物の生成
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
	//死体を一つ生成する
	objects_[ObjectType::Corpse].push_back(std::make_shared<Corpse>(handle, Material::Other, objInfo, animNo));

	//死体が4個未満だったらリターン
	if (objects_[ObjectType::Corpse].size() < 4) return;

	//死体のリストの一番先頭(古い)死体を削除する
	objects_[ObjectType::Corpse].remove(objects_[ObjectType::Corpse].front());
}

//更新
void ObjectManager::Update(Player& player, std::shared_ptr<ShotManager> shotManager)
{
	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	for (auto& list : objects_) {
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->GetIsEnabled(); });
	}

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//死体とその他のオブジェクトの衝突判定を行う
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

	//enemyのShot
	for (auto& obj : objects_[ObjectType::Enemy]) {
		if (std::dynamic_pointer_cast<ThrowEnemy>(obj) != nullptr) {
			std::dynamic_pointer_cast<ThrowEnemy>(obj)->Shot(shotManager, player.GetStatus().pos, player.GetStatus().height);
		}
	}

	//更新
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

//描画
void ObjectManager::Draw(const VECTOR PlayerPos)
{
	float distance = 0.0f;

	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			//オブジェクトからプレイヤーまでの距離をとりサイズ変換する
			distance = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), PlayerPos);
			//プレイヤーから距離が8000.0f未満だったら描画する
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

	//引数で指定されたオブジェクトのモデルポインタを上記で宣言した
	//配列に代入する
	for (auto& obj : objects_) {
		//引数と違うタイプだった場合continue
		if (obj.first != type) {
			continue;
		}
		//指定されたタイプだった場合
		//そのオブジェクトのリストからモデルポインタを取得し
		//specificListに代入する
		for (auto& objSecond : obj.second) {
			specificList.push_back(objSecond->GetModelPointer());
		}
	}

	//リストを返す
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
	//短縮化
	auto& model = ModelManager::GetInstance();

	//文字列のサイズを取得する
	int size = static_cast<int>(info.name.size());

	//「.」以降の文字列によって
	//エネミーの召喚パターンを変更する
	if (size > 0) {
		std::string numStr = StrUtil::GetStringAfterSign(info.name, ".");
		numStr = StrUtil::GetStringBeforeSign(numStr, "-");

		std::string str = StrUtil::GetStringBeforeSign(info.name, ".");

		//文字列が「ALL」だったら
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
	//短縮化
	auto& model = ModelManager::GetInstance();

	float angle = 0.0f;
	for (int i = 0; i < deathCount; i++) {
		info.pos.x += angle;

		//インスタンス化
		objects_[ObjectType::Enemy].push_back(std::make_shared<EnemyBase>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		angle -= 15.0f;

		usedEnemyList_[info.name] = true;
	}
}

void ObjectManager::GeneratedForTheNumberOfTimesYouDie(const int deathCount, LoadObjectInfo info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	float angle = 0.0f;
	for (int i = 0; i < deathCount; i++) {
		//一定範囲の中でランダムにスポーンさせる
		RandomPositionGenerator(info, info.pos);

		//プレイヤーを中心に円周上でスポーンさせる
		//CircumferencePosition(angle, info.pos, info.pos);

		//インスタンス化
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		angle -= 15.0f;

		usedEnemyList_[info.name] = true;
	}
}

void ObjectManager::GeneratePredeterminedNumberOfTimes(const int deathCount, const  std::string str, const  LoadObjectInfo info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//最後の文字列をint型の数値に変換する
	int num = atoi(str.c_str());

	//文字列の最後の数よりもdeathCountが多ければ
	//エネミーを召喚する
	if (num <= deathCount) {
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		usedEnemyList_[info.name] = true;
	}
}

void ObjectManager::GenerateCorpseMountain(const int deathCount, const  LoadObjectInfo info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	std::string str = StrUtil::GetStringAfterSign(info.name, ".");
	str = StrUtil::GetStringBeforeSign(str, "-");

	//最後の文字列をint型の数値に変換する
	int num = atoi(str.c_str());

	//文字列の最後の数よりもdeathCountが多ければ
	//エネミーを召喚する
	if (num <= deathCount) {
		objects_[ObjectType::CorpseMountain].push_back(std::make_shared<OrnamentBase>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::CorpseMountain)].name), Material::Other, info));
		usedCorpseMtList_[info.name] = true;
	}
}

//置物生成機
void ObjectManager::OrnamentGenerator(const std::string name, const ObjectType objType, const Material materialType, const  LoadObjectInfo objInfo)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(name), materialType, objInfo));
}

void ObjectManager::GimmickObjectGenerator(const std::string name, const ObjectType objType, const  Material materialType, const  LoadObjectInfo objInfo)
{
	//短縮化
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
