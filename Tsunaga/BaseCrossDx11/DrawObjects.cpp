/*!
@file DrawObjects.cpp
@brief �`��I�u�W�F�N�g����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�̕`��N���X
	//--------------------------------------------------------------------------------------
	float ShadowmapRenderer::m_LightHeight(20.0f);
	float ShadowmapRenderer::m_LightNear(1.0f);
	float ShadowmapRenderer::m_LightFar(50.0f);
	float ShadowmapRenderer::m_ViewWidth(32.0f);
	float ShadowmapRenderer::m_ViewHeight(32.0f);

	ShadowmapRenderer::ShadowmapRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		GameObject(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	ShadowmapRenderer::~ShadowmapRenderer() {}

	void ShadowmapRenderer::OnCreate() {
	}

	void ShadowmapRenderer::AddDrawObject(const shared_ptr<ShadowmapObject>& Obj) {
		m_ShadowmapObjectVec.push_back(Obj);
	}

	void ShadowmapRenderer::OnDrawShadowmap() {

		auto PtrStage = GetStage<Stage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�e�I�u�W�F�N�g���ʏ���
		//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

		//�ʏ���
		for (auto& v : m_ShadowmapObjectVec) {
			//�ʒu�̎擾
			auto Pos = v->m_WorldMatrix.transInMatrix();
			Vec4 LightDir4;
			PtrStage->GetLightDir(LightDir4);
			Vec3 LightDir(LightDir4.x, LightDir4.y, LightDir4.z);
			Vec3 PosSpan = LightDir;
			PosSpan *= 0.1f;
			Pos += PosSpan;
			//�s��̒�`
			Mat4x4 World, LightView, LightProj;
			//���[���h�s��̌���
			World.affineTransformation(
				v->m_WorldMatrix.scaleInMatrix(),			//�X�P�[�����O
				Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
				v->m_WorldMatrix.quatInMatrix(),				//��]�p�x
				Pos				//�ʒu
			);
			LightDir = LightDir  * -1.0;
			Vec3 LightAt = v->m_Camera.m_CamerAt;
			Vec3 LightEye = LightAt + (LightDir * m_LightHeight);
			//���C�g�̃r���[�Ǝˉe���v�Z
			LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_LightNear, m_LightFar);
			ShadowConstants Cb;
			Cb.mWorld = bsm::transpose(World);
			Cb.mView = bsm::transpose(LightView);
			Cb.mProj = bsm::transpose(LightProj);

			bool IsSkin = false;
			bool IsSkinStride = false;
			if (v->m_MeshRes->IsSkining()) {
				if (v->m_pLocalBoneVec) {
					//�{�[���̐ݒ�
					size_t BoneSz = v->m_pLocalBoneVec->size();
					UINT cb_count = 0;
					for (size_t b = 0; b < BoneSz; b++) {
						bsm::Mat4x4 mat = v->m_pLocalBoneVec->at(b);
						mat.transpose();
						Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
						Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
						Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
						cb_count += 3;
					}
					IsSkin = true;
				}
				IsSkinStride = true;
			}

			if (IsSkin) {
				if (v->m_IsNormalmap) {
					//���_�V�F�[�_�[�̃Z�b�g
					pID3D11DeviceContext->VSSetShader(VSShadowmapBoneWithTan::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̃Z�b�g
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBoneWithTan::GetPtr()->GetInputLayout());
				}
				else {
					//���_�V�F�[�_�[�̃Z�b�g
					pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̃Z�b�g
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
				}
			}
			else {
				//���_�V�F�[�_�[�̃Z�b�g
				pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
				//�C���v�b�g���C�A�E�g�̃Z�b�g
				pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
			}


			//�R���X�^���g�o�b�t�@�̍X�V
			pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = v->m_MeshRes->GetNumStride();
			UINT offset = 0;
			//�`����@�̃Z�b�g
			pID3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
			//���_�o�b�t�@���Z�b�g
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			//�`��
			pID3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
		}
		//��n��
		Dev->InitializeStates();
		m_ShadowmapObjectVec.clear();
	}

	//--------------------------------------------------------------------------------------
	///	Simple�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	void SimpleRenderer::AddDrawObject(const shared_ptr<SimpleDrawObject>& DrawObj) {
		if (DrawObj->m_OwnShadowmapActive) {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_ShadowTraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_ShadowDrawObjectVec.push_back(DrawObj);
			}

		}
		else {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_TraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_DrawObjectVec.push_back(DrawObj);
			}
		}
	}

	void SimpleRenderer::ClearDrawObject() {
		//�`��p�̔z����N���A
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
		m_ShadowDrawObjectVec.clear();
		m_ShadowTraceDrawObjectVec.clear();
	}

	void SimpleRenderer::SetConstants(const shared_ptr<SimpleDrawObject>& Obj, SimpleConstants& Cb) {
		auto PtrStage = GetStage<Stage>();
		//�}���`���C�g�𓾂�
		auto& MLight = PtrStage->GetMultiLights();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = Obj->m_WorldMatrix;
		//�]�u����
		World.transpose();
		//�J�����𓾂�
		Obj->m_Camera.GetViewProjMatrix(ViewMat, ProjMat);
		Vec4 LightDir;
		PtrStage->GetLightDir(LightDir);
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat.transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//�f�B�t���[�Y
		Cb.Diffuse = Obj->m_Diffuse;
		//�G�~�b�V�u���Z�B
		Cb.Emissive = Obj->m_Emissive;
		//�X�y�L�����[�B
		Cb.Specular = Obj->m_Specular;
		//���C�e�B���O
		Cb.LightDir = LightDir;
		Cb.LightDir.w = 1.0f;
		Cb.EyePos = Obj->m_Camera.m_CamerEye;
		Cb.EyePos.w = 1.0f;
		Vec3 CalcLightDir(LightDir.x, LightDir.y, LightDir.z);
		CalcLightDir = -1.0 * CalcLightDir;
		Vec3 LightAt = Obj->m_Camera.m_CamerAt;
		Vec3 LightEye = CalcLightDir;
		LightEye *= ShadowmapRenderer::m_LightHeight;
		LightEye = LightAt + LightEye;
		Cb.LightPos = LightEye;
		Cb.LightPos.w = 1.0f;
		bsm::Mat4x4 LightView, LightProj;
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(ShadowmapRenderer::m_ViewWidth, ShadowmapRenderer::m_ViewHeight,
			ShadowmapRenderer::m_LightNear, ShadowmapRenderer::m_LightFar);
		Cb.LightView = bsm::transpose(LightView);
		Cb.LightProjection = bsm::transpose(LightProj);
		//�{�[���̐ݒ�
		size_t BoneSz = Obj->m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = Obj->m_LocalBonesMatrix[b];
				mat.transpose();
				Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
				Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
	}

	//--------------------------------------------------------------------------------------
	///	SimplePCT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	SimplePCTStaticRenderer::SimplePCTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePCTStaticRenderer::~SimplePCTStaticRenderer() {}

	void SimplePCTStaticRenderer::OnCreate() {
	}

	void SimplePCTStaticRenderer::OnDraw() {
		Draw<VSPCTStatic, PSPCTStatic>(false);
	}



	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	SimplePNTStaticRenderer::SimplePNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePNTStaticRenderer::~SimplePNTStaticRenderer() {}

	void SimplePNTStaticRenderer::OnCreate() {
	}

	void SimplePNTStaticRenderer::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow>(false);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�I�u�W�F�N�g�̕`��N���X(�V���h�E�}�b�v�����ꍇ)
	//--------------------------------------------------------------------------------------
	SimplePNTStaticRenderer2::SimplePNTStaticRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePNTStaticRenderer2::~SimplePNTStaticRenderer2() {}

	void SimplePNTStaticRenderer2::OnCreate() {
	}

	void SimplePNTStaticRenderer2::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow2>(false);
	}


	//--------------------------------------------------------------------------------------
	///	SimplePNT���_���f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	SimplePNTStaticModelRenderer::SimplePNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePNTStaticModelRenderer::~SimplePNTStaticModelRenderer() {}

	void SimplePNTStaticModelRenderer::OnCreate() {
	}

	void SimplePNTStaticModelRenderer::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_���f���I�u�W�F�N�g�̕`��N���X(�V���h�E�}�b�v�����ꍇ)
	//--------------------------------------------------------------------------------------
	SimplePNTStaticModelRenderer2::SimplePNTStaticModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePNTStaticModelRenderer2::~SimplePNTStaticModelRenderer2() {}

	void SimplePNTStaticModelRenderer2::OnCreate() {
	}

	void SimplePNTStaticModelRenderer2::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow2>(true);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	SimplePNTBoneModelRenderer::SimplePNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePNTBoneModelRenderer::~SimplePNTBoneModelRenderer() {}

	void SimplePNTBoneModelRenderer::OnCreate() {
	}

	void SimplePNTBoneModelRenderer::OnDraw() {
		Draw<VSPNTBone, VSPNTBoneShadow, PSPNTStatic, PSPNTStaticShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X(�V���h�E�}�b�v�����ꍇ)
	//--------------------------------------------------------------------------------------
	SimplePNTBoneModelRenderer2::SimplePNTBoneModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	SimplePNTBoneModelRenderer2::~SimplePNTBoneModelRenderer2() {}

	void SimplePNTBoneModelRenderer2::OnCreate() {
	}

	void SimplePNTBoneModelRenderer2::OnDraw() {
		Draw<VSPNTBone, VSPNTBoneShadow, PSPNTStatic, PSPNTStaticShadow2>(true);
	}




	//--------------------------------------------------------------------------------------
	///	Bc�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	void BcRenderer::AddDrawObject(const shared_ptr<BcDrawObject>& DrawObj) {
		if (DrawObj->m_OwnShadowmapActive) {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_ShadowTraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_ShadowDrawObjectVec.push_back(DrawObj);
			}

		}
		else {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_TraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_DrawObjectVec.push_back(DrawObj);
			}
		}
	}

	void BcRenderer::ClearDrawObject() {
		//�`��p�̔z����N���A
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
		m_ShadowDrawObjectVec.clear();
		m_ShadowTraceDrawObjectVec.clear();
	}

	void BcRenderer::SetConstants(const shared_ptr<BcDrawObject>& Obj, BasicConstants& BcCb) {
		auto PtrStage = GetStage<Stage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�}���`���C�g�𓾂�
		auto& MLight = PtrStage->GetMultiLights();
		//���[���h�s��
		Mat4x4 world = Obj->m_WorldMatrix;
		//�r���[�g�ˉe�s��̒�`
		Mat4x4 view, proj;
		Obj->m_Camera.GetViewProjMatrix(view, proj);
		//�s��̐ݒ�
		auto worldView = world * view;
		BcCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, proj));

		//�t�H�O�̐ݒ�
		if (Obj->m_FogEnabled)
		{
			auto start = Obj->m_FogStart;
			auto end = Obj->m_FogEnd;
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				BcCb.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				BcCb.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			BcCb.fogColor = Obj->m_FogColor;
		}
		else
		{
			BcCb.fogVector = g_XMZero;
			BcCb.fogColor = g_XMZero;

		}
		//���C�g�̐ݒ�
		if (Obj->m_LightingEnabled)
		{
			for (int i = 0; i < 3; i++) {
				BcCb.lightDirection[i] = MLight.m_Directional[i];
				BcCb.lightDiffuseColor[i] = MLight.m_DiffuseColor[i];
				BcCb.lightSpecularColor[i] = MLight.m_SpecularColor[i];
			}
			BcCb.world = XMMatrixTranspose(world);
			XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);
			BcCb.worldInverseTranspose[0] = worldInverse.r[0];
			BcCb.worldInverseTranspose[1] = worldInverse.r[1];
			BcCb.worldInverseTranspose[2] = worldInverse.r[2];
			XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
			BcCb.eyePosition = viewInverse.r[3];
		}
		XMVECTOR diffuse = Obj->m_Diffuse;
		XMVECTOR alphaVector = XMVectorReplicate(Obj->m_Alpha);
		XMVECTOR emissiveColor = Obj->m_Emissive;
		XMVECTOR ambientLightColor = MLight.m_AmbientLightColor;
		if (Obj->m_LightingEnabled)
		{
			BcCb.activeFlg.x = 3;
			// Merge emissive and ambient light contributions.
			BcCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
			BcCb.specularColorAndPower = Obj->m_SpecularColorAndPower;
		}
		else
		{
			BcCb.activeFlg.x = 0;
			// Merge diffuse and emissive light contributions.
			diffuse += emissiveColor;
			BcCb.specularColorAndPower = Obj->m_SpecularColorAndPower;
		}
		// xyz = diffuse * alpha, w = alpha.
		BcCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		//�e�p
		if (Obj->m_OwnShadowmapActive) {
			SingleLight Light;
			MLight.GetMainLight(Light);
			bsm::Vec3 CalcLightDir(Light.m_Directional * -1.0);
			bsm::Vec3 LightAt(Obj->m_Camera.m_CamerAt);
			bsm::Vec3 LightEye(CalcLightDir);
			LightEye *= ShadowmapRenderer::m_LightHeight;
			LightEye = LightAt + LightEye;
			bsm::Vec4 LightEye4(LightEye, 1.0f);
			LightEye4.w = 1.0f;
			BcCb.lightPos = LightEye4;
			bsm::Vec4 eyePos4(Obj->m_Camera.m_CamerEye, 1.0f);
			eyePos4.w = 1.0f;
			BcCb.eyePos = eyePos4;
			bsm::Mat4x4 LightView, LightProj;
			//���C�g�̃r���[�Ǝˉe���v�Z
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(ShadowmapRenderer::m_ViewWidth, ShadowmapRenderer::m_ViewHeight,
				ShadowmapRenderer::m_LightNear, ShadowmapRenderer::m_LightFar);
			BcCb.lightView = bsm::transpose(LightView);
			BcCb.lightProjection = bsm::transpose(LightProj);
		}

		//�{�[���̐ݒ�
		size_t BoneSz = Obj->m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = Obj->m_LocalBonesMatrix[b];
				mat.transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
	}

	void BcRenderer::ConvertToNormalVertex(
		const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionNormalTangentTexture>& new_vertices
	) {
		new_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalTangentTexture new_pntnt_v;

			new_pntnt_v.position = vertices[i].position;
			new_pntnt_v.normal = vertices[i].normal;
			new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
			Vec3 n = cross((Vec3)new_pntnt_v.normal, Vec3(0, 1, 0));
			new_pntnt_v.tangent = Vec4(n.x, n.y, n.z, 0.0f);
			new_pntnt_v.tangent.w = 0.0f;
			new_vertices.push_back(new_pntnt_v);
		}
		MeshUtill::SetNormalTangent(new_vertices);
	}


	//--------------------------------------------------------------------------------------
	///	BcPNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	BcPNTStaticRenderer::BcPNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	BcPNTStaticRenderer::~BcPNTStaticRenderer() {}

	void BcPNTStaticRenderer::OnCreate() {
	}

	void BcPNTStaticRenderer::OnDraw() {
		Draw<BcVSPNTStaticPL, BcVSPNTStaticPLShadow, BcPSPNTPL, BcPSPNTPLShadow>(false);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnT���_�I�u�W�F�N�g(�@���}�b�v�g�p)�̕`��N���X
	//--------------------------------------------------------------------------------------
	BcPNTnTStaticRenderer::BcPNTnTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	BcPNTnTStaticRenderer::~BcPNTnTStaticRenderer() {}

	void BcPNTnTStaticRenderer::OnCreate() {
	}

	void BcPNTnTStaticRenderer::OnDraw() {
		Draw<BcVSPNTnTStaticPL, BcVSPNTnTStaticPLShadow, BcPSPNTnTPL, BcPSPNTnTPLShadow>(false);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNT���_���f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	BcPNTStaticModelRenderer::BcPNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	BcPNTStaticModelRenderer::~BcPNTStaticModelRenderer() {}

	void BcPNTStaticModelRenderer::OnCreate() {
	}

	void BcPNTStaticModelRenderer::OnDraw() {
		Draw<BcVSPNTStaticPL, BcVSPNTStaticPLShadow, BcPSPNTPL, BcPSPNTPLShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnT���_���f���I�u�W�F�N�g�̕`��N���X(�@���}�b�v�t�����f��)
	//--------------------------------------------------------------------------------------
	BcPNTnTStaticModelRenderer::BcPNTnTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	BcPNTnTStaticModelRenderer::~BcPNTnTStaticModelRenderer() {}

	void BcPNTnTStaticModelRenderer::OnCreate() {
	}

	void BcPNTnTStaticModelRenderer::OnDraw() {
		Draw<BcVSPNTnTStaticPL, BcVSPNTnTStaticPLShadow, BcPSPNTnTPL, BcPSPNTnTPLShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	BcPNTBoneModelRenderer::BcPNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	BcPNTBoneModelRenderer::~BcPNTBoneModelRenderer() {}

	void BcPNTBoneModelRenderer::OnCreate() {
	}

	void BcPNTBoneModelRenderer::OnDraw() {
		Draw<BcVSPNTBonePL, BcVSPNTBonePLShadow, BcPSPNTPL, BcPSPNTPLShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnT���_�{�[�����f���I�u�W�F�N�g�̕`��N���X(�@���}�b�v�t�����f��)
	//--------------------------------------------------------------------------------------
	BcPNTnTBoneModelRenderer::BcPNTnTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//�^�O�̒ǉ�
		AddTag(Tagname);
	}
	BcPNTnTBoneModelRenderer::~BcPNTnTBoneModelRenderer() {}

	void BcPNTnTBoneModelRenderer::OnCreate() {
	}

	void BcPNTnTBoneModelRenderer::OnDraw() {
		Draw<BcVSPNTnTBonePL, BcVSPNTnTBonePLShadow, BcPSPNTnTPL, BcPSPNTnTPLShadow>(true);
	}




}
//end basecross
