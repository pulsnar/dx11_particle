#pragma once
#include "GameObject.h"
class CParticle : public CGameObject
{
	struct PARTICLE
	{
		float x;
		float y;
		float z;
		float frame;
	};
private:
	//�o�t�@�[��V�F�[�_�[
	ID3D11InputLayout* m_ParticleVLayout	= nullptr;
	ID3D11VertexShader* m_ParticleVShader	= nullptr;
	ID3D11PixelShader* m_ParticlePShader	= nullptr;
	ID3D11GeometryShader* m_ParticleGShader = nullptr;
	ID3D11Buffer* m_PositionBuffer			= nullptr;
	ID3D11Buffer* m_VertexBuffer			= nullptr;
	ID3D11Buffer* m_IndexBuffer				= nullptr;
	//���_�f�[�^
	VERTEX_3D* pVertex;
	//�C���X�^���X�f�[�^
	PARTICLE* m_instanceData = nullptr;
	int m_instanceCount;
	int m_maxFrame;
	int* m_startFrame = nullptr;
	XMFLOAT3* m_movement = nullptr;
	//�e�N�X�`���ϐ�
	CTexture* m_Texture = nullptr;
public:


	void Init();
	void Finalize();
	void Update();
	void Draw();

	//�C���X�^���X���쐬�܂��́A�C���X�^���X�̐�������ݒ肷��֐�
	void CreateInstance(int count);
	int GetInstanceCount() { return m_instanceCount; }

	//�C���X�^���X�͉��t���[���܂Ő�����̂��̐ݒ肷��֐�
	void SetMaxFrame(int frame) { m_maxFrame = frame; }

	//n�Ԗڂ̃C���X�^���X�͂��̃t���[�����Ɍ���邩�܂��͓����̂��̐ݒ肷��֐�
	void SetStartFrame(int* frame) {
		m_startFrame = frame;
		for (int i = 0; i < m_instanceCount; i++) {
			m_instanceData[i].frame = m_startFrame[i];
		}
	}
	//n�Ԗڂ̃C���X�^���X�͂ǂ���������������̂��̐ݒ肷��֐�
	void SetMovement(XMFLOAT3* move) { m_movement = move; }
};