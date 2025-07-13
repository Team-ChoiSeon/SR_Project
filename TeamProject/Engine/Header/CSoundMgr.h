#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include <fmod.hpp>

BEGIN(Engine)

class ENGINE_DLL CSoundMgr :
	public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

public:
	HRESULT Ready_Sound();
	void Load_Sound(const string& key, const string& filepath);
	void Update_Sound();
	
	void Play(const string& key, const string& group = "SFX", bool loop = false);
	void Pause(const string& key, bool pause);
	void Stop(const string& key);
	void Set_Volume(const string& key, float volume);
	bool Is_Playing(const string& key) const;
	
	void Stop_Group(const string& group);
	void Set_GroupVolume(const string& group, float volume);
	void Mute_Group(const string& group, bool mute);

	void Free();
	
private:
	FMOD::System* m_pSystem = nullptr;

	std::unordered_map<std::string, FMOD::Sound*> m_mapSound;
	std::unordered_map<std::string, FMOD::Channel*> m_mapChannel;
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapGroup;
	unordered_map<std::string, float> m_mapVolume;


};

END

// 이하 사용법
/*
* 1. 초기화
* CSoundMgr::Get_Instance()->Ready_Sound();
* 
* 2. 사운드 로드
* CSoundMgr::Get_Instance()->Load_Sound("jump", "../Bin/Resource/Sound/Jump1.mp3");
* 
* 3. 사운드 재생
* CSoundMgr::Get_Instance()->Play("jump", "SFX", false);
* 
* 4. 개별 사운드 제어
* CSoundMgr::Get_Instance()->Set_Volume("jump", 0.5f);
* CSoundMgr::Get_Instance()->Pause("jump", true);
* CSoundMgr::Get_Instance()->Stop("jump");
* 
* 5. 그룹 제어
* 
* CSoundMgr::Get_Instance()->Set_GroupVolume("SFX", 0.3f);
* CSoundMgr::Get_Instance()->Mute_Group("SFX", true);
* CSoundMgr::Get_Instance()->Stop_Group("SFX");
* 
*/