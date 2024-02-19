#include "ModelManager.h"

namespace {
	const char* const player_model_Filename			= "data/model/player/mv1/player1.mv1";
	const char* const board_enemy_model_Filename	= "data/model/player/mv1/SignBoardPlayer.mv1";
	//モデルのファイルパス
	const char* const switch_filepath				= "data/model/switch.mv1";
	const char* const lever_filepath				= "data/model/other/mv1/Lever.mv1";
	const char* const trans_scaffold_filepath		= "data/model/other/mv1/TransScaffold.mv1";
	const char* const elevator_filepath				= "data/model/other/mv1/Elevator.mv1";
	const char* const crank_filepath				= "data/model/other/mv1/crankScaffold.mv1";
	const char* const signboardType1_filepath		= "data/model/other/mv1/SignBoardType1.mv1";
	const char* const signboardType2_filepath		= "data/model/other/mv1/SignBoardType2.mv1";
	const char* const hopStepJump_filepath			= "data/model/other/mv1/HopStepJump.mv1";
	const char* const fence_filepath				= "data/model/city/container/mv1/FenceType1.mv1";
	const char* const corpse_mountain_filepath		= "data/model/other/mv1/CorpseMountain.mv1";
	const char* const whiteBoard_filepath			= "data/model/other/mv1/WhiteBoard.mv1";
	const char* const number_filepath				= "data/model/other/mv1/Number.mv1";
	const char* const stone_filepath				= "data/model/other/mv1/rock.mv1";
	//でかいビル
	const char* const big_buildingA_filepath		= "data/model/city/building/mv1/BigBuildingA.mv1";
	const char* const buildingC_filepath			= "data/model/city//building/mv1/BuildingC.mv1";
	const char* const buildingA_type2_filepath		= "data/model/city//building/mv1/Building2A.mv1";
	const char* const storeC_filepath				= "data/model/city//building/mv1/StoreC.mv1";
	//道
	const char* const street_filepath				= "data/model/city//building/mv1/Street.mv1";
	const char* const T_street_filepath				= "data/model/city//building/mv1/TStreet.mv1";
	const char* const tile_filepath					= "data/model/city//building/mv1/Tile.mv1";
	const char* const tile_type2_filepath			= "data/model/city//building/mv1/TileType2.mv1";
	//壁
	const char* const wall_back_filepath			= "data/model/city//building/mv1/WallBack.mv1";
	const char* const wall_side_filepath			= "data/model/city//building/mv1/WallSide.mv1";
	//建物
	const char* const blueContainer_filepath		= "data/model/city/container/mv1/BlueContainer.mv1";
	const char* const redContainer_filepath			= "data/model/city/container/mv1/RedContainer.mv1";
	const char* const yellowContainer_filepath		= "data/model/city/container/mv1/YellowContainer.mv1";
	const char* const orangeContainer_filepath		= "data/model/city/container/mv1/OrangeContainer.mv1";
	//部屋
	const char* const bed_filepath					= "data/model/room/mv1/Bed.mv1";
	const char* const clock_filepath				= "data/model/room/mv1/Clock.mv1";
	const char* const desk_filepath					= "data/model/room/mv1/Desk.mv1";
	const char* const window_filepath				= "data/model/room/mv1/Window.mv1";
	const char* const roomWall_filepath				= "data/model/room/mv1/RoomWall.mv1";
	const char* const woodFloor_filepath			= "data/model/room/mv1/WoodFloor.mv1";
	const char* const woodenBox_filepath			= "data/model/room/mv1/WoodenBox.mv1";
	const char* const TV_filepath					= "data/model/room/mv1/TV.mv1";
	const char* const book_filepath					= "data/model/room/mv1/Book.mv1";
	const char* const lightBulb_filepath			= "data/model/room/mv1/LightBulb.mv1";
	const char* const door_filepath					= "data/model/room/mv1/Door.mv1";
	//駅
	const char* const train_filepath				= "data/model/station/mv1/Train.mv1";
	const char* const station_filepath				= "data/model/station/mv1/Station.mv1";
	//トンネル
	const char* const tunnel_wall_filepath			= "data/model/tunnel/mv1/TunnelWall.mv1";
	const char* const tunnel_entrance_filepath		= "data/model/tunnel/mv1/TunnelEntrance.mv1";
}

ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_) {
		MV1DeleteModel(type.second);
	}
}

