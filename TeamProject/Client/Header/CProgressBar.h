#pragma once
#include "CGameObject.h"
#include "CUiImage.h"
#include "CImageUIObject.h"


namespace Engine {
	class CTransform;
	class CModel;
}
class CProgressBar : public Engine::CGameObject
{
public:
	CProgressBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProgressBar();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CProgressBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	void Set_Progress(float fPercent);

private:
	CImageUIObject* m_pFillImage = nullptr;
	CImageUIObject* m_pBackImage = nullptr;

	float m_fProgress = 1.f;

};

