/*!
@file Player.cpp
@brief プレイヤー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	GameManager;

	//--------------------------------------------------------------------------------------
	///	球体のプレイヤー実体
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
		m_JumpLock(false)
	{}
	Player::~Player() {}

	Vec3 Player::GetMoveVector() {
		Vec3 Angle(0, 0, 0);
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
		body.m_IsFixed = true;
		body.SetToBefore();

		m_StepVec = Vec3(0.0f);
		m_KnockBackVec = Vec3(0.0f);

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

		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 30, true, 40.0f);
		//m_PtrObj->AddAnimation(L"Default", 0, 30, true, 40.0f);

		m_PtrObj->ChangeCurrentAnimation(L"Default");

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
		
	}

	void Player::OnUpdate2() {
		if (m_Rigidbody->m_Pos.y <= m_BaseY) {
			m_Rigidbody->m_Pos.y = m_BaseY;
			m_Rigidbody->m_Velocity.y = 0;
			if (m_JumpLock) {
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= m_Rigidbody->m_Scale.y / 2.0f;;
				//Spaerkの送出
				auto SpaerkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
				SpaerkPtr->InsertSpark(Emitter);
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
		m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
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
		//描画データの行列をコピー
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
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
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

	void Player::Step() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Aボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A && m_isStep == false) {
				m_isStep = true;
				m_StepVec = GetMoveVector();
				m_StepVec.normalize();
				//エフェクトの再生
				auto FirePtr = GetStage<GameStage>()->FindTagGameObject<StepEffect>(L"StepEffect");
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= m_Rigidbody->m_Scale.y / 2.0f;;
				FirePtr->InsertEffect(Emitter);
			}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				m_Rigidbody->m_Velocity.x *= 0.8f;
				m_Rigidbody->m_Velocity.z *= 0.8f;
			}
			if (m_isStep == true) {
				m_Rigidbody->m_Velocity += m_StepVec * 1.3f;
				m_FrameCount++;
			}
			else {
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

	void Player::Damage(Vec3 EnemyPos) {
		
	}

	void Player::DefaultBehaviour() {
		Step();

		if (m_isStep == false) {
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
							Vec3 Emitter = m_Rigidbody->m_Pos;
							Emitter.y -= 0.125f;
							//Sparkの送出
							auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
							SparkPtr->InsertSpark(Emitter);
							//ノックバック方向の設定
							m_KnockBackVec = m_Rigidbody->m_Pos - EnemyPos;
							m_KnockBackVec.y = 0.0f;
							m_KnockBackVec.normalize();
							//ダメージステートに変更
							m_StateMachine->ChangeState(DamagedState::Instance());
							return;
						}
					}
				}
			}
		}
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

					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y -= 0.125f;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
					SparkPtr->InsertSpark(Emitter);

					if (PtrCannon->GetCannonClass() == 0) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						s->SetEnemyToCannon(L"Green");
					}
					if (PtrCannon->GetCannonClass() == 1) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						s->SetEnemyToCannon(L"Red");
					}
					if (PtrCannon->GetCannonClass() == 2) {
						auto s = GetStage()->FindTagGameObject<Sword>(L"Sword");
						s->SetEnemyToCannon(L"Blue");
					}
				}
			}
		}
		//弾との衝突判定
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
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
					SparkPtr->InsertSpark(Emitter);
					//ノックバック方向の設定
					m_KnockBackVec = m_Rigidbody->m_Pos - BulletPos;
					m_KnockBackVec.y = 0.0f;
					m_KnockBackVec.normalize();
					//ダメージステートに変更
					m_StateMachine->ChangeState(DamagedState::Instance());
					return;
				}
			}
		}
	}

	void Player::DamagedBehaviour() {
		m_Rigidbody->m_Velocity += m_KnockBackVec * 0.7f;
	}

	//--------------------------------------------------------------------------------------
	///	通常のステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(DefaultState)

	void DefaultState::Enter(const shared_ptr<Player>& Obj) {
		
	}

	void DefaultState::Execute(const shared_ptr<Player>& Obj) {
		Obj->DefaultBehaviour();
	}

	void DefaultState::Exit(const shared_ptr<Player>& Obj) {

	}

	//--------------------------------------------------------------------------------------
	///	ダメージを受けた後のステート
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
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		//m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;
		m_PtrObj->m_Alpha = 0.0f;

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
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";
		FPS += L"うしろのフレンズ: ";
		FPS += Util::IntToWStr(m_friends_num);
		FPS += L"\n";
		if (!m_StringDrawObject) {
			m_StringDrawObject = GetStage<GameStage>()->FindTagGameObject<StringDrawObject>(L"StringDrawObject");
		}
		m_StringDrawObject->SetText(FPS);
	}


	void Sword::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//描画データの行列をコピー
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
		//行列の定義
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

		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Xボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y) {

				//二番目が死んだとしたら
				/*auto dead_friend = dynamic_pointer_cast<EnemyObject>(m_friends[1].lock());
				dead_friend->GetStateMachine()->ChangeState(EnemyOppositionState::Instance());
				dead_friend->SetPosition(Vec3(100, 100, 100));
				m_friends.erase(m_friends.begin() + 1);
				m_friends_num = m_friends.size();*/

				/*for (auto f : m_friends) {
					auto f_pointer = dynamic_pointer_cast<EnemyObject>(f.lock());
					if (f_pointer->FindTag(L"Red")) {
						f_pointer->GetStateMachine()->ChangeState(EnemyOppositionState::Instance());
						f_pointer->SetPosition(Vec3(0, 0, 0));
					};
				}*/

				/*vector<int> erase_num;
				int j = 0;*/

				
			}
		}
	}

	void Sword::AttackBehavior() {
		//攻撃の衝突判定
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
							//Sparkの送出
							auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
							SparkPtr->InsertSpark(Emitter);

							//ダメージ処理
							PtrEnemy->SetHP(0.0f);
							//配列に敵をセットする
							SetFriends(PtrEnemy);
							//配列の長さを取って2番目以降なら自分の前の敵を親にする
							m_friends_num = m_friends.size();

							auto GM = GameManager::getInstance();
							GM->SetFriendsNum(m_friends_num);

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

	void Sword::SetEnemyToCannon(wstring tag_name) {

		for (int i = 0; i < m_friends_num; i++) {
			auto f_pointer = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			if (f_pointer->FindTag(tag_name)) {
				auto boss = GetStage()->FindTagGameObject<BossEnemy>(L"BossEnemy");
				boss->Damage();

				f_pointer->GetStateMachine()->ChangeState(EnemyOppositionState::Instance());
				f_pointer->SetPosition(Vec3(100, 100, 100));
				m_friends.erase(m_friends.begin() + i);
				m_friends_num = m_friends.size();
			}
		}
		/*for (int i = 0; i < erase_num.size(); i++) {
		m_friends.erase(m_friends.begin() + erase_num[i]);
		}*/

		auto GM = GameManager::getInstance();
		GM->SetFriendsNum(m_friends_num);

		//再整列
		for (int i = 0; i < m_friends_num; i++) {
			auto this_friend = dynamic_pointer_cast<EnemyObject>(m_friends[i].lock());
			if (i == 0) {
				this_friend->SetParent(m_ParentPtr);
			}
			else {
				this_friend->SetParent(m_friends[i - 1]);
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

	}

	bool Sword::Attack1ExcuteBehavior() {
		m_Attack1ToRot += 0.2f;
		if (m_Attack1ToRot >= (XM_PI + 0.5f)) {
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

		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Xボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
				Obj->GetStateMachine()->ChangeState(Attack1State::Instance());
			}
		}

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
			return;
		}
		Obj->UpdateBehavior();
		Obj->AttackBehavior();
	}

	void Attack1State::Exit(const shared_ptr<Sword>& Obj) {
		
	}


}
//end basecross
