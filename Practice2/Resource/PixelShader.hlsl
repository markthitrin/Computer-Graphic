struct VS_INPUT
{
    float4 inPosition : SV_Position;
    float3 inColor : COLOR;
};

float4 main(VS_INPUT input) : SV_Target {
    return float4(input.inColor, 1.0f);
}