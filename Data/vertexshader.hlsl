
////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////
cbuffer MatricesBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


////////////////////////////////////////////////////////////////////////////////
// Input/Output data structures
////////////////////////////////////////////////////////////////////////////////
struct InputData
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct OutputData
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
OutputData Main(InputData input)
{
    OutputData output;

    //Set W component of vector to 1 for proper matrix calculations.
    input.position.w = 1.0f;

    output.position = input.position;
    output.color = input.color;

    return output;
}
