cbuffer camera : register(b0) {
    float4x4 _project_matrix;
    float4x4 _view_matrix;
};
cbuffer object : register(b1) {
    float4x4 _world_matrix;
}

struct vertex_in {
    float3 position : POSITION;
    float2 texcoord : TEXCOORD0;
};
struct vertex_out {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

vertex_out main(vertex_in _in)
{
    vertex_out _out;
    _out.position = float4(_in.position, 1.f);
    _out.texcoord = _in.texcoord;
    //Out.vPos = mul(float4(In.vPos, 1.f), g_WorldMatrix);
    //Out.vPos = mul(Out.vPos, g_ViewMatrix);
    //Out.vPos = mul(Out.vPos, g_ProjMatrix);

    //Out.vTexCoord = In.vTexCoord;
    return _out;
}