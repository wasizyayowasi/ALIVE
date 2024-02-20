#pragma once
#include <string>
#include <DxLib.h>

//�I�u�W�F�N�g�ׁ̂[�X
enum class ObjectBaseType {
	CharacterBase,			//�L�����N�^�[�x�[�X
	EnemyBase,				//�G�x�[�X
	OrnamentBase,			//�u���x�[�X
	CarryBase,				//�^�ׂ�I�u�W�F�N�g�x�[�X
	GimmickBase,			//���u�x�[�X
};

//�I�u�W�F�N�g�̍ޗ�(������)
enum class Material {
	Stone,					//��
	Iron,					//�S
	Wood,					//��
	Other,					//���̑�
	max,
};

//�I�u�W�F�N�g�̃^�C�v
enum class ObjectType{
	//������
	Enemy,
	Player,
	Corpse,
	CorpseMountain,
	SignBoardPlayer,

	//�M�~�b�N��
	Lever,
	Switch,
	Elevator,
	HopStepJump,
	CrankScaffold,
	TransScaffold,
	PenetrationScaffld,

	//�����Ɏg��
	TV,
	Bed,
	Door,
	Book,
	Desk,
	Clock,
	Window,
	RoomWall,
	LightBulb,
	WoodenBox,
	WoodFloor,
	SignBoardType2,

	//���C���X�e�[�W�Ɏg��
	Tile,
	Fence,
	Train,
	Street,
	StoreC,
	TStreet,
	Station,
	WallBack,
	WallSide,
	TileType2,
	BuildingC,
	Building2A,
	TunnelWall,
	BigBuildingA,
	RedContainer,
	BlueContainer,
	SignBoardType1,
	TunnelEntrance,
	YellowContainer,
	OrangeContainer,

	//���̑�
	Rock,
	Number,
	WhiteBoard,

	Max,
};

struct ObjectData
{
	ObjectType type = {};			//�I�u�W�F�N�g�̃^�C�v
	std::string name = {};			//���O
	Material material = {};			//�f��
	ObjectBaseType baseType = {};	//base�N���X
};

static ObjectData objData[static_cast<int>(ObjectType::Max)]
{
	//�G
	ObjectType::Enemy,
	"Enemy",
	Material::Other,
	ObjectBaseType::EnemyBase,

	//�v���C���[
	ObjectType::Player,
	"Player",
	Material::Other,
	ObjectBaseType::CharacterBase,

	//����
	ObjectType::Corpse,
	"Corpse",
	Material::Other,
	ObjectBaseType::CharacterBase,

	//���̂̎R
	ObjectType::CorpseMountain,
	"CorpseMountain",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//�Ŕ̓G
	ObjectType::SignBoardPlayer,
	"SignBoardPlayer",
	Material::Other,
	ObjectBaseType::EnemyBase,


	//���o�[
	ObjectType::Lever,
	"Lever",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//�X�C�b�`
	ObjectType::Switch,
	"Switch",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//�G���x�[�^�[
	ObjectType::Elevator,
	"Elevator",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//�z�b�v�X�e�b�v�W�����v
	ObjectType::HopStepJump,
	"HopStepJump",
	Material::Other,
	ObjectBaseType::GimmickBase,

	//�N�����N�̑���
	ObjectType::CrankScaffold,
	"CrankScaffold",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//�����ȑ���
	ObjectType::TransScaffold,
	"TransScaffold",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//�ђʂ��鑫��
	ObjectType::PenetrationScaffld,
	"PenetrationScaffld",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//�e���r
	ObjectType::TV,
	"TV",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�x�b�h
	ObjectType::Bed,
	"Bed",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//�h�A
	ObjectType::Door,
	"Door",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//�{
	ObjectType::Book,
	"Book",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//��
	ObjectType::Desk,
	"Desk",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//���v
	ObjectType::Clock,
	"Clock",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//��
	ObjectType::Window,
	"Window",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//�����̕�
	ObjectType::RoomWall,
	"RoomWall",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�d��
	ObjectType::LightBulb,
	"LightBulb",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//�؂̔�
	ObjectType::WoodenBox,
	"WoodenBox",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//�؂̏�
	ObjectType::WoodFloor,
	"WoodFloor",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//�Ŕ^�C�v2
	ObjectType::SignBoardType2,
	"SignBoardType2",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�^�C��
	ObjectType::Tile,
	"Tile",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�t�F���X
	ObjectType::Fence,
	"Fence",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�d��
	ObjectType::Train,
	"Train",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//��
	ObjectType::Street,
	"Street",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�XC
	ObjectType::StoreC,
	"StoreC",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//T�^�̓�
	ObjectType::TStreet,
	"TStreet",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�w
	ObjectType::Station,
	"Station",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//���̕�
	ObjectType::WallBack,
	"WallBack",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�ǂ̉�
	ObjectType::WallSide,
	"WallSide",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�^�C���^�C�v2
	ObjectType::TileType2,
	"TileType2",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//����C
	ObjectType::BuildingC,
	"BuildingC",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//����A
	ObjectType::Building2A,
	"Building2A",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�g���l���̕�
	ObjectType::TunnelWall,
	"TunnelWall",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�ł�������
	ObjectType::BigBuildingA,
	"BigBuildingA",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�Ԃ��R���e�i
	ObjectType::RedContainer,
	"RedContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//���R���e�i
	ObjectType::BlueContainer,
	"BlueContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�Ŕ̃^�C�v1
	ObjectType::SignBoardType1,
	"SignBoardType1",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�g���l���̓���
	ObjectType::TunnelEntrance,
	"TunnelEntrance",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//�C�G���[�R���e�i
	ObjectType::YellowContainer,
	"YellowContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//�I�����W�R���e�i
	ObjectType::OrangeContainer,
	"OrangeContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//��
	ObjectType::Rock,
	"Rock",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//����
	ObjectType::Number,
	"Number",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//�z���C�g�{�[�h
	ObjectType::WhiteBoard,
	"WhiteBoard",
	Material::Other,
	ObjectBaseType::OrnamentBase,
};

//�v�@76byte
struct LoadObjectInfo {
	std::string name;	//�I�u�W�F�N�g�̖��O	//40byte
	VECTOR pos = {};	//�|�W�V����			//12byte
	VECTOR rot = {};	//��]��				//12byte
	VECTOR scale = {};	//�g�k��				//12byte
};