#pragma once
#include "pch.h"
#include "CGameObject.h"
class CCameraObject : public CGameObject
{
protected:
	explicit CCameraObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCameraObject(const CCameraObject& rhs);

public:
	virtual ~CCameraObject();

public:
	virtual HRESULT		Ready_CameraObject();
	virtual int			Update_CameraObject(const _float& fTimeDelta);
	virtual void		LateUpdate_CameraObject(const _float& fTimeDelta);
	virtual void		Render_CameraObject();
	void				Render_GameObject() override;

	void				Key_Input();
	void				Rotate_ByCursor();

	virtual CCameraObject* Clone();
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Free();

protected:

private:
	bool	m_bActive = false;
};

