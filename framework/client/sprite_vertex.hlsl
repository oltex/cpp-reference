#include "camera.hlsli"

cbuffer object : register(b1) {
    float4x4 _world_matrix;
}
struct vertex_in {
    float3 _position : POSITION;
    float2 _texcoord : TEXCOORD0;
};
struct vertex_out {
    float4 _position : SV_POSITION;
    float2 _texcoord : TEXCOORD0;
};
vertex_out main(vertex_in _in) {
    vertex_out _out;
    //_out._position = float4(_in._position, 1.f);
    _out._position = mul(float4(_in._position, 1.f), _world_matrix);
    _out._position = mul(_out._position, _view_matrix);
    _out._position = mul(_out._position, _project_matrix);
    _out._texcoord = _in._texcoord;
    return _out;
}




//struct vertex_in {
//    float3 _position : POSITION;
//    float3 _texcoord : TEXCOORD0;
//};
//struct vertex_out {
//    float4 _position : SV_POSITION;
//    float3 _texcoord : TEXCOORD0;
//};
//vertex_out vertex_main(vertex_in _in) {
//    VS_OUT Out = (VS_OUT) 0;
//    Out.vPos = mul(float4(In.vPos, 1.f), g_WorldMatrix);
//    Out.vPos = mul(Out.vPos, g_ViewMatrix);
//    Out.vPos = mul(Out.vPos, g_ProjMatrix);
//    Out.vTexCoord = In.vTexCoord;
//    return Out;
//}