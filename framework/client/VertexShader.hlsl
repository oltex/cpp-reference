struct VS_IN {
    float3 position : POSITION;
};
struct VS_OUT {
    float4 position : SV_POSITION;
};

VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    Out.position = float4(In.position, 1.f);

    //Out.vPos = mul(float4(In.vPos, 1.f), g_WorldMatrix);
    //Out.vPos = mul(Out.vPos, g_ViewMatrix);
    //Out.vPos = mul(Out.vPos, g_ProjMatrix);

    //Out.vTexCoord = In.vTexCoord;
    return Out;
}