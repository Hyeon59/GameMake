struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float2 tileUV : TEXCOORD1;
    float4 color : COLOR0;
    float4 redcell : COLOR1;
    uint index : INDEX;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float2 tileUV : TEXCOORD1;
    float4 color : COLOR0;
    float4 redcell : COLOR1;
    uint index : INDEX;
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
    output.color = input.color;
    output.tileUV = input.tileUV;
    output.index = input.index;
    output.redcell = input.redcell;
    
    return output;
}

cbuffer TileBuffer : register(b0)
{
    uint _tileIndex;
    uint _bRedcell;
    //float3 _dummy
}

Texture2D sourceTex : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 result = 0;
    float4 color = sourceTex.Sample(samp, input.uv);
    if(input.uv.x > 0 || input.uv.y > 0)
        result = color;
    else
        result = float4(input.color.rgb, 0);
    
    if(_bRedcell != 0)
        result = saturate(result + input.redcell);
    
    if(_tileIndex == input.index)
    {
        if(input.tileUV.x < 0.1f
            || input.tileUV.x > 0.9f
            || input.tileUV.y < 0.1f
            || input.tileUV.y > 0.9f)
            result = float4(0, 1, 0, 1);
    }
    
    return result;
}