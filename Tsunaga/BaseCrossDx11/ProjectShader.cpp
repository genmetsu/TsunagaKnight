/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�`��p�̃V�F�[�_�N���X�̎��́i�}�N���g�p�j
	//--------------------------------------------------------------------------------------
	//Shadowmap
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmap, App::GetApp()->GetShadersPath() + L"VSShadowmap.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBone, App::GetApp()->GetShadersPath() + L"VSShadowmapBone.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, App::GetApp()->GetShadersPath() + L"VSShadowmapBoneWithTan.cso")


	//--------------------------------------------------------------------------------------
	///	SimpleConstants�R���X�^���g�o�b�t�@�\����(�V���v���R�c���L)
	//--------------------------------------------------------------------------------------
	//CBSimple
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSimple)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")

	//�e����Bone
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBone, App::GetApp()->GetShadersPath() + L"VSPNTBone.cso")
	//�e�t��Bone
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBoneShadow, App::GetApp()->GetShadersPath() + L"VSPNTBoneShadow.cso")


	//PNTStaticShadow
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow2.cso")

	//PCTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStatic, App::GetApp()->GetShadersPath() + L"VSPCTStatic.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPCTStaticInstance.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTStatic, App::GetApp()->GetShadersPath() + L"PSPCTStatic.cso")


	//PCTSprite
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->GetShadersPath() + L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->GetShadersPath() + L"PSPCTSprite.cso")

	//--------------------------------------------------------------------------------------
	///	Basic�V�F�[�_�[
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	/// �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBBasic)

	//--------------------------------------------------------------------------------------
	/// PNT���_�V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPL, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticPL.cso")
	//�e�t��
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticPLShadow.cso")
	//�e����Bone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePL, App::GetApp()->GetShadersPath() + L"BcVSPNTBonePL.cso")
	//�e�t��Bone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTBonePLShadow.cso")


	//--------------------------------------------------------------------------------------
	/// PNTnT���_�V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTStaticPL, App::GetApp()->GetShadersPath() + L"BcVSPNTnTStaticPL.cso")
	//�e��
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTStaticPLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTnTStaticPLShadow.cso")
	//�e����Bone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTBonePL, App::GetApp()->GetShadersPath() + L"BcVSPNTnTBonePL.cso")
	//�e��Bone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTBonePLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTnTBonePLShadow.cso")


	//--------------------------------------------------------------------------------------
	/// PNT�s�N�Z���V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTPL, App::GetApp()->GetShadersPath() + L"BcPSPNTPL.cso")
	//�e�t��
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTPLShadow, App::GetApp()->GetShadersPath() + L"BcPSPNTPLShadow.cso")

	//--------------------------------------------------------------------------------------
	/// PNTnT�s�N�Z���V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTnTPL, App::GetApp()->GetShadersPath() + L"BcPSPNTnTPL.cso")
	//�e��
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTnTPLShadow, App::GetApp()->GetShadersPath() + L"BcPSPNTnTPLShadow.cso")


}
//end basecross


