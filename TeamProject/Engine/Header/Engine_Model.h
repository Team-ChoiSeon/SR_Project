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
    std::wstring meshKey = L"DirtObj.obj";
    std::wstring materialKey = L"DirtObj.mtl";
};

struct ENGINE_DLL DefaultTileModel
{
    std::wstring meshKey = L"BrickRoad.obj";
    std::wstring materialKey = L"BrickRoad.mtl";
};

struct ENGINE_DLL MeshKey { const std::wstring& value; };
struct ENGINE_DLL MaterialKey { const std::wstring& value; };

//=============================
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


//=============================
template <typename TArg>
inline void ApplyArg(DefaultTileModel& model, TArg) {}

template <>
inline void ApplyArg(DefaultTileModel& model, MeshKey arg) {
    model.meshKey = arg.value;
}
template <>
inline void ApplyArg(DefaultTileModel& model, MaterialKey arg) {
    model.materialKey = arg.value;
}
END
