//--------------------------------------------------------------------------------------
// File: PSPCTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
};

float4 main(PSPCTInput input) : SV_TARGET
{
	//�e�N�X�`���ƃJ���[���牼�z���C�e�B���O���쐬
	float4 Light = g_texture.Sample(g_sampler, input.tex) * input.color;
	//�G�~�b�V�u�𑫂�
	return saturate(Light + Emissive);
}

