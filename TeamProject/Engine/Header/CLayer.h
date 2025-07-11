#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

typedef struct OBJECTINFO {

	wstring		 szName;
	CGameObject* pObj;

}OBJINFO;

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

	const vector<OBJINFO>& Get_ObjVec() const { return m_vObject; }
	

public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(const _float& fTimeDelta);
	void			Render_Layer();

public:
	static CLayer* Create();

private:
	virtual void	Free();


private:
	// ������Ʈ ����Ʈ�� ��� ���� �����Դϴ�.
	// ���� ����ȭ ����ÿ� vector -> unordered_map���� ����
	vector<OBJINFO>			m_vObject;
};

END


template <typename T>
T* CLayer::Get_GameObject(const wstring& wObjTag)
{
	auto iter = find_if(m_vObject.begin(), m_vObject.end(),
		[&](const OBJINFO& info) {
			return info.szName == wObjTag;
		});

	if (iter != m_vObject.end())
	{
		if (iter->pObj == nullptr)
		{
			MSG_BOX("CLayer::pObj is nullptr!");
			return nullptr;
		}
		return dynamic_cast<T*>(iter->pObj);
	}

	MSG_BOX("CLayer::can't find gameobject");
	return nullptr;
}

