#include "CMesh.h"
#include "CFactory.h"
#include "CGraphicDev.h"
CMesh::CMesh() : m_pVB(nullptr), m_pIB(nullptr),
m_dwVtxSize(0), m_dwTriCnt(0), m_dwVtxCnt(0), m_dwFVF(0), m_dwIdxSize(0)
{
}

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
    : CResource(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr),
    m_dwVtxSize(0), m_dwTriCnt(0), m_dwVtxCnt(0), m_dwFVF(0), m_dwIdxSize(0)
{
}

CMesh::CMesh(const CMesh& rhs)
    : CResource(rhs), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB),
    m_dwVtxSize(rhs.m_dwVtxSize), m_dwTriCnt(rhs.m_dwTriCnt),
    m_dwVtxCnt(rhs.m_dwVtxCnt), m_dwFVF(rhs.m_dwFVF),
    m_dwIdxSize(rhs.m_dwIdxSize), m_IdxFmt(rhs.m_IdxFmt)
{
    if (m_pVB) m_pVB->AddRef();
    if (m_pIB) m_pIB->AddRef();
}

HRESULT CMesh::Ready_Mesh()
{
	m_pGraphicDev = CGraphicDev::Get_Instance()->Get_GraphicDev();
	return S_OK;
}

HRESULT CMesh::LoadOBJ(LPDIRECT3DDEVICE9 pDevice, const wstring& path)
{
	//wstring basePath = L"../Bin/Resource/Obj/";

	//파일 열기
	//CFactory::
	string basePath = CFactory::ToString(path); 
	ifstream in(basePath);
	if (!in.is_open()) {
		return E_FAIL;
	}

	vector<D3DXVECTOR3> positions;
	vector<D3DXVECTOR3> normals;
	vector<D3DXVECTOR2> texcoords;
	vector<VTXTILE>    vertices;
	vector<DWORD>        indices;

	std::string line;
	while (std::getline(in, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v") {
			D3DXVECTOR3 pos;
			iss >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}
		else if (prefix == "vt") {
			D3DXVECTOR2 uv;
			iss >> uv.x >> uv.y;
			uv.y = 1.0f - uv.y; // Flip V (Blender 기준)
			texcoords.push_back(uv);
		}
		else if (prefix == "vn") {
			D3DXVECTOR3 nv;
			iss >> nv.x >> nv.y >> nv.z;
			normals.push_back(nv);
		}
		else if (prefix == "f") {
			std::vector<std::string> tokens;
			std::string token;
			while (iss >> token)
				tokens.push_back(token);

			// 삼각형 분할 (fan 방식)
			for (size_t i = 1; i + 1 < tokens.size(); ++i) {
				std::string vStr[3] = { tokens[0], tokens[i], tokens[i + 1] };

				for (int k = 0; k < 3; ++k) {
					std::istringstream vstream(vStr[k]);
					std::string viStr, vtiStr, vniStr;
					std::getline(vstream, viStr, '/');
					std::getline(vstream, vtiStr, '/');
					std::getline(vstream, vniStr, '/');

					int vi = std::stoi(viStr) - 1;
					int vti = std::stoi(vtiStr) - 1;
					int vni = std::stoi(vniStr) - 1;

					VTXTILE vertex;
					// 오브젝트의 Transform에서 스케일 가져오기
					//D3DXVECTOR3 scale = m_pOwner.->Get_Scale();

					// UV 스케일을 비례해서 설정
					//D3DXVECTOR2 uvScale = { scale.x, scale.y };
					vertex.vTexUV.x = texcoords[vti].x;
					vertex.vTexUV.y = texcoords[vti].y;

					vertex.vPosition = positions[vi];
					vertex.vNormal = normals[vni];

					vertices.push_back(vertex);
					indices.push_back(static_cast<DWORD>(vertices.size() - 1));
				}
			}
		}
	}

	// 정점 버퍼 생성

	m_dwFVF = FVF_TILE;
	m_dwVtxSize = sizeof(VTXTILE);
	m_dwTriCnt = indices.size() / 3;
	m_dwVtxCnt = vertices.size();

	if (FAILED(pDevice->CreateVertexBuffer(
		vertices.size() * m_dwVtxSize,
		0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0))) {
		return E_FAIL;
	}

	void* pVBData = nullptr;
	m_pVB->Lock(0, 0, &pVBData, 0);
	memcpy(pVBData, vertices.data(), vertices.size() * m_dwVtxSize);
	m_pVB->Unlock();

	// 인덱스 버퍼 생성
	if (FAILED(pDevice->CreateIndexBuffer(
		indices.size() * sizeof(DWORD),
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0))) {
		return E_FAIL;
	}

	void* pIBData = nullptr;
	m_pIB->Lock(0, 0, &pIBData, 0);
	memcpy(pIBData, indices.data(), indices.size() * sizeof(DWORD));
	m_pIB->Unlock();

	//m_wKey = path;

	return S_OK;
}

CMesh::~CMesh()
{
}

CMesh* CMesh::Create()
{
	CMesh* instance = new CMesh;

	if (FAILED(instance->Ready_Mesh())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CMesh::Ready_Buffer()
{

    if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 버텍스 버퍼 크기
        0,      // 버퍼 속성(0인 경우 정적 버퍼)
        m_dwFVF,    // 버텍스 속성
        D3DPOOL_MANAGED, // 정적 버퍼일 경우 MANAGED
        &m_pVB, // 생성한 버텍스 버퍼를 정보를 저장할 객체 주소
        NULL)))
    {
        MSG_BOX("VertexBuffer 생성 실패");
        return E_FAIL;
    }

    if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, // 인덱스 버퍼 크기
        0,      // 버퍼 속성(0인 경우 정적 버퍼)
        m_IdxFmt,    // 인덱스 속성
        D3DPOOL_MANAGED, // 정적 버퍼일 경우 MANAGED
        &m_pIB, // 생성한 인덱스 버퍼를 정보를 저장할 객체 주소
        NULL)))
    {
        MSG_BOX("IndexBuffer 생성 실패");
        return E_FAIL;
    }
	m_wKey = L"DirtObj.obj";
    return S_OK;
}

void CMesh::Render_Buffer()
{
    m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
    m_pGraphicDev->SetFVF(m_dwFVF);
    m_pGraphicDev->SetIndices(m_pIB);
    m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}


void CMesh::Free()
{
    Safe_Release(m_pVB);
    Safe_Release(m_pIB);
    CResource::Free();
}