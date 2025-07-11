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

    // 기본 그룹 생성
    FMOD::ChannelGroup* pMaster = nullptr;
    m_pSystem->getMasterChannelGroup(&pMaster);
    m_mapGroup["Master"] = pMaster;

    FMOD::ChannelGroup* pBGM = nullptr;
    m_pSystem->createChannelGroup("BGM", &pBGM);
    m_mapGroup["BGM"] = pBGM;

    FMOD::ChannelGroup* pSFX = nullptr;
    m_pSystem->createChannelGroup("SFX", &pSFX);
    m_mapGroup["SFX"] = pSFX;

    FMOD::ChannelGroup* pUI = nullptr;
    m_pSystem->createChannelGroup("UI", &pUI);
    m_mapGroup["UI"] = pUI;

    FMOD::ChannelGroup* pEnv = nullptr;
    m_pSystem->createChannelGroup("ENV", &pEnv);
    m_mapGroup["ENV"] = pEnv;

    FMOD::ChannelGroup* pVoice = nullptr;
    m_pSystem->createChannelGroup("VOICE", &pVoice);
    m_mapGroup["VOICE"] = pVoice;

    Load_Sound("test", "../Bin/Resource/Sound/test.wav");
    Load_Sound("jump", "../Bin/Resource/Sound/Jump1.mp3");

    return S_OK;
}

void CSoundMgr::Load_Sound(const string& key, const string& filepath)
{
    if (m_mapSound.find(key) != m_mapSound.end())
        return; // 이미 로드된 키면 무시

    FMOD::Sound* pSound = nullptr;
    if (FMOD_OK == m_pSystem->createSound(filepath.c_str(), FMOD_DEFAULT, nullptr, &pSound))
    {
        m_mapSound[key] = pSound;
    }
    else
    {
        MSG_BOX("CSoundMgr:: Load_Sound Failed");
    }
}

void CSoundMgr::Update_Sound()
{
    if (m_pSystem)
        m_pSystem->update();
}

void CSoundMgr::Play(const string& key, const string& group, bool loop)
{
    auto it = m_mapSound.find(key);
    if (it == m_mapSound.end()) return;

    FMOD::Sound* pSound = it->second;
    pSound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

    FMOD::Channel* pChannel = nullptr;
    if (FMOD_OK == m_pSystem->playSound(pSound, nullptr, false, &pChannel))
    {
        pChannel->setVolume(1.0f);
        m_mapChannel[key] = pChannel;

        auto groupIt = m_mapGroup.find(group);
        if (groupIt != m_mapGroup.end())
            pChannel->setChannelGroup(groupIt->second);
    }
}

void CSoundMgr::Pause(const string& key, bool pause)
{
    auto it = m_mapChannel.find(key);
    if (it != m_mapChannel.end())
        it->second->setPaused(pause);
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

bool CSoundMgr::Is_Playing(const string& key) const
{
    auto it = m_mapChannel.find(key);
    if (it != m_mapChannel.end())
    {
        bool playing = false;
        it->second->isPlaying(&playing);
        return playing;
    }
    return false;
}

void CSoundMgr::Stop_Group(const string& group)
{
    auto it = m_mapGroup.find(group);
    if (it != m_mapGroup.end())
        it->second->stop();
}

void CSoundMgr::Set_GroupVolume(const string& group, float volume)
{
    auto it = m_mapGroup.find(group);
    if (it != m_mapGroup.end())
        it->second->setVolume(volume);
}

void CSoundMgr::Mute_Group(const string& group, bool mute)
{
    auto it = m_mapGroup.find(group);
    if (it != m_mapGroup.end())
        it->second->setMute(mute);
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