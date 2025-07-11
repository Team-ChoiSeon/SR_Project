#include "pch.h"
#include"Engine_Define.h"
#include "CBlock.h"
#include "CTransform.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidBody.h"

CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pModel(nullptr), m_pTransform(nullptr),m_pCollider(nullptr),m_pRigid(nullptr)
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CTransform>());
    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CRigidBody>());

    m_pTransform = Get_Component<CTransform>();
    m_pModel = Get_Component<CModel>();
    m_pRigid = Get_Component<CRigidBody>();
    m_pCollider = Get_Component<CCollider>();

}

CBlock::~CBlock()
{
}
