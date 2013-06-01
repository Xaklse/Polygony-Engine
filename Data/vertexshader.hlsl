struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut Main(float4 position : POSITION,float4 color : COLOR)
{
    VSOut output;

    output.position = position;
    output.color = color;

    return output;
}