#pragma once
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CInputMgr :
    public CBase
{
    DECLARE_SINGLETON(CInputMgr)
private:
    explicit CInputMgr();
    virtual ~CInputMgr();

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);
	void	LateUpdate_InputDev(void);

public:
	_bool Key_Tap(int vkKey) { return  (m_CurByKeyState[vkKey] & 0x80) && !(m_PrevByKeyState[vkKey] & 0x80);}
	_bool Key_Hold(int vkKey) { return (m_CurByKeyState[vkKey] & 0x80)&&(m_PrevByKeyState[vkKey] & 0x80); }
	_bool Key_Down(int vkKey) { return m_CurByKeyState[vkKey]& 0x80; }
	_bool Key_Away(int vkKey) { return!(m_CurByKeyState[vkKey] & 0x80) && (m_PrevByKeyState[vkKey] & 0x80);}
	_bool Key_Release(int vkKey) { return!(m_CurByKeyState[vkKey] & 0x80); }

public:
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_CurByKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ���� (���� ������)
	_byte					m_PrevByKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����(���� ������)
	DIMOUSESTATE			m_tMouseState; //���콺 ����

private:
    void Free() override;
};

END