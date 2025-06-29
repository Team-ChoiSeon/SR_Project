#pragma once
#include "CComponent.h"

BEGIN(Engine)
class ENGINE_DLL CPicking : public CComponent
{
private:
	explicit CPicking();
	explicit CPicking(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPicking(const CPicking& rhs);

public:
	virtual ~CPicking();

	//Basic Functions
	HRESULT			Ready_Picking();
	virtual void	Update_Picking(const float& fTimeDelta);
	virtual void	LateUpdate_Picking();


	//Create, Clone, Free
	static			CPicking* Create(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			CComponent* Clone();
	virtual void	Free();

	//Main Functions
	void			ComputeRay();

public:			
	//Getter
	_vec2			Get_CursorPos() { return m_vCursorPos; }

	//Setter
	void			Set_CursorPos(_vec2 cusor) { m_vCursorPos = cusor; }

protected:

private:
	HWND					m_HWND;
	_vec2					m_vCursorPos;
};

END