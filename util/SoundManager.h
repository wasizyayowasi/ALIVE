#pragma once
#include <DxLib.h>
#include<unordered_map>
#include<string>

enum class SoundPlay {
	notPlaying,		//再生していない
	nowPlaying,		//再生中
};

/// <summary>
/// SE、BGMの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SoundManager();

	/// <summary>
	/// インスタンス作成
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	/// <summary>
	/// 音声ファイルをロードする
	/// </summary>
	void LoadSound();

	/// <summary>
	/// サウンドボリュームを変更する
	/// </summary>
	void ChangeVolumeMem();

	/// <summary>
	/// 読み込んだサウンドを流す
	/// </summary>
	/// <param name="name">ファイル名</param>
	void PlaySE(std::string name);

	/// <summary>
	/// 読み込んだBGMを流す
	/// </summary>
	/// <param name="name">ファイル名</param>
	void PlayBGM(std::string name);

	/// <summary>
	/// SE音量を調整する
	/// </summary>
	/// <param name="volume">音量</param>
	void SetSEVolume(unsigned int volume);

	/// <summary>
	/// SEの音量を取得する
	/// </summary>
	/// <returns>音量</returns>
	int GetSEVolume()const { return volumeSE_; };

	/// <summary>
	/// BGMの音量を調整する
	/// </summary>
	/// <param name="volume">音量</param>
	void SetBGMVolume(unsigned int volume);

	/// <summary>
	/// BGMの音量を取得する
	/// </summary>
	/// <returns>音量</returns>
	int GetBGMVolume()const { return volumeBGM_; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="rate"></param>
	void SetBGMRate(float rate);
	
	/// <summary>
	/// SEを止める
	/// </summary>
	/// <param name="name">音のファイル名</param>
	void StopSE(std::string name = "");

	/// <summary>
	/// BGMを止める
	/// </summary>
	void StopBGM();

	/// <summary>
	/// BGM、SEの音量を外部出力する
	/// </summary>
	void SaveSoundConfig();

	/// <summary>
	/// サウンドのポジション、聞こえる範囲を設定する
	/// </summary>
	/// <param name="pos">サウンドのポジション</param>
	/// <param name="audioRange">聞こえる範囲</param>
	/// <param name="name">サウンド名</param>
	void Set3DSoundInfo(VECTOR pos,float audioRange,std::string name);

	/// <summary>
	/// 3Dサウンドを再生する際のリスナーの位置を決める
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Set3DSoundListenerInfo(VECTOR pos, VECTOR rot);

	/// <summary>
	/// サウンドが再生中か調べる
	/// </summary>
	/// <returns>1:再生中 1:終了</returns>
	int CheckSoundFile(std::string name);

private:
	//計　12byte
	struct SoundConfigInfo {
		char signature[4];				//4byte
		float version;					//4byte
		unsigned short volumeSE;		//2byte
		unsigned short volumeBGM;		//2byte
	};

	int volumeSE_ = 255;
	int volumeBGM_ = 255;

	std::unordered_map<std::string, int> nameAndHandleTable_;

	/// <summary>
	/// 2DSEをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load2DSoundSEFile(std::string fileName);
 
	/// <summary>
	/// 2DBGMをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load2DSoundBGMFile(std::string fileName);

	/// <summary>
	/// 3DSEをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load3DSoundSEFile(std::string fileName);

	/// <summary>
	/// 3DBGMをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load3DSoundBGMFile(std::string fileName);

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;

	void LoadSoundConfig();

};

