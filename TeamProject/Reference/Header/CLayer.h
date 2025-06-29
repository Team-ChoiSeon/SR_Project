#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:

	// ��� ����
	// CSkyBox* pSkyBox = pLayer->Get_GameObject<CSkyBox>(L"SkyBox");
	template <typename T = CGameObject>
	T*		Get_GameObject(const wstring& wObjTag);

	// ��� ���� 
	// pLayer->Add_GameObject(L"SkyBox", pSkyBoxObj);
	void	Add_GameObject(const wstring& wObjTag, CGameObject* pGameObject);
	

public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(const _float& fTimeDelta);
	void			Render_Layer();

private:
	unordered_map<wstring, CGameObject*>			m_umObject;

public:
	static CLayer* Create();

private:
	virtual void	Free();
};

END


template <typename T>
T* CLayer::Get_GameObject(const wstring& wObjTag)
{
	auto iter = m_umObject.find(wObjTag);
	if (iter == m_umObject.end())
	{
		MSG_BOX("[Layer] Get_GameObjectt ����: nullptr ���޵�");
		return nullptr;
	}

	return dynamic_cast<T*>(iter->second);  // �ڵ� ĳ����
}

