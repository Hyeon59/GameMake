struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0) //0 ~ 13
{
    matrix _world;
}

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
  
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    
    output.uv = input.uv;
    
    return output;
}

Texture2D sourceTex : register(t5);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = sourceTex.Sample(samp, input.uv);
    return color;
}