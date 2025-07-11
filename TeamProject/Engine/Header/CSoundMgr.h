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


};

END