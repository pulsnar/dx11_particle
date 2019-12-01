#include "particle.h"
#include "GameObject.h"
#include "manager.h"
#include "shader.h"
void CParticle::Init()
{
	m_Position = XMFLOAT3(14, 14, -64);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(4.0f, 4.0f, 4.0f);

	//�e�N�X�`���Ǘ��V�X�e���i����j
	m_Texture = new CTexture();
	m_Texture = CTextureList::GetTextureList(0);

	//�o�t�@�[�ݒ�Ə�����
	D3D11_BUFFER_DESC bd;
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(m_Position) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.StructureByteStride = 0;

	//SUBRESOURCE�ݒ�Ə�����
	D3D11_SUBRESOURCE_DATA sd;
	sd = {};
	sd.pSysMem = pVertex;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	CShader::LoadVertexShader("Particle_VShader.cso", &m_ParticleVShader, &m_ParticleVLayout, layout, ARRAYSIZE(layout));
	CShader::LoadPixelShader("Particle_PShader.cso", &m_ParticlePShader);
	CShader::LoadGeometryShader("Particle_GShader.cso", &m_ParticleGShader);


}
void CParticle::CreateInstance(int count)
{
	if (m_PositionBuffer == nullptr) {
		m_instanceCount = count;
		m_instanceData = new PARTICLE[m_instanceCount];
		m_startFrame = new int[m_instanceCount];
		for (int i = 0; i < m_instanceCount; i++) {
			m_instanceData[i].x = 0;
			m_instanceData[i].y = 0.0f;
			m_instanceData[i].z = 0;
		}
		
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(PARTICLE) * m_instanceCount;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(m_instanceData);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_PositionBuffer);
		CRenderer::GetDeviceContext()->VSSetConstantBuffers(6, 1, &m_PositionBuffer);
	}
	//�o�t�@�[�쐬�Ɏ��s������A�f�[�^���������
	if (m_PositionBuffer == nullptr) delete this;
}
void CParticle::Update()
{
		for (int i = 0; i < m_instanceCount; i++) {
			//frame�̒l��-1�ȉ��̏ꍇ�́A�\�����Ȃ����Ƃɂ���
			if (m_instanceData[i].frame < 0) {
				m_instanceData[i].x = -std::numeric_limits<float>::max();
				m_instanceData[i].y = -std::numeric_limits<float>::max();
				m_instanceData[i].z = -std::numeric_limits<float>::max();
			}
			//frame�̒l��0�̏ꍇ�́A�l������������
			if (m_instanceData[i].frame == 0) {
				m_instanceData[i].x = 0;
				m_instanceData[i].y = 0;
				m_instanceData[i].z = 0;
			}
			//frame�̒l��1�ȏ�̏ꍇ�́A�ʒu�l���i�[����m_movement�̒l�ő���������
			if (m_instanceData[i].frame > 0) {
				m_instanceData[i].x += m_movement[i].x;
				m_instanceData[i].y += m_movement[i].y;
				m_instanceData[i].z += m_movement[i].z;
			}
			m_instanceData[i].frame += 1;
			//frame�̒l��m_maxFrame�ɂ��ǂ蒅������A�������l�ɖ߂�
			if (m_instanceData[i].frame > m_maxFrame) {
				m_instanceData[i].frame = m_startFrame[i];
			}
	}
	//GPU�Ƀf�[�^�𑗂�
	CRenderer::GetDeviceContext()->UpdateSubresource(m_PositionBuffer, 0, NULL, m_instanceData, 0, 0);

}
void CParticle::Draw()
{
	//���_�o�b�t�@������
	UINT stride = sizeof(m_Position);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	//�����h�}�g���N�X������
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);																

	//�e�N�X�`���ݒ�
	CRenderer::SetTexture(m_Texture);

	//�|�C���g���X�g�ɂ��Ȃ��Ƃ����Ȃ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	CRenderer::SetRasterizerCullBack(false);

	//�V�F�[�_�[��K�p����֐��i����j
	CRenderer::SetCustomShader(m_ParticleVShader, m_ParticleVLayout, m_ParticlePShader);
	CRenderer::GetDeviceContext()->GSSetShader(m_ParticleGShader, NULL, 0);

	//�C���X�^���X�`��
	CRenderer::GetDeviceContext()->DrawInstanced(1, m_instanceCount,0, 0);

	//GS���g���I�������ANULL�ɂ���
	CRenderer::GetDeviceContext()->GSSetShader(nullptr, NULL, 0);

	//���̃V�F�[�_�[�ɖ߂��֐��i����j
	CRenderer::SetDefaultShader();

	CRenderer::SetRasterizerCullBack(true);

}

void CParticle::Finalize()
{
	if (m_PositionBuffer) m_PositionBuffer->Release();
	//	m_Texture->Unload();
	delete m_Texture;
}
