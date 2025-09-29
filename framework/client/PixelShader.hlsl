struct PS_IN {
    float4 position : SV_POSITION;
};

float4 main(PS_IN In) : SV_TARGET {
	return float4(1.f, 1.f, 1.f, 1.f);
}