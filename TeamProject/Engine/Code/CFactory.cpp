#pragma once
#include "json.hpp"
#include "CFactory.h"

using json = nlohmann::json;

void CFactory::Register(const wstring& className, CreatorFunc func)
{
    Get_Map()[className] = func;
}

CGameObject* CFactory::Create(const wstring& className, LPDIRECT3DDEVICE9 pGraphicDev)
{
    auto it = Get_Map().find(className);
    if (it != Get_Map().end())
        return it->second(pGraphicDev); // 함수 호출
    return nullptr;
}

unordered_map<wstring, CreatorFunc>& CFactory::Get_Map()
{
    static unordered_map<wstring, CreatorFunc> umap;
    return umap;
}