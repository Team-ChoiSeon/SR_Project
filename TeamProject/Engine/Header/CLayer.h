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
	// 오브젝트 리스트를 담기 위한 벡터입니다.
	// 이후 최적화 고려시에 vector -> unordered_map으로 변경
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

	return dynamic_cast<T*>(iter->pObj);  // 자동 캐스팅
}

