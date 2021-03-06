/*!
@file Player.cpp
@brief プレイヤー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	GameManager;

	//--------------------------------------------------------------------------------------
	///	プレイヤー実体
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
		m_AttackDis(1.7f),
		m_FOV(0.707f),
		m_NowCannonClass(3),
		m_JumpLock(false),
		m_isInvincible(false),
		m_isDamaging(false),
		m_DamageFrameCount(0.0f),
		m_DamagingTime(3.5f)
	{
		//メッシュとトランスフォームの差分の設定
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
		if (m_UpdateActive) {
			auto PtrGameStage = GetStage<GameStage>();
			Vec3 CameraEye, CameraAt;
			PtrGameStage->GetCamera().GetCameraEyeAt(CameraEye, CameraAt);

			//コントローラの取得
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
					float MoveLength = 0;	//動いた時のスピード
											//進行方向の向きを計算
					Vec3 Front = m_Rigidbody->m_Pos - CameraEye;
					Front.y = 0;
					Front.normalize();
					//進行方向向きからの角度を算出
					float FrontAngle = atan2(Front.z, Front.x);
					//コントローラの向き計算
					float MoveX = CntlVec[0].fThumbLX;
					float MoveZ = CntlVec[0].fThumbLY;
					Vec2 MoveVec(MoveX, MoveZ);
					float MoveSize = length(MoveVec);
					//コントローラの向きから角度を計算
					float CntlAngle = atan2(-MoveX, MoveZ);
					//トータルの角度を算出
					float TotalAngle = FrontAngle + CntlAngle;
					//角度からベクトルを作成
					Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
					//正規化する
					Angle.normalize();
					//移動サイズを設定。
					Angle *= MoveSize;
					//Y軸は変化させない
					Angle.y = 0;
				}
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
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//タグの追加
		AddTag(L"Player");
		//Rigidbodyの初期化
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

		m_UpdateActive = true;

		m_CannonHP = 30.0f;
		m_DefaultCannonHP = m_CannonHP;

		m_StepSound = ObjectFactory::Create<SoundObject>(L"step_sound");
		m_DamageSound = ObjectFactory::Create<SoundObject>(L"damage_voice");
		m_AttackSound = ObjectFactory::Create<SoundObject>(L"attack_voice");
		m_BombSound = ObjectFactory::Create<SoundObject>(L"Pan");

		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			body.m_Scale,
			Vec3(0, 0, 0),
			body.m_Quat,
			body.m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = App::GetApp()->GetResource<MeshResource>(L"KUREHA_MESH");;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;

		m_PtrObj->m_Emissive = Col4(0.5f, 0.5f, 0.9f, 1.0f);

		m_PtrObj->m_UsedModelColor = true;

		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 30, true, 40.0f);
		m_PtrObj->AddAnimation(L"RunStart", 30, 20, false, 60.0f);
		m_PtrObj->AddAnimation(L"Running", 50, 40, true, 60.0f);
		m_PtrObj->AddAnimation(L"RunEnd", 90, 20, false, 60.0f);
		m_PtrObj->AddAnimation(L"Attack", 120, 50, false, 90.0f);
		m_PtrObj->AddAnimation(L"Damage", 170, 30, false, 60.0f);
		m_PtrObj->AddAnimation(L"Step", 200, 30, false, 90.0f);
		m_PtrObj->AddAnimation(L"CannonStart", 230, 60, false, 45.0f);

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_PtrObj->m_MeshRes;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//ステート初期値設定
		m_StateMachine->ChangeState(DefaultState::Instance());
	}


	void Player::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);
		m_StateMachine->Update();
		CollisionWithCannon();

		if (m_isDamaging) {
			if (m_DamageFrameCount >= m_DamagingTime) {
				m_DamageFrameCount = 0.0f;
				m_PtrObj->m_UsedModelColor = true;
				SetDamaging(false);
				return;
			}
			//色を点滅させる
			if (m_PtrObj->m_UsedModelColor == true) {
				m_PtrObj->m_UsedModelColor = false;
			}
			else if (m_PtrObj->m_UsedModelColor == false) {
				m_PtrObj->m_UsedModelColor = true;
			}
			m_DamageFrameCount += ElapsedTime;
		}

		if (m_PtrObj->m_CurrentAnimeName == L"CannonStart" && m_PtrObj->m_CurrentAnimeTime > 0.99f) {
			ChangeAnimation(L"Default");
			m_CannonAnimation = false;
			vector<shared_ptr<GameObject>> ShootedEnemyVec;
			GetStage()->FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
			for (auto enemy : ShootedEnemyVec) {
				if (enemy) {
					auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
					PtrEnemy->ChangeState(L"Bullet");
				}
			}
		}
	}

	void Player::OnUpdate2() {
		if (m_Rigidbody->m_Pos.y <= m_BaseY) {
			m_Rigidbody->m_Pos.y = m_BaseY;
			m_Rigidbody->m_Velocity.y = 0;
			if (m_JumpLock) {
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= m_Rigidbody->m_Scale.y / 2.0f;;
				//Sparkの送出
				auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
				SparkPtr->InsertSpark(Emitter);
			}
			m_JumpLock = false;
		}
		
		//回転の更新
		//Velocityの値で、回転を変更する
		Vec3 Temp = m_Rigidbody->m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		//移動しないときは回転しない
		if (m_Rigidbody->m_Velocity.length() > 0.0f) {
			m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.3f);
		}
	}

	void Player::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		//描画データの行列をコピー
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
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
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
		//行列の定義
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
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			Vec3 Direction = GetMoveVector();
			if (!m_UpdateActive) {
				Direction = Vec3(0);
			}
			//Aボタン
			if ((CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) && m_UpdateActive) {
				m_StateMachine->ChangeState(StepState::Instance());
				return;
			}
			else if ((CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && m_UpdateActive) {
				m_StateMachine->ChangeState(PlayerAttackState::Instance());
				return;
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
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (m_PtrObj->m_CurrentAnimeName != L"CannonStart") {
			if (Direction.length() < 0.1f) {
				if (isRunning != RunEnd) {
					isRunning = RunEnd;
					ChangeAnimation(L"RunEnd");
					m_RunAnimationFrameCount = 0.0f;
				}
			}
			else if (m_RunAnimationFrameCount > 0.333f && m_PtrObj->m_CurrentAnimeName == L"RunStart") {
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
			if (m_PtrObj->m_CurrentAnimeName == L"RunEnd") {
				if (m_RunAnimationFrameCount > 0.333f) {
					ChangeAnimation(L"Default");
					m_RunAnimationFrameCount = 0.0f;
				}
				else
					m_RunAnimationFrameCount += ElapsedTime;
			}
			if (isRunning == RunStart) {
				m_RunAnimationFrameCount += ElapsedTime;
			}
		}
	}

	void Player::ChangeDefaultState() {
		m_StateMachine->ChangeState(DefaultState::Instance());
	}

	void Player::DefaultBehaviour() {

		RunningAnimation();
		MoveControll();
		if (m_isDamaging == false && m_NowCannonClass == 3) {
			//敵との当たり判定
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
							DamagedStartBehaviour(EnemyPos);
							return;
						}
					}
				}
			}
			//ボスハンドとの衝突判定
			vector<shared_ptr<GameObject>> HandVec;
			GetStage<GameStage>()->FindTagGameObjectVec(L"BossHand", HandVec);
			for (auto hand : HandVec) {
				if (hand) {
					auto PtrHand = dynamic_pointer_cast<BossHand>(hand);

					Vec3 HandPos = PtrHand->GetPosition();
					float length = (HandPos - m_Rigidbody->m_Pos).length();

					float Radius = PtrHand->GetScale() / 2.0f;
					float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

					if (length < Radius + PlayerRadius) {
						DamagedStartBehaviour(HandPos);
						return;
					}
				}
			}
		}
	}

	void Player::DamagedStartBehaviour(Vec3 StartPos) {
		Vec3 Emitter = m_Rigidbody->m_Pos;
		//Sparkの送出
		auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
		SparkPtr->InsertSpark(Emitter);

		m_DamageSound->Start(0, 0.4f);
		m_BombSound->Start(0, 0.5f);

		//ノックバック方向の設定
		m_KnockBackVec = m_Rigidbody->m_Pos - StartPos;
		m_KnockBackVec.y = 0.0f;
		m_KnockBackVec.normalize();
		//つながりを消す
		auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
		s->DeleteChains(3);
		//ダメージステートに変更
		m_StateMachine->ChangeState(DamagedState::Instance());
	}

	void Player::DamagedBehaviour() {
		m_Rigidbody->m_Velocity = m_KnockBackVec * 5.0f;
	}

	void Player::PlayerStepEffect() {
		m_StepSound->Start(0, 0.2f);
		//エフェクトの再生
		auto FirePtr = GetStage<GameStage>()->FindTagGameObject<StepEffect>(L"StepEffect");
		Vec3 Emitter = m_Rigidbody->m_Pos;
		Emitter.y -= m_Rigidbody->m_Scale.y / 2.0f;;
		FirePtr->InsertEffect(Emitter);
	}

	void Player::InitVelocity() {
		m_Rigidbody->m_Velocity.x *= 0.0f;
		m_Rigidbody->m_Velocity.z *= 0.0f;
	}

	void Player::CollisionWithCannon() {
		//大砲との衝突判定
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
					if (PtrCannon->GetCannonClass() == 0) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						int bullet_num = s->SetEnemyToCannon(L"Green");
						if (bullet_num == 0) {
							Vec3 MoveVec = m_Rigidbody->m_Pos - CannonPos;
							MoveVec.y = 0.0f;
							MoveVec.normalize();
							float MoveLength = PtrCannon->GetScale() / 2.0f + GetScale() / 2.0f;
							SetPosition(MoveVec * MoveLength + PtrCannon->GetPosition());
							return;
						}
						else {
							m_StateMachine->ChangeState(DefaultState::Instance());
							ChangeAnimation(L"CannonStart");
							m_CannonAnimation = true;
							GetStage<GameStage>()->CannonStateStartBehaviour();
							m_NowCannonClass = 0;
							Vec3 NewPos = PtrCannon->GetPosition();
							NewPos.x -= 3.0f;
							NewPos.z += 3.0f;
							SetPosition(NewPos);
							m_Rigidbody->m_Velocity = Vec3(0);
							m_Rigidbody->m_Velocity.z += 0.1f;
							return;
						}
					}
					if (PtrCannon->GetCannonClass() == 1) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						int bullet_num = s->SetEnemyToCannon(L"Red");
						if (bullet_num == 0) {
							Vec3 MoveVec = m_Rigidbody->m_Pos - CannonPos;
							MoveVec.y = 0.0f;
							MoveVec.normalize();
							float MoveLength = PtrCannon->GetScale() / 2.0f + GetScale() / 2.0f;
							SetPosition(MoveVec * MoveLength + PtrCannon->GetPosition());
							return;
						}
						else {
							m_StateMachine->ChangeState(DefaultState::Instance());
							ChangeAnimation(L"CannonStart");
							m_CannonAnimation = true;
							GetStage<GameStage>()->CannonStateStartBehaviour();
							m_NowCannonClass = 1;
							Vec3 NewPos = PtrCannon->GetPosition();
							NewPos.x -= 3.0f;
							NewPos.z += 3.0f;
							SetPosition(NewPos);
							m_Rigidbody->m_Velocity = Vec3(0);
							m_Rigidbody->m_Velocity.z += 0.1f;
							return;
						}
					}
					if (PtrCannon->GetCannonClass() == 2) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						int bullet_num = s->SetEnemyToCannon(L"Blue");
						if (bullet_num == 0) {
							Vec3 MoveVec = m_Rigidbody->m_Pos - CannonPos;
							MoveVec.y = 0.0f;
							MoveVec.normalize();
							float MoveLength = PtrCannon->GetScale() / 2.0f + GetScale() / 2.0f;
							SetPosition(MoveVec * MoveLength + PtrCannon->GetPosition());
							return;
						}
						else {
							m_StateMachine->ChangeState(DefaultState::Instance());
							ChangeAnimation(L"CannonStart");
							m_CannonAnimation = true;
							GetStage<GameStage>()->CannonStateStartBehaviour();
							m_NowCannonClass = 2;
							Vec3 NewPos = PtrCannon->GetPosition();
							NewPos.x -= 3.0f;
							NewPos.z += 3.0f;
							SetPosition(NewPos);
							m_Rigidbody->m_Velocity = Vec3(0);
							m_Rigidbody->m_Velocity.z += 0.1f;
							return;
						}
					}
				}
			}
		}
	}

	void Player::StepBehaviour() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		m_Rigidbody->m_Velocity += m_StepVec * (1.05f - m_FrameCount * 3.0f);
		m_FrameCount += ElapsedTime;

		if (m_FrameCount >= 0.3f) {
			m_StateMachine->ChangeState(DefaultState::Instance());
			m_FrameCount = 0.0f;
			return;
		}
		m_Rigidbody->m_Force += m_Rigidbody->m_Gravity * m_Rigidbody->m_Mass;

		if (m_isDamaging == false) {
			//色を点滅させる
			if (m_PtrObj->m_UsedModelColor == true) {
				m_PtrObj->m_UsedModelColor = false;
			}
			else if (m_PtrObj->m_UsedModelColor == false) {
				m_PtrObj->m_UsedModelColor = true;
			}
		}
	}

	void Player::AttackBehaviour() {
		auto sword = GetStage()->FindTagGameObject<Sword>(L"Sword");
		sword->SetState(L"Attack");

		//攻撃中の移動の補正
		vector<shared_ptr<GameObject>> EnemyVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Zako", EnemyVec);

		//最短距離と方向
		float ShortDis = 0.0f;
		Vec3 MoveVec = Vec3(0,0,0);

		//自分の向き
		Vec3 MyVec = m_Rigidbody->m_Velocity;
		MyVec.normalize();

		//敵のポジションを取り、視野内にいる最短距離を求める
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

				//自分の視野に敵がいて、その距離が最短だった場合ShortDisを上書きする
				float angle = dot(MyVec, ToVec);
				if (angle > m_FOV) {
					if (ShortDis > length) {
						ShortDis = length;
						MoveVec = ToVec;
					}
				}
			}
		}
		
		//敵が近ければ補正をかける
		if (ShortDis < m_AttackDis) {
			//自分のベクトルを最短の敵に向かわせる
			m_Rigidbody->m_Velocity.x = MoveVec.x * ShortDis * 1.5f;
			m_Rigidbody->m_Velocity.z = MoveVec.z * ShortDis * 1.5f;
		}
		else {
			m_Rigidbody->m_Velocity.x *= 0.1f;
			m_Rigidbody->m_Velocity.z *= 0.1f;
		}
	}

	//--------------------------------------------------------------------------------------
	///	通常のステート
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
	///	攻撃中のステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAttackState)

	void PlayerAttackState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChangeAnimation(L"Attack");
		Obj->AttackBehaviour();
		Obj->SetInvincible(true);
	}

	void PlayerAttackState::Execute(const shared_ptr<Player>& Obj) {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Lボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				Obj->GetStateMachine()->ChangeState(StepState::Instance());
			}
		}
	}

	void PlayerAttackState::Exit(const shared_ptr<Player>& Obj) {
		Obj->SetInvincible(false);
	}

	//--------------------------------------------------------------------------------------
	///	ステップ中のステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(StepState)

	void StepState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChangeAnimation(L"Step");
		Obj->PlayerStepEffect();
		Obj->InitVelocity();
		Obj->SetStepVec(Obj->GetMoveVector());
		Obj->SetInvincible(true);
		auto sword = Obj->GetStage()->FindTagGameObject<Sword>(L"Sword");
		sword->SetState(L"Default");
	}

	void StepState::Execute(const shared_ptr<Player>& Obj) {
		Obj->StepBehaviour();
	}

	void StepState::Exit(const shared_ptr<Player>& Obj) {
		Obj->SetInvincible(false);
		Obj->m_PtrObj->m_UsedModelColor = true;
	}

	//--------------------------------------------------------------------------------------
	///	ダメージを受けた後のステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(DamagedState)

	void DamagedState::Enter(const shared_ptr<Player>& Obj) {
		frame_count = 0.0f;
		Obj->SetDamaging(true);
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
	/// プレイヤーの持つ剣
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
		//タグの追加
		AddTag(L"Sword");

		m_friends_num = 0;

		//Rigidbodyの初期化
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

		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = false;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;

		m_AttackSound = ObjectFactory::Create<SoundObject>(L"buki");

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Sword>(GetThis<Sword>()));
		//ステート初期値設定
		m_StateMachine->ChangeState(NonAttackState::Instance());

	}

	void Sword::OnUpdate() {
		//ステートマシン更新
		m_StateMachine->Update();
	}

	void Sword::OnUpdate2() {
		/*auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"\n\n\n\n\nFPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";
		FPS += L"うしろのフレンズ: ";
		FPS += Util::IntToWStr(m_friends_num);
		FPS += L"\n";
		FPS += L"Camera: ";
		auto GM = GameManager::getInstance();
		FPS += Util::FloatToWStr(GM->m_camera_length);
		FPS += L"\n";
		if (!m_StringDrawObject) {
			m_StringDrawObject = GetStage<GameStage>()->FindTagGameObject<StringDrawObject>(L"StringDrawObject");
		}
		m_StringDrawObject->SetText(FPS);*/
	}


	void Sword::OnDrawShadowmap() {
	}

	void Sword::OnDraw() {
		//行列の定義
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
		//行列の定義
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
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_ParentPtr.lock();
		//親のワールド行列を取得する変数
		Mat4x4 ParMat;
		if (shptr) {
			ParentFlg flg = ParentFlg::NoParent;
			//行列取得用のインターフェイスを持ってるかどうか
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
				//行列の定義
				World = m_PlayerLocalMatrix;
				LerpNum = m_LerpToParent;
				break;
			case ParentFlg::Child:
				//行列の定義
				World = m_ChildLocalMatrix;
				LerpNum = m_LerpToChild;
				break;
			default:
				break;
			}
			if (flg != ParentFlg::NoParent) {
				//スケーリングを1.0にした行列に変換
				ParMat.scaleIdentity();
				//行列の反映
				World *= ParMat;
				//この時点でWorldは目標となる位置
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
		//攻撃の衝突判定
		vector<shared_ptr<GameObject>> EnemyVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Zako", EnemyVec);
		for (auto enemy : EnemyVec) {
			if (enemy) {
				auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);

				Vec3 EnemyPos = PtrEnemy->GetPosition();
				float length = (EnemyPos - m_Rigidbody->m_Pos).length();

				float EnemyRadius = PtrEnemy->GetScale() / 2.0f;
				float SwordRadius = m_Scale.x / 2.0f;

				if (length < EnemyRadius + SwordRadius) {
					if (PtrEnemy->GetHP() > 0) {

						auto PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");
						PlayerPtr->InitVelocity();

						//ダメージ処理
						PtrEnemy->SetHP(0.0f);
						//配列に敵をセットする
						SetFriends(PtrEnemy);
						//配列の長さを取って2番目以降なら自分の前の敵を親にする
						m_friends_num = m_friends.size();

						auto GM = GameManager::getInstance();
						GM->SetFriendsNum(m_friends_num);
						if (m_friends_num == 1) {
							auto PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");
							PtrEnemy->SetParent(PlayerPtr);
						}
						if (m_friends_num >= 2) {
							PtrEnemy->SetParent(m_friends[m_friends_num - 2]);
							auto parent = dynamic_pointer_cast<EnemyObject>(m_friends[m_friends_num - 2].lock());
							//親がエンジェルエネミーなら何番目にいるかSetする
							int ParentFollowingAngelNum = parent->GetFollowingAngelNum();
							if (parent->FindTag(L"Green")) {
								PtrEnemy->SetFollowingAngelNum(1);
							}
							else if (ParentFollowingAngelNum && ParentFollowingAngelNum < 3) {
								PtrEnemy->SetFollowingAngelNum(ParentFollowingAngelNum + 1);
							}
							else
								PtrEnemy->SetFollowingAngelNum(0);
						}
						PtrEnemy->CheckHealth();
						break;
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
				//つながり消す
				f_pointer->GetStateMachine()->ChangeState(EnemyBulletPrepareState::Instance());
				//配列を消し、つながりの数を再計算
				m_friends.erase(m_friends.begin() + i);
				m_friends_num = m_friends.size();
				//いくつのチェインを発射するか
				bullet_num++;
				f_pointer->SetShootNumber(bullet_num);
			}
			else if (bullet_num > 0) {
				break;
			}
			else
				i++;
		}

		auto GM = GameManager::getInstance();
		GM->SetFriendsNum(m_friends_num);

		//再整列
		for (int i = 0; i < m_friends_num; i++) {
			auto this_friend = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			this_friend->SetFollowingAngelNum(0);
			if (i == 0) {
				auto PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");
				this_friend->SetParent(PlayerPtr);
			}
			else {
				this_friend->SetParent(m_friends[i - 1]);
				auto parent = dynamic_pointer_cast<EnemyObject>(m_friends[i - 1].lock());
				//親がエンジェルエネミーなら何番目にいるかSetする
				int ParentFollowingAngelNum = parent->GetFollowingAngelNum();
				if (parent->FindTag(L"Green")) {
					this_friend->SetFollowingAngelNum(1);
				}
				else if (ParentFollowingAngelNum && ParentFollowingAngelNum < 3) {
					this_friend->SetFollowingAngelNum(ParentFollowingAngelNum + 1);
				}
			}
		}

		return bullet_num;
	}

	//つながりを消す処理
	void Sword::DeleteChains(int num) {
		if (m_friends_num < num) {
			num = m_friends_num;
		}
		int delete_count = 0;
		for (int i = 0; i < m_friends_num;) {
			auto enemy = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			if (enemy->GetFollowingAngelNum() == 0) {
				Vec3 Emitter = enemy->GetPosition();
				//Fireの送出
				auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
				SparkPtr->InsertFire(Emitter, 1.0f);
				enemy->GetStateMachine()->ChangeState(EnemyWaitingState::Instance());
				
				//配列を消し、つながりの数を再計算
				m_friends.erase(m_friends.begin() + i);
				m_friends_num = m_friends.size();
				delete_count++;
				if (delete_count >= num) {
					break;
				}
			}
			else
				i++;
		}

		auto GM = GameManager::getInstance();
		GM->SetFriendsNum(m_friends_num);

		//再整列
		for (int i = 0; i < m_friends_num; i++) {
			auto this_friend = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			this_friend->SetFollowingAngelNum(0);
			if (i == 0) {
				auto PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");
				this_friend->SetParent(PlayerPtr);
			}
			else {
				this_friend->SetParent(m_friends[i - 1]);
				auto parent = dynamic_pointer_cast<EnemyObject>(m_friends[i - 1].lock());
				//親がエンジェルエネミーなら何番目にいるかSetする
				int ParentFollowingAngelNum = parent->GetFollowingAngelNum();
				if (parent->FindTag(L"Green")) {
					this_friend->SetFollowingAngelNum(1);
				}
				else if (ParentFollowingAngelNum && ParentFollowingAngelNum < 3) {
					this_friend->SetFollowingAngelNum(ParentFollowingAngelNum + 1);
				}
			}
		}
	}

	void Sword::ComplianceStartBehavior() {
		//ローカル行列の定義
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, m_Scale.y/2.0f, -0.5f)
		);
		//このステートではチャイルドの場合も同じ
		m_ChildLocalMatrix = m_PlayerLocalMatrix;
		m_LerpToParent = m_LerpToChild = 0.2f;
	}

	//攻撃１行動の開始
	void Sword::Attack1StartBehavior() {
		m_Attack1ToRot = 0.1f;
		//ローカル行列の定義
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

		m_AttackSound->Start(0, 0.2f);
	}

	bool Sword::Attack1ExcuteBehavior() {
		m_Attack1ToRot += 0.15f;
		if (m_Attack1ToRot >= (XM_PI + 2.0f)) {
			m_Attack1ToRot = 0.0f;
			return true;
		}
		//ローカル行列の定義
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

	wstring Sword::GetChainTX(int num) {
		if (num >= m_friends_num) {
			return L"TRANS_TX";
		}
		auto shptr = m_friends[num].lock();
		if (shptr->FindTag(L"Red")) {
			return L"NEEDLE_ICON_TX";
		}
		if (shptr->FindTag(L"Green")) {
			return L"ANGEL_ICON_TX";
		}
		if (shptr->FindTag(L"Blue")) {
			return L"GUN_ICON_TX";
		}
		else {
			return L"TRANS_TX";
		}
	}

	//--------------------------------------------------------------------------------------
	///	追従ステート（Sword）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NonAttackState)

	void NonAttackState::Enter(const shared_ptr<Sword>& Obj) {
		Obj->ComplianceStartBehavior();
		//何もしない
	}

	void NonAttackState::Execute(const shared_ptr<Sword>& Obj) {
		Obj->UpdateBehavior();
	}

	void NonAttackState::Exit(const shared_ptr<Sword>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	攻撃ステート１（Sword）
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
