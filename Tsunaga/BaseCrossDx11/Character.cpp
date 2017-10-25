/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���ʎ���
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers(float WrapX, float WrapY) {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vec3(-HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, WrapY)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, WrapY)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers(m_Scale.x, m_Scale.y);
		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SquareMesh,
			m_TextureResource,
			World,
			false,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	�V�����_�[����
	//--------------------------------------------------------------------------------------
	CylinderObject::CylinderObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	CylinderObject::~CylinderObject() {}

	CYLINDER CylinderObject::GetCYLINDER()const {
		CYLINDER cy;
		cy.m_Radius = m_Scale.x;
		float halfY = m_Scale.y * 0.5f;
		cy.m_PointTop = bsm::Vec3(m_Pos.x, m_Pos.y + halfY, m_Pos.z);
		cy.m_PointBottom = bsm::Vec3(m_Pos.x, m_Pos.y - halfY, m_Pos.z);
		return cy;
	}


	void CylinderObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCylinder(1.0f, 2.0f,18, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CylinderMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void CylinderObject::OnUpdate() {
	}

	void CylinderObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_CylinderMesh,
			m_TextureResource,
			World,
			true
		);
	}






	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(0.25f, 0.25f, 0.25f),
		m_BaseY(m_Scale.y / 2.0f),
		m_Qt(),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Gravity(0,-9.8f,0),
		m_GravityVelocity(0,0,0),
		m_JumpLock(false),
		m_BeforePos(Pos),
		m_Mass(1.0f)
	{}
	SphereObject::~SphereObject() {}

	Vec3 SphereObject::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return Angle;
		}
		Vec3 CameraEye, CameraAt;
		ShPtrScene->GetCameraEyeAt(CameraEye, CameraAt);

		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
										//�i�s�����̌������v�Z
				Vec3 Front = m_Pos - CameraEye;
				Front.y = 0;
				Front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				Angle.normalize();
				//�ړ��T�C�Y��ݒ�B
				Angle *= MoveSize;
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}




	SPHERE SphereObject::GetSPHERE()const {
		SPHERE sp;
		sp.m_Center = m_Pos;
		sp.m_Radius =  m_Scale.y * 0.5f;
		return sp;
	}

	void SphereObject::CollisionWithCylinder(const Vec3& BeforePos) {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�Փ˔���
		auto ShPtrScene = m_Scene.lock();
		auto cyOb = ShPtrScene->GetCylinderObject();
		CYLINDER cy = cyOb->GetCYLINDER();
		SPHERE Sp = GetSPHERE();
		Sp.m_Center = BeforePos;
		float HitTime;
		//����̑��x
		Vec3 DestVelocity(0, 0, 0);
		Vec3 SrcVelocity = m_Pos - BeforePos;
		Vec3 CollisionVelosity = (SrcVelocity - DestVelocity) / ElapsedTime;
		if (HitTest::CollisionTestSphereCylinder(Sp, CollisionVelosity, cy, 0, ElapsedTime, HitTime)) {
			m_JumpLock = false;
			m_Pos = BeforePos + CollisionVelosity * HitTime;
			float SpanTime = ElapsedTime - HitTime;
			//m_Pos���������̂�SPHERE���Ď擾
			Sp = GetSPHERE();
			Vec3 HitPoint;
			//�ŋߐړ_�𓾂邽�߂̔���
			HitTest::SPHERE_CYLINDER(Sp, cy, HitPoint);
			//�Փ˖@����HitPoint��m_Pos���瓱��
			Vec3 Normal = m_Pos - HitPoint;
			Normal.normalize();
			Vec3 angle(XMVector3AngleBetweenNormals(Normal, Vec3(0, 1, 0)));
			if (angle.x <= 0.01f) {
				//���ʂ̏�
				m_GravityVelocity = Vec3(0, 0, 0);
			}
			else {
				//�d�͂��X���C�h�����Đݒ肷��
				//����ŁA�΂߂̃{�b�N�X�����藎����悤�ɂȂ�
				m_GravityVelocity = ProjUtil::Slide(m_GravityVelocity, Normal);
			}
			//���x���X���C�h�����Đݒ肷��
			m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
			//Y�����͏d�͂ɔC����
			m_Velocity.y = 0;
			//�Ō�ɏՓ˓_����]�������ԕ������V�����l�ňړ�������
			m_Pos = m_Pos + m_Velocity * SpanTime;
			m_Pos = m_Pos + m_GravityVelocity * SpanTime;
			//������x�Փ˔���
			//m_Pos���������̂�SPHERE���Ď擾
			Sp = GetSPHERE();
			if (HitTest::SPHERE_CYLINDER(Sp, cy, HitPoint)) {
				//�Փ˂��Ă�����ǂ��o������
				Vec3 EscapeNormal = Sp.m_Center - HitPoint;
				EscapeNormal.normalize();
				m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
			}
		}
	}


	void SphereObject::CollisionWithBoxes(const Vec3& BeforePos) {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�Փ˔���
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxVec()) {
			OBB Obb = v->GetOBB();
			SPHERE Sp = GetSPHERE();
			Sp.m_Center = BeforePos;
			float HitTime;
			//����̑��x
			Vec3 DestVelocity(0, 0, 0);
			auto MovBoxPtr = dynamic_pointer_cast<MoveBoxObject>(v);
			if (MovBoxPtr) {
				DestVelocity = MovBoxPtr->GetPosition() - MovBoxPtr->GetBeforePos();
				Obb.m_Center = MovBoxPtr->GetBeforePos();
			}
			Vec3 SrcVelocity = m_Pos - BeforePos;

			Vec3 CollisionVelosity = (SrcVelocity - DestVelocity) / ElapsedTime;
			if (HitTest::CollisionTestSphereObb(Sp, CollisionVelosity, Obb, 0, ElapsedTime, HitTime)) {
				m_JumpLock = false;
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Pos���������̂�SPHERE���Ď擾
				Sp = GetSPHERE();
				Vec3 HitPoint;
				//�ŋߐړ_�𓾂邽�߂̔���
				HitTest::SPHERE_OBB(Sp, Obb, HitPoint);
				//�Փ˖@����HitPoint��m_Pos���瓱��
				Vec3 Normal = m_Pos - HitPoint;
				Normal.normalize();
				Vec3 angle(XMVector3AngleBetweenNormals(Normal, Vec3(0, 1, 0)));
				if (angle.x <= 0.01f) {
					//���ʂ̏�
					m_GravityVelocity = Vec3(0, 0, 0);
				}
				else {
					//�d�͂��X���C�h�����Đݒ肷��
					//����ŁA�΂߂̃{�b�N�X�����藎����悤�ɂȂ�
					m_GravityVelocity = ProjUtil::Slide(m_GravityVelocity, Normal);
				}
				if (MovBoxPtr) {
					//���݂��ɔ�������
					Vec3 TgtVelo = CollisionVelosity * 0.5f;
					if (TgtVelo.length() < 1.0f) {
						//�Փˎ��̑��x��������������A���x�����o��
						TgtVelo = MovBoxPtr->GetPosition() - m_Pos;
						TgtVelo.normalize();
						TgtVelo *= 2.0f;
					}
					Vec3 DestVelo(XMVector3Reflect(-TgtVelo, Normal));
					DestVelo.y = 0;
					MovBoxPtr->SetVelocity(DestVelo);
					//���x�𔽔������Đݒ肷��
					m_Velocity = XMVector3Reflect(TgtVelo, -Normal);
				}
				else {
					//���x���X���C�h�����Đݒ肷��
					m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
				}
				//Y�����͏d�͂ɔC����
				m_Velocity.y = 0;
				//�Ō�ɏՓ˓_����]�������ԕ������V�����l�ňړ�������
				m_Pos = m_Pos + m_Velocity * SpanTime;
				m_Pos = m_Pos + m_GravityVelocity * SpanTime;
				//������x�Փ˔���
				//m_Pos���������̂�SPHERE���Ď擾
				Sp = GetSPHERE();
				if (HitTest::SPHERE_OBB(Sp, Obb, HitPoint)) {
					//�Փ˂��Ă�����ǂ��o������
					Vec3 EscapeNormal = Sp.m_Center - HitPoint;
					EscapeNormal.normalize();
					m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
				}
			}
		}
	}



	void SphereObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division,vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void SphereObject::OnUpdate() {
		//1�O�̈ʒu������Ă���
		m_BeforePos = m_Pos;
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//A�{�^��
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					m_BeforePos.y += 0.01f;
					m_Pos.y += 0.01f;
					m_GravityVelocity = Vec3(0, 4.0f, 0);
					m_JumpLock = true;
				}
			}
			Vec3 Direction = GetMoveVector();
			if (Direction.length() < 0.1f) {
				m_Velocity *= 0.9f;
			}
			else {
				m_Velocity = Direction * 5.0f;
			}
		}
		m_Pos += (m_Velocity * ElapsedTime);
		m_GravityVelocity += m_Gravity * ElapsedTime;
		m_Pos += m_GravityVelocity * ElapsedTime;
		if (m_Pos.y <= m_BaseY) {
			m_Pos.y = m_BaseY;
			m_GravityVelocity = Vec3(0, 0, 0);
			m_JumpLock = false;
		}
	}

	void SphereObject::OnCollision() {
		//�Փ˔���
		CollisionWithBoxes(m_BeforePos);
		CollisionWithCylinder(m_BeforePos);
	}

	void SphereObject::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		Vec3 Temp = m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//���݂ƖڕW����
		if (LerpFact >= 1.0f) {
			m_Qt = Qt;
		}
		else {
			m_Qt = XMQuaternionSlerp(m_Qt, Qt, LerpFact);
		}
	}

	void SphereObject::OnRotation() {
		//��]
		RotToHead(0.1f);
	}



	void SphereObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	�Œ�̃{�b�N�X����
	//--------------------------------------------------------------------------------------
	BoxObject::BoxObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, bool Trace, 
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		BoxBase(),
		m_Scene(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	BoxObject::~BoxObject() {}

	OBB BoxObject::GetOBB()const {
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		OBB obb(Vec3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}


	void BoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void BoxObject::OnUpdate() {
	}

	void BoxObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_BoxMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	�ړ��{�b�N�X����
	//--------------------------------------------------------------------------------------
	MoveBoxObject::MoveBoxObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, bool Trace,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		BoxBase(),
		m_Scene(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Mass(1.0f),
		m_Speed(4.0f),
		m_BeforePos(Pos)
	{}
	MoveBoxObject::~MoveBoxObject() {}

	OBB MoveBoxObject::GetOBB()const {
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		OBB obb(Vec3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}


	void MoveBoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_MoveBoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}

	void MoveBoxObject::UpdateVelosity() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�t�H�[�X�i�́j
		Vec3 Force(0, 0, 0);
		//�v���C���[�����������x�N�g��
		Vec3 ToPlayerVec = 
			ShPtrScene->GetSphereObject()->GetPosition() - m_Pos;
		//�c�����͌v�Z���Ȃ�
		ToPlayerVec.y = 0;
		ToPlayerVec *= m_Speed;
		//�͂��|������������߂�
		Force = ToPlayerVec - m_Velocity;
		//�͂Ǝ��ʂ�����������߂�
		Vec3 Accel = Force / m_Mass;
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//���x����������
		m_Velocity += Accel * ElapsedTime;
	}

	void MoveBoxObject::CollisionWithBoxes(const Vec3& BeforePos) {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�Փ˔���
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxVec()) {
			if (v == GetThis<BoxBase>()) {
				//���肪�������g�Ȃ珈�����Ȃ�
				continue;
			}
			OBB DestObb = v->GetOBB();
			OBB SrcObb = GetOBB();
			SrcObb.m_Center = BeforePos;
			float HitTime;
			Vec3 CollisionVelosity = (m_Pos - BeforePos) / ElapsedTime;
			if (HitTest::CollisionTestObbObb(SrcObb, CollisionVelosity, DestObb, 0, ElapsedTime, HitTime)) {
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Pos���������̂�OBB���Ď擾
				SrcObb = GetOBB();
				Vec3 HitPoint;
				//�ŋߐړ_�𓾂邽�߂̔���
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, HitPoint);
				//�Փ˖@����HitPoint��m_Pos���瓱��
				Vec3 Normal = m_Pos - HitPoint;
				Normal.normalize();
				//���x���X���C�h�����Đݒ肷��
				m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
				//Y�����͂Ȃ�
				m_Velocity.y = 0;
				//�Ō�ɏՓ˓_����]�������ԕ������V�����l�ňړ�������
				m_Pos = m_Pos + m_Velocity * SpanTime;
				//�ǂ��o������
				//�����Â���̗̈悩��ޔ�����
				//�ő�10��ޔ����邪�A����ł��Փ˂��Ă����玟��^�[���ɔC����
				int count = 0;
				while (count < 20) {
					//�ޔ�����W��
					float MiniSpan = 0.001f;
					//������x�Փ˔���
					//m_Pos���������̂�OBB���Ď擾
					SrcObb = GetOBB();
					if (HitTest::OBB_OBB(SrcObb, DestObb)) {
						//�ŋߐړ_�𓾂邽�߂̔���
						HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, HitPoint);
						//�Փ˂��Ă�����ǂ��o������
						Vec3 EscapeNormal = SrcObb.m_Center - HitPoint;
						EscapeNormal.y = 0;
						EscapeNormal.normalize();
						m_Pos = m_Pos + EscapeNormal * MiniSpan;
					}
					else {
						break;
					}
					count++;
				}
			}
		}
	}

	void MoveBoxObject::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		Vec3 Temp = m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//���݂ƖڕW����
		if (LerpFact >= 1.0f) {
			m_Qt = Qt;
		}
		else {
			m_Qt = XMQuaternionSlerp(m_Qt, Qt, LerpFact);
		}
	}

	void MoveBoxObject::OnUpdate() {
		//1�O�̈ʒu������Ă���
		m_BeforePos = m_Pos;
		//���x��ω�������
		UpdateVelosity();
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//���x�ɍ��킹�Ĉʒu�̕ύX
		m_Pos += m_Velocity * ElapsedTime;
	}

	void MoveBoxObject::OnCollision() {
		//�Փ˔���
		CollisionWithBoxes(m_BeforePos);
	}

	void MoveBoxObject::OnRotation() {
		//��]
		RotToHead(0.1f);
	}



	void MoveBoxObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_MoveBoxMesh,
			m_TextureResource,
			World,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	PNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Scene> PtrScene) :
		m_Scene(PtrScene)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		m_DrawObjectVec.push_back(Obj);
	}

	void PNTDrawObject::OnUpdate() {
		m_DrawObjectVec.clear();
	}

	void PNTDrawObject::OnDraw() {
		if (m_Scene.expired()) {
			//�V�[���������Ȃ烊�^�[��
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�e�I�u�W�F�N�g���ʏ���
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�T���v���[�̏���
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//�s��̒�`
		Mat4x4 View, Proj;
		//���C�e�B���O
		Vec4 LightDir;
		auto ShPtrScene = m_Scene.lock();
		ShPtrScene->GetViewProjMatrix(View, Proj);
		ShPtrScene->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		//�]�u����
		Proj.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�B
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//�ʏ���
		for (auto& v : m_DrawObjectVec) {
			//�]�u����
			v.m_WorldMatrix.transpose();
			//���[���h�s��̌���
			sb.World = v.m_WorldMatrix;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�N�X�`���̐ݒ�
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//�T���v���[
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//�u�����h�X�e�[�g
			if (v.m_Trace) {
				//��������
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//���������̏ꍇ�́A���X�^���C�U�X�e�[�g��ύX����2��`��
				//���X�^���C�U�X�e�[�g�i���ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//�����������Ȃ�
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
		//��n��
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	���b�s���O�������ꂽ�X�v���C�g
	//--------------------------------------------------------------------------------------
	WrappedSprite::WrappedSprite(const wstring& TextureFileName, bool Trace, 
		const Vec2& StartScale, const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(StartScale),
		m_Rot(0),
		m_Pos(StartPos),
		m_XWrap(XWrap),
		m_YWrap(YWrap),
		m_TotalTime(0)
	{}
	WrappedSprite::~WrappedSprite() {}
	void WrappedSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��10���\��)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,0,0,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(0, 1.0f, 0, 1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(0, 0, 1.0f, 1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 0, 1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬�i�ύX�ł���j
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
	}

	void WrappedSprite::UpdateVertex(float ElapsedTime) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 1.0f) {
			m_TotalTime = 0;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//���_�̕ύX
		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//���_�̕ύX
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vec2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);

		}
		//�A���}�b�v
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);

	}


	void WrappedSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Rot += ElapsedTime;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}
		UpdateVertex(ElapsedTime);
	}



	void WrappedSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//�s��̒�`
		Mat4x4 World, Proj;
		//���[���h�s��̌���
		World.affineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vec2(0, 0),		//��]�̒��S�i�d�S�j
			m_Rot,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixOrthographicLH(w, h, -1.0, 1.0f);
		//�s��̍���
		World *= Proj;

		//�R���X�^���g�o�b�t�@�̏���
		SpriteConstantBuffer sb;
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = Col4(0, 0, 0, 0);
		//�s��̐ݒ�
		sb.World = World;
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		//�u�����h�X�e�[�g
		if (m_Trace) {
			//��������
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else {
			//�����������Ȃ�
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//���X�^���C�U�X�e�[�g
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		//���b�s���O�T���v���[
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//�`��
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}

	

	EnemyObject::EnemyObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(0.25f, 0.25f, 0.25f),
		m_BaseY(m_Scale.y / 2.0f),
		m_Qt(),
		m_Pos(Pos),
		m_Velocity(0, 0, 0),
		m_Gravity(0, -9.8f, 0),
		m_GravityVelocity(0, 0, 0),
		m_JumpLock(false),
		m_Speed(4.0f),
		m_BeforePos(Pos),
		m_Mass(1.0f)
	{}
	EnemyObject::~EnemyObject() {}

	OBB EnemyObject::GetOBB()const {
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		OBB obb(Vec3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}

	void EnemyObject::CollisionWithBoxes(const Vec3 & BeforePos)
	{
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�Փ˔���
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxVec())
		{
			//if (v == GetThis<BoxBase>())
			//{
			//	//���肪�������g�Ȃ珈�����Ȃ�
			//	continue;
			//}
			OBB DestObb = v->GetOBB();
			OBB SrcObb = GetOBB();
			SrcObb.m_Center = BeforePos;
			float HitTime;
			Vec3 CollisionVelosity = (m_Pos - BeforePos) / ElapsedTime;
			if (HitTest::CollisionTestObbObb(SrcObb, CollisionVelosity, DestObb, 0, ElapsedTime, HitTime))
			{
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Pos���������̂�OBB���Ď擾
				SrcObb = GetOBB();
				Vec3 HitPoint;
				//�ŋߐړ_�𓾂邽�߂̔���
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, HitPoint);
				//�Փ˖@����HitPoint��m_Pos���瓱��
				Vec3 Normal = m_Pos - HitPoint;
				Normal.normalize();
				//���x���X���C�h�����Đݒ肷��
				m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
				//Y�����͂Ȃ�
				m_Velocity.y = 0;
				//�Ō�ɏՓ˓_����]�������ԕ������V�����l�ňړ�������
				m_Pos = m_Pos + m_Velocity * SpanTime;
				//�ǂ��o������
				//�����Â���̗̈悩��ޔ�����
				//�ő�10��ޔ����邪�A����ł��Փ˂��Ă����玟��^�[���ɔC����
				int count = 0;
				while (count < 20)
				{
					//�ޔ�����W��
					float MiniSpan = 0.001f;
					//������x�Փ˔���
					//m_Pos���������̂�OBB���Ď擾
					SrcObb = GetOBB();
					if (HitTest::OBB_OBB(SrcObb, DestObb))
					{
						//�ŋߐړ_�𓾂邽�߂̔���
						HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, HitPoint);
						//�Փ˂��Ă�����ǂ��o������
						Vec3 EscapeNormal = SrcObb.m_Center - HitPoint;
						EscapeNormal.y = 0;
						EscapeNormal.normalize();
						m_Pos = m_Pos + EscapeNormal * MiniSpan;
					}
					else
					{
						break;
					}
					count++;
				}
			}
		}
	}

	void EnemyObject::CollisionWithCylinder(const Vec3 & BeforePos)
	{
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�Փ˔���
		auto ShPtrScene = m_Scene.lock();
		auto cyOb = ShPtrScene->GetCylinderObject();
		CYLINDER cy = cyOb->GetCYLINDER();
		SPHERE Sp = GetSPHERE();
		Sp.m_Center = BeforePos;
		float HitTime;
		//����̑��x
		Vec3 DestVelocity(0, 0, 0);
		Vec3 SrcVelocity = m_Pos - BeforePos;
		Vec3 CollisionVelosity = (SrcVelocity - DestVelocity) / ElapsedTime;
		if (HitTest::CollisionTestSphereCylinder(Sp, CollisionVelosity, cy, 0, ElapsedTime, HitTime)) {
			m_JumpLock = false;
			m_Pos = BeforePos + CollisionVelosity * HitTime;
			float SpanTime = ElapsedTime - HitTime;
			//m_Pos���������̂�SPHERE���Ď擾
			Sp = GetSPHERE();
			Vec3 HitPoint;
			//�ŋߐړ_�𓾂邽�߂̔���
			HitTest::SPHERE_CYLINDER(Sp, cy, HitPoint);
			//�Փ˖@����HitPoint��m_Pos���瓱��
			Vec3 Normal = m_Pos - HitPoint;
			Normal.normalize();
			Vec3 angle(XMVector3AngleBetweenNormals(Normal, Vec3(0, 1, 0)));
			if (angle.x <= 0.01f) {
				//���ʂ̏�
				m_GravityVelocity = Vec3(0, 0, 0);
			}
			else {
				//�d�͂��X���C�h�����Đݒ肷��
				//����ŁA�΂߂̃{�b�N�X�����藎����悤�ɂȂ�
				m_GravityVelocity = ProjUtil::Slide(m_GravityVelocity, Normal);
			}
			//���x���X���C�h�����Đݒ肷��
			m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
			//Y�����͏d�͂ɔC����
			m_Velocity.y = 0;
			//�Ō�ɏՓ˓_����]�������ԕ������V�����l�ňړ�������
			m_Pos = m_Pos + m_Velocity * SpanTime;
			m_Pos = m_Pos + m_GravityVelocity * SpanTime;
			//������x�Փ˔���
			//m_Pos���������̂�SPHERE���Ď擾
			Sp = GetSPHERE();
			if (HitTest::SPHERE_CYLINDER(Sp, cy, HitPoint)) {
				//�Փ˂��Ă�����ǂ��o������
				Vec3 EscapeNormal = Sp.m_Center - HitPoint;
				EscapeNormal.normalize();
				m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
			}
		}
	}
	void EnemyObject::RotToHead(float LerpFact)
	{
	}
	SPHERE EnemyObject::GetSPHERE() const
	{
		SPHERE sp;
		sp.m_Center = m_Pos;
		sp.m_Radius = m_Scale.y * 0.5f;
		return sp;
	}

	void EnemyObject::UpdateVelosity() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�t�H�[�X�i�́j
		Vec3 Force(0, 0, 0);
		//�v���C���[�����������x�N�g��
		Vec3 ToPlayerVec =
			ShPtrScene->GetSphereObject()->GetPosition() - m_Pos;
		//�c�����͌v�Z���Ȃ�
		ToPlayerVec.y = 0;
		ToPlayerVec *= m_Speed;
		//�͂��|������������߂�
		Force = ToPlayerVec - m_Velocity;
		//�͂Ǝ��ʂ�����������߂�
		Vec3 Accel = Force / m_Mass;
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//���x����������
		m_Velocity += Accel * ElapsedTime;
	}

	void EnemyObject::OnCreate()
	{
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void EnemyObject::OnUpdate()
	{
		//1�O�̈ʒu������Ă���
		m_BeforePos = m_Pos;
		//���x��ω�������
		UpdateVelosity();
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//���x�ɍ��킹�Ĉʒu�̕ύX
		m_Pos += m_Velocity * ElapsedTime;
	}
	void EnemyObject::OnCollision()
	{
		CollisionWithBoxes(m_BeforePos);
		CollisionWithCylinder(m_BeforePos);
	}
	void EnemyObject::OnRotation()
	{
	}
	void EnemyObject::OnDraw()
	{
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Mat4x4 World;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			m_TextureResource,
			World,
			true
		);
	}
}
//end basecross
