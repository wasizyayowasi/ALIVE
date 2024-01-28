#include "ModelManager.h"

namespace {
	const char* const player_model_Filename = "data/player/mv1/player.mv1";
	const char* const enemy_model_Filename = "data/enemy/mv1/enemy.mv1";
	//モデルのファイルパス
	const char* const switch_filepath = "data/model/switch.mv1";
	const char* const trans_scaffold_filepath = "data/model/other/mv1/TransScaffold.mv1";
	const char* const elevator_filepath = "data/model/other/mv1/Elevator.mv1";
	const char* const crank_filepath = "data/model/other/mv1/crankScaffold.mv1";
	const char* const signboardType1_filepath = "data/model/other/mv1/SignBoardType1.mv1";
	const char* const signboardType2_filepath = "data/model/other/mv1/SignBoardType2.mv1";
	const char* const fence_filepath = "data/model/city/container/mv1/FenceType1.mv1";

	//実際に使う予定のモデルパス
	//でかいビル
	const char* const big_buildingA_filepath = "data/model/city/building/mv1/BigBuildingA.mv1";
	//Aみたいな形のビル
	const char* const buildingC_filepath = "data/model/city//building/mv1/BuildingC.mv1";
	//長方形のビル
	const char* const buildingA_type2_filepath = "data/model/city//building/mv1/Building2A.mv1";
	//海外で見るような飲食店
	const char* const storeC_filepath = "data/model/city//building/mv1/StoreC.mv1";
	//道
	const char* const street_filepath = "data/model/city//building/mv1/Street.mv1";
	const char* const T_street_filepath = "data/model/city//building/mv1/TStreet.mv1";
	const char* const Tile_filepath = "data/model/city//building/mv1/Tile.mv1";
	const char* const scaffold_filepath = "data/model/city/others/mv1/Scaffold.mv1";
	//建物
	const char* const blueContainer_filepath = "data/model/city/container/mv1/BlueContainer.mv1";
	const char* const redContainer_filepath = "data/model/city/container/mv1/RedContainer.mv1";
	const char* const yellowContainer_filepath = "data/model/city/container/mv1/YellowContainer.mv1";
	const char* const orangeContainer_filepath = "data/model/city/container/mv1/OrangeContainer.mv1";
	//部屋
	const char* const bed_filepath = "data/model/room/mv1/Bed.mv1";
	const char* const clock_filepath = "data/model/room/mv1/Clock.mv1";
	const char* const desk_filepath = "data/model/room/mv1/Desk.mv1";
	const char* const window_filepath = "data/model/room/mv1/Window.mv1";
	const char* const roomWall_filepath = "data/model/room/mv1/RoomWall.mv1";
	const char* const woodFloor_filepath = "data/model/room/mv1/WoodFloor.mv1";
	const char* const woodenBox_filepath = "data/model/room/mv1/WoodenBox.mv1";
	const char* const TV_filepath = "data/model/room/mv1/TV.mv1";
	const char* const book_filepath = "data/model/room/mv1/Book.mv1";
	//駅
	const char* const train_filepath = "data/model/station/mv1/Train.mv1";
	const char* const station_filepath = "data/model/station/mv1/Station.mv1";
}

ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_) {
		MV1DeleteModel(type.second);
	}
}

void ModelManager::LoadModel()
{
	modelHandle_[ObjectType::Player] = MV1LoadModel(player_model_Filename);					//200MB
	modelHandle_[ObjectType::Enemy] = MV1LoadModel(enemy_model_Filename);					//200
	modelHandle_[ObjectType::Switch] = MV1LoadModel(switch_filepath);						//100MB未満
	modelHandle_[ObjectType::Trans] = MV1LoadModel(trans_scaffold_filepath);				//300MB
	modelHandle_[ObjectType::Elevator] = MV1LoadModel(elevator_filepath);					//200MB
	modelHandle_[ObjectType::CrankScaffold] = MV1LoadModel(crank_filepath);					//500MBぐらい
	modelHandle_[ObjectType::SignBoardType1] = MV1LoadModel(signboardType1_filepath);		//100MB
	modelHandle_[ObjectType::SignBoardType2] = MV1LoadModel(signboardType2_filepath);		//100MB未満
	modelHandle_[ObjectType::Fence] = MV1LoadModel(fence_filepath);							//300MB

	modelHandle_[ObjectType::BigBuildingA] = MV1LoadModel(big_buildingA_filepath);			//よくわからん
	modelHandle_[ObjectType::BuildingCType1] = MV1LoadModel(buildingC_filepath);			//よくわからん
	modelHandle_[ObjectType::BuildingAType2] = MV1LoadModel(buildingA_type2_filepath);		//よくわからん
	modelHandle_[ObjectType::StoreC] = MV1LoadModel(storeC_filepath);						//よくわからん

	modelHandle_[ObjectType::Street] = MV1LoadModel(street_filepath);						//
	modelHandle_[ObjectType::TStreet] = MV1LoadModel(T_street_filepath);					//
	modelHandle_[ObjectType::Tile] = MV1LoadModel(Tile_filepath);							//
	modelHandle_[ObjectType::Scaffold] = MV1LoadModel(scaffold_filepath);					//

	modelHandle_[ObjectType::BlueContainer] = MV1LoadModel(blueContainer_filepath);			//
	modelHandle_[ObjectType::RedContainer] = MV1LoadModel(redContainer_filepath);			//
	modelHandle_[ObjectType::YellowContainer] = MV1LoadModel(yellowContainer_filepath);		//
	modelHandle_[ObjectType::OrangeContainer] = MV1LoadModel(orangeContainer_filepath);		//

	modelHandle_[ObjectType::Bed] = MV1LoadModel(bed_filepath);								//
	modelHandle_[ObjectType::Clock] = MV1LoadModel(clock_filepath);							//
	modelHandle_[ObjectType::Desk] = MV1LoadModel(desk_filepath);							//
	modelHandle_[ObjectType::Window] = MV1LoadModel(window_filepath);						//
	modelHandle_[ObjectType::RoomWall] = MV1LoadModel(roomWall_filepath);					//
	modelHandle_[ObjectType::WoodFloor] = MV1LoadModel(woodFloor_filepath);					//
	modelHandle_[ObjectType::WoodenBox] = MV1LoadModel(woodenBox_filepath);					//
	modelHandle_[ObjectType::TV] = MV1LoadModel(TV_filepath);								//
	modelHandle_[ObjectType::Book] = MV1LoadModel(book_filepath);							//

	modelHandle_[ObjectType::Train] = MV1LoadModel(train_filepath);							//300MB
	modelHandle_[ObjectType::Station] = MV1LoadModel(station_filepath);						//
}

int ModelManager::GetModelHandle(ObjectType type)
{
	return modelHandle_[type];
}