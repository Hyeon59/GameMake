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

Texture2D sourceTex : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = sourceTex.Sample(samp, input.uv);
   
    float3 greyScale = 0;
    
    //greyScale = (color.r + color.g + color.b) / 3.0f;
    //greyScale = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
    
    //Sepia
    greyScale.r = dot(color.rgb, float3(0.393f, 0.796f, 0.189f));
    // greyScale.r = color.r * 0.393f + color.g * 0.796f + color.b * 0.189f;
    greyScale.g = dot(color.rgb, float3(0.349f, 0.686f, 0.168f));
    greyScale.b = dot(color.rgb, float3(0.272f, 0.534f, 0.131f));
    
    if(input.uv.x < 0.01f || input.uv.x > 0.99f ||
        input.uv.y < 0.01f || input.uv.y > 0.99f)
        greyScale = float4(1, 1, 1, 1);
    
    return float4(greyScale, 1);
}