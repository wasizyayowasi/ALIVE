#include "EffectManager.h"
#include "Effect2D.h"
#include <algorithm>

namespace {
	//�t�@�C���p�X
	const char* const hit_effect_filepath = "data/effect/BigHit.png";
	const char* const smoke_effect_filepath = "data/effect/Smoke.png";
	const char* const leaves_effect_filepath = "data/effect/Leaves.png";
}

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
	for (auto& graph : arrayHandle_) {
		DeleteGraph(graph);
	}
}

void EffectManager::Load()
{
	//�摜�̕����ǂݍ���
	LoadDivGraph(hit_effect_filepath, 27, 9, 3, 516, 528, arrayHandle_, true);

	//�e�[�u���Ɉڂ��ւ���
	for (auto& graph : arrayHandle_) {
		if (graph == 0) {
			continue;
		}

		handle_["hit"].push_back(graph);
	}

	//�摜�̕����ǂݍ���
	LoadDivGraph(smoke_effect_filepath, 20, 4, 5, 144, 144, arrayHandle_, true);

	//�e�[�u���Ɉڂ��ւ���
	for (auto& graph : arrayHandle_) {
		if (graph == 0) {
			continue;
		}

		handle_["smoke"].push_back(graph);
	}

	//�摜�̕����ǂݍ���
	LoadDivGraph(leaves_effect_filepath, 20, 4, 5, 150, 150, arrayHandle_, true);

	//�e�[�u���Ɉڂ��ւ���
	for (auto& graph : arrayHandle_) {
		if (graph == 0) {
			continue;
		}

		handle_["leaves"].push_back(graph);
	}
}

void EffectManager::Update()
{
	effect2D_.remove_if([](std::shared_ptr<Effect2D> effectPoint) {return !effectPoint->IsEnable(); });

	//�X�V
	for (auto& effect : effect2D_) {
		effect->Update();
	}
}

void EffectManager::Draw()
{
	//�`��
	for (auto& effect : effect2D_) {
		effect->DrawBillboard();
	}
}

void EffectManager::AddEffect(std::string name,float size,VECTOR pos)
{
	effect2D_.push_back(std::make_shared<Effect2D>(handle_[name], size, pos));
}