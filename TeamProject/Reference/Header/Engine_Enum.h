#pragma once
namespace Engine {
	enum WINMODE { MODE_FULL, MODE_WIN };
	
	enum COMPONENTID {ID_DYNAMIC, ID_STATIC, ID_END};
	
	enum INFO {INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS,INFO_END};
	enum ROTATION {ROT_X,ROT_Y,ROT_Z, ROT_END};

	enum TEXTUREID{TEX_NORMAL, TEX_CUBE, TEX_END};
	enum RENDERID {RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI,RENDER_END};
	
	enum MOUSEKEYSTATE{DIM_LB,DIM_RB,DIM_MB,DIM_END};
	enum MOUSEMOVESTATE{DIMS_X,DIMD_Y,DIMD_Z,DIMS_END};
	
	enum class RENDER_PASS
	{
		RP_SKYBOX,        // // ��ī�̹ڽ� (Z-write ����, ���� ����)
		RP_SHADOW,        // �׸��� ���� �����ϴ� �н� (��: Depth-Only ������ �� ShadowMap ������)
		RP_OPAQUE,        // ������ ������Ʈ ������ (Z-Buffer ���, �Ϲ����� Mesh��)
		RP_STENCIL,   // ������ ������Ʈ ������ (Z-Buffer ���� �ʿ�, ���� ���� ���)
		RP_TRANSPARENT,   // ������ ������Ʈ ������ (Z-Buffer ���� �ʿ�, ���� ���� ���)
		RP_UI,            // 2D UI ��� ������ (HUD, �ؽ�Ʈ ��, ���� ������ ������� ���)
		RP_POSTPROCESS,   // ��ó�� �ܰ� (Bloom, Blur, Color Correction �� �� FullScreen Quad ���)
		RP_DEBUG,   // ����׿� ��� ǥ�õ�
		RP_END            // enum �� ǥ�� (�迭 ũ�� ���� �� ���� �뵵)
	};

}