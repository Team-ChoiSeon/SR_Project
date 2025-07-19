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

	// 사용 예시
	// CSkyBox* pSkyBox = pLayer->Get_GameObject<CSkyBox>(L"SkyBox");
	template <typename T = CGameObject>
	T*		Get_GameObject(const wstring& wObjTag);

	// 사용 예시 
	// pLayer->Add_GameObject(L"SkyBox", pSkyBoxObj);
	void	Add_GameObject(const wstring& wObjTag, CGameObject* pGameObject);

	const list<OBJINFO>& Get_ObjVec() const { return m_ObjectList; }
	

public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(const _float& fTimeDelta);
	/*void			Render_Layer();*/

public:
	static CLayer* Create();

private:
	virtual void	Free();


private:
	// 오브젝트 리스트를 담기 위한 리스트입니다.
	// 이후 최적화 고려시에 list -> unordered_map으로 변경
	list<OBJINFO>			m_ObjectList;
};

END


template <typename T>
T* CLayer::Get_GameObject(const wstring& wObjTag)
{
	auto iter = find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[&](const OBJINFO& info) {
			return info.szName == wObjTag;
		});

	if (iter != m_ObjectList.end())
	{
		if (iter->pObj == nullptr)
		{
			MSG_BOX("CLayer::pObj is nullptr!");
			return nullptr;
		}
		return dynamic_cast<T*>(iter->pObj);
	}

	// MSG_BOX("CLayer::can't find gameobject");
	return nullptr;
}

