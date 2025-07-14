#pragma once
#include "CGameObject.h"
#include "CUiImage.h"

namespace Engine {
	class CTransform;
	class CModel;
	class CTexture;
}
class CImageUIObject : public Engine::CGameObject
{
public:
	CImageUIObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CImageUIObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CImageUIObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	void Set_Texture(CTexture* pTex);
	void Set_Position(const D3DXVECTOR2& pos);
	void Set_RenderSize(_float width, _float height);

private:

};

