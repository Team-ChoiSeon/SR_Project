#include "Engine_Define.h"
#include "CInputMgr.h"

IMPLEMENT_SINGLETON(CInputMgr)

CInputMgr::CInputMgr()
{
}

CInputMgr::~CInputMgr()
{
}

HRESULT CInputMgr::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput �İ�ü�� �����ϴ� �Լ�
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;

	// Ű���� ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;


	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();


	return S_OK;
}

void CInputMgr::Update_InputDev()
{
	memcpy(m_PrevByKeyState, m_CurByKeyState, sizeof(BYTE) * 256); //���� ������ �� ����
	m_pKeyBoard->GetDeviceState(256, m_CurByKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void CInputMgr::LateUpdate_InputDev(void)
{
}

_long CInputMgr::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	switch (eMouseState)
	{
	case Engine::DIMS_X:
		return m_tMouseState.lX;

	case Engine::DIMD_Y:
		return m_tMouseState.lY;

	case Engine::DIMD_Z: // ���콺 �� �̵���  �̰� ��!
		return m_tMouseState.lZ;

	case Engine::DIMS_END:
		return 0;

	default:
		return 0;
	}
}

_byte CInputMgr::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	switch (eMouse)
	{
	case Engine::DIM_LB:
		return m_tMouseState.rgbButtons[0];
	case Engine::DIM_RB:
		return m_tMouseState.rgbButtons[1];
	case Engine::DIM_MB:
		return m_tMouseState.rgbButtons[2];
	case Engine::DIM_END:
		return 0;
	default:
		return 0;

	}
}

void CInputMgr::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}