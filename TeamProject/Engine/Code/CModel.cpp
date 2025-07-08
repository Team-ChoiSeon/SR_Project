#include "CModel.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCameraMgr.h"

CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CModel::~CModel()
{
}

CModel* CModel::Create(LPDIRECT3DDEVICE9 pDevice, const DefaultCubeModel& model)
{
	auto pMesh = CResourceMgr::Get_Instance()->Get_Mesh(model.meshKey);
	auto pMaterial = CResourceMgr::Get_Instance()->Get_Material(model.materialKey);

	if (!pMesh || !pMaterial) {
		MSG_BOX("CModel::Create - Resource Missing");
		return nullptr;
	}
	auto pModel = new CModel(pDevice);
	pModel->Set_Mesh(pMesh);
	pModel->Set_Material(pMaterial);
	pMaterial->Set_Shader(L"g_UVScale.fx"); //임시

	OutputDebugString("[CModel] Create 호출됨\n");
	return pModel;
}

CModel* CModel::Create(LPDIRECT3DDEVICE9 pDevice, const DefaultTileModel& model)
{
	auto pMesh = CResourceMgr::Get_Instance()->Get_Mesh(model.meshKey);
	auto pMaterial = CResourceMgr::Get_Instance()->Get_Material(model.materialKey);

	if (!pMesh || !pMaterial) {
		MSG_BOX("CModel::Create - Resource Missing");
		return nullptr;
	}
	auto pModel = new CModel(pDevice);
	pModel->Set_Mesh(pMesh);
	pModel->Set_Material(pMaterial);
	OutputDebugString("[CModel] Create 호출됨\n");
	return pModel;
}

void CModel::LateUpdate_Component()
{
	OutputDebugString("[CModel] LateUpdate_Component 호출됨\n");
	CRenderMgr::Get_Instance()->Add_Model(this);
	OutputDebugString("[CModel] 등록됨\n");
}

void CModel::Render(LPDIRECT3DDEVICE9 m_pDevice)
{
	if (!m_pMesh)
	{
		OutputDebugString("[CModel] m_pMesh is nullptr\n");
		return;
	}
	if (!m_pMaterial)
	{
		OutputDebugString("[CModel] m_pMaterial is nullptr\n");
		return;
	}

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
	if (pTransform == nullptr)
	{
		MSG_BOX("CModel::Render : pTransform is nullptr");
		return;
	}

	m_pDevice->SetTransform(D3DTS_WORLD, pTransform->Get_WorldMatrix());

	if (!m_pMaterial)
		return;

	LPD3DXEFFECT shader = m_pMaterial->Get_Effect();

	UINT passCount = 0;
	m_pMaterial->Apply(m_pDevice); // 내부에서 텍스처를 Bind

	if (shader)
	{
		const D3DXVECTOR3& scale = pTransform->Get_Scale();

		D3DXMATRIX world = *(pTransform->Get_WorldMatrix());
		D3DXMATRIX view = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());
		D3DXMATRIX proj = *(CCameraMgr::Get_Instance()->Get_MainProjectionMatrix());

		D3DXMATRIX wvp = world * view * proj;
		shader->SetMatrix("g_matWorldViewProj", &wvp);

		D3DXVECTOR4 uvScale(
			scale.x * 3.f,  
			scale.z * 3.f,
			0.f, 0.f);
		shader->SetVector("g_UVScale", &uvScale);
	}

	if (shader) {
		shader->Begin(&passCount, 0);
		for (UINT i = 0; i < passCount; ++i) {
			shader->BeginPass(i);
			m_pMesh->Render_Buffer();
			shader->EndPass();
		}
		shader->End();
	}
	else {
		m_pMesh->Render_Buffer();
	}
}

void CModel::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pMesh);
	Safe_Release(m_pMaterial);
	// Safe_Release(m_pTexture);
}
