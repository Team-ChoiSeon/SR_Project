#include "pch.h"
#include "Engine_Define.h"
#include "UIPanel.h"

UIPanel::UIPanel()
{
	
}

UIPanel::UIPanel(_vec2 pos, _vec2 size)
	:	vPos(pos), vSize(size)
{
}

UIPanel::~UIPanel()
{
}

_bool UIPanel::Open_While(_vec2 targetSize, _float dt)
{
	if (vSize.x < targetSize.x)
		vSize.x += dt;
	if (vSize.y < targetSize.y)
		vSize.y += dt;

	if (vSize.x > targetSize.x)
		vSize.x = targetSize.x;
	if (vSize.y > targetSize.y)
		vSize.y = targetSize.y;

	return (vSize.x >= targetSize.x && vSize.y >= targetSize.y);
}

_bool UIPanel::Close_While(_vec2 targetSize, _float dt)
{
	if (vSize.x > targetSize.x)
		vSize.x -= dt;
	if (vSize.y > targetSize.y)
		vSize.y -= dt;

	if (vSize.x < targetSize.x)
		vSize.x = targetSize.x;
	if (vSize.y < targetSize.y)
		vSize.y = targetSize.y;

	return (vSize.x <= targetSize.x && vSize.y <= targetSize.y);
}

RECT& UIPanel::Get_ByRect()
{

	m_tRect.left = static_cast<LONG>(LT().x);
	m_tRect.top = static_cast<LONG>(LT().y);
	m_tRect.right = static_cast<LONG>(RB().x);
	m_tRect.bottom = static_cast<LONG>(RB().y);

	return m_tRect;
}

void UIPanel::Set_Anchor(Anchor _anchor, _vec2 _pivot)
{
	switch (_anchor)
	{
	case Anchor::Left:
		vPos.x = _pivot.x + vSize.x * 0.5f;
		break;
	case Anchor::Right:
		vPos.x = _pivot.x - vSize.x * 0.5f;
		break;
	case Anchor::Top:
		vPos.y = _pivot.y+ vSize.y * 0.5f;
		break;
	case Anchor::Bottom:
		vPos.y = _pivot.y - vSize.y * 0.5f;
		break;
	case Anchor::Center:
		vPos.x = _pivot.x;
		vPos.y = _pivot.y;
		break;	
	case Anchor::LeftCenter:
		vPos.x = _pivot.x + vSize.x * 0.5f;
		vPos.y = _pivot.y;
		break;
	case Anchor::RightCenter:
		vPos.x = _pivot.x - vSize.x * 0.5f;
		vPos.y = _pivot.y;
		break;	
	case Anchor::LeftTop:
		vPos.x = _pivot.x + vSize.x * 0.5f;
		vPos.y = _pivot.y + vSize.y * 0.5f;
		break;	
	case Anchor::TopCenter:
		vPos.x = _pivot.x ;
		vPos.y = _pivot.y + vSize.y * 0.5f;
		break;
	default:
		vPos.x = _pivot.x;
		vPos.y = _pivot.y;
		break;
	}
}
