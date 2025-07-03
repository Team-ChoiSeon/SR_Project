#pragma once
#include "Engine_Define.h"
#include "CResourceMgr.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CModel.h"

struct DefaultCubeModel
{
	// ����
	wstring meshKey = L"CCubeTex";
	// wstring textureKey = L"texture.jpg";
	wstring materialKey = L"brick.mtl";
};

struct MeshKey { const wstring& value; };
// struct TextureKey { const wstring& value; };
struct MaterialKey { const wstring& value; };

template <typename TArg>
void ApplyArg(DefaultCubeModel& model, TArg arg)
{    
	// �⺻�� �ƹ� �͵� ���� ���� 
}

template <>
void ApplyArg(DefaultCubeModel & model, MeshKey arg)
{
	model.meshKey = arg.value;
}

//template <>
//void ApplyArg(DefaultCubeModel & model, TextureKey arg)
//{
//	model.textureKey = arg.value;
//}

template <>
void ApplyArg(DefaultCubeModel & model, MaterialKey arg)
{
	model.materialKey = arg.value;
}

