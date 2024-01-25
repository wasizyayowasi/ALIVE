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
	//短縮化
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
	//死体を一つ生成する
	objects_[ObjectType::DeadPerson].push_back(std::make_shared<DeadPerson>(handle, objInfo, animNo));

	//死体が4個未満だったらリターン
	if(objects_[ObjectType::DeadPerson].size() < 4) return;

	//死体のリストの一番先頭(古い)死体を削除する
	objects_[ObjectType::DeadPerson].remove(objects_[ObjectType::DeadPerson].front());

}

//更新
void ObjectManager::Update(Player& player,std::shared_ptr<ShotManager> shotManager)
{
	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	for (auto& list : objects_) {
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->IsEnabled(); });
	}

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//死体とその他のオブジェクトの衝突判定を行う
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

	//enemyのShot
	for (auto obj : objects_[ObjectType::Enemy]) {
		if (std::dynamic_pointer_cast<EnemyBase>(obj) != nullptr) {
			std::dynamic_pointer_cast<EnemyBase>(obj)->Shot(shotManager, player.GetStatus().pos, player.GetStatus().height);
		}
	}

	
	//更新
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

//描画
void ObjectManager::Draw(VECTOR PlayerPos)
{
	float distance = 0.0f;

	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			//オブジェクトからプレイヤーまでの距離をとりサイズ変換する
			distance = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), PlayerPos);
			//プレイヤーから距離が5000.0f未満だったら描画する
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
	//objectBaseTypeを元にインスタンス化するクラスを決める
	switch (baseType) {
		//置物を生成
	case ObjectBaseType::OrnamentBase:
		OrnamentGenerator(objType, objInfo);
		break;
		//ギミックを生成
	case ObjectBaseType::GimmickBase:
		GimmickObjectGenerator(objType, objInfo);
		break;
	}
}

std::list<std::shared_ptr<Model>> ObjectManager::GetSpecificModel(ObjectType type)
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

//一定範囲内でランダムに召喚ポジションを取得する
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

//エネミーを召喚させるポジションを角度によって取得する
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

//敵生成機
void ObjectManager::EnemyGenerator(int deathCount,VECTOR playerPos)
{
	//外部ファイルから読み込んだ「エネミーオブジェクト」の情報を取得する
	auto loadInfo = ExternalFile::GetInstance().GetEnemyInfo(playerPos);

	//短縮化
	auto& model = ModelManager::GetInstance();
	
	//文字列のサイズを取得する
	int size = static_cast<int>(loadInfo.name.size());

	for (auto& enemy : objects_[ObjectType::Enemy]) {
		if (enemy.get()->GetName() == loadInfo.name) {
			return;
		}
	}

	//「.」以降の文字列によって
	//エネミーの召喚パターンを変更する
	if (size > 0) {
		std::string str = StrUtil::GetStringAfterSign(loadInfo.name, ".");

		//文字列が「ALL」だったら
		if (str == "ALL") {
			float angle = 0.0f;
			for (int i = 0; i < deathCount; i++) {
				//一定範囲の中でランダムにスポーンさせる
				//RandomPositionGenerator(info, loadInfo.pos);
				
				//プレイヤーを中心に円周上でスポーンさせる
				//CircumferencePosition(angle, loadInfo.pos, playerPos);
				CircumferencePosition(angle, loadInfo.pos, loadInfo.pos);

				//インスタンス化
				objects_[ObjectType::Enemy].push_back(std::make_shared<EnemyBase>(model.GetModelHandle(ObjectType::Enemy), loadInfo));
				angle -= 15.0f;
			}
		}
		else {
			//最後の文字列をint型の数値に変換する
			int num = atoi(str.c_str());

			//文字列の最後の数よりもdeathCountが多ければ
			//エネミーを召喚する
			if (num <= deathCount) {
				objects_[ObjectType::Enemy].push_back(std::make_shared<EnemyBase>(model.GetModelHandle(ObjectType::Enemy), loadInfo));
			}
		}
	}
}

//置物生成機
void ObjectManager::OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(objType), objInfo));
}

void ObjectManager::GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	//短縮化
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
