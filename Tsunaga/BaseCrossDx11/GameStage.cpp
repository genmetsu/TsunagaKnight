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
			MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE",
				MeshResource::CreateMeshResource(vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {

		//シャドウマップの描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

		//背景スプライトの作成
		/*AddGameObject<MultiSprite>(
			L"SKY_TX",
			Vec2(1280, 800),
			0.0f,
			Vec2(0, 0),
			1, 1
			);*/
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
		/*AddGameObject<SquareObject>(
			L"WALL_TX",
			L"WALL_NORMAL_TX",
			Vec3(20.0f, 100.0f, 1.0f),
			Qt,
			Vec3(0.0f, 0.0f, 40.0f)
			);*/

		AddGameObject<YamatoStage>(
			L"YAMATO_TX",
			Vec3(4.0f, 4.0f, 4.0f),
			Quat(),
			Vec3(0.0f, -12.8f, -20.0f),
			true);

		//プレイヤーの作成
		//shared_ptr<GameObject> 
		auto Par =
			AddGameObject<Player>(
				L"KUREHA_TX",
				true,
				Vec3(0.0f, 0.5f, 10.0f)
				);
		//SetPlayerPtr(Par);

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
			switch (i) {
			case 0:
				texture = L"GREEN_CANNON_TX";
				break;
			case 1:
				texture = L"RED_CANNON_TX";
				break;
			case 2:
				texture = L"BLUE_CANNON_TX";
				break;
			}
			AddGameObject<Cannon>(
				texture,
				Vec3(4.0f, 4.0f, 4.0f),
				Quat(),
				Vec3(x * 6.0f - 6.0f, 0.0f, -3.0f),
				i, false);
		}

		for (int i = 0; i < 20; i++) {
			float x = (float)(i + 1);
			AddGameObject<NeedleEnemy>(
				Par,
				L"NEEDLE_MESH",
				L"NEEDLE_TX",
				L"30frame",
				Vec3(0.3f, 0.3f, 0.3f),
				Quat(),
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
				Vec3(0.25f, 0.25f, 0.25f),
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
			for (int j = 0; j < 2; j++) {
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
		for (int i = 0; i < 1; i++) {
			float x = (float)(i + 1);
			AddGameObject<BossEnemy>(
				Par,
				L"NEEDLE_MESH",
				L"NEEDLE_TX",
				L"30frame",
				Vec3(10.0f, 10.0f, 10.0f),
				Quat(),
				Vec3(0.0f, 3.0f, 80.0f),
				false);
			for (int j = 0; j < 3; j++) {
				AddGameObject<BulletObject>(
					L"RED_TX",
					Vec3(0.5f, 0.5f, 0.5f),
					Quat(),
					Vec3(0.0f, -10.0f, 0.0f),
					false,
					L"BossBullet");
			}
		}

		for (int i = 0; i < 10; i++) {
			float x = (float)(i + 1);
			AddGameObject<AngelEnemy>(
				Par,
				L"ANGEL_MESH",
				L"ANGEL_TX",
				L"30frame",
				Vec3(0.25f, 0.25f, 0.25f),
				Quat(),
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
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"LEFT_HAND",
				Vec3(0.5f, 1.0f, 0.5f),
				Quat(),
				Vec3(0.0f, 0.5f, 0.0f),
				true);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"RIGHT_HAND",
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
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
		}


		//スパークエフェクト
		AddGameObject<MultiSpark>();
		//ファイアエフェクト
		AddGameObject<MultiFire>();
		AddGameObject<AttackSigns>();
		AddGameObject<StepEffect>();

		//描画オブジェクトの追加
		CreateDrawObjects();


		//回転するスプライトの作成
		/*AddGameObject<RotateSprite>(
			L"TRACE_TX",
			Vec2(160, 160),
			0.0f,
			Vec2(-480, 260),
			4, 4
			);*/


			//メッセージを表示するスプライトの作成
			/*AddGameObject<MessageSprite>(
				L"MESSAGE_TX",
				Vec2(256, 64),
				0.0f,
				Vec2(480, 260),
				1, 1
				);*/

				//文字列描画オブジェクトの作成
		AddGameObject<StringDrawObject>();

		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"Nanika");
		m_AudioObjectPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.0f);

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
		auto& camera = GetCamera();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				camera.m_CameraArmLen += 0.3f;
				if (GetCamera().m_CameraArmLen >= 50.0f) {
					GetCamera().m_CameraArmLen = 50.0f;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				camera.m_CameraArmLen -= 0.3f;
				if (GetCamera().m_CameraArmLen <= 2.0f) {
					camera.m_CameraArmLen = 2.0f;
				}
			}

			if (CntlVec[0].fThumbRX != 0) {
				camera.m_CameraXZRad -= CntlVec[0].fThumbRX * 0.05f;
				if (abs(camera.m_CameraXZRad) >= XM_2PI) {
					camera.m_CameraXZRad = 0;
				}
			}

			if (CntlVec[0].fThumbRY != 0) {
				camera.m_CameraYRad += CntlVec[0].fThumbRY * 0.05f;
				if (camera.m_CameraYRad >= XM_PIDIV2 - 0.05f) {
					camera.m_CameraYRad = XM_PIDIV2 - 0.05f;
				}
				else if (camera.m_CameraYRad <= 0.5f) {
					camera.m_CameraYRad = 0.5f;
				}
			}

			/*auto GM = GameManager::getInstance();
			int now_friends_num = GM->GetFriendsNum();
			if (now_friends_num >= 6) {
				camera.m_CameraArmLen = (float)now_friends_num * 0.5f;
				if (GetCamera().m_CameraArmLen >= 50.0f) {
					GetCamera().m_CameraArmLen = 50.0f;
				}
			}*/

			camera.m_CamerAt = FindTagGameObject<GameObject>(L"Player")->GetPosition();
			camera.m_CamerAt.y += 0.5f;
			Vec3 CameraLocalEye =
				Vec3(
					sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
					cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
					-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
				);
			camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
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
