#pragma once
#include "CBase.h"

class Player;
namespace Engine {
	class CGraphicDev;
	class CCameraObject;
}

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public: 
	HRESULT Ready_MainApp();
	int Update_MainApp(_float&fTimeDelta);
	void LateUpdate_MainApp(_float&fTimeDelta);
	void Render_MainApp();

private:
	CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	Player* m_pPlayer;	
	ID3DXFont* m_pFont;

public:
	static CMainApp* Create();
private:
	virtual void Free() override;
};