void ModelManager::LoadModel()
{
	modelHandle_[ObjectType::Player] = MV1LoadModel(player_model_Filename);					//67
	modelHandle_[ObjectType::BoardEnemy] = MV1LoadModel(board_enemy_model_Filename);		//67
	modelHandle_[ObjectType::Switch] = MV1LoadModel(switch_filepath);						//54
	modelHandle_[ObjectType::Trans] = MV1LoadModel(trans_scaffold_filepath);				//58
	modelHandle_[ObjectType::Elevator] = MV1LoadModel(elevator_filepath);					//178
	modelHandle_[ObjectType::CrankScaffold] = MV1LoadModel(crank_filepath);					//510
	modelHandle_[ObjectType::SignBoardType1] = MV1LoadModel(signboardType1_filepath);		//103
	modelHandle_[ObjectType::SignBoardType2] = MV1LoadModel(signboardType2_filepath);		//66
	modelHandle_[ObjectType::HopStepJump] = MV1LoadModel(hopStepJump_filepath);				//
	modelHandle_[ObjectType::Fence] = MV1LoadModel(fence_filepath);							//172
	modelHandle_[ObjectType::CorpseMountain] = MV1LoadModel(corpse_mountain_filepath);		//
	modelHandle_[ObjectType::WhiteBoard] = MV1LoadModel(whiteBoard_filepath);				//
	modelHandle_[ObjectType::Number] = MV1LoadModel(number_filepath);						//
	modelHandle_[ObjectType::Lever] = MV1LoadModel(lever_filepath);							//
	modelHandle_[ObjectType::Stone] = MV1LoadModel(stone_filepath);							//

	modelHandle_[ObjectType::BigBuildingA] = MV1LoadModel(big_buildingA_filepath);			//55
	modelHandle_[ObjectType::BuildingCType1] = MV1LoadModel(buildingC_filepath);			//59
	modelHandle_[ObjectType::BuildingAType2] = MV1LoadModel(buildingA_type2_filepath);		//58
	modelHandle_[ObjectType::StoreC] = MV1LoadModel(storeC_filepath);						//57

	modelHandle_[ObjectType::Street] = MV1LoadModel(street_filepath);						//55
	modelHandle_[ObjectType::TStreet] = MV1LoadModel(T_street_filepath);					//54
	modelHandle_[ObjectType::Tile] = MV1LoadModel(tile_filepath);							//58
	modelHandle_[ObjectType::TileType2] = MV1LoadModel(tile_type2_filepath);				//53

	modelHandle_[ObjectType::WallBack] = MV1LoadModel(wall_back_filepath);					//53
	modelHandle_[ObjectType::WallSide] = MV1LoadModel(wall_side_filepath);					//53

	modelHandle_[ObjectType::BlueContainer] = MV1LoadModel(blueContainer_filepath);			//66
	modelHandle_[ObjectType::RedContainer] = MV1LoadModel(redContainer_filepath);			//180
	modelHandle_[ObjectType::YellowContainer] = MV1LoadModel(yellowContainer_filepath);		//179
	modelHandle_[ObjectType::OrangeContainer] = MV1LoadModel(orangeContainer_filepath);		//178

	modelHandle_[ObjectType::Bed] = MV1LoadModel(bed_filepath);								//174
	modelHandle_[ObjectType::Clock] = MV1LoadModel(clock_filepath);							//56
	modelHandle_[ObjectType::Desk] = MV1LoadModel(desk_filepath);							//391
	modelHandle_[ObjectType::Window] = MV1LoadModel(window_filepath);						//70
	modelHandle_[ObjectType::RoomWall] = MV1LoadModel(roomWall_filepath);					//101
	modelHandle_[ObjectType::WoodFloor] = MV1LoadModel(woodFloor_filepath);					//174
	modelHandle_[ObjectType::WoodenBox] = MV1LoadModel(woodenBox_filepath);					//175
	modelHandle_[ObjectType::TV] = MV1LoadModel(TV_filepath);								//513
	modelHandle_[ObjectType::Book] = MV1LoadModel(book_filepath);							//172
	modelHandle_[ObjectType::LightBulb] = MV1LoadModel(lightBulb_filepath);					//
	modelHandle_[ObjectType::Door] = MV1LoadModel(door_filepath);							//

	modelHandle_[ObjectType::Train] = MV1LoadModel(train_filepath);							//307
	modelHandle_[ObjectType::Station] = MV1LoadModel(station_filepath);						//713

	modelHandle_[ObjectType::TunnelWall] = MV1LoadModel(tunnel_wall_filepath);				//595
	modelHandle_[ObjectType::TunnelEntrance] = MV1LoadModel(tunnel_entrance_filepath);		//621
}

int ModelManager::GetModelHandle(ObjectType type)
{
	return modelHandle_[type];
}
