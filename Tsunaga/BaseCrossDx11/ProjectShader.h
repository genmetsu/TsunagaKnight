/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Shadow�R���X�^���g�o�b�t�@�\����
	//--------------------------------------------------------------------------------------
	struct ShadowConstants
	{
		/// ���[���h�s��
		bsm::Mat4x4 mWorld;
		/// �r���[�s��
		bsm::Mat4x4 mView;
		/// �ˉe�s��
		bsm::Mat4x4 mProj;
		/// Bone�p
		bsm::Vec4 Bones[3 * 72];
		ShadowConstants() {
			memset(this, 0, sizeof(ShadowConstants));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstants)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, VertexPositionNormalTangentTextureSkinning)

	//--------------------------------------------------------------------------------------
	///	SimpleConstants�R���X�^���g�o�b�t�@�\����(�V���v���R�c���L)
	//--------------------------------------------------------------------------------------
	struct SimpleConstants
	{
		/// ���[���h�s��
		Mat4x4 World;
		/// �r���[�s��
		Mat4x4 View;
		/// �ˉe�s��
		Mat4x4 Projection;
		/// �G�~�b�V�u�F
		Col4 Emissive;
		/// �f�t���[�Y�F
		Col4 Diffuse;
		/// �X�y�L�����[
		Col4 Specular;
		/// �e�N�X�`��=x���A�N�e�B�u���ǂ���
		XMUINT4 ActiveFlg;
		/// ���C�C�g����
		Vec4 LightDir;
		/// ���C�g�ʒu
		Vec4 LightPos;
		/// Eye�̈ʒu
		Vec4 EyePos;
		/// ���C�g�r���[�s��
		Mat4x4 LightView;
		/// ���C�g�ˉe�s��
		Mat4x4 LightProjection;
		/// Bone�z��
		Vec4 Bones[3 * 72];
		SimpleConstants() {
			memset(this, 0, sizeof(SimpleConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//CBSimple
	DECLARE_DX11_CONSTANT_BUFFER(CBSimple, SimpleConstants)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	///PNTStaticShadow
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	//�e����Bone
	DECLARE_DX11_VERTEX_SHADER(VSPNTBone, VertexPositionNormalTextureSkinning)
	//�e�t��Bone
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneShadow, VertexPositionNormalTextureSkinning)

	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)

	//PCTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCTStatic, VertexPositionColorTexture)
	DECLARE_DX11_VERTEX_SHADER(VSPCTStaticInstance, VertexPositionColorTextureMatrix)
	DECLARE_DX11_PIXEL_SHADER(PSPCTStatic)


	//�X�v���C�g�p�R���X�^���g�o�b�t�@�\����
	struct SpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstantBuffer)

	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)


	//--------------------------------------------------------------------------------------
	///	Basic�V�F�[�_�[
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	Basic�V�F�[�_�[�p�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct BasicConstants
	{
		bsm::Col4 diffuseColor;
		bsm::Col4 emissiveColor;
		bsm::Col4 specularColorAndPower;

		bsm::Col4 lightDirection[3];
		bsm::Col4 lightDiffuseColor[3];
		bsm::Col4 lightSpecularColor[3];

		bsm::Vec4 eyePosition;

		bsm::Col4 fogColor;
		bsm::Vec4 fogVector;

		bsm::Mat4x4 world;
		bsm::Vec4 worldInverseTranspose[3];
		bsm::Mat4x4 worldViewProj;
		//�ėp�t���O
		XMUINT4 activeFlg;
		//�ȉ��e
		bsm::Vec4 lightPos;
		bsm::Vec4 eyePos;
		bsm::Mat4x4 lightView;
		bsm::Mat4x4 lightProjection;

		bsm::Vec4 bones[3 * 72];

	};

	//--------------------------------------------------------------------------------------
	/// �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_CONSTANT_BUFFER(CBBasic, BasicConstants)

	//--------------------------------------------------------------------------------------
	/// PNT���_�V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPL, VertexPositionNormalTexture)
	//�e�t��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, VertexPositionNormalTexture)
	//�e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePL, VertexPositionNormalTextureSkinning)
	//�e�t��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, VertexPositionNormalTextureSkinning)

	//--------------------------------------------------------------------------------------
	/// PNTnT���_�V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPL, VertexPositionNormalTangentTexture)
	//�e��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPLShadow, VertexPositionNormalTangentTexture)
	//�e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePL, VertexPositionNormalTangentTextureSkinning)
	//�e��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePLShadow, VertexPositionNormalTangentTextureSkinning)

	//--------------------------------------------------------------------------------------
	/// PNT�s�N�Z���V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPL);
	//�e�t��
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPLShadow);

	//--------------------------------------------------------------------------------------
	/// PNTnT�s�N�Z���V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPL);
	//�e��
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPLShadow);


}
//end basecross

