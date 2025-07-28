// ===== 상수 정의 =====
float4x4 g_matWorld;
float4x4 g_matView;
float4x4 g_matProj;

float g_Alpha = 1.f;
texture g_DiffuseTex;

float4 g_UVScale = float4(1.0f, 1.0f, 0, 0); // xy만 사용
float2 g_Ratio;

// ===== 정점 구조체 =====
struct VS_IN
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

// ===== 정점 셰이더 =====
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    float4x4 g_matWorldViewProj = mul(mul(g_matWorld, g_matView), g_matProj);
    output.pos = mul(input.pos, g_matWorldViewProj);
    output.uv = input.uv; //슬라이스 용도

    return output;
}

// ===== 샘플러 정의 =====
sampler DiffuseSampler = sampler_state
{
    Texture = <g_DiffuseTex>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

// ===== 픽셀 셰이더 =====
float4 PS_Main(VS_OUT input) : COLOR0
{
    if (input.uv.x >  g_Ratio.x)
        discard;
    if (input.uv.y > g_Ratio.y)
    discard;
    
    float4 baseColor = tex2D(DiffuseSampler, input.uv).rgba;
    
    return float4(baseColor.rgb, baseColor.a * g_Alpha);
}

// ===== 기법 정의 =====
technique Orthograph
{
    pass P0
    {
        CullMode = NONE; // 컬링 끄기
        ZWriteEnable = FALSE; // Z버퍼 쓰기 끄기 (순서 문제 방지)
        AlphaBlendEnable = TRUE;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_Main();
    }
}
