#pragma once
#include "../object/ObjectData.h"
#include <DxLib.h>
#include <unordered_map>

class ModelManager
{
public:
	~ModelManager();

	static ModelManager& GetInstance() {
		static ModelManager instance;
		return instance;
	}

	/// <summary>
	/// ���f�������[�h����
	/// </summary>
	void LoadModel();

	/// <summary>
	/// 3D���f���n���h�����擾����
	/// </summary>
	/// <returns>���f���n���h��</returns>
	int GetModelHandle(ObjectType type);

private:

	std::unordered_map<ObjectType, int> modelHandle_;

	ModelManager() = default;

	ModelManager(const ModelManager&) = delete;
	void operator = (const ModelManager&) = delete;
};

