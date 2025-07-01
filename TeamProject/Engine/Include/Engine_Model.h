#pragma once
#include "Engine_Define.h"
#include "CResourceMgr.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CModel.h"

struct CubeModel
{
	wstring meshKey = L"CCubeTex";
	wstring textureKey = L"brick.png";
	wstring materialKey = L"metal";
};

struct MeshKey { wstring value; };
struct TextureKey { wstring value; };
struct MaterialKey { wstring value; };

template <typename TArg> void ApplyArg(DefaultCubeModel& model, TArg arg)
{    
	// 기본은 아무 것도 하지 않음 
}
template <> void ApplyArg(DefaultCubeModel & model, MeshKey arg)
{
	model.meshKey = arg.value;
}
template <> void ApplyArg(DefaultCubeModel & model, TextureKey arg)
{
	model.textureKey = arg.value;
}
template <> void ApplyArg(DefaultCubeModel & model, MaterialKey arg)
{
	model.materialKey = arg.value;
}

