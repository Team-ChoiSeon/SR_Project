#pragma once
#include "pch.h"
#include "CVellum.h"

#include "CInputMgr.h"

CVellum::CVellum(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CVellum::~CVellum()
{
}

CVellum* CVellum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVellum* pVellum = new CVellum(pGraphicDev);

	if (FAILED(pVellum->Ready_GameObject()))
	{
		Safe_Release(pVellum);
		MSG_BOX("Vellum Create Failed");
		return nullptr;
	}

	return pVellum;
}

HRESULT CVellum::Ready_GameObject()
{
    m_vPart.reserve(m_iPartCnt);

    CMonsterPart* pPrev = nullptr;

    for (int i = 0; i < m_iPartCnt; ++i)
    {
        CMonsterPart* pPart = CMonsterPart::Create(m_pGraphicDev);
        if (!pPart)
        {
            MSG_BOX("Vellum 파츠 생성 실패");
            return E_FAIL;
        }

        // 파츠 위치 초기화 (선형 배열 형태)
        _vec3 partPos = { 0.f, - 2.f * i, 0.f };
        pPart->Get_Component<CTransform>()->Set_Pos(partPos);

        // 앞 파츠를 따라가게 연결
        if (pPrev)
            pPart->Set_Target(pPrev);

        m_vPart.push_back(pPart);
        pPrev = pPart;
    }
	return CGameObject::Ready_GameObject();;
}

int CVellum::Update_GameObject(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);
	CGameObject::Update_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
        pPart->Update_GameObject(fTimeDelta);
	return 0;
}

void CVellum::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
        pPart->LateUpdate_GameObject(fTimeDelta);
}

void CVellum::Key_Input(const _float& fTimeDelta)
{
    if (m_vPart.empty())
        return;

    CTransform* pHeadTransform = m_vPart[0]->Get_Component<CTransform>();
    if (!pHeadTransform)
        return;

    const float speed = 10.0f;
    _vec3 pos = pHeadTransform->Get_Pos();

    if (CInputMgr::Get_Instance()->Key_Down(DIK_UP))
        pHeadTransform->Move(DIR_UP, speed, fTimeDelta);

    if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
        pHeadTransform->Move(DIR_DOWN, speed, fTimeDelta);

    if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
        pHeadTransform->Move(DIR_LEFT, speed, fTimeDelta);

    if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
        pHeadTransform->Move(DIR_RIGHT, speed, fTimeDelta);

    // 넘버패드숫자키 1~6 (x/y/z축 직접 이동)
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD1)) // +X
        pos.x += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD2)) // -X
        pos.x -= speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD3)) // +Y
        pos.y += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD4)) // -Y
        pos.y -= speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD5)) // +Z
        pos.z += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD6)) // -Z
        pos.z -= speed * fTimeDelta;

    pHeadTransform->Set_Pos(pos); // 적용
}

void CVellum::Free()
{
    for (auto* pPart : m_vPart)
        Safe_Release(pPart);


    m_vPart.clear();

    CGameObject::Free();

}
