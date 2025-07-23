#pragma once

class UIPanel
{
public:
	UIPanel();
	UIPanel(_vec2 pos, _vec2 size);
	~UIPanel();

public:
	void Set_Pos(_vec2 pos) { vPos = pos; };
	void Set_Size(_vec2 size) { vSize = size; };
	void Add_Size(_vec2 size) { vSize += size; };

	_bool Open_While(_vec2 targetSize, _float dt);
	_bool Close_While(_vec2 targetSize, _float dt);

	_vec2 Get_Pos() { return vPos; };
	_vec2 Get_Size() { return vSize; };
	_vec2 Get_hSize() { return vSize*0.5f; };
	RECT& Get_ByRect();
public:
	_vec2 LT()	const { return _vec2(vPos.x - (0.5f * vSize.x), vPos.y - (0.5f * vSize.y)); }
	_vec2 LC()	const { return _vec2(vPos.x - (0.5f * vSize.x), vPos.y ); }
	_vec2 LB()	const { return _vec2(vPos.x - (0.5f * vSize.x), vPos.y + (0.5f * vSize.y)); }
	_vec2 RT()	const { return _vec2(vPos.x + (0.5f * vSize.x), vPos.y - (0.5f * vSize.y)); }
	_vec2 RC()	const { return _vec2(vPos.x + (0.5f * vSize.x), vPos.y ); }
	_vec2 RB()	const { return _vec2(vPos.x + (0.5f * vSize.x), vPos.y + (0.5f * vSize.y)); }

	enum class Anchor { Left, Right, Top, Bottom, Center, LeftCenter,RightCenter,TopCenter,BottomCenter,LeftTop};
	void Set_Anchor(Anchor _anchor, _vec2 _pivot);

	_vec3 Get_WorldPos(_long _wincx, _long _wincy) {
		return _vec3(vPos.x - _wincx * 0.5f, -vPos.y + _wincy * 0.5f, 0.f);
	}
	_vec3 Get_WorldScale() {
		return _vec3(vSize.x * 0.5f, vSize.y * 0.5f, 1.f);
	}

private:
	_vec2 vPos;
	_vec2 vSize;
	RECT m_tRect;
};


