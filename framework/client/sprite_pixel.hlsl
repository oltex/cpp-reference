Texture2D _texture : register(t0);
SamplerState _sampler : register(s0);
struct pixel_in
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};
float4 main(pixel_in _in) : SV_TARGET
{
    float4 _out = float4(0, 0, 0, 1);
    //_out = _texture.Sample(_sampler, _in.texcoord);
    return _out;
}