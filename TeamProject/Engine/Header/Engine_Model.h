#pragma once
#include "Engine_Define.h"
/*
#include "CResourceMgr.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CModel.h"
*/

BEGIN(Engine)

struct ENGINE_DLL DefaultCubeModel
{
    std::wstring meshKey = L"CCubeTex";
    std::wstring materialKey = L"DirtObj.mtl";
};

struct ENGINE_DLL MeshKey { const std::wstring& value; };
struct ENGINE_DLL MaterialKey { const std::wstring& value; };

template <typename TArg>
inline void ApplyArg(DefaultCubeModel& model, TArg arg)
{
    // 기본적으로 아무 작업 없음
}

template <>
inline void ApplyArg(DefaultCubeModel& model, MeshKey arg)
{
    model.meshKey = arg.value;
}

template <>
inline void ApplyArg(DefaultCubeModel& model, MaterialKey arg)
{
    model.materialKey = arg.value;
}

END
