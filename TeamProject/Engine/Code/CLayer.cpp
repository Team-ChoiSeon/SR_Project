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
	auto it = std::find_if(m_vObject.begin(), m_vObject.end(),
		[&](const OBJINFO& info) {
			return info.szName == wObjTag;
		});
	if (it == m_vObject.end())
	{
		MSG_BOX("[Layer] Add_GameObject ����: �ߺ� �±�");
		return;
	}

	m_vObject.push_back({ wObjTag, pGameObject });

	return;
}

HRESULT CLayer::Ready_Layer()
{
	for (auto& pObj : m_vObject)
	{
		pObj.pObj->Ready_GameObject();
	}
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto& pObj : m_vObject)
	{
		iResult = pObj.pObj->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_vObject)
		pObj.pObj->LateUpdate_GameObject(fTimeDelta);
}

void CLayer::Render_Layer()
{
	for (auto& pObj : m_vObject)
		pObj.pObj->Render_GameObject();
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
	for (auto& object : m_vObject)
		Safe_Release(object.pObj);
	m_vObject.clear();
}


