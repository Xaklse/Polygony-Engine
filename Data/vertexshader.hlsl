
////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////
cbuffer MatricesBuffer
{
    //Data is stored in row-major order.
    float4x4 projectionMatrix;
    float4x4 viewMatrix;
    float4x4 worldMatrix;
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

    //Apply vertex transformations.
    output.position = mul(worldMatrix,input.position);
    output.position = mul(viewMatrix,output.position);
    output.position = mul(projectionMatrix,output.position);

    output.color = input.color;

    return output;
}
