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
		MSG_BOX("[Layer] Add_GameObject ���� : nullptr ���޵�");
		return;
	}

	// �̹� ���� �±װ� �����ϴ� ��� ����� �ʱ�
	if (m_umObject.find(wObjTag) != m_umObject.end())
	{
		MSG_BOX("[Layer] Add_GameObject ����: �ߺ� �±�");
		return;
	}

	m_umObject.emplace(wObjTag, pGameObject);

	return;
}

HRESULT CLayer::Ready_Layer()
{
	for (auto& pObj : m_umObject)
	{
		pObj.second->Ready_GameObject();
	}
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto& pObj : m_umObject)
	{
		iResult = pObj.second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_umObject)
		pObj.second->LateUpdate_GameObject(fTimeDelta);
}

void CLayer::Render_Layer()
{
	for (auto& pObj : m_umObject)
		pObj.second->Render_GameObject();
}


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
	for (auto& object : m_umObject)
		Safe_Release(object.second);
	m_umObject.clear();
}


