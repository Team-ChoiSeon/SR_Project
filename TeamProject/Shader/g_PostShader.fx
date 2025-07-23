
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
}
