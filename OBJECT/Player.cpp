#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include "../UTIL/CheckCollitionModel.h"
#include<algorithm>
#include<string>

namespace {
	//アニメーション
	constexpr int anim_idle_no = 0;			//待機状態
	constexpr int anim_run_no = 1;			//走る
	constexpr int anim_stairs_no = 2;		//階段を上る
	constexpr int anim_clim_no = 3;			//上る
	constexpr int anim_jump_no = 4;			//ジャンプ
	constexpr int anim_runningJump_no = 5;	//走りジャンプ
	constexpr int anim_death_no = 6;		//走りジャンプ
	constexpr int anim_walk_no = 7;			//歩く

	//ジャンプ
	constexpr float jump_power = 15.0f;
	constexpr float gravity = -1.0f;

	//回転スピード
	constexpr float rot_speed = 15.0f;

	//ファイルパス
	const char* const player_Filename = "DATA/player/player7.mv1";
	

	//プレイヤーサイズ
	const VECTOR player_scale = { 0.5f,0.5f ,0.5f };

	//初期プレイヤーの回転角度
	const VECTOR start_player_rot = { 0.0f,0.0f ,0.0f };

	//プレイヤーの高さ
	constexpr float player_hegiht = 150.0f;

	//移動スピード
	constexpr float walk_speed = 3.0f;
	constexpr float run_speed = 7.0f;
}

using namespace std;

Player::Player()
{
	//プレイヤーモデルの生成
	PModel_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	PModel_->setAnimation(anim_idle_no, true, false);
	//プレイヤーの大きさの調整
	PModel_->setScale(player_scale);
	//マップやブロックなどの当たり判定の生成
	checkCollitionModel_ = make_shared<CheckCollitionModel>(std::shared_ptr<Player>(this));

	deadPlayer_.push_back(make_shared<Model>(PModel_->getModelHandle()));

	deadPlayer_.back()->setScale(player_scale);

	jump_.isJump = false;
	jump_.jumpVec = 0.0f;

}

Player::~Player()
{
}

void Player::update(const InputState& input, std::vector<std::shared_ptr<Model>> models)
{

	moveVec_ = { 0.0f,0.0f,0.0f };
	movingSpeed_ = 0.0f;

	PModel_->update();

	if (!isDead_) {
		movingUpdate(input);
		rotationUpdate();
		jumpUpdate(input);
		death(input);
		changeAnimIdle();
	}
	else {
		if (PModel_->isAnimEnd()) {

			isDead_ = false;
		}
	}

	for (auto& deadPerson : deadPlayer_) {
		models.push_back(deadPerson);
	}
	

	PModel_->setPos(pos_);

	checkCollitionModel_->checkCollition(moveVec_, models, player_hegiht, jump_.isJump, jump_.jumpVec);
}

