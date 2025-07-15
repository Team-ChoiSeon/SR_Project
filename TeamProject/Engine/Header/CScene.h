#pragma once
#include "CBase.h"
#include "CLayer.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	// LAYERID {LAYER_TILE, LAYER_OBJECT, LAYER_PLAYER, LAYER_CAMERA, LAYER_LIGHT, LAYER_UI, LAYER_END};
	// ��� ���̾ �߰��մϴ�.
	virtual			void		Init_Layers();
	// LAYERID�� �ش��ϴ� ���̾ �߰��մϴ�.
	virtual			void		Add_Layer(LAYERID eID);
	// LAYERID�� �ش��ϴ� ���̾ �����մϴ�.
	virtual			void		Remove_Layer(LAYERID eID);
	// ��� ���̾ �����մϴ�.
	virtual			void		Clear_Layers();
	// LAYERID�� �ش��ϴ� ���̾������͸� �����ɴϴ�.
	virtual			CLayer* Get_Layer(LAYERID eID)
	{
		auto iter = m_umLayer.find(eID);
		if (iter == m_umLayer.end())
			return nullptr;

		return iter->second;
	}

	virtual			HRESULT		Ready_Scene();
	virtual	_int	Update_Scene(const _float& fTimeDelta);
	virtual	void	LateUpdate_Scene(const _float& fTimeDelta);

	virtual	void	Exit_Scene();

	virtual HRESULT LoadScene(CScene* from, CScene* to);
	virtual const wstring Get_ScenePath() { return L""; };
protected:
	unordered_map<LAYERID, CLayer*>			m_umLayer;
	LPDIRECT3DDEVICE9						m_pGraphicDev;

protected:
	virtual void			Free();

};

END
