#include "CLayer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

void CLayer::Add_GameObject(const wstring& wObjTag, CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
	{
		MSG_BOX("[Layer] Add_GameObject : nullptr ");
		return;
	}

	// 이미 동일 태그가 존재하는 경우 덮어쓰지 않기
	auto it = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[&](const OBJINFO& info) {
			return info.szName == wObjTag;
		});
	if (it != m_ObjectList.end())
	{
		MSG_BOX("[Layer] Add_GameObject : tag");
		return;
	}

	m_ObjectList.push_back({ wObjTag, pGameObject });

	return;
}

HRESULT CLayer::Ready_Layer()
{

	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto iter = m_ObjectList.begin(); iter != m_ObjectList.end(); )
	{
		iResult = iter->pObj->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;

		if (iResult == 1)
		{
			Safe_Release(iter->pObj);
			iter = m_ObjectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& object : m_ObjectList)
	{
		if (object.pObj)
			object.pObj->LateUpdate_GameObject(fTimeDelta);
	}
}

//void CLayer::Render_Layer()
//{
//	for (auto& pObj : m_vObject)
//		pObj.pObj->Render_GameObject();
//}


CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for (auto& object : m_ObjectList)
		Safe_Release(object.pObj);
	m_ObjectList.clear();
}