void Player::draw()
{
	PModel_->draw();
	for (auto& deadPlayer : deadPlayer_) {
		if (deadPlayer->getEnable()) {
			deadPlayer->draw();
		}
	}
	
	//cube_->draw();

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);
	DrawFormatString(0, 16, 0x448844, "targetAngle : %.2f", targetAngle_);
	DrawFormatString(0, 32, 0x448844, "rot_ : %.2f", rot_.y);
	DrawFormatString(0, 48, 0x448844, "tempAngle : %.2f", tempAngle_);
	DrawFormatString(0, 64, 0x448844, "jumpFlag : %d", jump_.isJump);
	DrawFormatString(0, 80, 0x448844, "x:%.2f y:%.2f z:%.2f", pos_.x, pos_.y, pos_.z);
	for (auto& deadPlayer : deadPlayer_) {
		if (deadPlayer->getEnable()) {
			DrawFormatString(0, 96, 0x448844, "x:%.2f y:%.2f z:%.2f", deadPlayer->getPos().x, deadPlayer->getPos().y, deadPlayer->getPos().z);
		}
	}

	for (const auto person : deadPlayer_) {
		if (person->getEnable()) {
			DrawSphere3D(person->getPos(), 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}

void Player::setJumpInfo(bool isJump, float jumpVec)
{
	jump_.isJump = isJump;
	jump_.jumpVec = jumpVec;
}

void Player::movingUpdate(const InputState& input)
{

	isMoving = false;
	

	{
		if (input.isPressed(InputType::up)) {
			movingSpeed_ = walk_speed;
			moveVec_.z += movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			movingSpeed_ = walk_speed;
			moveVec_.z -= movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 0.0f;
		}
		if (input.isPressed(InputType::left)) {
			movingSpeed_ = walk_speed;
			moveVec_.x -= movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			movingSpeed_ = walk_speed;
			moveVec_.x += movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 270.0f;
		}
		if (input.isPressed(InputType::up) && input.isPressed(InputType::right)) {
			targetAngle_ = 225.0f;
		}
		if (input.isPressed(InputType::up) && input.isPressed(InputType::left)) {
			targetAngle_ = 135.0f;
		}
		if (input.isPressed(InputType::down) && input.isPressed(InputType::left)) {
			targetAngle_ = 45.0f;
		}
		if (input.isPressed(InputType::down) && input.isPressed(InputType::right)) {
			targetAngle_ = 315.0f;
		}
		if ((input.isPressed(InputType::up) || input.isPressed(InputType::down) || input.isPressed(InputType::left) || input.isPressed(InputType::right)) && input.isPressed(InputType::shift)) {
			movingSpeed_ = run_speed;
			animNo_ = anim_run_no;
		}
		
		moveVec_ = VScale(VNorm(moveVec_),movingSpeed_);

		rotationUpdate();

		PModel_->changeAnimation(animNo_, true, false, 20);

		//デバッグ用
		/*{
			if (input.isPressed(InputType::next)) {
				pos_.y += movingSpeed_;
			}
			if (input.isPressed(InputType::prev)) {
				pos_.y -= movingSpeed_;
			}
		}*/
	}
}

void Player::jumpUpdate(const InputState& input)
{
	//ジャンプ処理
	{
		if (!jump_.isJump) {
			if (input.isPressed(InputType::space)) {
				animNo_ = anim_jump_no;
				jump_.jumpVec += jump_power;
				jump_.isJump = true;
			}
		}

		if (jump_.isJump) {
			jump_.jumpVec += gravity;
			pos_.y += jump_.jumpVec;
			if (pos_.y <= 0.0f) {
				jump_.isJump = false;
			}
		}
	}

	PModel_->changeAnimation(animNo_, false, false, 20);

}

void Player::death(const InputState& input)
{
	//死亡
	{
		if (input.isTriggered(InputType::z)) {
			DeadPlayer deadPerson;
			deadPerson.isEnable = true;
			deadPerson.deathPos = pos_;

			isDead_ = true;
			
			deadPlayer_.back()->setPos(deadPerson.deathPos);
			deadPlayer_.back()->setScale(player_scale);
			deadPlayer_.back()->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
			deadPlayer_.back()->setUseCollision(true, false);

			animNo_ = anim_death_no;

			deathNum = 0;
			for (const auto person : deadPlayer_) {
				if (person->getEnable()) {
					deathNum++;
					if (deathNum > 9) {
						deadPlayer_.erase(deadPlayer_.begin());
						deathNum--;
					}
				}
			}

		}

		PModel_->changeAnimation(animNo_, false, false, 10);

	}
}

void Player::changeAnimIdle()
{
	//待機アニメーションに戻す
	if (!isMoving) {
		if (animNo_ == anim_walk_no || animNo_ == anim_run_no) {
			animNo_ = anim_idle_no;
			PModel_->changeAnimation(animNo_, true, false, 10);
		}
	}
}

//完成品だから今後いじらなくていいと思う
void Player::rotationUpdate()
{
	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - tempAngle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	if (differenceAngle_ >= 180.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ - 360.0f;
	}
	else if (differenceAngle_ <= -180.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ + 360.0f;
	}

	//滑らかに回転させるため
	//現在の角度に回転スピードを増減させている
	if (differenceAngle_ < 0.0f) {
		rot_.y -= rot_speed;
		tempAngle_ -= rot_speed;
	}
	else if(differenceAngle_ > 0.0f){
		rot_.y += rot_speed;
		tempAngle_ += rot_speed;
	}

	//360度、一周したら0度に戻すようにしている
	if (tempAngle_ == 360.0f || tempAngle_ == -360.0f) {
		tempAngle_ = 0.0f;
	}
	if (rot_.y == 360.0f || rot_.y == -360.0f) {
		rot_.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	PModel_->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
}
