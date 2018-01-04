/*!
@file Player.cpp
@brief �v���C���[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	GameManager;

	//--------------------------------------------------------------------------------------
	///	�v���C���[����
	//--------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_BaseY(0.15f),
		m_Posision(Pos),
		m_FrameCount(0.0f),
		m_isStep(false),
		m_AttackDis(2.0f),
		m_FOV(0.707f),
		m_NowCannonClass(3),
		m_JumpLock(false)
	{
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);
	}
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
		body.m_Scale = Vec3(0.15f);
		body.m_Quat = Quat();
		body.m_Pos = m_Posision;
		body.m_CollType = CollType::typeCAPSULE;
		//body.m_IsDrawActive = true;
		body.m_IsFixed = false;
		body.SetToBefore();

		m_StepVec = Vec3(0.0f);
		m_KnockBackVec = Vec3(0.0f);
		m_RunAnimationFrameCount = 0.0f;

		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			body.m_Scale,
			Vec3(0, 0, 0),
			body.m_Quat,
			body.m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = App::GetApp()->GetResource<MeshResource>(L"KUREHA_MESH");;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;

		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 30, true, 40.0f);
		m_PtrObj->AddAnimation(L"RunStart", 30, 20, false, 60.0f);
		m_PtrObj->AddAnimation(L"Running", 50, 40, true, 60.0f);
		m_PtrObj->AddAnimation(L"RunEnd", 90, 20, false, 60.0f);
		m_PtrObj->AddAnimation(L"Attack", 120, 50, false, 90.0f);
		m_PtrObj->AddAnimation(L"Damage", 170, 30, false, 60.0f);
		m_PtrObj->AddAnimation(L"Step", 200, 30, false, 90.0f);

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_PtrObj->m_MeshRes;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//�X�e�[�g�����l�ݒ�
		m_StateMachine->ChangeState(DefaultState::Instance());
	}


	void Player::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);
		m_StateMachine->Update();
	}

	void Player::OnUpdate2() {
		if (m_Rigidbody->m_Pos.y <= m_BaseY) {
			m_Rigidbody->m_Pos.y = m_BaseY;
			m_Rigidbody->m_Velocity.y = 0;
			if (m_JumpLock) {
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= m_Rigidbody->m_Scale.y / 2.0f;;
				//Spaerk�̑��o
				auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
				SpaerkPtr->InsertSpark(Emitter);
			}
			m_JumpLock = false;
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
		//�ړ����Ȃ��Ƃ��͉�]���Ȃ�
		if (m_Rigidbody->m_Velocity.length() > 0.01f) {
			m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
		}
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
		//�������v�Z
		World = m_MeshToTransformMatrix * World;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_pLocalBoneVec = &m_PtrObj->m_LocalBonesMatrix;
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

		//�������v�Z
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
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

	void Player::MoveControll() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			Vec3 Direction = GetMoveVector();
			//A�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				m_StateMachine->ChangeState(StepState::Instance());
				return;
			}
			else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
				m_StateMachine->ChangeState(PlayerAttackState::Instance());
				return;
			}
			else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y) {
				auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
				s->SetEnemyToCannon(L"Green");
			}
			else if (length(Direction) < 0.1f) {
				m_Rigidbody->m_Velocity.x *= 0.8f;
				m_Rigidbody->m_Velocity.z *= 0.8f;
				
			}
			else {
				m_Rigidbody->m_Velocity += Direction * 0.5f;
				Vec2 TempVelo(m_Rigidbody->m_Velocity.x, m_Rigidbody->m_Velocity.z);
				TempVelo = XMVector2ClampLength(TempVelo, 0, 5.0f);
				m_Rigidbody->m_Velocity.x = TempVelo.x;
				m_Rigidbody->m_Velocity.z = TempVelo.y;
				
			}
			m_Rigidbody->m_Force += m_Rigidbody->m_Gravity * m_Rigidbody->m_Mass;
		}
	}

	void Player::Damage(Vec3 EnemyPos) {
		
	}

	void Player::ChangeAnimation(wstring animation_key) {
		m_PtrObj->ChangeCurrentAnimation(animation_key);
	}

	void Player::RunningAnimation() {
		Vec3 Direction = GetMoveVector();
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (Direction.length() < 0.1f) {
			if (isRunning != RunEnd) {
				isRunning = RunEnd;
				ChangeAnimation(L"RunEnd");
			}
		}
		else if (m_RunAnimationFrameCount > 0.333f) {
			ChangeAnimation(L"Running");
			isRunning = Running;
			m_RunAnimationFrameCount = 0.0f;
		}
		else {
			if (isRunning == RunEnd) {
				isRunning = RunStart;
				ChangeAnimation(L"RunStart");
				m_RunAnimationFrameCount = 0.0f;
			}
		}
		if (isRunning == RunStart) {
			m_RunAnimationFrameCount += ElapsedTime;
		}
	}

	void Player::ChangeDefaultState() {
		m_StateMachine->ChangeState(DefaultState::Instance());
	}

	void Player::DefaultBehaviour() {

		RunningAnimation();
		MoveControll();
		
		//�G�Ƃ̓����蔻��
		vector<shared_ptr<GameObject>> EnemyVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"EnemyObject", EnemyVec);
		for (auto enemy : EnemyVec) {
			if (enemy) {
				auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);

				Vec3 EnemyPos = PtrEnemy->GetPosition();
				float length = (EnemyPos - m_Rigidbody->m_Pos).length();

				float EnemyRadius = PtrEnemy->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x;

				if (length <= EnemyRadius + PlayerRadius) {
					if (PtrEnemy->GetHP() > 0) {
						Vec3 Emitter = m_Rigidbody->m_Pos;
						Emitter.y -= 0.125f;
						//Spark�̑��o
						auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
						SparkPtr->InsertSpark(Emitter);
						//�m�b�N�o�b�N�����̐ݒ�
						m_KnockBackVec = m_Rigidbody->m_Pos - EnemyPos;
						m_KnockBackVec.y = 0.0f;
						m_KnockBackVec.normalize();
						//�_���[�W�X�e�[�g�ɕύX
						m_StateMachine->ChangeState(DamagedState::Instance());
						return;
					}
				}
			}
		}
		//��C�Ƃ̏Փ˔���
		vector<shared_ptr<GameObject>> CannonVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Cannon", CannonVec);
		for (auto cannon : CannonVec) {
			if (cannon) {
				auto PtrCannon = dynamic_pointer_cast<Cannon>(cannon);

				Vec3 CannonPos = PtrCannon->GetPosition();
				float length = (CannonPos - m_Rigidbody->m_Pos).length();

				float CannonRadius = PtrCannon->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

				if (length < CannonRadius + PlayerRadius) {

					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y -= 0.125f;
					//Spark�̑��o
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
					SparkPtr->InsertSpark(Emitter);

					if (PtrCannon->GetCannonClass() == 0) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						s->SetEnemyToCannon(L"Green");
						m_NowCannonClass = 0;
					}
					if (PtrCannon->GetCannonClass() == 1) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						s->SetEnemyToCannon(L"Red");
						m_NowCannonClass = 1;
					}
					if (PtrCannon->GetCannonClass() == 2) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						s->SetEnemyToCannon(L"Blue");
						m_NowCannonClass = 2;
					}
				}
			}
		}
		//�e�Ƃ̏Փ˔���
		vector<shared_ptr<GameObject>> BulletVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Bullet", BulletVec);
		for (auto bullet : BulletVec) {
			if (bullet) {
				auto PtrBullet = dynamic_pointer_cast<BulletObject>(bullet);

				Vec3 BulletPos = PtrBullet->GetPosition();
				float length = (BulletPos - m_Rigidbody->m_Pos).length();

				float Radius = PtrBullet->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

				if (length < Radius + PlayerRadius) {

					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y -= 0.125f;
					//Spark�̑��o
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
					SparkPtr->InsertSpark(Emitter);
					//�m�b�N�o�b�N�����̐ݒ�
					m_KnockBackVec = m_Rigidbody->m_Pos - BulletPos;
					m_KnockBackVec.y = 0.0f;
					m_KnockBackVec.normalize();
					//�_���[�W�X�e�[�g�ɕύX
					m_StateMachine->ChangeState(DamagedState::Instance());
					return;
				}
			}
		}
	}

	void Player::DamagedBehaviour() {
		m_Rigidbody->m_Velocity += m_KnockBackVec * 0.7f;
	}

	void Player::PlayerStepEffect() {
		//�G�t�F�N�g�̍Đ�
		auto FirePtr = GetStage<GameStage>()->FindTagGameObject<StepEffect>(L"StepEffect");
		Vec3 Emitter = m_Rigidbody->m_Pos;
		Emitter.y -= m_Rigidbody->m_Scale.y / 2.0f;;
		FirePtr->InsertEffect(Emitter);
	}

	void Player::InitVelocity() {
		m_Rigidbody->m_Velocity.x *= 0.01f;
		m_Rigidbody->m_Velocity.z *= 0.01f;
	}

	void Player::StepBehaviour() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		m_Rigidbody->m_Velocity += m_StepVec * (1.0f - m_FrameCount);
		m_FrameCount += ElapsedTime;

		if (m_FrameCount >= 0.3f) {
			m_StateMachine->ChangeState(DefaultState::Instance());
			m_FrameCount = 0.0f;
		}
		m_Rigidbody->m_Force += m_Rigidbody->m_Gravity * m_Rigidbody->m_Mass;
	}

	void Player::AttackBehaviour() {
		auto sword = GetStage()->FindTagGameObject<Sword>(L"Sword");
		sword->SetState(L"Attack");

		//�U�����̈ړ��̕␳
		vector<shared_ptr<GameObject>> EnemyVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Zako", EnemyVec);

		//�ŒZ�����ƕ���
		float ShortDis = 0.0f;
		Vec3 MoveVec = Vec3(0,0,0);

		//�����̌���
		Vec3 MyVec = m_Rigidbody->m_Velocity;
		MyVec.normalize();

		//�G�̃|�W�V���������A������ɂ���ŒZ���������߂�
		for (auto enemy : EnemyVec) {
			if (enemy) {
				auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
				Vec3 EnemyPos = PtrEnemy->GetPosition();
				Vec3 ToVec = EnemyPos - m_Rigidbody->m_Pos;
				ToVec.normalize();
				float length = (EnemyPos - m_Rigidbody->m_Pos).length();

				if (ShortDis == 0.0f) {
					ShortDis = length;
					MoveVec = ToVec;
				}

				//�����̎���ɓG�����āA���̋������ŒZ�������ꍇShortDis���㏑������
				float angle = dot(MyVec, ToVec);
				if (angle > m_FOV) {
					if (ShortDis > length) {
						ShortDis = length;
						MoveVec = ToVec;
					}
				}
			}
		}
		
		//�G���߂���Ε␳��������
		if (ShortDis < m_AttackDis) {
			//�����̃x�N�g�����ŒZ�̓G�Ɍ����킹��
			m_Rigidbody->m_Velocity.x = MoveVec.x * ShortDis * 1.5f;
			m_Rigidbody->m_Velocity.z = MoveVec.z * ShortDis * 1.5f;
		}
		else {
			m_Rigidbody->m_Velocity.x *= 0.1f;
			m_Rigidbody->m_Velocity.z *= 0.1f;
		}
	}

	//--------------------------------------------------------------------------------------
	///	�ʏ�̃X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(DefaultState)

	void DefaultState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChangeAnimation(L"Default");
		Obj->InitRunFrameCount();
	}

	void DefaultState::Execute(const shared_ptr<Player>& Obj) {
		Obj->DefaultBehaviour();
	}

	void DefaultState::Exit(const shared_ptr<Player>& Obj) {

	}

	//--------------------------------------------------------------------------------------
	///	�U�����̃X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAttackState)

	void PlayerAttackState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChangeAnimation(L"Attack");
		Obj->AttackBehaviour();
	}

	void PlayerAttackState::Execute(const shared_ptr<Player>& Obj) {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//X�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				Obj->GetStateMachine()->ChangeState(StepState::Instance());
			}
		}
	}

	void PlayerAttackState::Exit(const shared_ptr<Player>& Obj) {

	}

	//--------------------------------------------------------------------------------------
	///	�X�e�b�v���̃X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(StepState)

	void StepState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChangeAnimation(L"Step");
		Obj->PlayerStepEffect();
		Obj->InitVelocity();
		Obj->SetStepVec(Obj->GetMoveVector());
		auto sword = Obj->GetStage()->FindTagGameObject<Sword>(L"Sword");
		sword->SetState(L"Default");
	}

	void StepState::Execute(const shared_ptr<Player>& Obj) {
		Obj->StepBehaviour();
	}

	void StepState::Exit(const shared_ptr<Player>& Obj) {

	}

	//--------------------------------------------------------------------------------------
	///	�_���[�W���󂯂���̃X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(DamagedState)

	void DamagedState::Enter(const shared_ptr<Player>& Obj) {
		frame_count = 0.0f;
	}

	void DamagedState::Execute(const shared_ptr<Player>& Obj) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Obj->DamagedBehaviour();
		if (frame_count > 0.3f) {
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
			return;
		}
		frame_count += ElapsedTime;
	}

	void DamagedState::Exit(const shared_ptr<Player>& Obj) {

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

		m_friends_num = 0;

		//Rigidbody�̏�����
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = false;
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
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = false;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;
		//m_PtrObj->m_Alpha = 0.0f;

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
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";
		FPS += L"������̃t�����Y: ";
		FPS += Util::IntToWStr(m_friends_num);
		FPS += L"\n";
		FPS += L"Camera: ";
		auto GM = GameManager::getInstance();
		FPS += Util::FloatToWStr(GM->m_camera_length);
		FPS += L"\n";
		if (!m_StringDrawObject) {
			m_StringDrawObject = GetStage<GameStage>()->FindTagGameObject<StringDrawObject>(L"StringDrawObject");
		}
		m_StringDrawObject->SetText(FPS);
	}


	void Sword::OnDrawShadowmap() {
	}

	void Sword::OnDraw() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0, 0, 0),
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

	void Sword::AttackBehavior() {
		//�U���̏Փ˔���
		vector<shared_ptr<GameObject>> EnemyVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"EnemyObject", EnemyVec);
		for (auto enemy : EnemyVec) {
			if (enemy) {
				auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);

				Vec3 EnemyPos = PtrEnemy->GetPosition();
				float length = (EnemyPos - m_Rigidbody->m_Pos).length();

				float EnemyRadius = PtrEnemy->GetScale() / 2.0f;
				float SwordRadius = m_Scale.x / 2.0f;

				if (length < EnemyRadius + SwordRadius) {
					if (PtrEnemy->GetHP() > 0) {
						
						if (!(PtrEnemy->FindTag(L"LongBoss") || PtrEnemy->FindTag(L"CloseBoss"))){
							Vec3 Emitter = m_Rigidbody->m_Pos;
							Emitter.y -= 0.125f;
							//Spark�̑��o
							auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
							SparkPtr->InsertSpark(Emitter);

							//�_���[�W����
							PtrEnemy->SetHP(0.0f);
							//�z��ɓG���Z�b�g����
							SetFriends(PtrEnemy);
							//�z��̒����������2�Ԗڈȍ~�Ȃ玩���̑O�̓G��e�ɂ���
							m_friends_num = m_friends.size();

							auto GM = GameManager::getInstance();
							GM->SetFriendsNum(m_friends_num);
							if (m_friends_num == 1) {
								auto PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");
								PtrEnemy->SetParent(PlayerPtr);
							}
							if (m_friends_num >= 2) {
								PtrEnemy->SetParent(m_friends[m_friends_num - 2]);
							}
							PtrEnemy->CheckHealth();
							break;
						}	
					}
				}
			}
		}
	}

	int Sword::SetEnemyToCannon(wstring tag_name) {

		int bullet_num = 0;
		for (int i = 0; i < m_friends_num;) {
			auto f_pointer = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			if (f_pointer->FindTag(tag_name)) {
				//�Ȃ������
				f_pointer->GetStateMachine()->ChangeState(EnemyBulletState::Instance());
				//�z��������A�Ȃ���̐����Čv�Z
				m_friends.erase(m_friends.begin() + i);
				m_friends_num = m_friends.size();
				//�����̃`�F�C���𔭎˂��邩
				bullet_num++;
			}
			else
				i++;
		}

		auto GM = GameManager::getInstance();
		GM->SetFriendsNum(m_friends_num);

		//�Đ���
		for (int i = 0; i < m_friends_num; i++) {
			auto this_friend = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			if (i == 0) {
				this_friend->SetParent(m_ParentPtr);
			}
			else {
				this_friend->SetParent(m_friends[i - 1]);
			}
		}

		return bullet_num;
	}

	void Sword::ComplianceStartBehavior() {
		//���[�J���s��̒�`
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, m_Scale.y/2.0f, -0.5f)
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
			Vec3(0, 0.5f, 0.0f)
		);
		m_ChildLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, 0.5f, -0.5f)
		);
		m_LerpToParent = m_LerpToChild = 0.5f;

	}

	bool Sword::Attack1ExcuteBehavior() {
		m_Attack1ToRot += 0.15f;
		if (m_Attack1ToRot >= (XM_PI + 2.0f)) {
			m_Attack1ToRot = 0.0f;
			return true;
		}
		//���[�J���s��̒�`
		Vec3 Pos(0, sin(m_Attack1ToRot) * 0.5f, -cos(m_Attack1ToRot) * 0.5f);
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(Vec3(1.0, 0, 0), m_Attack1ToRot),
			Pos
		);
		return false;
	}

	void Sword::SetState(wstring state_name) {
		if (state_name == L"Attack") {
			m_StateMachine->ChangeState(Attack1State::Instance());
		}
		else if (state_name == L"Default") {
			m_StateMachine->ChangeState(NonAttackState::Instance());
		}
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

		////�R���g���[���̎擾
		//auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//if (CntlVec[0].bConnected) {
		//	//X�{�^��
		//	if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
		//		Obj->GetStateMachine()->ChangeState(Attack1State::Instance());
		//	}
		//}

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
	
	}

	void Attack1State::Execute(const shared_ptr<Sword>& Obj) {
		if (Obj->Attack1ExcuteBehavior()) {
			Obj->GetStateMachine()->ChangeState(NonAttackState::Instance());
			auto PlayerPtr = Obj->GetStage()->FindTagGameObject<Player>(L"Player");
			PlayerPtr->ChangeDefaultState();
			return;
		}
		Obj->UpdateBehavior();
		Obj->AttackBehavior();
	}

	void Attack1State::Exit(const shared_ptr<Sword>& Obj) {
		
	}


}
//end basecross
