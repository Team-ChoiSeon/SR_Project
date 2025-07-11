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
	void Update_Sound(void);
	void Play(const std::string& key, bool loop = false);
	void Stop(const std::string& key);

	void Free();
	
private:
	FMOD::System* m_pSystem = nullptr;
	std::unordered_map<std::string, FMOD::Sound*>   m_mapSound;
	std::unordered_map<std::string, FMOD::Channel*> m_mapChannel;


};

END