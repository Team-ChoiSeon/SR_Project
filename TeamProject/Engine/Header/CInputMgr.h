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
	_bool	Mouse_Tap(MOUSEKEYSTATE eMouse);
	_bool	Mouse_Hold(MOUSEKEYSTATE eMouse);
	_bool	Mouse_Down(MOUSEKEYSTATE eMouse);
	_bool	Mouse_Away(MOUSEKEYSTATE eMouse);

public:
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_CurByKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수 (현재 프레임)
	_byte					m_PrevByKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수(이전 프레임)
	DIMOUSESTATE			m_tCurMouseState; //마우스 상태
	DIMOUSESTATE			m_PreMouseState; //마우스 상태

private:
    void Free() override;
};

END