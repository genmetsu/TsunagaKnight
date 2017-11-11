/*!
@file Player.cpp
@brief �v���C���[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���̂̃v���C���[����
	//--------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_BaseY(0.25f / 2.0f),
		m_Posision(Pos),
		m_FrameCount(0.0f),
		m_isStep(false),
		m_JumpLock(false)
	{}
	Player::~Player() {}

	Vec3 Player::GetMoveVector() {
		Vec3 Angle(0, 0, 0);
		auto PtrGameStage = GetStage<GameStage>();
		Vec3 CameraEye, CameraAt;
		PtrGameStage->GetCamera().GetCameraEyeAt(CameraEye, CameraAt);

		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
										//�i�s�����̌������v�Z
				Vec3 Front = m_Rigidbody->m_Pos - CameraEye;
				Front.y = 0;
				Front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = length(MoveVec);
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

	Vec3 Player::GetPosition() {
		return m_Rigidbody->m_Pos;
	}

	void Player::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�^�O�̒ǉ�
		AddTag(L"Player");
		//Rigidbody�̏�����
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = Vec3(0.25f);
		body.m_Quat = Quat();
		body.m_Pos = m_Posision;
		body.m_CollType = CollType::typeSPHERE;
//		body.m_IsDrawActive = true;
		body.SetToBefore();

		m_StepVec = Vec3(0.0f);

		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			body.m_Scale,
			Vec3(0, 0, 0),
			body.m_Quat,
			body.m_Pos
		);

		m_PtrObj = make_shared<BcDrawObject>();
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;

	}


	void Player::OnUpdate() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//if (!m_JumpLock) {
				//A�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A && m_isStep == false) {
				
				//m_Rigidbody->m_Force += Direction * 1000.0f;
				m_isStep = true;

				m_StepVec = GetMoveVector();
				m_StepVec.normalize();

				//�G�t�F�N�g�̍Đ�
				auto FirePtr = GetStage<GameStage>()->FindTagGameObject<StepEffect>(L"StepEffect");
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= 0.125f;
				FirePtr->InsertEffect(Emitter);
			}
				
			//}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				m_Rigidbody->m_Velocity.x *= 0.8f;
				m_Rigidbody->m_Velocity.z *= 0.8f;
			}
			if (m_isStep == true) {
				m_Rigidbody->m_Velocity += m_StepVec * 2.0f;

				//Vec2 TempVelo(m_Rigidbody->m_Velocity.x, m_Rigidbody->m_Velocity.z);
				//TempVelo = XMVector2ClampLength(TempVelo, 0, 100.0f);
				//m_Rigidbody->m_Velocity.x = TempVelo.x;
				//m_Rigidbody->m_Velocity.z = TempVelo.y;

				m_FrameCount++;
			}
			else {
				//�t�H�[�X�ŕύX����ꍇ�͈ȉ��̂悤�ɋL�q
				//body.m_Force += Direction * 10.0f;
				//���x�ŕύX����ꍇ�͈ȉ��̂悤�ɋL�q
				m_Rigidbody->m_Velocity += Direction * 0.5f;
				Vec2 TempVelo(m_Rigidbody->m_Velocity.x, m_Rigidbody->m_Velocity.z);
				TempVelo = XMVector2ClampLength(TempVelo, 0, 5.0f);
				m_Rigidbody->m_Velocity.x = TempVelo.x;
				m_Rigidbody->m_Velocity.z = TempVelo.y;
				
			}
		}
		if (m_FrameCount >= 10.0f) {
			m_isStep = false;
			m_FrameCount = 0.0f;
		}
		m_Rigidbody->m_Force += m_Rigidbody->m_Gravity * m_Rigidbody->m_Mass;
	}

	void Player::OnUpdate2() {
		if (m_Rigidbody->m_Pos.y <= m_BaseY) {
			m_Rigidbody->m_Pos.y = m_BaseY;
			m_Rigidbody->m_Velocity.y = 0;
			if (m_JumpLock) {
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= 0.125f;
				//Spaerk�̑��o
				auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
				SpaerkPtr->InsertSpark(Emitter);
			}
			m_JumpLock = false;
		}
		auto& StateVec = GetStage<GameStage>()->GetCollisionStateVec();
		for (auto& v : StateVec) {
			if (v.m_Src == m_Rigidbody.get()) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					if (m_JumpLock) {
						Vec3 Emitter = m_Rigidbody->m_Pos;
						Emitter.y -= 0.125f;
						//Spaerk�̑��o
						auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
						SpaerkPtr->InsertSpark(Emitter);
					}
					m_JumpLock = false;
					break;
				}
			}
			else if (v.m_Dest == m_Rigidbody.get()) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					if (m_JumpLock) {
						Vec3 Emitter = m_Rigidbody->m_Pos;
						Emitter.y -= 0.125f;
						//Spaerk�̑��o
						auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
						SpaerkPtr->InsertSpark(Emitter);
					}
					m_JumpLock = false;
					break;
				}
			}
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		Vec3 Temp = m_Rigidbody->m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//���݂ƖڕW����
		m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);


/*

		auto LenVec = m_Rigidbody->m_Pos - m_Rigidbody->m_BeforePos;
		LenVec.y = 0;
		auto Len = LenVec.length();
		if (Len > 0) {
			Vec3 Cross = cross(Vec3(0, 1, 0), LenVec);
			Quat Span(Cross, Len / 0.5f);
			m_Rigidbody->m_Quat *= Span;
		}
*/
	}

	void Player::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void Player::OnDraw() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	void Player::GetWorldMatrix(Mat4x4& m) const {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m = World;

	}

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̎���
	//--------------------------------------------------------------------------------------

	Sword::Sword(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<GameObject>& ParentPtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_ParentPtr(ParentPtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive),
		m_LerpToParent(0.2f),
		m_LerpToChild(0.2f),
		m_Attack1ToRot(0)
	{}
	Sword::~Sword() {}

	void Sword::OnCreate() {
		//�^�O�̒ǉ�
		AddTag(L"Sword");

		//Rigidbody�̏�����
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = true;
		body.m_IsFixed = false;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Sword>(GetThis<Sword>()));
		//�X�e�[�g�����l�ݒ�
		m_StateMachine->ChangeState(NonAttackState::Instance());

	}

	void Sword::OnUpdate() {
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
	}

	void Sword::OnUpdate2() {
		

		auto& StateVec = GetStage<GameStage>()->GetCollisionStateVec();
		for (auto& v : StateVec) {
			if (v.m_Src == m_Rigidbody.get()) {
				vector<shared_ptr<GameObject>> EnemyVec;
				GetStage<GameStage>()->FindTagGameObjectVec(L"EnemyObject", EnemyVec);

				for (auto v : EnemyVec) {
					if (v) {
						auto Ptr = dynamic_pointer_cast<EnemyObject>(v);
						Vec3 EnemyPos = Ptr->GetPosition();
						float length = (EnemyPos - m_Rigidbody->m_Pos).length();
						if (length < 0.3f) {
							Vec3 Emitter = m_Rigidbody->m_Pos;
							Emitter.y -= 0.125f;
							//Spaerk�̑��o
							auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
							SpaerkPtr->InsertSpark(Emitter);
						}
					}
				}
				break;

			}
			else if (v.m_Dest == m_Rigidbody.get()) {
				vector<shared_ptr<GameObject>> EnemyVec;
				GetStage<GameStage>()->FindTagGameObjectVec(L"EnemyObject", EnemyVec);

				for (auto v : EnemyVec) {

					if (v) {
						auto Ptr = dynamic_pointer_cast<EnemyObject>(v);
						Vec3 EnemyPos = Ptr->GetPosition();
						float length = (EnemyPos - m_Rigidbody->m_Pos).length();
						if (length < 0.3f) {
							Vec3 Emitter = m_Rigidbody->m_Pos;
							Emitter.y -= 0.125f;
							//Spaerk�̑��o
							auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
							SpaerkPtr->InsertSpark(Emitter);
						}
					}
				}
				break;
			}
		}
	}


	void Sword::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void Sword::OnDraw() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	void Sword::GetWorldMatrix(Mat4x4& m) const {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m = World;
	}

	enum class ParentFlg {
		NoParent,
		Player,
		Child
	};

	void Sword::UpdateBehavior() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_ParentPtr.lock();
		//�e�̃��[���h�s����擾����ϐ�
		Mat4x4 ParMat;
		if (shptr) {
			ParentFlg flg = ParentFlg::NoParent;
			//�s��擾�p�̃C���^�[�t�F�C�X�������Ă邩�ǂ���
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
				if (shptr->FindTag(L"Player")) {
					flg = ParentFlg::Player;
				}
				else if (shptr->FindTag(L"Sword")) {
					flg = ParentFlg::Child;
				}
			}
			Mat4x4 World;
			World.identity();
			float LerpNum = 0.2f;
			switch (flg) {
			case ParentFlg::Player:
				//�s��̒�`
				World = m_PlayerLocalMatrix;
				LerpNum = m_LerpToParent;
				break;
			case ParentFlg::Child:
				//�s��̒�`
				World = m_ChildLocalMatrix;
				LerpNum = m_LerpToChild;
				break;
			default:
				break;
			}
			if (flg != ParentFlg::NoParent) {
				//�X�P�[�����O��1.0�ɂ����s��ɕϊ�
				ParMat.scaleIdentity();
				//�s��̔��f
				World *= ParMat;
				//���̎��_��World�͖ڕW�ƂȂ�ʒu
				Vec3 toPos = World.transInMatrix();
				Vec3 DammiPos;
				World.decompose(m_Rigidbody->m_Scale, m_Rigidbody->m_Quat, DammiPos);
				Vec3 Velo = toPos - m_Rigidbody->m_Pos;
				Velo /= ElapsedTime;
				m_Rigidbody->m_Velocity = Velo;
			}
		}
	}


	void Sword::ComplianceStartBehavior() {
		//���[�J���s��̒�`
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, 0, -0.25f)
		);
		//���̃X�e�[�g�ł̓`���C���h�̏ꍇ������
		m_ChildLocalMatrix = m_PlayerLocalMatrix;
		m_LerpToParent = m_LerpToChild = 0.2f;
	}

	//�U���P�s���̊J�n
	void Sword::Attack1StartBehavior() {
		m_Attack1ToRot = 0.1f;
		//���[�J���s��̒�`
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(Vec3(1.0, 0, 0), m_Attack1ToRot),
			Vec3(0, 0.25f, 0.0f)
		);
		m_ChildLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, 0.25f, -0.25f)
		);
		m_LerpToParent = m_LerpToChild = 0.5f;

	}

	bool Sword::Attack1ExcuteBehavior() {
		m_Attack1ToRot += 0.2f;
		if (m_Attack1ToRot >= (XM_PI + 0.5f)) {
			m_Attack1ToRot = 0.0f;
			return true;
		}
		//���[�J���s��̒�`
		Vec3 Pos(0, sin(m_Attack1ToRot) * 0.25f, -cos(m_Attack1ToRot) * 0.25f);
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(Vec3(1.0, 0, 0), m_Attack1ToRot),
			Pos
		);
		return false;
	}




	//--------------------------------------------------------------------------------------
	///	�Ǐ]�X�e�[�g�iSword�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NonAttackState)

		void NonAttackState::Enter(const shared_ptr<Sword>& Obj) {
		Obj->ComplianceStartBehavior();
		//�������Ȃ�
	}

	void NonAttackState::Execute(const shared_ptr<Sword>& Obj) {
		Obj->UpdateBehavior();

		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//X�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
				Obj->GetStateMachine()->ChangeState(Attack1State::Instance());
			}
		}

	}

	void NonAttackState::Exit(const shared_ptr<Sword>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�U���X�e�[�g�P�iSword�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Attack1State)

		void Attack1State::Enter(const shared_ptr<Sword>& Obj) {
		Obj->Attack1StartBehavior();
		//�������Ȃ�
	}

	void Attack1State::Execute(const shared_ptr<Sword>& Obj) {
		if (Obj->Attack1ExcuteBehavior()) {
			Obj->GetStateMachine()->ChangeState(NonAttackState::Instance());
			return;
		}
		Obj->UpdateBehavior();
	}

	void Attack1State::Exit(const shared_ptr<Sword>& Obj) {
		//�������Ȃ�
	}


}
//end basecross
