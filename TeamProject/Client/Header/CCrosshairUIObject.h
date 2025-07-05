#pragma once
#include "CGameObject.h"
#include "CUiImage.h"

namespace Engine {
	class CTransform;
	class CModel;
}
class CCrosshairUIObject : public Engine::CGameObject
{
public:
	enum class CROSSHAIR_STATE { CROSS_DEFAULT = 0, CROSS_HOVER, CROSS_HOLD };

public:
	CCrosshairUIObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrosshairUIObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	void Set_State(CROSSHAIR_STATE eState);

	static CCrosshairUIObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

private:
	void Update_Texture_State();

private:
	CROSSHAIR_STATE m_eState = CROSSHAIR_STATE::CROSS_DEFAULT;
	//CModel* m_pModel;
};

