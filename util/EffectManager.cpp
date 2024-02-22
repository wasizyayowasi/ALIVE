#include "EffectManager.h"
#include "Effect2D.h"

#include "../util/ExternalFile.h"

#include <algorithm>

namespace {
	//�t�@�C���p�X
	const char* const hit_effect_filepath = "data/effect/BigHit.png";
	const char* const smoke_effect_filepath = "data/effect/Smoke.png";
	const char* const leaves_effect_filepath = "data/effect/Leaves.png";
}

//�R���X�g���N�^
EffectManager::EffectManager()
{
}

//�f�X�g���N�^
EffectManager::~EffectManager()
{
	for (auto& table : handle_)
	{
		for (auto& graph : table.second)
		{
			DeleteGraph(graph);
		}
	}

	for (auto& graph : arrayHandle_) {
		DeleteGraph(graph);
	}
}

//�摜�̓ǂݍ���
void EffectManager::Load()
{
	//�����摜�̓ǂݍ���
	for (auto& place : ExternalFile::GetInstance().GetDivGraphData())
	{
		for (auto& info : place.second)
		{
			//�p�X�̍쐬
			std::string path = "data/" + place.first + "/" + info.name + ".png";

			//���������̍쐬
			int totalDivNum = info.divXNum * info.divYNum;

			//�摜�̕����ǂݍ���
			LoadDivGraph(path.c_str(), totalDivNum, info.divXNum, info.divYNum, info.divXSize, info.divYSize, arrayHandle_);

			for (int i = 0; i < totalDivNum; i++)
			{
				handle_[info.name].push_back(arrayHandle_[i]);
				arrayHandle_[i] = 0;
			}
		}
	}
}

//�X�V
void EffectManager::Update()
{
	effect2D_.remove_if([](std::shared_ptr<Effect2D> effectPoint) {return !effectPoint->IsEnable(); });

	//�X�V
	for (auto& effect : effect2D_) {
		effect->Update();
	}
}

//�`��
void EffectManager::Draw()
{
	//�`��
	for (auto& effect : effect2D_) {
		effect->DrawBillboard();
	}
}

//�G�t�F�N�g�̒ǉ�
void EffectManager::AddEffect(const std::string& name, const float size, const VECTOR& pos)
{
	effect2D_.push_back(std::make_shared<Effect2D>(handle_[name], size, pos));
}