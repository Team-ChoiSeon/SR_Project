
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
    output.pos = input.pos; 
    output.uv = input.uv;
    return output;
}

// ===== 샘플러 정의 =====
sampler RenderTexture = sampler_state
{
    Texture = <g_RenderTex>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

// ===== 픽셀 셰이더 =====
float4 PS_Main(VS_OUT input) : COLOR0
{
   
    float4 baseColor = tex2D(RenderTexture, input.uv).rgba;
    
    return float4(baseColor.rgba);
}

// ===== 기법 정의 =====
technique PostProcessing
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_Main();
    }
}
