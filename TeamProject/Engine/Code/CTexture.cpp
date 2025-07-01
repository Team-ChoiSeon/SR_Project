#include "CTexture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResource(pGraphicDev)
{
}

CTexture::~CTexture()
{
}

void CTexture::Free()
{
	CResource::Free();
}
