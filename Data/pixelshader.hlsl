
////////////////////////////////////////////////////////////////////////////////
// Input/Output data structures
////////////////////////////////////////////////////////////////////////////////
struct InputData
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 Main(InputData input) : SV_TARGET
{
    return input.color;
}
