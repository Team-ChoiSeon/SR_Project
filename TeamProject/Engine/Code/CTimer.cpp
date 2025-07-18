#include "Engine_Define.h"
#include "CTimer.h"

CTimer::CTimer()
	:m_fTimeDelta(0.f)
{
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FrameTime);

	QueryPerformanceFrequency(&m_CpuTick);
	return S_OK;
}

void CTimer::Update_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);
	if (m_FrameTime.QuadPart - m_FixTime.QuadPart > m_CpuTick.QuadPart) {
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart)/ (_float)m_CpuTick.QuadPart;
	if (m_fTimeDelta > 0.033f) {
		m_fTimeDelta = 0.033f;
	}
	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer;
	if (FAILED(pInstance->Ready_Timer())) {
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{
}
