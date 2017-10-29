/*!
@file DrawObjects.h
@brief �`��I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�`��Ɏg�p����\����
	//--------------------------------------------------------------------------------------
	struct ShadowmapObject {
		//���b�V��
		shared_ptr<MeshResource> m_MeshRes;
		//���[���h�s��
		Mat4x4 m_WorldMatrix;
		//�`��p�J����
		Camera m_Camera;
		vector<Mat4x4>* m_pLocalBoneVec;
		//�@���}�b�v���g�p���邩
		bool m_IsNormalmap;
		ShadowmapObject() :
			m_MeshRes(nullptr),
			m_WorldMatrix(),
			m_pLocalBoneVec(nullptr),
			m_IsNormalmap(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�̕`��N���X
	//--------------------------------------------------------------------------------------
	class ShadowmapRenderer : public GameObject {
		vector<shared_ptr<ShadowmapObject>> m_ShadowmapObjectVec;
	public:
		static float m_LightHeight;	//���C�g�̍����i���������̒l�Ŋ|����j
		static float m_LightNear;	//���C�g��Near
		static float m_LightFar;		//���C�g��Far
		static float m_ViewWidth;
		static float m_ViewHeight;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit ShadowmapRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShadowmapRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�悷��I�u�W�F�N�g��ǉ�����
		@param[in]	Obj	ShadowmapObject�\����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawObject(const shared_ptr<ShadowmapObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	};

	//--------------------------------------------------------------------------------------
	///	�A�j���[�V�����f�[�^�\����.
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		///	�X�^�[�g�T���v��
		UINT	m_StartSample;
		///	�T���v���̒���
		UINT	m_SampleLength;
		///	���[�v���邩�ǂ���
		bool	m_IsLoop;
		///	�A�j�����I���������ǂ���
		bool	m_IsAnimeEnd;
		///	1�b������̃t���[��
		float	m_SamplesParSecond;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^.
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^.
		@param[in]	StartSample	�X�^�[�g�t���[��
		@param[in]	SampleLength	�t���[���̒���
		@param[in]	bLoop	���[�v���邩�ǂ���
		@param[in]	SamplesParSecond = 30.0f	1�b������̃t���[����
		*/
		//--------------------------------------------------------------------------------------
		AnimationData(UINT StartSample, UINT SampleLength, bool bLoop,
			float SamplesParSecond = 30.0f) :
			m_StartSample{ StartSample },
			m_SampleLength{ SampleLength },
			m_IsLoop{ bLoop },
			m_IsAnimeEnd{ false },
			m_SamplesParSecond{ SamplesParSecond }
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�`��Ɏg�p����\����(�e)
	//--------------------------------------------------------------------------------------
	struct DrawObjectBase {
		//���b�V�����\�[�X
		shared_ptr<MeshResource> m_MeshRes;
		//�ȉ��A�{�[�����f���p
		//���[�J���{�[���s��̔z��
		vector<Mat4x4> m_LocalBonesMatrix;
		//�A�j���[�V������`�̃}�b�v
		map<wstring, AnimationData> m_AnimationMap;
		//���݂̃A�j���[�V������
		wstring m_CurrentAnimeName;
		//�J�����g�A�j���[�V����
		float m_CurrentAnimeTime;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		DrawObjectBase():
			m_MeshRes(nullptr),
			m_CurrentAnimeName(L""),
			m_CurrentAnimeTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �{�[���̏�����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void BoneInit() {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				//�擪�̃{�[�����̍s��ŏ�����
				m_LocalBonesMatrix.resize(m_MeshRes->GetBoneCount());
				auto& SampleMatrixVec = m_MeshRes->GetSampleMatrixVec();
				for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
					m_LocalBonesMatrix[i] = SampleMatrixVec[i];
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������ύX����
		@param[in]	AnemationName	�A�j���[�V�������i���łɒǉ�����Ă�����́j
		@param[in]	StartTime = 0.0f	�J�n����̕b��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				if (AnemationName == L"") {
					throw BaseException(
						L"�A�j���[�V���������󔒂ł�",
						L"if (AnemationName == L\"\")",
						L"BcDrawObject::SetCurrentAnimation()"
					);
				}
				auto it = m_AnimationMap.find(AnemationName);
				if (it != m_AnimationMap.end()) {
					//�w��̖��O����������
					m_CurrentAnimeName = AnemationName;
					m_CurrentAnimeTime = StartTime;
					//�A�j���[�V�����͏I�����Ă��Ȃ�
					it->second.m_IsAnimeEnd = false;
				}
				else {
					//������Ȃ�
					throw BaseException(
						L"�w��̃A�j���[�V�����͓o�^����Ă܂���",
						AnemationName,
						L"BcDrawObject::SetCurrentAnimation()"
					);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�j���[�V������ǉ�����
		@param[in]	Name	�A�j���[�V������
		@param[in]	StartSample	�J�n�T���v��
		@param[in]	SampleLength	�T���v���̒���
		@param[in]	Loop	���[�v���邩�ǂ���
		@param[in]	SamplesParSecond = 30.0f	1�b������̃T���v����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				if (Name == L"") {
					throw BaseException(
						L"�A�j���[�V���������󔒂ł�",
						L"if (Name == L\"\")",
						L"BcDrawObject::AddAnimation()"
					);
				}
				if (StartSample < 0 || SampleLength < 0) {
					throw BaseException(
						L"�J�n�T���v�����T���v������0�����ł�",
						L"if (StartSample < 0 || SampleLength < 0)",
						L"BcDrawObject::AddAnimation()"
					);
				}
				if (SamplesParSecond <= 0.0f) {
					throw BaseException(
						L"�T���v�����b��0�ȉ��ł�",
						L"if (SamplesParSecond <= 0.0f)",
						L"BcDrawObject::AddAnimation()"
					);
				}
				//�d���L�[������΍����ւ���
				AnimationData Data((UINT)StartSample, (UINT)SampleLength, Loop, SamplesParSecond);
				m_AnimationMap[Name] = Data;
				if (m_AnimationMap.size() == 1) {
					//1�����o�^���Ȃ�������A�J�����g�A�j���͊Y���A�j���ƂȂ�
					ChangeCurrentAnimation(Name, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�n�s�񂩂�I���s��̊Ԃ�t���Ԏ��̍s���Ԃ�
		@param[in]	m1	�J�n�̍s��
		@param[in]	m2	�I���̍s��
		@param[in]	t	���ԁi0����1.0f�j
		@param[out]	out	���ʂ��󂯎��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				Vec3 Scale1, Pos1;
				Quat Qt1;
				m1.decompose(Scale1, Qt1, Pos1);
				Qt1.normalize();

				Vec3 Scale2, Pos2;
				Quat Qt2;

				m2.decompose(Scale2, Qt2, Pos2);
				Qt2.normalize();

				Vec3 ScaleOut, PosOut;
				Quat QtOut;

				ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
				PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
				QtOut = XMQuaternionSlerp(Qt1, Qt2, t);
				out.affineTransformation(ScaleOut, Vec3(0, 0, 0), QtOut, PosOut);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������i�߂�
		@param[in]	ElapsedTime	�o�ߎ���
		@return	�A�j���[�V�������I�������true
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				if (ElapsedTime < 0.0f) {
					throw BaseException(
						L"�A�j���[�V�����X�V�Ƀ}�C�i�X�͐ݒ�ł��܂���",
						L"if (ElapsedTime < 0.0f)",
						L"BcDrawObject::UpdateAnimation()"
					);
				}
				if (m_CurrentAnimeName == L"") {
					//������Ȃ�
					throw BaseException(
						L"�J�����g�A�j���[�V�������ݒ肳��Ă܂���",
						L"if (m_CurrentAnimeName == L\"\")",
						L"BcDrawObject::UpdateAnimation()"
					);
				}
				auto PtrMesh = m_MeshRes;
				UINT SampleCount = PtrMesh->GetSampleCount();
				auto& SampleMatrixVec = PtrMesh->GetSampleMatrixVec();
				UINT BoneCount = PtrMesh->GetBoneCount();
				auto& TgtAnimeData = m_AnimationMap[m_CurrentAnimeName];
				if (TgtAnimeData.m_StartSample >= SampleCount) {
					//�X�^�[�g�̃T���v�����Ō�̃T���v���ȍ~������
					TgtAnimeData.m_StartSample = SampleCount - 1;
					TgtAnimeData.m_SampleLength = 0;
					UINT UITgtSample = TgtAnimeData.m_StartSample;
					//�Ō�̃T���v����\��
					for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
						m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
					}
					m_CurrentAnimeTime = 0;
					if (TgtAnimeData.m_IsLoop) {
						TgtAnimeData.m_IsAnimeEnd = false;
						return false;
					}
					else {
						TgtAnimeData.m_IsAnimeEnd = true;
						return true;
					}
				}
				//���łɃA�j�����I�����Ă���
				if (TgtAnimeData.m_IsAnimeEnd) {
					//���݂̃��[�J���s����g�p
					return true;
				}
				//�J�����g�^�C�����X�V
				m_CurrentAnimeTime += ElapsedTime;
				//�X�^�[�g�ʒu���v�Z
				auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample + m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
				UINT UITgtSample = (UINT)FLOATTgtSample;
				UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
				if (UILastSample >= SampleCount) {
					UILastSample = SampleCount - 1;
				}
				if (UITgtSample >= UILastSample) {
					UITgtSample = UILastSample - 1;
					//�Ō�̃T���v����\��
					for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
						m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
					}
					if (TgtAnimeData.m_IsLoop) {
						TgtAnimeData.m_IsAnimeEnd = false;
						//���[�v����̂ŃJ�����g�^�C����0�ɂ���
						m_CurrentAnimeTime = 0;
						return false;
					}
					else {
						m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
						TgtAnimeData.m_IsAnimeEnd = true;
						return true;
					}
				}
				else {
					//�T���v���ƃT���v���̊Ԃ̊������v�Z
					FLOATTgtSample -= (float)UITgtSample;
					UINT UINextSample = UITgtSample + 1;
					for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
						InterpolationMatrix(
							SampleMatrixVec[BoneCount * UITgtSample + i],
							SampleMatrixVec[BoneCount * UINextSample + i],
							FLOATTgtSample, m_LocalBonesMatrix[i]);
					}
					//�A�j���͏I����ĂȂ�
					return false;
				}
			}
			return true;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�V���v���`��Ɏg�p����\����
	//--------------------------------------------------------------------------------------
	struct SimpleDrawObject :public DrawObjectBase {
		//�e�N�X�`�����\�[�X
		shared_ptr<TextureResource> m_TextureRes;
		//�u�����h�X�e�[�g
		BlendState m_BlendState;
		//�f�v�X�X�e���V���X�e�[�g
		DepthStencilState m_DepthStencilState;
		//���X�^���C�U�X�e�[�g
		RasterizerState m_RasterizerState;
		//�T���v���[�X�e�[�g
		SamplerState m_SamplerState;
		//���[���h�s��
		Mat4x4 m_WorldMatrix;
		//�`��p�J����
		Camera m_Camera;
		//���f���ɓ����Ă���F���g�����ǂ���
		bool m_UsedModelColor;
		//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
		bool m_UsedModelTextre;
		//���ȉe�����邩�ǂ���
		bool m_OwnShadowmapActive;
		//�V���h�E�}�b�v�𔭍s���Ă��邩
		bool m_ShadowmapUse;
		//�G�~�b�V�u�F
		Col4 m_Emissive;
		//�f�t���[�Y�F
		Col4 m_Diffuse;
		//�X�y�L�����[
		Col4 m_Specular;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SimpleDrawObject() :
			DrawObjectBase(),
			m_TextureRes(nullptr),
			m_BlendState(BlendState::Opaque),
			m_DepthStencilState(DepthStencilState::Default),
			m_RasterizerState(RasterizerState::CullBack),
			m_SamplerState(SamplerState::LinearClamp),
			m_WorldMatrix(),
			m_Camera(),
			m_UsedModelColor(true),
			m_UsedModelTextre(true),
			m_OwnShadowmapActive(false),
			m_ShadowmapUse(false),
			m_Emissive(0.4f, 0.4f, 0.4f, 0),
			m_Diffuse(0.8f, 0.8f, 0.8f, 1.0f),
			m_Specular(0.2f, 0.2f, 0.2f, 0)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	Simple�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class SimpleRenderer : public GameObject {
	protected:
		vector<shared_ptr<SimpleDrawObject>> m_DrawObjectVec;
		vector<shared_ptr<SimpleDrawObject>> m_TraceDrawObjectVec;
		vector<shared_ptr<SimpleDrawObject>> m_ShadowDrawObjectVec;
		vector<shared_ptr<SimpleDrawObject>> m_ShadowTraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit SimpleRenderer(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimpleRenderer() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		@param[in]	Obj	�ݒ肷��f�[�^
		@param[out]	BcCb	�ݒ肷��R���X�^���g�o�b�t�@�\����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(const shared_ptr<SimpleDrawObject>& Obj, SimpleConstants& Cb);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�e�B�b�N���b�V���̕`��(�z��P��)
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	ObjectVec	�ݒ肷��f�[�^�z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStaticVec(const vector<shared_ptr<SimpleDrawObject>>& ObjectVec) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			for (auto& v : ObjectVec) {
				SimpleConstants Cb;
				//�R���X�^���g�o�b�t�@�̍쐬
				SetConstants(v, Cb);
				if (v->m_TextureRes) {
					//�e�N�X�`��������
					Cb.ActiveFlg.x = 1;
				}
				else {
					//�e�N�X�`�����Ȃ�
					Cb.ActiveFlg.x = 0;
				}
				//�R���X�^���g�o�b�t�@�̍X�V
				pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//���_�V�F�[�_�ɓn��
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�s�N�Z���V�F�[�_�ɓn��
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//�`����@�̃Z�b�g
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//���_�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�e�����_�����O�X�e�[�g�̐ݒ�
				//�u�����h�X�e�[�g
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//�f�v�X�X�e���V���X�e�[�g
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//�e�N�X�`���ƃT���v���[
				if (v->m_TextureRes) {
					pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
					//�T���v���[��ݒ�
					RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
				}
				else {
					//�V�F�[�_�[���\�[�X���N���A
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//�T���v���[���N���A
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
				//�e�ƃT���v���[
				if (v->m_OwnShadowmapActive) {
					//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�}�b�v�T���v���[
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				//���X�^���C�U�X�e�[�g�ƕ`��
				if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
					//���������p
					//���X�^���C�U�X�e�[�g(���`��)
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//�`��
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
					//���X�^���C�U�X�e�[�g�i�\�`��j
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//�`��
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
					//�`��
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���f�����b�V���̕`��(�z��P��)
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	ObjectVec	�ݒ肷��f�[�^�z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawModelVec(const vector<shared_ptr<SimpleDrawObject>>& ObjectVec) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			for (auto& v : ObjectVec) {
				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//�`����@�̃Z�b�g
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//���_�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�e�����_�����O�X�e�[�g�̐ݒ�
				//�u�����h�X�e�[�g
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//�f�v�X�X�e���V���X�e�[�g
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//�e�ƃT���v���[
				if (v->m_OwnShadowmapActive) {
					//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�}�b�v�T���v���[
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				SimpleConstants Cb;
				//�R���X�^���g�o�b�t�@�̍쐬
				SetConstants(v, Cb);
				//���b�V�����̃}�e���A���̔z��
				auto& MatVec = v->m_MeshRes->GetMaterialExVec();
				size_t MatIndex = 0;
				for (auto& m : MatVec) {
					if (v->m_UsedModelColor) {
						Cb.Emissive = m.m_Emissive;
						Cb.Emissive.w = 0;
						Cb.Diffuse = m.m_Diffuse;
					}
					//�e�N�X�`�����L�������̐ݒ�
					if (v->m_UsedModelTextre) {
						//���f���̃e�N�X�`�����L��
						if (m.m_TextureResource) {
							//�e�N�X�`��������
							Cb.ActiveFlg.x = 1;
						}
						else {
							//�e�N�X�`�����Ȃ�
							Cb.ActiveFlg.x = 0;
						}
					}
					else {
						//���f���̃e�N�X�`�����L���ł͂Ȃ�
						if (v->m_TextureRes) {
							//�������A�f�[�^�Ƀe�N�X�`��������
							Cb.ActiveFlg.x = 1;
						}
						else {
							//�e�N�X�`�����Ȃ�
							Cb.ActiveFlg.x = 0;
						}
					}
					//�R���X�^���g�o�b�t�@�̍X�V
					pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
					//�R���X�^���g�o�b�t�@�̐ݒ�
					ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//���_�V�F�[�_�ɓn��
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//�s�N�Z���V�F�[�_�ɓn��
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					if (Cb.ActiveFlg.x) {
						//�V�F�[�_�Ƀe�N�X�`���̐ݒ肪����Ă���
						//�T���v���[�̐ݒ�
						RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
						//�e�N�X�`���̑I��
						if (v->m_UsedModelTextre) {
							//���f���̃e�N�X�`�����L��
							pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						}
						else {
							//���f���̃e�N�X�`��������
							pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
						}
					}
					else {
						//�V�F�[�_�[���\�[�X���N���A
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//�T���v���[���N���A
						RenderState->SetSamplerAllClear(pD3D11DeviceContext);
					}
					//���X�^���C�U�X�e�[�g�ƕ`��
					if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
						//���������p
						//���X�^���C�U�X�e�[�g(���`��)
						pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
						//�`��
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
						//���X�^���C�U�X�e�[�g�i�\�`��j
						pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
						//�`��
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					}
					else {
						RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
						//�`��
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					}
					MatIndex++;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���̕`��
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_�i�e�����j
		@tparam[in]	T_VShaderShadow	�g�p���钸�_�V�F�[�_�i�e�t���j
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_�i�e�����j
		@tparam[in]	T_PShaderShadow	�g�p����s�N�Z���V�F�[�_�i�e�t���j
		@param[in]	IsModel	���f�����ǂ����B���f���Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_VShaderShadow, typename T_PShader, typename T_PShaderShadow>
		void Draw(bool IsModel) {
			auto PtrStage = GetStage<Stage>();
			//�T�u�֐��Ăяo��(�s����)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			//�J�����̈ʒu
			Vec3 CameraEye = PtrStage->GetCamera().m_CamerEye;
			//--------------------------------------------------------
			//������3D�I�u�W�F�N�g���J��������̋����Ń\�[�g
			//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
			//--------------------------------------------------------
			auto func = [&](shared_ptr<SimpleDrawObject>& Left, shared_ptr<SimpleDrawObject>& Right)->bool {
				auto LeftPos = Left->m_WorldMatrix.transInMatrix();
				auto RightPos = Right->m_WorldMatrix.transInMatrix();
				auto LeftLen = bsm::length(LeftPos - CameraEye);
				auto RightLen = bsm::length(RightPos - CameraEye);
				return (LeftLen > RightLen);
			};
			//�����_�����g���ă\�[�g
			std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
			std::sort(m_ShadowTraceDrawObjectVec.begin(), m_ShadowTraceDrawObjectVec.end(), func);
			//�T�u�֐��Ăяo��(����)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			//��n��
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->InitializeStates();
			ClearDrawObject();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���̕`��(�e�`�斳��)
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_�i�e�����j
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_�i�e�����j
		@param[in]	IsModel	���f�����ǂ����B���f���Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void Draw(bool IsModel) {
			auto PtrStage = GetStage<Stage>();
			//�T�u�֐��Ăяo��(�s����)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_DrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_DrawObjectVec);
			}
			//�J�����̈ʒu
			Vec3 CameraEye = PtrStage->GetCamera().m_CamerEye;
			//--------------------------------------------------------
			//������3D�I�u�W�F�N�g���J��������̋����Ń\�[�g
			//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
			//--------------------------------------------------------
			auto func = [&](shared_ptr<SimpleDrawObject>& Left, shared_ptr<SimpleDrawObject>& Right)->bool {
				auto LeftPos = Left->m_WorldMatrix.transInMatrix();
				auto RightPos = Right->m_WorldMatrix.transInMatrix();
				auto LeftLen = bsm::length(LeftPos - CameraEye);
				auto RightLen = bsm::length(RightPos - CameraEye);
				return (LeftLen > RightLen);
			};
			//�����_�����g���ă\�[�g
			std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
			std::sort(m_ShadowTraceDrawObjectVec.begin(), m_ShadowTraceDrawObjectVec.end(), func);
			//�T�u�֐��Ăяo��(����)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
			}
			//��n��
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->InitializeStates();
			ClearDrawObject();
		}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�悷��I�u�W�F�N�g��ǉ�����
		@param[in]	DrawObj	SimpleDrawObject�\����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawObject(const shared_ptr<SimpleDrawObject>& DrawObj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�悷��I�u�W�F�N�g���N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ClearDrawObject();
	};

	//--------------------------------------------------------------------------------------
	///	SimplePCT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class SimplePCTStaticRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePCTStaticRenderer(const shared_ptr<Stage>& StagePtr,const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePCTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	
	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�I�u�W�F�N�g�̕`��N���X(�V���h�E�}�b�v�����ꍇ)
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticRenderer2 : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticRenderer2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	SimplePNT���_���f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticModelRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_���f���I�u�W�F�N�g�̕`��N���X(�V���h�E�}�b�v�����ꍇ)
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticModelRenderer2 : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticModelRenderer2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class SimplePNTBoneModelRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTBoneModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X(�V���h�E�}�b�v�����ꍇ)
	//--------------------------------------------------------------------------------------
	class SimplePNTBoneModelRenderer2 : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	Tagname		�^�O��
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTBoneModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTBoneModelRenderer2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};




	//--------------------------------------------------------------------------------------
	///	Bc�`��Ɏg�p����\����(�e�Ή�)
	//--------------------------------------------------------------------------------------
	struct BcDrawObject:public DrawObjectBase {
		//�e�N�X�`�����\�[�X
		shared_ptr<TextureResource> m_TextureRes;
		//�@���}�b�v�e�N�X�`�����\�[�X
		shared_ptr<TextureResource> m_NormalTextureRes;
		//���f���ɕ����̃e�N�X�`�����������ꍇ�̖@���}�b�v�e�N�X�`�����\�[�X
		vector<shared_ptr<TextureResource>> m_NormalTextureResVec;
		//�u�����h�X�e�[�g
		BlendState m_BlendState;
		//�f�v�X�X�e���V���X�e�[�g
		DepthStencilState m_DepthStencilState;
		//���X�^���C�U�X�e�[�g
		RasterizerState m_RasterizerState;
		//�T���v���[�X�e�[�g
		SamplerState m_SamplerState;
		//���[���h�s��
		Mat4x4 m_WorldMatrix;
		//�`��p�J����
		Camera m_Camera;
		//�����x
		float m_Alpha;
		//���f���ɓ����Ă���F���g�����ǂ���
		bool m_UsedModelColor;
		//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
		bool m_UsedModelTextre;
		//���ȉe�����邩�ǂ���
		bool m_OwnShadowmapActive;
		//�V���h�E�}�b�v�𔭍s���Ă��邩
		bool m_ShadowmapUse;
		//�G�~�b�V�u�F
		Col4 m_Emissive;
		//�f�t���[�Y�F
		Col4 m_Diffuse;
		//�X�y�L�����[�ƃp���[�F
		Col4 m_SpecularColorAndPower;
		//���C�e�B���O���邩�ǂ���
		bool m_LightingEnabled;
		//�t�H�O���L�����ǂ���
		bool m_FogEnabled;
		//�t�H�O�̊J�n�ʒu
		float m_FogStart;
		//�t�H�O�̏I���ʒu
		float m_FogEnd;
		//�t�H�O�F
		Col4 m_FogColor;
		//�t�H�O�x�N�g��
		Vec3 m_FogVector;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		BcDrawObject() :
			DrawObjectBase(),
			m_TextureRes(nullptr),
			m_NormalTextureRes(nullptr),
			m_BlendState(BlendState::Opaque),
			m_DepthStencilState(DepthStencilState::Default),
			m_RasterizerState(RasterizerState::CullBack),
			m_SamplerState(SamplerState::LinearClamp),
			m_WorldMatrix(),
			m_Camera(),
			m_Alpha(1.0f),
			m_UsedModelColor(true),
			m_UsedModelTextre(true),
			m_OwnShadowmapActive(false),
			m_ShadowmapUse(false),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(true),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f)
		{}
	};



	//--------------------------------------------------------------------------------------
	///	Bc�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class BcRenderer : public GameObject {
	protected:
		vector<shared_ptr<BcDrawObject>> m_DrawObjectVec;
		vector<shared_ptr<BcDrawObject>> m_TraceDrawObjectVec;
		vector<shared_ptr<BcDrawObject>> m_ShadowDrawObjectVec;
		vector<shared_ptr<BcDrawObject>> m_ShadowTraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit BcRenderer(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcRenderer() {}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		@param[in]	Obj	�ݒ肷��f�[�^
		@param[out]	BcCb	�ݒ肷��R���X�^���g�o�b�t�@�\����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(const shared_ptr<BcDrawObject>& Obj, BasicConstants& BcCb);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�e�B�b�N���b�V���̕`��(�z��P��)
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	ObjectVec	�ݒ肷��f�[�^�z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStaticVec(const vector<shared_ptr<BcDrawObject>>& ObjectVec) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			for (auto& v : ObjectVec) {
				BasicConstants BcCb;
				//�R���X�^���g�o�b�t�@�̍쐬
				SetConstants(v, BcCb);
				if (v->m_TextureRes) {
					//�e�N�X�`��������
					BcCb.activeFlg.y = 1;
				}
				else {
					//�e�N�X�`�����Ȃ�
					BcCb.activeFlg.y = 0;
				}
				//�R���X�^���g�o�b�t�@�̍X�V
				pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//���_�V�F�[�_�ɓn��
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�s�N�Z���V�F�[�_�ɓn��
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//�`����@�̃Z�b�g
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//���_�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�e�����_�����O�X�e�[�g�̐ݒ�
				//�u�����h�X�e�[�g
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//�f�v�X�X�e���V���X�e�[�g
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//�e�N�X�`���ƃT���v���[
				if (v->m_TextureRes) {
					pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
					//�T���v���[��ݒ�
					RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
					//�@���}�b�v
					if (v->m_NormalTextureRes) {
						if (v->m_OwnShadowmapActive) {
							//�Q�Ԗڂɓ����
							pD3D11DeviceContext->PSSetShaderResources(2, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
							//�T���v���[��ݒ�
							RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 2);
						}
						else {
							//1�Ԗڂɓ����
							pD3D11DeviceContext->PSSetShaderResources(1, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
							//�T���v���[��ݒ�
							RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 1);
						}
					}
				}
				else {
					//�V�F�[�_�[���\�[�X���N���A
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//�T���v���[���N���A
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
				//�e�ƃT���v���[
				if (v->m_OwnShadowmapActive) {
					//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�}�b�v�T���v���[
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				//���X�^���C�U�X�e�[�g�ƕ`��
				if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
					//���������p
					//���X�^���C�U�X�e�[�g(���`��)
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//�`��
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
					//���X�^���C�U�X�e�[�g�i�\�`��j
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//�`��
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
					//�`��
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���f�����b�V���̕`��(�z��P��)
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	ObjectVec	�ݒ肷��f�[�^�z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawModelVec(const vector<shared_ptr<BcDrawObject>>& ObjectVec) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			for (auto& v : ObjectVec) {
				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//�`����@�̃Z�b�g
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//���_�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�e�����_�����O�X�e�[�g�̐ݒ�
				//�u�����h�X�e�[�g
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//�f�v�X�X�e���V���X�e�[�g
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//�e�ƃT���v���[
				if (v->m_OwnShadowmapActive) {
					//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�}�b�v�T���v���[
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				BasicConstants BcCb;
				//�R���X�^���g�o�b�t�@�̍쐬
				SetConstants(v, BcCb);
				//���b�V�����̃}�e���A���̔z��
				auto& MatVec = v->m_MeshRes->GetMaterialExVec();
				size_t MatIndex = 0;
				for (auto& m : MatVec) {
					if (v->m_UsedModelColor) {
						BcCb.emissiveColor = m.m_Emissive;
						BcCb.emissiveColor.w = 0;
						BcCb.diffuseColor = m.m_Diffuse;
					}
					//�e�N�X�`�����L�������̐ݒ�
					if (v->m_UsedModelTextre) {
						//���f���̃e�N�X�`�����L��
						if (m.m_TextureResource) {
							//�e�N�X�`��������
							BcCb.activeFlg.y = 1;
						}
						else {
							//�e�N�X�`�����Ȃ�
							BcCb.activeFlg.y = 0;
						}
					}
					else {
						//���f���̃e�N�X�`�����L���ł͂Ȃ�
						if (v->m_TextureRes) {
							//�������A�f�[�^�Ƀe�N�X�`��������
							BcCb.activeFlg.y = 1;
						}
						else {
							//�e�N�X�`�����Ȃ�
							BcCb.activeFlg.y = 0;
						}
					}
					//�R���X�^���g�o�b�t�@�̍X�V
					pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
					//�R���X�^���g�o�b�t�@�̐ݒ�
					ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//���_�V�F�[�_�ɓn��
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//�s�N�Z���V�F�[�_�ɓn��
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					if (BcCb.activeFlg.y) {
						//�V�F�[�_�Ƀe�N�X�`���̐ݒ肪����Ă���
						//�T���v���[�̐ݒ�
						RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
						//�e�N�X�`���̑I��
						if (v->m_UsedModelTextre) {
							//���f���̃e�N�X�`�����L��
							pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						}
						else {
							//���f���̃e�N�X�`��������
							pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
						}
						//�@���}�b�v
						if (v->m_NormalTextureResVec.size() > MatIndex) {
							//�����̖@���}�b�v������
							if (v->m_OwnShadowmapActive) {
								//�Q�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(2, 1, v->m_NormalTextureResVec[MatIndex]->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 2);
							}
							else {
								//1�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(1, 1, v->m_NormalTextureResVec[MatIndex]->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 1);
							}
						}
						else if (v->m_NormalTextureRes) {
							//�����ł͂Ȃ��Ĉ�����@���}�b�v������
							if (v->m_OwnShadowmapActive) {
								//�Q�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(2, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 2);
							}
							else {
								//1�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(1, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 1);
							}
						}
					}
					else {
						//�V�F�[�_�[���\�[�X���N���A
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//�T���v���[���N���A
						RenderState->SetSamplerAllClear(pD3D11DeviceContext);
					}
					//���X�^���C�U�X�e�[�g�ƕ`��
					if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
						//���������p
						//���X�^���C�U�X�e�[�g(���`��)
						pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
						//�`��
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
						//���X�^���C�U�X�e�[�g�i�\�`��j
						pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
						//�`��
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					}
					else {
						RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
						//�`��
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					}
					MatIndex++;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���̕`��
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_�i�e�����j
		@tparam[in]	T_VShaderShadow	�g�p���钸�_�V�F�[�_�i�e�t���j
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_�i�e�����j
		@tparam[in]	T_PShaderShadow	�g�p����s�N�Z���V�F�[�_�i�e�t���j
		@param[in]	IsModel	���f�����ǂ����B���f���Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_VShaderShadow, typename T_PShader, typename T_PShaderShadow>
		void Draw(bool IsModel) {
			auto PtrStage = GetStage<Stage>();
			//�T�u�֐��Ăяo��(�s����)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			//�J�����̈ʒu
			Vec3 CameraEye = PtrStage->GetCamera().m_CamerEye;
			//--------------------------------------------------------
			//������3D�I�u�W�F�N�g���J��������̋����Ń\�[�g
			//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
			//--------------------------------------------------------
			auto func = [&](shared_ptr<BcDrawObject>& Left, shared_ptr<BcDrawObject>& Right)->bool {
				auto LeftPos = Left->m_WorldMatrix.transInMatrix();
				auto RightPos = Right->m_WorldMatrix.transInMatrix();
				auto LeftLen = bsm::length(LeftPos - CameraEye);
				auto RightLen = bsm::length(RightPos - CameraEye);
				return (LeftLen > RightLen);
			};
			//�����_�����g���ă\�[�g
			std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
			std::sort(m_ShadowTraceDrawObjectVec.begin(), m_ShadowTraceDrawObjectVec.end(), func);
			//�T�u�֐��Ăяo��(����)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			//��n��
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->InitializeStates();
			ClearDrawObject();
		}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�悷��I�u�W�F�N�g��ǉ�����
		@param[in]	DrawObj	BcDrawObject�\����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawObject(const shared_ptr<BcDrawObject>& DrawObj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�悷��I�u�W�F�N�g���N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ClearDrawObject();
		//���[�e�B���e�B�֐��istatic�j
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�t�H�[�}�b�g��PNT����PNTnT�ɃR���o�[�g������
		@param[in]	vertices	���ɂȂ�PNT���_�̔z��
		@param[out]	new_vertices	�R���o�[�g�����PNTnT���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertToNormalVertex(
			const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionNormalTangentTexture>& new_vertices
		);
	};



	//--------------------------------------------------------------------------------------
	///	BcPNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class BcPNTStaticRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnT���_�I�u�W�F�N�g(�@���}�b�v�g�p)�̕`��N���X
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	BcPNT���_���f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class BcPNTStaticModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnT���_���f���I�u�W�F�N�g�̕`��N���X(�@���}�b�v�t�����f��)
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class BcPNTBoneModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTBoneModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X(�@���}�b�v�t�����f��)
	//--------------------------------------------------------------------------------------
	class BcPNTnTBoneModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Tagname		�^�O��
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTBoneModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



}
//end basecross
