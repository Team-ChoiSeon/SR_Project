#pragma once
#include "pch.h"
#include "CGameObject.h"
#include "CCamera.h"
namespace Engine {
	class CComponent;
}
class CCameraObject : public CGameObject
{
protected:
	explicit CCameraObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCameraObject(const CCameraObject& rhs);
	virtual ~CCameraObject();

public:
	//Basic Function
	virtual HRESULT		Ready_GameObject() override;
	virtual int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void		Render_GameObject() override;

	//Create, Release Function
	virtual	void		Free();
	
	//Getter, Setter Function
	virtual _matrix	Get_ViewMatrix() { return *m_pCamera->Get_ViewMatrix(); }
	virtual _matrix	Get_ProjectionMatrix() { return *m_pCamera->Get_ProjectionMatrix(); }

protected:
	CCamera*	m_pCamera;
	CComponent* m_pTransform;
private:

	bool	m_bMain = false;
	bool	m_bActive = false;
};

