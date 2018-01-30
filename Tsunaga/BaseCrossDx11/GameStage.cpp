/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	GameManager;

	//--------------------------------------------------------------------------------------
	///	ゲームステージ
	//--------------------------------------------------------------------------------------
	GameStage::GameStage()
	{
	}

	void GameStage::RegisterDefaultSphere() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"DEFAULT_SPHERE")) {
			//メッシュが登録されてなければ登録
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(1.0f, 6, vertices, indices);
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE",
				MeshResource::CreateMeshResource(vertices, indices, false));
		}
		if (!App::GetApp()->CheckResource<MeshResource>(L"BOSS_BULLET")) {
			//メッシュが登録されてなければ登録
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(1.0f, 3, vertices, indices);
			App::GetApp()->RegisterResource(L"BOSS_BULLET",
				MeshResource::CreateMeshResource(vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {
		m_FrameCount = 0.0f;
		m_isClear = false;
		m_isFail = false;
		m_isGameStart = false;
		m_FogActive = false;
		m_CameraPos = Vec3(50, 10, 150);
		m_StartCameraY = 6.0f;
		
		//シャドウマップの描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

		AddGameObject<SkyBox>(
			L"SKY_TX",
			Vec3(2000.0f, 2000.0f, 2000.0f),
			Quat(),
			Vec3(0.0f),
			false);

		//複数使用する球体の登録（リソース登録する）
		RegisterDefaultSphere();

		//平面の作成
		Quat Qt;
		Qt.rotationX(XM_PIDIV2);

		AddGameObject<YamatoStage>(
			L"YAMATO_TX",
			Vec3(4.0f, 4.0f, 4.0f),
			Quat(),
			Vec3(0.0f, -12.74f, -18.2f - 1.5f),
			true);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 60.0f),
			Quat(Vec3(0, 1.0f, 0), -XM_PI / 180.0f * 4.18f),
			Vec3(15.15f, 0.25f, 14.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 60.0f),
			Quat(Vec3(0, 1.0f, 0), XM_PI / 180.0f * 4.18f),
			Vec3(-15.15f, 0.25f, 14.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 40.0f),
			Quat(Vec3(0, 1.0f, 0), -XM_PI / 180.0f * 17.0f),
			Vec3(10.0f, 0.25f, 54.0f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 40.0f),
			Quat(Vec3(0, 1.0f, 0), XM_PI / 180.0f * 17.0f),
			Vec3(-10.0f, 0.25f, 54.0f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 10.0f),
			Quat(Vec3(0, 1.0f, 0), -XM_PI / 180.0f * 62.0f),
			Vec3(3.0f, 0.25f, 72.9f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 10.0f),
			Quat(Vec3(0, 1.0f, 0), XM_PI / 180.0f * 62.0f),
			Vec3(-3.0f, 0.25f, 72.9f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 5.0f),
			Quat(),
			Vec3(-16.4f, 0.25f, -5.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 5.0f),
			Quat(),
			Vec3(16.4f, 0.25f, -5.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(50.0f, 0.5f, 0.5f),
			Quat(),
			Vec3(0.0f, 0.25f, -7.0f)
			);

		

		//プレイヤーの作成
		auto Par =
			AddGameObject<Player>(
				L"KUREHA_TX",
				true,
				Vec3(0.0f, 0.5f, 11.0f)
				);

		//剣の作成
		AddGameObject<Sword>(
			Par,
			L"SPARK_TX",
			Vec3(0.5f, 0.5f, 0.8f),
			Quat(),
			Vec3(0.0f, 0.5f, 0.0f),
			true);

		for (int i = 0; i < 3; i++) {
			float x = (float)(i);
			wstring texture;
			wstring tag;
			switch (i) {
			case 0:
				texture = L"GREEN_CANNON_TX";
				tag = L"GREEN_CANNON";
				break;
			case 1:
				texture = L"RED_CANNON_TX";
				tag = L"RED_CANNON";
				break;
			case 2:
				texture = L"BLUE_CANNON_TX";
				tag = L"BLUE_CANNON";
				break;
			}
			AddGameObject<Cannon>(
				texture,
				tag,
				Vec3(5.0f, 5.0f, 5.0f),
				Quat(),
				Vec3(x * 9.0f - 9.0f, 0.0f, 0.0f),
				i, false);
		}

		AddGameObject<Boss>(
			L"BOSS_TX",
			Vec3(10.0f, 10.0f, 10.0f),
			Quat(),
			Vec3(0.0f, 12.0f, 60.0f),
			true);

		for (int i = 0; i < 20; i++) {
			float x = (float)(i + 1);
			AddGameObject<NeedleEnemy>(
				Par,
				L"NEEDLE_MESH",
				L"NEEDLE_TX",
				L"30frame",
				Vec3(0.25f, 0.25f, 0.25f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 15.0f),
				false);
		}

		for (int i = 0; i < 20; i++) {
			float x = (float)(i + 1);
			AddGameObject<ShootEnemy>(
				Par,
				L"GUN_MESH",
				L"GUN_TX",
				L"NonMove",
				Vec3(0.21f, 0.21f, 0.21f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
			for (int j = 0; j < 5; j++) {
				AddGameObject<BulletObject>(
					L"SPARK_TX",
					Vec3(0.125f, 0.125f, 0.125f),
					Quat(),
					Vec3(0.0f, -10.0f, 0.0f),
					false,
					L"Bullet");
				AddGameObject<BulletObject>(
					L"SPARK_TX",
					Vec3(0.125f, 0.125f, 0.125f),
					Quat(),
					Vec3(0.0f, -10.0f, 0.0f),
					false,
					L"PlayerBullet");
			}
		}

		for (int i = 0; i < 200; i++) {
			AddGameObject<BulletObject>(
				L"SPARK_TX",
				Vec3(0.5f, 0.5f, 0.5f),
				Quat(),
				Vec3(0.0f, -10.0f, 0.0f),
				false,
				L"BossBullet");
		}

		for (int i = 0; i < 10; i++) {
			float x = (float)(i + 1);
			AddGameObject<AngelEnemy>(
				Par,
				L"ANGEL_MESH",
				L"ANGEL_TX",
				L"30frame",
				Vec3(0.19f, 0.19f, 0.19f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
		}


		for (int i = 0; i < 1; i++) {
			float x = (float)(i + 1);
			auto ptr = AddGameObject<CR_BossEnemy>(
				Par,
				L"HAND_BOSS_MESH",
				L"HAND_BOSS_TX",
				L"30frame",
				Vec3(1.8f, 1.8f, 1.8f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 10.0f - 5.0f, 0.125f, 60.0f - (float)rand() / 32767 * 10.0f),
				false);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"LeftHand",
				Vec3(0.5f, 1.0f, 0.5f),
				Quat(),
				Vec3(0.0f, 0.5f, 0.0f),
				true);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"RightHand",
				Vec3(0.5f, 1.0f, 0.5f),
				Quat(),
				Vec3(0.0f, 0.5f, 0.0f),
				true);
		}


		for (int i = 0; i < 1; i++) {
			float x = (float)(i + 1);
			AddGameObject<LD_BossEnemy>(
				Par,
				L"THOR_BOSS_MESH",
				L"THOR_BOSS_TX",
				L"30frame",
				Vec3(3.0f, 0.8f, 3.0f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 10.0f - 5.0f, 0.125f, 60.0f - (float)rand() / 32767 * 10.0f),
				false);
		}


		//スパークエフェクト
		AddGameObject<AttackSpark>();
		AddGameObject<MultiSpark>();
		//ファイアエフェクト
		AddGameObject<MultiFire>();
		AddGameObject <ChainEffect>();
		AddGameObject<AttackSigns>();
		AddGameObject<HandBossSpark>();
		AddGameObject<BossAttackSigns>();
		AddGameObject<MiddleBossAttackSigns>();
		AddGameObject<StepEffect>();
		AddGameObject<ShootingEffect>();
		AddGameObject<CannonEffect>();
		AddGameObject<BossEffect>();
		AddGameObject<BossPrepareAttackEffect>();
		AddGameObject<BossBulletEffect>();
		AddGameObject<MultiGuardEffect>();
		AddGameObject<EnemyMoveEffect>();

		//描画オブジェクトの追加
		CreateDrawObjects();

		AddGameObject<BossHPGauge>(
			L"BOSS_BAR_TX",
			Vec2(960, 24),
			0.0f,
			Vec2(-480, -330),
			1, 1
			);

		AddGameObject<UISprite>(
			L"BOSS_FRAME_TX",
			Vec2(1000, 50),
			0.0f,
			Vec2(0, -330),
			1, 1
			);
		AddGameObject<CannonGauge>(
			L"CANNON_BAR_TX",
			Vec2(300, 18),
			0.0f,
			Vec2(-555 + 64, 330),
			1, 1
			);
		AddGameObject<UISprite>(
			L"CANNON_FRAME_TX",
			Vec2(320, 40),
			0.0f,
			Vec2(-405 + 64, 330),
			1, 1
			);

		AddGameObject<UISprite>(
			L"BOSS_ICON_TX",
			Vec2(128.0f * 0.8f, 128.0f * 0.8f),
			0.0f,
			Vec2(-530, -320),
			1, 1
			);
		AddGameObject<UISprite>(
			L"KUREHA_ICON_TX",
			Vec2(64, 64),
			0.0f,
			Vec2(-600, 325),
			1, 1
			);
		AddGameObject<UISprite>(
			L"CANNON_ICON_TX",
			Vec2(52, 52),
			0.0f,
			Vec2(-530, 325),
			1, 1
			);

		for (int i = 0; i < 20; i++) {
			AddGameObject<ChainUISprite>(
				L"TRANS_TX",
				Vec2(30, 30),
				0.0f,
				Vec2(-600, 270 - i * 30),
				1, 1,
				i);
		}

		SetActiveObjects(false);
		//文字列描画オブジェクトの作成
		AddGameObject<StringDrawObject>();

		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"BGM_1");
		m_AudioObjectPtr->Start(L"BGM_1", XAUDIO2_LOOP_INFINITE, 0.4f);

	}

	//描画オブジェクトの追加
	void GameStage::CreateDrawObjects() {
		//シャドウマップ描画オブジェクトの作成
		AddGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");

		//SimplePCTStaticRenderer描画オブジェクトの作成
		AddGameObject<SimplePCTStaticRenderer>(L"SimplePCTStaticRenderer");

		//SimplePNTStaticRenderer描画オブジェクトの作成
		AddGameObject<SimplePNTStaticRenderer>(L"SimplePNTStaticRenderer");
		//SimplePNTStaticRenderer2描画オブジェクトの作成
		AddGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");

		//SimplePNTStaticModelRenderer描画オブジェクトの作成
		AddGameObject<SimplePNTStaticModelRenderer>(L"SimplePNTStaticModelRenderer");
		//SimplePNTStaticModelRenderer2描画オブジェクトの作成
		AddGameObject<SimplePNTStaticModelRenderer2>(L"SimplePNTStaticModelRenderer2");

		//SimplePNTBoneModelRenderer描画オブジェクトの作成
		AddGameObject<SimplePNTBoneModelRenderer>(L"SimplePNTBoneModelRenderer");
		//SimplePNTBoneModelRenderer2描画オブジェクトの作成
		AddGameObject<SimplePNTBoneModelRenderer2>(L"SimplePNTBoneModelRenderer2");

		//BcPNTStaticDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
		//BcPNTnTStaticDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");

		//BcPNTStaticModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTStaticModelRenderer>(L"BcPNTStaticModelRenderer");
		//BcPNTnTStaticModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTnTStaticModelRenderer>(L"BcPNTnTStaticModelRenderer");

		//BcPNTBoneModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
		//BcPNTnTBoneModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTnTBoneModelRenderer>(L"BcPNTnTBoneModelRenderer");

		//Particle描画オブジェクトの作成(加算処理しない)
		AddGameObject<ParticleDrawObject>(false);
		//Particle描画オブジェクトの作成(加算処理する)
		AddGameObject<ParticleDrawObject>(true);

	}


	void GameStage::OnUpdateStage() {
		//ターン毎の初期化
		GetRigidbodyManager()->InitRigidbody();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの更新
			v->OnUpdate();
		}
		//Rigidbodyマネージャの更新（衝突判定など）
		GetRigidbodyManager()->OnUpdate();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの最終更新
			v->OnUpdate2();
		}
		//自分自身の更新(カメラ)
		this->OnUpdate();
		//Rigidbodyマネージャの最終更新（衝突判定情報のクリア）
		GetRigidbodyManager()->OnUpdate2();
	}


	void GameStage::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto& camera = GetCamera();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			auto GM = GameManager::getInstance();
			GM->m_camera_length = camera.m_CameraArmLen;
			auto PlayerPtr = FindTagGameObject<Player>(L"Player");
			auto boss = FindTagGameObject<Boss>(L"BossEnemy");

			//チェインがある程度長いときは移動しているときカメラを引く
			int now_friends_num = GM->GetFriendsNum();
			if (now_friends_num >= 3) {
				Vec3 PlayerVelo = PlayerPtr->GetVelocity();
				if (PlayerVelo.length() > 3.0f && camera.m_CameraArmLen < 5.0f) {
					camera.m_CameraArmLen += 0.02f;
				}
				else if (PlayerVelo.length() < 3.0f && camera.m_CameraArmLen > 2.6f) {
					camera.m_CameraArmLen -= 0.08f;
				}
			}

			int now_cannon = PlayerPtr->GetIsCannon();
			if (now_cannon < 3) {
				Vec3 boss_pos = boss->GetPosition();

				if (PlayerPtr->GetCannonAnimation()) {
					camera.m_CamerAt = PlayerPtr->GetPosition();
					camera.m_CamerAt.y += 0.7f;
					camera.m_CamerAt.z += 0.5f;
					camera.m_CamerEye = PlayerPtr->GetPosition();
					camera.m_CamerEye.x -= 1.8f;
					camera.m_CamerEye.z += 2.0f;
					camera.m_CamerEye.y += 0.5f;
				}
				else {
					if (now_cannon == 0) {
						vector<shared_ptr<GameObject>> ShootedEnemyVec;
						FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
						auto c = FindTagGameObject<Cannon>(L"GREEN_CANNON");
						int max_num = 0;
						Vec3 ShootPos;
						for (auto enemy : ShootedEnemyVec) {
							if (enemy) {
								auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
								if (PtrEnemy->GetShootNumber() > max_num) {
									max_num = PtrEnemy->GetShootNumber();
									ShootPos = PtrEnemy->GetPosition();
								}
							}
						}
						if (max_num == 0) {
							if (m_FrameCount > 1.0f) {
								CannonStateEndBehaviour();
								PlayerPtr->SetIsCannnon(3);
								m_FrameCount = 0.0f;
								if (boss->GetHP() > 0.0f) {
									boss->ChangeAnimation(L"Default");
								}
								return;
							}
							m_FrameCount += ElapsedTime;
							return;
						}
						c->Rotation(boss_pos);
						camera.m_CamerAt = boss_pos;
						camera.m_CameraArmLen = 30;
						camera.m_CamerEye = ShootPos;
						camera.m_CamerEye.x -= 4.0f;
						camera.m_CamerEye.z -= 7.0f;
					}
					if (now_cannon == 1) {
						vector<shared_ptr<GameObject>> ShootedEnemyVec;
						FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
						auto c = FindTagGameObject<Cannon>(L"RED_CANNON");
						int max_num = 0;
						Vec3 ShootPos;
						for (auto enemy : ShootedEnemyVec) {
							if (enemy) {
								auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
								if (PtrEnemy->GetShootNumber() > max_num) {
									max_num = PtrEnemy->GetShootNumber();
									ShootPos = PtrEnemy->GetPosition();
								}
							}
						}
						if (max_num == 0) {
							if (m_FrameCount > 1.0f) {
								CannonStateEndBehaviour();
								PlayerPtr->SetIsCannnon(3);
								m_FrameCount = 0.0f;
								if (boss->GetHP() > 0.0f) {
									boss->ChangeAnimation(L"Default");
								}
								return;
							}
							m_FrameCount += ElapsedTime;
							return;
						}

						c->Rotation(boss_pos);
						camera.m_CamerAt = boss_pos;
						camera.m_CameraArmLen = 30;
						camera.m_CamerEye = ShootPos;
						camera.m_CamerEye.x -= 3.0f;
						camera.m_CamerEye.z -= 7.0f;
					}
					if (now_cannon == 2) {
						vector<shared_ptr<GameObject>> ShootedEnemyVec;
						FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
						auto c = FindTagGameObject<Cannon>(L"BLUE_CANNON");
						int max_num = 0;
						Vec3 ShootPos;
						for (auto enemy : ShootedEnemyVec) {
							if (enemy) {
								auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
								if (PtrEnemy->GetShootNumber() > max_num) {
									max_num = PtrEnemy->GetShootNumber();
									ShootPos = PtrEnemy->GetPosition();
								}
							}
						}
						if (max_num == 0) {
							if (m_FrameCount > 1.0f) {
								CannonStateEndBehaviour();
								PlayerPtr->SetIsCannnon(3);
								m_FrameCount = 0.0f;
								if (boss->GetHP() > 0.0f) {
									boss->ChangeAnimation(L"Default");
								}
								return;
							}
							m_FrameCount += ElapsedTime;
							return;
						}
						c->Rotation(boss_pos);
						camera.m_CamerAt = boss_pos;
						camera.m_CameraArmLen = 30;
						camera.m_CamerEye = ShootPos;
						camera.m_CamerEye.x -= 2.0f;
						camera.m_CamerEye.z -= 7.0f;
					}
				}
			}
			else if (boss->GetIsLooked()) {

				Vec3 boss_pos = boss->GetPosition();
				camera.m_CamerAt = boss_pos;
				camera.m_CamerAt.y -= 2.0f;
				camera.m_CameraArmLen = 30;
				camera.m_CamerEye = Vec3(-5, 5, 50);
				camera.m_CamerEye.x -= 4.0f;
				camera.m_CamerEye.z -= 7.0f;

			}
			else if (m_isGameStart == false) {
				//最初のカメラワーク
				if (m_CameraPos.z > -150) {
					camera.m_CamerAt = Vec3(0, 0, 30);
					camera.m_CameraArmLen = 100;
					//ここ
					m_CameraPos.z -= ElapsedTime * 500.0f;
					camera.m_CamerEye = m_CameraPos;
				}
				else if (m_CameraPos.z <= -150) {
					m_FogActive = true;
					camera.m_CameraArmLen = 2.6f;
					
					camera.m_CamerAt = PlayerPtr->GetPosition();
					camera.m_CamerAt.y += m_StartCameraY;
					if (m_StartCameraY > 0.5f) {
						m_StartCameraY -= ElapsedTime * 5.0f;
					}
					else {
						m_isGameStart = true;
						SetActiveObjects(true);
					}

					Vec3 CameraLocalEye =
						Vec3(
							sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
							cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
							-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
						);
					camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
				}
			}
			else {
				if (camera.m_CameraArmLen > 20) {
					camera.m_CameraArmLen = 2.6f;
				}

				if (!m_isClear) {
					if (CntlVec[0].fThumbRX != 0) {
						camera.m_CameraXZRad -= CntlVec[0].fThumbRX * 0.05f;
						if (abs(camera.m_CameraXZRad) >= XM_2PI) {
							camera.m_CameraXZRad = 0;
						}
					}
					if (CntlVec[0].fThumbRY != 0) {
						camera.m_CameraYRad += CntlVec[0].fThumbRY * 0.05f;
						if (camera.m_CameraYRad >= 2.0f) {
							camera.m_CameraYRad = 2.0f;
						}
						else if (camera.m_CameraYRad <= 0.5f) {
							camera.m_CameraYRad = 0.5f;
						}
						if (camera.m_CameraYRad >= XM_PIDIV2 - 0.3f) {
							if (CntlVec[0].fThumbRY > 0) {
								camera.m_CameraArmLen -= CntlVec[0].fThumbRY * 0.1f;
								if (GetCamera().m_CameraArmLen <= 1.5f) {
									camera.m_CameraArmLen = 1.5f;
								}
							}
							else if (CntlVec[0].fThumbRY < 0) {
								camera.m_CameraArmLen -= CntlVec[0].fThumbRY * 0.1f;
								if (GetCamera().m_CameraArmLen >= 50.0f) {
									GetCamera().m_CameraArmLen = 50.0f;
								}
							}
						}
					}
				}

				camera.m_CamerAt = PlayerPtr->GetPosition();
				camera.m_CamerAt.y += 0.5f;

				Vec3 CameraLocalEye =
					Vec3(
						sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
						cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
						-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
					);
				camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
			}
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				//PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
				//SetIsFail(true);
			}
			
		}
	}

	void GameStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));

		//シャドオウマップ描画の開始
		Dev->ClearShadowmapViews();
		Dev->StartShadowmapDraw();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの描画
			v->OnDrawShadowmap();
		}
		Dev->EndShadowmapDraw();

		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの描画
			v->OnDraw();
		}
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void GameStage::OnDraw() {
		m_RigidbodyManager->OnDraw();
	}

	void GameStage::SetActiveObjects(bool value) {
		vector<shared_ptr<GameObject>> ZakoVec;
		FindTagGameObjectVec(L"EnemyObject", ZakoVec);
		for (auto zako : ZakoVec) {
			if (zako) {
				auto PtrZako = dynamic_pointer_cast<EnemyObject>(zako);
				PtrZako->SetUpdateActive(value);
			}
		}
		vector<shared_ptr<GameObject>> BulletVec;
		FindTagGameObjectVec(L"AllBullet", BulletVec);
		for (auto bullet : BulletVec) {
			if (bullet) {
				auto PtrBullet = dynamic_pointer_cast<BulletObject>(bullet);
				PtrBullet->SetActive(value);
			}
		}
		vector<shared_ptr<GameObject>> HandVec;
		FindTagGameObjectVec(L"BossHand", HandVec);
		for (auto hand : HandVec) {
			if (hand) {
				auto PtrHand = dynamic_pointer_cast<BossHand>(hand);
				PtrHand->SetUpdateActive(value);
			}
		}
		auto player = FindTagGameObject<Player>(L"Player");
		if (player) {
			player->SetUpdateActive(value);
		}
	}

	//砲撃が始まったとき敵の動きを止める
	void GameStage::CannonStateStartBehaviour() {
		SetActiveObjects(false);
	}

	//砲撃が終わった後の呼ばれる関数
	void GameStage::CannonStateEndBehaviour() {
		SetActiveObjects(true);
	}

	//ゲームクリアしたあとによばれる
	void GameStage::GameClearBehaviour() {
		m_AudioObjectPtr->Stop(L"BGM_1");
		
		SetActiveObjects(false);

		AddGameObject<ResultSprite>(
			L"RESULT_BACK",
			Vec2(1280, 720),
			0.0f,
			Vec2(0, 0),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"CLEAR_LOGO",
			Vec2(600, 300),
			0.0f,
			Vec2(0, 250),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TIME_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-200, -30),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"LIFE_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-250, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"PAERCENT_LOGO",
			Vec2(80, 80),
			0.0f,
			Vec2(300, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RANK_LOGO",
			Vec2(400, 100),
			0.0f,
			Vec2(-100, 120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RESULT_S",
			Vec2(180, 180),
			0.0f,
			Vec2(110, 120),
			1, 1
			);

		AddGameObject<ResultCursorSprite>(
			L"RESULT_ARROW",
			Vec2(320, 80),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"RETRY_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TO_TITLE_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(130, -240),
			1, 1
			);
	}

	//ゲームおーばーしたとき
	void GameStage::GameOverBehaviour() {
		m_AudioObjectPtr->Stop(L"BGM_1");

		SetActiveObjects(false);

		AddGameObject<ResultSprite>(
			L"RESULT_BACK",
			Vec2(1280, 720),
			0.0f,
			Vec2(0, 0),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"FAIL_LOGO",
			Vec2(600, 300),
			0.0f,
			Vec2(0, 250),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TIME_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-200, -30),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"LIFE_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-250, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"PAERCENT_LOGO",
			Vec2(80, 80),
			0.0f,
			Vec2(300, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RANK_LOGO",
			Vec2(400, 100),
			0.0f,
			Vec2(-100, 120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RESULT_D",
			Vec2(180, 180),
			0.0f,
			Vec2(110, 120),
			1, 1
			);
		AddGameObject<ResultCursorSprite>(
			L"RESULT_ARROW",
			Vec2(320, 80),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"RETRY_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TO_TITLE_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(130, -240),
			1, 1
			);
		
	}

	//--------------------------------------------------------------------------------------
	///	空のステージ（メッセージのみある）
	//--------------------------------------------------------------------------------------
	void EmptyStage::OnCreate() {
		//メッセージスプライト
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(0, 0),
			1, 1);
	}
	void EmptyStage::OnUpdateStage() {
		//スプライトの更新
		m_MessageSprite->OnUpdate();
		//自分自身の更新
		this->OnUpdate();
	}
	void EmptyStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void EmptyStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		//スプライト描画
		m_MessageSprite->OnDraw();
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void EmptyStage::OnDraw() {
		//何もしない
	}




}
//end basecross
