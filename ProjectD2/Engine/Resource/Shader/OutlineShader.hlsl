texture map;

sampler samp
{
	Texture = map;
};

float4 Color;
float2 ImageSize;

float4 main(float2 uv : TEXCOORD) : SV_TARGET
{
	float a = 0;

	if (tex2D(samp, uv).a != 0)
		return tex2D(samp, uv);
	
	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2 offset = float2(x, y) / ImageSize;
			a += tex2D(samp, uv + offset).a;
		}
	}
	
	if (a > 0)
		return Color;
	else
		return tex2D(samp, uv);
}

technique tech
{

	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		PixelShader = compile ps_2_0 main();
	}

}