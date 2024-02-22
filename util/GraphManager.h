#pragma once
#include <map>
#include <string>

class GraphManager
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GraphManager();

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static GraphManager& GetInstance() {
		static GraphManager instance;
		return instance;
	}

	/// <summary>
	/// �摜��ǂݍ���
	/// </summary>
	void Load();
	
	/// <summary>
	/// �摜���擾����
	/// </summary>
	/// <param name="name">�摜�̖��O</param>
	/// <returns>�摜�n���h��</returns>
	int GetGraph(const std::string& name);

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GraphManager() = default;

	GraphManager(const GraphManager&) = delete;
	void operator = (const GraphManager&) = delete;

	std::map<std::string, int> graphHandle_ = {};		//�O���t�e�[�u��

};

