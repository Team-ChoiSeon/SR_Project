#pragma once
#include "Engine_Typedef.h"

namespace Engine {

	typedef struct tagVertexColor
	{
		_vec3 vPosition;
		_ulong dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture {
		_vec3 vPosition;
		_vec2 vTexUV;
	}VTXTEX;
	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;


	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)

	typedef struct tagVertexLine
	{
		D3DXVECTOR3 vPos;
		D3DCOLOR    color;
	}VTXLINE;

	const _ulong	FVF_LINE(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;

	struct Ray {
		_vec3 _position;
		_vec3 _direction;
	};

	class CGameObject;
	typedef struct Ray_HitInfo {
		CGameObject*	_hittedobject;
		LAYERID			_objectlayer;
		float			_distance;
		_vec2			_hitteduv;
		_vec3			_hittedcoord;
		_vec3			_hittedcoordnormal;
	}Ray_Hit;

	typedef struct Ray_BoundTri {
		_vec3 _v0, _v1,_v2;
		_vec3 _nv0, _nv1, _nv2;
	}Ray_Tri;
  
	typedef struct  tagAABB
	{
		_vec3 vMin;
		_vec3 vMax;
	}AABB;
}