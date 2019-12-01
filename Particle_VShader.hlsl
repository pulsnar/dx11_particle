

//=============================================================================
// �萔�o�b�t�@
//=============================================================================

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}
cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}
cbuffer PositionBuffer : register(b6) //�����̍D���Ȕԍ��ɕς�����������
{
	float4 instanceNewPos[1024];
}
struct vertexInputType
{
	float4 position : POSITION0;
	uint instanceId : SV_InstanceID;

};
struct geometryInputType
{
	float4 position : SV_POSITION;
};

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
geometryInputType main(vertexInputType input)
{
	geometryInputType output;

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	input.position.x = instanceNewPos[input.instanceId].x;
	input.position.y = instanceNewPos[input.instanceId].y;
	input.position.z = instanceNewPos[input.instanceId].z;
	output.position = input.position;

	return output;
}