
float4x4 g_matIdentity;
texture g_SceneTex;

float g_Time;
float g_EffectTime;
float g_TotalTime;

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
    output.pos = mul(input.pos, g_matIdentity); //어차피 화면에 그리니께
    output.uv = input.uv;
    
    return output;
}

// ===== 샘플러 정의 =====
sampler SceneSampler = sampler_state
{
    Texture = <g_SceneTex>;
    MinFilter = Point; //이거 안하면 화질 깨짐
    MagFilter = Point;
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler SceneSampler_Wrap = sampler_state
{
    Texture = <g_SceneTex>;
    MinFilter = Point; //이거 안하면 화질 깨짐
    MagFilter = Point;
    MipFilter = None;
    AddressU = Wrap;
    AddressV = Wrap;
};

float hash(float2 p)
{
    // 난수 같이 보이는 것인듯?
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}


// ===== 픽셀 셰이더 =====
float4 PS_HIT(VS_OUT input) : COLOR0
{
    float2 uv = input.uv;

    //좌우로 노이즈 줌.
    float offset = hash(float2(g_Time * 20.0, uv.y)) * 0.09 * g_EffectTime; //타임 점점 줄어들거임
    uv.x += offset;
    
    float4 base = tex2D(SceneSampler, uv);
    return base;
}

float4 PS_DEAD(VS_OUT input) : COLOR0
{
    float2 uv = input.uv;
    float offset = hash(float2(g_Time * 10.0, uv.y)) * 0.09 * g_EffectTime; //타임 점점 줄어들거임
    uv.x += offset;
    
    float hTime = g_TotalTime * 0.5f;
    
    float4 base = tex2D(SceneSampler, uv);
    base.rgb *= (g_EffectTime - hTime);
    return base;
}

float4 PS_ALIVE(VS_OUT input) : COLOR0
{
    
    float2 uv = input.uv;
    
    float hTime = g_TotalTime * 0.5f;
    if (g_EffectTime < hTime)
    {
        float offset = hash(float2(g_Time * 10.0, uv.y)) * 0.09 * g_EffectTime; //타임 점점 줄어들거임
        uv.x += offset;
    }
    
    float4 base = tex2D(SceneSampler, uv);
    base.rgb *= (hTime - g_EffectTime);
    return base;
}

float4 PS_ASSEMBLE(VS_OUT input) : COLOR0
{
    float2 uv = input.uv;

    // 블록 크기
    float blockSize = 0.05f;

    // 몇 번째 블럭에 속하는지 인덱스 계산
    int2 blockIndex = int2(uv / blockSize);

    // 블록별 시간차용 키값 (ex: 대각선)
    int blockKey = blockIndex.x + blockIndex.y;

    // 타이밍 오프셋: 블럭마다 시간차 반영
    float localTime = g_EffectTime - (blockKey * 0.05f);

    // 클램핑
    localTime = saturate(localTime); // 0~1

    // 처음에는 제자리였다가 점점 퍼져나감 (localTime 커질수록 멀어짐)
    float2 offset;
    offset.x = (blockIndex.x % 2 == 0 ? 1 : -1) * localTime * 0.1;
    offset.y = (blockIndex.y % 2 == 0 ? 1 : -1) * localTime * 0.1;

    float2 displacedUV = uv + offset;

    float4 base = tex2D(SceneSampler_Wrap, displacedUV);
    return base;
}



float4 PS_DISASSEMBLE(VS_OUT input) : COLOR0
{
    float2 uv = input.uv;

    float blockWidth = 0.03f;
    int xIndex = int(uv.x / blockWidth); // 0~4
    int yIndex = int(uv.y / blockWidth); // 0~4
    float hTime = g_TotalTime * 0.5f;
    int dir = 1;

    if (xIndex % 2 == 1)
    {
        uv.y += blockWidth * g_EffectTime * dir;
    }
    if (yIndex % 2 == 1)
    {
        uv.x += blockWidth * g_EffectTime * dir;
    }
    float4 base = tex2D(SceneSampler_Wrap, uv);
    return base;
}

// ===== 기법 정의 =====
technique PostProcessing
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_HIT();
    }

    pass P1
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_DEAD();
    }
    pass P2
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_ALIVE();
    }
    pass P3
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_ASSEMBLE();
    }
    pass P4
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_DISASSEMBLE();
    }
}
