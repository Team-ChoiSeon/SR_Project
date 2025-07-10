#include "CModel.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCameraMgr.h"

CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice), m_uvScale{1.f,1.f,0.f,0.f}
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CModel::~CModel()
{
}

CModel* CModel::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CModel* instance = new CModel(pDevice);

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
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
		_vec4 tmp = { 1.f,1.f,0.f,0.f };
		if (m_uvScale != tmp){
			D3DXVECTOR4 transScale(scale.x, scale.y, 0.f, 0.f); // Z, W는 임시값
			shader->SetVector("g_UVScale", &transScale);
			//shader->SetVector("g_UVPosition", &uvPos);
		}
		else {
			shader->SetVector("g_UVScale", &m_uvScale);
		}
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

HRESULT CModel::Set_Model(const wstring& meshType, const wstring& matType)
{

	if (!meshType.empty()) {
		Safe_Change(m_pMesh, CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev,meshType));
	}

	if (!matType.empty()) {
		Safe_Change(m_pMaterial, CResourceMgr::Get_Instance()->Load_Material(matType));
	}

	return S_OK;
}

void CModel::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pMesh);
	Safe_Release(m_pMaterial);
	// Safe_Release(m_pTexture);
}
