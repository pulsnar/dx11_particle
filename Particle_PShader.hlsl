

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );

struct pixelInputType
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;

};

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
float4 main(pixelInputType input) : SV_TARGET
{
	float4 output = float4(1,1,1,1);

	output = g_Texture.Sample(g_SamplerState, input.texcoord);
	clip(output.a - 0.25f);

	return output;
}
