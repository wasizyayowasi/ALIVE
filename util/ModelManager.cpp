#include "ModelManager.h"

namespace {
	const char* const player_model_Filename = "data/player/mv1/player.mv1";
	const char* const enemy_model_Filename = "data/enemy/mv1/enemy.mv1";
	//モデルのファイルパス
	const char* const switch_filepath = "data/model/switch.mv1";
	const char* const transparent_filepath = "data/model/other/mv1/ElevatingMovableScaffolding.mv1";
	const char* const elevator_filepath = "data/model/other/mv1/Elevator.mv1";
	const char* const crank_filepath = "data/model/other/mv1/crankScaffold.mv1";
	const char* const signboardType1_filepath = "data/model/other/mv1/SignBoardType1.mv1";
	const char* const signboardType2_filepath = "data/model/other/mv1/SignBoardType2.mv1";

	//実際に使う予定のモデルパス
	//でかいビル
	const char* const big_buildingA_filepath = "data/model/city/model/BigBuildingA.mv1";
	const char* const big_buildingB_filepath = "data/model/city/model/BigBuildingB.mv1";
	const char* const big_buildingC_filepath = "data/model/city/model/BigBuildingC.mv1";
	const char* const big_buildingD_filepath = "data/model/city/model/BigBuildingD.mv1";
	//Aみたいな形のビル
	const char* const buildingA_filepath = "data/model/city/model/BuildingA.mv1";
	const char* const buildingB_filepath = "data/model/city/model/BuildingB.mv1";
	const char* const buildingC_filepath = "data/model/city/model/BuildingC.mv1";
	//長方形のビル
	const char* const buildingA_type2_filepath = "data/model/city/model/Building2A.mv1";
	const char* const buildingB_type2_filepath = "data/model/city/model/Building2B.mv1";
	const char* const buildingC_type2_filepath = "data/model/city/model/Building2C.mv1";
	//海外で見るような飲食店
	const char* const storeA_filepath = "data/model/city/model/StoreA.mv1";
	const char* const storeB_filepath = "data/model/city/model/StoreB.mv1";
	const char* const storeC_filepath = "data/model/city/model/StoreC.mv1";
	//道
	const char* const street_filepath = "data/model/city/model/Street.mv1";
	const char* const T_street_filepath = "data/model/city/model/TStreet.mv1";
	const char* const Tile_filepath = "data/model/city/model/Tile.mv1";
	const char* const scaffold_filepath = "data/model/city/others/Scaffold.mv1";
	//建物
	const char* const blueContainer_filepath = "data/model/city/container/mv1/BlueContainer.mv1";
	const char* const redContainer_filepath = "data/model/city/container/mv1/RedContainer.mv1";
	const char* const yellowContainer_filepath = "data/model/city/container/mv1/YellowContainer.mv1";
	const char* const orangeContainer_filepath = "data/model/city/container/mv1/OrangeContainer.mv1";
	//部屋
	const char* const bed_filepath = "data/model/room/mv1/Bed.mv1";
	const char* const chair_filepath = "data/model/room/mv1/Chair.mv1";
	const char* const clock_filepath = "data/model/room/mv1/Clock.mv1";
	const char* const desk_filepath = "data/model/room/mv1/Desk.mv1";
	const char* const window_filepath = "data/model/room/mv1/Window.mv1";
	const char* const roomWall_filepath = "data/model/room/mv1/RoomWall.mv1";
	const char* const woodFloor_filepath = "data/model/room/mv1/WoodFloor.mv1";
	const char* const woodenBox_filepath = "data/model/room/mv1/WoodenBox.mv1";
}

ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_) {
		MV1DeleteModel(type.second);
	}
}

void ModelManager::LoadModel()
{
	modelHandle_[ObjectType::Player] = MV1LoadModel(player_model_Filename);
	modelHandle_[ObjectType::Enemy] = MV1LoadModel(enemy_model_Filename);
	modelHandle_[ObjectType::GimmickSwitch] = MV1LoadModel(switch_filepath);
	modelHandle_[ObjectType::Trans] = MV1LoadModel(transparent_filepath);
	modelHandle_[ObjectType::Elevator] = MV1LoadModel(elevator_filepath);
	modelHandle_[ObjectType::CrankScaffold] = MV1LoadModel(crank_filepath);
	modelHandle_[ObjectType::SignBoardType1] = MV1LoadModel(signboardType1_filepath);
	modelHandle_[ObjectType::SignBoardType2] = MV1LoadModel(signboardType2_filepath);

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

	modelHandle_[ObjectType::BlueContainer] = MV1LoadModel(blueContainer_filepath);
	modelHandle_[ObjectType::RedContainer] = MV1LoadModel(redContainer_filepath);
	modelHandle_[ObjectType::YellowContainer] = MV1LoadModel(yellowContainer_filepath);
	modelHandle_[ObjectType::OrangeContainer] = MV1LoadModel(orangeContainer_filepath);

	modelHandle_[ObjectType::Bed] = MV1LoadModel(bed_filepath);
	modelHandle_[ObjectType::Chair] = MV1LoadModel(chair_filepath);
	modelHandle_[ObjectType::Clock] = MV1LoadModel(clock_filepath);
	modelHandle_[ObjectType::Desk] = MV1LoadModel(desk_filepath);
	modelHandle_[ObjectType::Window] = MV1LoadModel(window_filepath);
	modelHandle_[ObjectType::RoomWall] = MV1LoadModel(roomWall_filepath);
	modelHandle_[ObjectType::WoodFloor] = MV1LoadModel(woodFloor_filepath);
	modelHandle_[ObjectType::WoodenBox] = MV1LoadModel(woodenBox_filepath);
}

int ModelManager::GetModelHandle(ObjectType type)
{
	return modelHandle_[type];
}