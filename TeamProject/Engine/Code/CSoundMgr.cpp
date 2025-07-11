#include "CSoundMgr.h"

#if defined(_DEBUG)
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
}

HRESULT CSoundMgr::Ready_Sound()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

    FMOD::Sound* pSound = nullptr;
    if (FMOD_OK != m_pSystem->createSound("../Bin/Resource/Sound/test.wav", FMOD_DEFAULT, nullptr, &pSound))
    {
        OutputDebugString("=== failed ===\n");
        return E_FAIL;
    }

    m_mapSound["Test"] = pSound;
    return S_OK;
}

void CSoundMgr::Update_Sound(void)
{
    if (m_pSystem)
        m_pSystem->update();
}

void CSoundMgr::Play(const std::string& key, bool loop)
{
    auto it = m_mapSound.find(key);
    if (it == m_mapSound.end()) return;

    FMOD::Sound* pSound = it->second;
    pSound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

    FMOD::Channel* pChannel = nullptr;
    FMOD_RESULT result = m_pSystem->playSound(pSound, nullptr, false, &pChannel);
    if (result != FMOD_OK) {
        MSG_BOX("sound fail!");
        return;
    }

    // 볼륨 명시적으로 설정
    pChannel->setVolume(1.0f);

    m_mapChannel[key] = pChannel;
}

void CSoundMgr::Stop(const std::string& key)
{
    auto it = m_mapChannel.find(key);
    if (it != m_mapChannel.end())
    {
        it->second->stop();
        m_mapChannel.erase(it);
    }
}

void CSoundMgr::Free()
{
    for (auto& pair : m_mapSound)
        pair.second->release();
    m_mapSound.clear();

    if (m_pSystem)
    {
        m_pSystem->close();
        m_pSystem->release();
        m_pSystem = nullptr;
    }
}