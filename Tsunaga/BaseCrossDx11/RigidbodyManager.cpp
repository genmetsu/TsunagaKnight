/*!
@file RigidbodyManager.cpp
@brief ���̂ƏՓ˔������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	�R���W�������x�v�Z�p
	//--------------------------------------------------------------------------------------
	shared_ptr<MeshResource> Rigidbody::m_SPHEREMesh(nullptr);
	shared_ptr<MeshResource> Rigidbody::m_CAPSULEMesh(nullptr);
	shared_ptr<MeshResource> Rigidbody::m_OBBMesh(nullptr);

	void Rigidbody::Move(float Spantime) {
		m_Pos += m_Velocity * Spantime;
	}

	bool Rigidbody::CollisionSPHERE(Rigidbody& Dest, float ElapsedTime, CollisionState& state) {
		switch (Dest.m_CollType) {
		case CollType::typeSPHERE:
		{
			if (HitTest::CollisionTestSphereSphere(GetSPHERE(),
				m_Velocity - Dest.m_Velocity,
				Dest.GetSPHERE(),
				0, ElapsedTime, state.m_HitTime)) {
				SPHERE SrcSp = GetSPHERE();
				SPHERE DestSp = Dest.GetSPHERE();
				SrcSp.m_Center += m_Velocity * state.m_HitTime;
				DestSp.m_Center += Dest.m_Velocity * state.m_HitTime;
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SrcSp.m_Center - DestSp.m_Center;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = DestSp.m_Center - SrcSp.m_Center;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeCAPSULE:
		{
			if (HitTest::CollisionTestSphereCapsule(GetSPHERE(), m_Velocity - Dest.m_Velocity,
				Dest.GetCAPSULE(),
				0, ElapsedTime, state.m_HitTime)) {
				SPHERE SrcSp = GetSPHERE();
				SrcSp.m_Center += m_Velocity * state.m_HitTime;
				CAPSULE DestCap = Dest.GetCAPSULE();
				Vec3 CapCenter = DestCap.GetCenter();
				CapCenter + Dest.m_Velocity * state.m_HitTime;
				DestCap.SetCenter(CapCenter);
				Vec3 Ret;
				HitTest::SPHERE_CAPSULE(SrcSp, DestCap, Ret);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SrcSp.m_Center - Ret;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = Ret - SrcSp.m_Center;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeOBB:
		{
			if (HitTest::CollisionTestSphereObb(GetSPHERE(),
				m_Velocity - Dest.m_Velocity,
				Dest.GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				SPHERE SrcSp = GetSPHERE();
				SrcSp.m_Center += m_Velocity * state.m_HitTime;
				OBB DestObb = Dest.GetOBB();
				DestObb.m_Center += Dest.m_Velocity * state.m_HitTime;
				Vec3 Ret;
				HitTest::SPHERE_OBB(SrcSp, DestObb, Ret);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SrcSp.m_Center - Ret;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = Ret - SrcSp.m_Center;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		}
		return false;
	}

	bool Rigidbody::CollisionCAPSULE(Rigidbody& Dest, float ElapsedTime, CollisionState& state) {
		switch (Dest.m_CollType) {
		case CollType::typeSPHERE:
		{
			if (HitTest::CollisionTestSphereCapsule(Dest.GetSPHERE(),
				Dest.m_Velocity - m_Velocity,
				GetCAPSULE(),
				0, ElapsedTime, state.m_HitTime)) {
				CAPSULE SrcCap = GetCAPSULE();
				Vec3 CapCenter = SrcCap.GetCenter();
				CapCenter + m_Velocity * state.m_HitTime;
				SrcCap.SetCenter(CapCenter);
				SPHERE DestSp = Dest.GetSPHERE();
				DestSp.m_Center += Dest.m_Velocity * state.m_HitTime;
				Vec3 Ret;
				HitTest::SPHERE_CAPSULE(DestSp, SrcCap, Ret);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = Ret - DestSp.m_Center;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = DestSp.m_Center - Ret;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeCAPSULE:
		{
			if (HitTest::CollisionTestCapsuleCapsule(GetCAPSULE(),
				m_Velocity - Dest.m_Velocity,
				Dest.GetCAPSULE(),
				0, ElapsedTime, state.m_HitTime)) {
				CAPSULE SrcCap = GetCAPSULE();
				Vec3 CapCenter = SrcCap.GetCenter();
				CapCenter + m_Velocity * state.m_HitTime;
				SrcCap.SetCenter(CapCenter);

				CAPSULE DestCap = Dest.GetCAPSULE();
				CapCenter = DestCap.GetCenter();
				CapCenter + Dest.m_Velocity * state.m_HitTime;
				DestCap.SetCenter(CapCenter);

				Vec3 RetVec1, RetVec2;
				HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, RetVec1, RetVec2);

				//�ړ_�ւ̃x�N�g��
				//DestCap�̐�����RetVec1�̐�����̍ŋߐړ_��RetVec1�̖@��
				float t;
				Vec3 SegVecVec1, SegVecVec2;
				HitTest::ClosetPtPointSegment(RetVec1, DestCap.m_PointBottom, DestCap.m_PointTop, t, SegVecVec1);
				HitTest::ClosetPtPointSegment(RetVec2, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegVecVec2);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SegVecVec1 - RetVec1;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = SegVecVec2 - RetVec2;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeOBB:
		{
			if (HitTest::CollisionTestCapsuleObb(GetCAPSULE(),
				m_Velocity - Dest.m_Velocity,
				Dest.GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				Vec3 RetVec;
				CAPSULE SrcCap = GetCAPSULE();
				Vec3 CapCenter = SrcCap.GetCenter();
				CapCenter + m_Velocity * state.m_HitTime;
				SrcCap.SetCenter(CapCenter);
				OBB DestObb = Dest.GetOBB();
				DestObb.m_Center += Dest.m_Velocity * state.m_HitTime;
				HitTest::CAPSULE_OBB(SrcCap, DestObb, RetVec);
				//�ړ_�ւ̃x�N�g��
				float t;
				Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(RetVec, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SegPoint - RetVec;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = RetVec - SegPoint;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		}
		return false;
	}

	bool Rigidbody::CollisionOBB(Rigidbody& Dest, float ElapsedTime, CollisionState& state) {
		switch (Dest.m_CollType) {
		case CollType::typeSPHERE:
		{
			if (HitTest::CollisionTestSphereObb(Dest.GetSPHERE(),
				Dest.m_Velocity - m_Velocity,
				GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				OBB SrcObb = GetOBB();
				SrcObb.m_Center += m_Velocity * state.m_HitTime;
				SPHERE DestSp = Dest.GetSPHERE();
				DestSp.m_Center += Dest.m_Velocity * state.m_HitTime;
				Vec3 Ret;
				HitTest::SPHERE_OBB(DestSp, SrcObb, Ret);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = Ret - DestSp.m_Center;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = DestSp.m_Center - Ret;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeCAPSULE:
		{
			if (HitTest::CollisionTestCapsuleObb(Dest.GetCAPSULE(),
				Dest.m_Velocity - m_Velocity,
				GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				Vec3 RetVec;
				OBB SrcObb = GetOBB();
				CAPSULE DestCap = Dest.GetCAPSULE();
				HitTest::CAPSULE_OBB(DestCap, SrcObb, RetVec);
				//�ړ_�ւ̃x�N�g��
				float t;
				Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(RetVec, DestCap.m_PointBottom, DestCap.m_PointTop, t, SegPoint);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = RetVec - SegPoint;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = SegPoint - RetVec;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeOBB:
		{
			if (HitTest::CollisionTestObbObb(GetOBB(),
				m_Velocity - Dest.m_Velocity,
				Dest.GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				OBB SrcObb = GetOBB();
				SrcObb.m_Center += m_Velocity * state.m_HitTime;
				OBB DestObb = Dest.GetOBB();
				DestObb.m_Center += Dest.m_Velocity * state.m_HitTime;
				Vec3 RetVec;
				//Src��OBB��Dest�̍ŋߐړ_�𓾂�
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, RetVec);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SrcObb.m_Center - RetVec;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = RetVec - SrcObb.m_Center;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		}
		return false;
	}




	void Rigidbody::EscapeSPHERE(Rigidbody* Dest) {
		switch (Dest->m_CollType) {
		case CollType::typeSPHERE:
		{
			bool hit = HitTest::SPHERE_SPHERE(GetSPHERE(), Dest->GetSPHERE());
			if (hit) {
				auto Norm = GetSPHERE().m_Center - Dest->GetSPHERE().m_Center;
				Norm.normalize();
				auto TotalRad = Dest->GetSPHERE().m_Radius + GetSPHERE().m_Radius;
				Norm *= TotalRad;
				m_Pos = Dest->m_Pos + Norm;
			}
		}
		break;
		case CollType::typeCAPSULE:
		{
			Vec3 Ret;
			bool hit = HitTest::SPHERE_CAPSULE(GetSPHERE(), Dest->GetCAPSULE(), Ret);
			if (hit) {
				auto Norm = GetSPHERE().m_Center - Ret;
				Norm.normalize();
				Norm *= GetSPHERE().m_Radius;
				m_Pos = Ret + Norm;
			}
		}
		break;
		case CollType::typeOBB:
		{
			Vec3 Ret;
			bool hit = HitTest::SPHERE_OBB(GetSPHERE(), Dest->GetOBB(), Ret);
			if (hit) {
				auto Norm = GetSPHERE().m_Center - Ret;
				Norm.normalize();
				Norm *= GetSPHERE().m_Radius;
				m_Pos = Ret + Norm;
			}
		}
		break;
		}
	}

	void Rigidbody::EscapeCAPSULE(Rigidbody* Dest) {
		switch (Dest->m_CollType) {
		case CollType::typeSPHERE:
		{
			Vec3 Ret;
			bool hit = HitTest::SPHERE_CAPSULE(Dest->GetSPHERE(), GetCAPSULE(), Ret);
			if (hit) {
				auto Norm = Ret - Dest->GetSPHERE().m_Center;
				Norm.normalize();
				Norm *= GetCAPSULE().m_Radius;
				m_Pos = Ret + Norm;
			}
		}
		break;
		case CollType::typeCAPSULE:
		{
			CAPSULE SrcCap = GetCAPSULE();
			auto Center = SrcCap.GetCenter();
			CAPSULE DestCap = Dest->GetCAPSULE();
			Vec3 Ret1, Ret2;
			if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
				int count = 0;
				//�ړ_�ւ̃x�N�g��
				float t;
				Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(Ret1, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
				Vec3 span = SegPoint - Ret1;
				span.normalize();
				span *= 0.01f;
				while (1) {
					Center += span;
					SrcCap.SetCenter(Center);
					if (!HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
						break;
					}
					count++;
					if (count > 50) {
						break;
					}
				}
				m_Pos = Center;
			}
		}
		break;
		case CollType::typeOBB:
		{
			CAPSULE SrcCap = GetCAPSULE();
			auto Center = SrcCap.GetCenter();
			OBB DestObb = Dest->GetOBB();
			Vec3 Ret;
			if (HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
				int count = 0;
				//�ړ_�ւ̃x�N�g��
				float t;
				bsm::Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(Ret, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
				Vec3 span = SegPoint - Ret;
				span.normalize();
				span *= 0.01f;
				while (1) {
					Center += span;
					SrcCap.SetCenter(Center);
					if (!HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
						break;
					}
					count++;
					if (count > 50) {
						break;
					}
				}
				m_Pos = Center;
			}
		}
		break;
		}
	}

	void Rigidbody::EscapeOBB(Rigidbody* Dest) {
		switch (Dest->m_CollType) {
		case CollType::typeSPHERE:
		{
			OBB SrcObb = GetOBB();
			SPHERE DestSphere = Dest->GetSPHERE();
			Vec3 Ret;
			bool Ishit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
			if (Ishit) {
				Vec3 NowSpan = Ret - DestSphere.m_Center;
				Vec3 NewSpan = NowSpan;
				NewSpan.normalize();
				NewSpan *= (DestSphere.m_Radius);
				auto MoveSpan = NewSpan - NowSpan;
				m_Pos += MoveSpan;
			}
		}
		break;
		case CollType::typeCAPSULE:
		{
			OBB SrcObb = GetOBB();
			auto Center = SrcObb.m_Center;
			CAPSULE DestCapsule = Dest->GetCAPSULE();
			Vec3 Ret;
			bool Ishit = HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret);
			if (Ishit) {
				int count = 0;
				//�ړ_�ւ̃x�N�g��
				float t;
				Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(Ret, DestCapsule.m_PointBottom, DestCapsule.m_PointTop, t, SegPoint);
				Vec3 span = Ret - SegPoint;
				span.normalize();
				span *= 0.01f;
				while (1) {
					Center += span;
					SrcObb.m_Center = Center;
					if (!HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret)) {
						break;
					}
					count++;
					if (count > 50) {
						break;
					}
				}
				m_Pos = Center;
			}
		}
		break;
		case CollType::typeOBB:
		{
			OBB SrcObb = GetOBB();
			OBB DestObb = Dest->GetOBB();
			bool Ishit = HitTest::OBB_OBB(SrcObb, DestObb);
			if (Ishit) {
				bsm::Vec3 Ret;
				//Src��OBB��Dest�̍ŋߐړ_�𓾂�
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, Ret);
				bsm::Vec3 span = SrcObb.m_Center - Ret;
				span.normalize();
				span *= 0.01f;
				auto Center = SrcObb.m_Center;
				int count = 0;
				while (1) {
					Center += span;
					SrcObb.m_Center = Center;
					if (!HitTest::OBB_OBB(SrcObb, DestObb)) {
						break;
					}
					count++;
					if (count > 50) {
						break;
					}
				}
				m_Pos = Center;
			}
		}
		break;
		}
	}

	void Rigidbody::DrawWireFlame(const Camera& camera) {
		shared_ptr<MeshResource> DrawMesh;
		switch (m_CollType) {
		case CollType::typeNone:
			return;
			break;
		case CollType::typeSPHERE:
			if (!m_SPHEREMesh) {
				vector<VertexPositionNormalTexture> vertices;
				vector<uint16_t> indices;
				MeshUtill::CreateSphere(1.0f, 12, vertices, indices);
				//���b�V���̍쐬�i�ύX�ł��Ȃ��j
				m_SPHEREMesh = MeshResource::CreateMeshResource(vertices, indices, false);
			}
			DrawMesh = m_SPHEREMesh;
			break;
		case CollType::typeCAPSULE:
			if (!m_CAPSULEMesh) {
				vector<VertexPositionNormalTexture> vertices;
				vector<uint16_t> indices;
				MeshUtill::CreateCapsule(1.0f,
					Vec3(0, -0.5f, 0), Vec3(0, 0.5f, 0), 12, vertices, indices);
				//���b�V���̍쐬�i�ύX�ł��Ȃ��j
				m_CAPSULEMesh = MeshResource::CreateMeshResource(vertices, indices, false);
			}
			DrawMesh = m_CAPSULEMesh;
			break;
		case CollType::typeOBB:
			if (!m_OBBMesh) {
				vector<VertexPositionNormalTexture> vertices;
				vector<uint16_t> indices;
				MeshUtill::CreateCube(1.0f, vertices, indices);
				//���b�V���̍쐬�i�ύX�ł��Ȃ��j
				m_OBBMesh = MeshResource::CreateMeshResource(vertices, indices, false);
			}
			DrawMesh = m_OBBMesh;
			break;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//�T���v���[�̏���
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//�V�F�[�_�̐ݒ�
		//���_�V�F�[�_
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//�s�N�Z���V�F�[�_
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		SimpleConstants Cb;
		//�s��̒�`
		Mat4x4 World, ViewMat, ProjMat;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Quat,
			m_Pos
		);
		camera.GetViewProjMatrix(ViewMat, ProjMat);
		//�]�u����
		World.transpose();
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat.transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//�f�B�t���[�Y
		Cb.Diffuse = Col4(1.0f);
		//�G�~�b�V�u�i���j
		Cb.Emissive = Col4(1.0f);
		//���C�e�B���O�i�^��j
		Cb.LightDir = Vec4(0,-1.0f,0,1.0);

		//�e�N�X�`�����Ȃ�
		Cb.ActiveFlg.x = 0;
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
		UINT stride = DrawMesh->GetNumStride();
		UINT offset = 0;
		//�`����@�̃Z�b�g
		pD3D11DeviceContext->IASetPrimitiveTopology(DrawMesh->GetPrimitiveTopology());
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, DrawMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(DrawMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�e�����_�����O�X�e�[�g�̐ݒ�
		//�u�����h�X�e�[�g
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�[���\�[�X���N���A
		pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//�T���v���[���N���A
		pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		//���X�^���C�U�X�e�[�g�i���C�A�t���[���j
		pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
		//�`��
		pD3D11DeviceContext->DrawIndexed(DrawMesh->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}



	//--------------------------------------------------------------------------------------
	///	Rigidbody�}�l�[�W��
	//--------------------------------------------------------------------------------------
	RigidbodyManager::RigidbodyManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	RigidbodyManager::~RigidbodyManager() {}

	shared_ptr<Rigidbody>  RigidbodyManager::GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		for (auto& v : m_RigidbodyVec) {
			auto shptr = v->m_Owner.lock();
			if (shptr == OwnerPtr) {
				return v;
			}
		}
		throw BaseException(
			L"�w���Rigidbody��������܂���",
			L"!Rigidbody",
			L"RigidbodyManager::GetOwnRigidbody()"
		);
	}

	void RigidbodyManager::RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		auto it = m_RigidbodyVec.begin();
		while (it != m_RigidbodyVec.end()) {
			auto shptr = (*it)->m_Owner.lock();
			if (shptr == OwnerPtr) {
				m_RigidbodyVec.erase(it);
				return;
			}
			it++;
		}
	}



	//�^�[�����̏�����
	void RigidbodyManager::InitRigidbody() {
		//1�O�̈ʒu�ɃZ�b�g�ƃt�H�[�X�̏�����
		for (auto& v : m_RigidbodyVec) {
			v->SetToBefore();
			v->m_Force = Vec3(0);
		}
	}

	shared_ptr<Rigidbody> RigidbodyManager::AddRigidbody(const Rigidbody& body) {
		size_t index = m_RigidbodyVec.size();
		auto shptr = make_shared<Rigidbody>(body);
		m_RigidbodyVec.push_back(shptr);
		return m_RigidbodyVec[index];
	}


	//�Փ˔���
	bool RigidbodyManager::CollisionTest(Rigidbody& Src, Rigidbody& Dest, CollisionState& state) {
		if (Src.m_IsFixed) {
			return false;
		}
		if (CollisionStateChk(&Src, &Dest)) {
			return false;
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		switch (Src.m_CollType) {
		case CollType::typeSPHERE:
			return Src.CollisionSPHERE(Dest, ElapsedTime, state);
			break;
		case CollType::typeCAPSULE:
			return Src.CollisionCAPSULE(Dest, ElapsedTime, state);
			break;
		case CollType::typeOBB:
			return Src.CollisionOBB(Dest, ElapsedTime, state);
			break;
		}
		return false;
	}

	bool RigidbodyManager::CollisionStateChk(Rigidbody* p1, Rigidbody* p2) {
		for (auto& v : m_CollisionStateVec) {
			if ((v.m_Src == p1 && v.m_Dest == p2) ||
				(v.m_Src == p2 && v.m_Dest == p1)) {
				return true;
			}
		}
		return false;
	}

	void RigidbodyManager::CollisionDest(Rigidbody& Src) {
		for (auto& v : m_RigidbodyVec) {
			if (v->m_IsCollisionActive) {
				if (Src.m_Owner.lock() != v->m_Owner.lock()) {
					CollisionState state;
					if (CollisionTest(Src, *v, state)) {
						m_CollisionStateVec.push_back(state);
					}
				}
			}
		}
	}

	void RigidbodyManager::OnUpdate() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�t�H�[�X���瑬�x�ɕϊ�
		for (auto& v : m_RigidbodyVec) {
			Vec3 accel = v->m_Force * v->m_Mass;
			v->m_Velocity += accel * ElapsedTime;
		}

		//�Փ˔�����s���A�q�b�g������Α��x��ύX����
		if (m_RigidbodyVec.size() >= 2) {
			//�Փ˔���
			for (auto& v : m_RigidbodyVec) {
				if (v->m_IsCollisionActive) {
					CollisionDest(*v);
				}
			}
		}
		if (m_CollisionStateVec.size() >= 2) {
			//�q�b�g�^�C���Ń\�[�g�i�q�b�g�^�C�����j�߂����̂ɑΉ�
			auto func = [&](CollisionState& Left, CollisionState& Right)->bool {
				return (Left.m_HitTime < Right.m_HitTime);
			};
			std::sort(m_CollisionStateVec.begin(), m_CollisionStateVec.end(), func);
		}
		//�Փˉ���
		for (auto& v : m_CollisionStateVec) {
			if (!v.m_Src->m_IsFixed) {
				v.m_Src->Move(v.m_HitTime);
				v.m_Src->m_Velocity.slide(v.m_SrcHitNormal);
			}
			if (!v.m_Dest->m_IsFixed) {
				v.m_Dest->Move(v.m_HitTime);
				v.m_Dest->m_Velocity.slide(v.m_DestHitNormal);
			}
		}

		//�ݒ肳�ꂽ���x�����ƂɏՓ˖����̃I�u�W�F�N�g�̈ʒu�̌���
		for (auto& v : m_RigidbodyVec) {
			v->Move(ElapsedTime);
		}

		//�G�X�P�[�v����
		for (auto& v : m_CollisionStateVec) {
			if (!v.m_Src->m_IsFixed) {
				Escape(v.m_Src, v.m_Dest);
			}
			if (!v.m_Dest->m_IsFixed) {
				Escape(v.m_Dest, v.m_Src);
			}
		}


	}

	void RigidbodyManager::Escape(Rigidbody* Src, Rigidbody* Dest) {
		switch (Src->m_CollType) {
		case CollType::typeSPHERE:
			Src->EscapeSPHERE(Dest);
			break;
		case CollType::typeCAPSULE:
			Src->EscapeCAPSULE(Dest);
			break;
		case CollType::typeOBB:
			Src->EscapeOBB(Dest);
			break;
		}
	}


	void RigidbodyManager::OnUpdate2() {
		//�Փˏ��̃N���A
		m_CollisionStateVec.clear();
	}

	void RigidbodyManager::OnDraw() {
		auto& camera = GetStage<Stage>()->GetCamera();
		for (auto& v : m_RigidbodyVec) {
			if (v->m_IsDrawActive) {
				v->DrawWireFlame(camera);
			}
		}
	}


}
// end basecross
