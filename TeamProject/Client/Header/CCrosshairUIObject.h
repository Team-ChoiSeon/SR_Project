#pragma once
#include "CGameObject.h"
#include "CUiImage.h"


namespace Engine {
	class CTransform;
	class CModel;
	class CTexture;
}
class CCrosshairUIObject : public Engine::CGameObject
{
public:
	enum class CROSSHAIR_STATE { CROSS_DEFAULT = 0, CROSS_HOVER, CROSS_HOLD };
	enum class CROSSHAIR_TRANSITION { NONE = 0, DEFAULT_TO_HOVER, HOVER_TO_HOLD, HOLD_TO_HOVER, HOVER_TO_DEFAULT };

public:
	CCrosshairUIObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrosshairUIObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	CROSSHAIR_STATE Get_State() { return m_eState; };

	void Set_State(CROSSHAIR_STATE eState);

	static CCrosshairUIObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

private:
	void Update_Animation(const _float& fTimeDelta);

private:
	CROSSHAIR_STATE m_eState = CROSSHAIR_STATE::CROSS_DEFAULT;
	CROSSHAIR_STATE m_ePrevState = CROSSHAIR_STATE::CROSS_DEFAULT;

	CROSSHAIR_TRANSITION m_eTransitionState = CROSSHAIR_TRANSITION::NONE;

	vector<CTexture*> m_vecHoverAnim;
	vector<CTexture*> m_vecHoldAnim;

	float m_fAnimTime = 0.f;
	int m_iAnimFrame = 0;
	bool m_bAnimating = false;

	CUiImage* m_pImage = nullptr;
	//CModel* m_pModel;
};

