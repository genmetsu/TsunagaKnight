/*!
@file Scene.cpp
@brief シーンなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ステージ（シーンで管理するインターフェイス）
	//--------------------------------------------------------------------------------------
	void Stage::OnPreCreate() {
		//Rigidbodyマネージャの初期化
		m_RigidbodyManager
			= ObjectFactory::Create<RigidbodyManager>(GetThis<Stage>());
	}


	//追加オブジェクトの指定
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//このステージはクリエイト後である
		if (IsCreated()) {
			m_WaitAddObjectVec.push_back(Ptr);
		}
		else {
			//クリエイト前
			m_GameObjectVec.push_back(Ptr);
		}
	}
	//削除オブジェクトの指定
	void Stage::RemoveBackGameObject(const shared_ptr<GameObject>& Ptr) {
		m_WaitRemoveObjectVec.push_back(Ptr);
	}
	//オブジェクトの削除
	void Stage::RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj) {
		auto it = m_GameObjectVec.begin();
		while (it != m_GameObjectVec.end()) {
			if (*it == targetobj) {
				m_GameObjectVec.erase(it);
				return;
			}
			it++;
		}
		m_RigidbodyManager->RemoveOwnRigidbody(targetobj);
	}
	//追加や削除待ちになってるオブジェクトを追加削除する
	void Stage::SetWaitToObjectVec() {
		if (!m_WaitRemoveObjectVec.empty()) {
			for (auto Ptr : m_WaitRemoveObjectVec) {
				RemoveTargetGameObject(Ptr);
			}
		}
		m_WaitRemoveObjectVec.clear();
		if (!m_WaitAddObjectVec.empty()) {
			for (auto Ptr : m_WaitAddObjectVec) {
				m_GameObjectVec.push_back(Ptr);
			}
		}
		m_WaitAddObjectVec.clear();
	}

	void Stage::FindTagGameObjectVec(const wstring& TagName, vector<shared_ptr<GameObject>>& Ret) const {
		Ret.clear();
		for (auto& v : GetGameObjectVec()) {
			if (v->FindTag(TagName)) {
				Ret.push_back(v);
			}
		}
	}

	shared_ptr<RigidbodyManager> Stage::GetRigidbodyManager() const {
		return m_RigidbodyManager;
	}

	shared_ptr<Rigidbody> Stage::AddRigidbody(const Rigidbody& body) {
		return m_RigidbodyManager->AddRigidbody(body);
	}

	void Stage::RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		m_RigidbodyManager->RemoveOwnRigidbody(OwnerPtr);
	}

	const vector<shared_ptr<Rigidbody>>& Stage::GetRigidbodyVec()const {
		return m_RigidbodyManager->GetRigidbodyVec();
	}
	const vector<CollisionState>& Stage::GetCollisionStateVec()const {
		return m_RigidbodyManager->GetCollisionStateVec();
	}
	shared_ptr<Rigidbody> Stage::GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		return m_RigidbodyManager->GetOwnRigidbody(OwnerPtr);
	}




	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	Scene::Scene() :
		SceneInterface()
	{
	}

	void Scene::CreateResources() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"space4.png";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"StageMessage.png";
		App::GetApp()->RegisterTexture(L"MESSAGE_TX", strTexture);
		strTexture = DataDir + L"Red.png";
		App::GetApp()->RegisterTexture(L"RED_TX", strTexture);
		strTexture = DataDir + L"Balloon0.png";
		App::GetApp()->RegisterTexture(L"Balloon0_TX", strTexture);
		strTexture = DataDir + L"Checker.png";
		App::GetApp()->RegisterTexture(L"Checker_TX", strTexture);
		strTexture = DataDir + L"gray2.png";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"wall_normal.png";
		App::GetApp()->RegisterTexture(L"WALL_NORMAL_TX", strTexture);
		strTexture = DataDir + L"Brown.png";
		App::GetApp()->RegisterTexture(L"BROWN_TX", strTexture);
		strTexture = DataDir + L"normal2.png";
		App::GetApp()->RegisterTexture(L"NORMAL2_TX", strTexture);
		strTexture = DataDir + L"Kureha_ALL_Tex.tga";
		App::GetApp()->RegisterTexture(L"KUREHA_TX", strTexture);
		strTexture = DataDir + L"Ymato_TX_02.tga";
		App::GetApp()->RegisterTexture(L"YAMATO_TX", strTexture);
		strTexture = DataDir + L"Boss_tx_2.tga";
		App::GetApp()->RegisterTexture(L"BOSS_TX", strTexture);
		strTexture = DataDir + L"E_Needle_UV2.png";
		App::GetApp()->RegisterTexture(L"THOR_BOSS_TX", strTexture);
		strTexture = DataDir + L"Hand_Main_UV.png";
		App::GetApp()->RegisterTexture(L"HAND_BOSS_TX", strTexture);
		strTexture = DataDir + L"Hand_Sub_UV.png";
		App::GetApp()->RegisterTexture(L"HAND_TX", strTexture);
		strTexture = DataDir + L"E_Needle.UV2.png";
		App::GetApp()->RegisterTexture(L"NEEDLE_TX", strTexture);
		strTexture = DataDir + L"E_Rocket2.png";
		App::GetApp()->RegisterTexture(L"GUN_TX", strTexture);
		strTexture = DataDir + L"E_Angel_UV.png";
		App::GetApp()->RegisterTexture(L"ANGEL_TX", strTexture);
		strTexture = DataDir + L"Brown.png";
		App::GetApp()->RegisterTexture(L"Brown_TX", strTexture);
		strTexture = DataDir + L"Brown2.png";
		App::GetApp()->RegisterTexture(L"Brown2_TX", strTexture);
		strTexture = DataDir + L"Blue.png";
		App::GetApp()->RegisterTexture(L"Blue_TX", strTexture);
		strTexture = DataDir + L"Large_Cannon_UV_2.png";
		App::GetApp()->RegisterTexture(L"BLUE_CANNON_TX", strTexture);
		strTexture = DataDir + L"Large_Cannon_UV_R.png";
		App::GetApp()->RegisterTexture(L"RED_CANNON_TX", strTexture);
		strTexture = DataDir + L"Large_Cannon_UV_G.png";
		App::GetApp()->RegisterTexture(L"GREEN_CANNON_TX", strTexture);
		strTexture = DataDir + L"Angel_Icon.png";
		App::GetApp()->RegisterTexture(L"ANGEL_ICON_TX", strTexture);
		strTexture = DataDir + L"Gun_Icon.png";
		App::GetApp()->RegisterTexture(L"GUN_ICON_TX", strTexture);
		strTexture = DataDir + L"Needle_Icon.png";
		App::GetApp()->RegisterTexture(L"NEEDLE_ICON_TX", strTexture);
		strTexture = DataDir + L"UI_Head.png";
		App::GetApp()->RegisterTexture(L"KUREHA_ICON_TX", strTexture);

		strTexture = DataDir + L"TsunagaKnight.png";
		App::GetApp()->RegisterTexture(L"TsunagaKnight_TX", strTexture);
		strTexture = DataDir + L"PresAnyButton.png";
		App::GetApp()->RegisterTexture(L"PresAnyButton_TX", strTexture);
		strTexture = DataDir + L"kuro.png";
		App::GetApp()->RegisterTexture(L"kuro_TX", strTexture);



		strTexture = DataDir + L"Boss_HP_Icon.png";
		App::GetApp()->RegisterTexture(L"BOSS_ICON_TX", strTexture);
		strTexture = DataDir + L"Cannon_HP_Icon.png";
		App::GetApp()->RegisterTexture(L"CANNON_ICON_TX", strTexture);
		strTexture = DataDir + L"Boss_HP_Bar_v2.png";
		App::GetApp()->RegisterTexture(L"BOSS_BAR_TX", strTexture);
		strTexture = DataDir + L"Boss_HP_Frame.png";
		App::GetApp()->RegisterTexture(L"BOSS_FRAME_TX", strTexture);
		strTexture = DataDir + L"Cannon_HP_Bar_v2.png";
		App::GetApp()->RegisterTexture(L"CANNON_BAR_TX", strTexture);
		strTexture = DataDir + L"Cannon_HP_Frame.png";
		App::GetApp()->RegisterTexture(L"CANNON_FRAME_TX", strTexture);
		strTexture = DataDir + L"Player_HP_Bar.png";
		App::GetApp()->RegisterTexture(L"PLAYER_BAR_TX", strTexture);
		strTexture = DataDir + L"Player_HP_Frame.png";
		App::GetApp()->RegisterTexture(L"PLAYER_FRAME_TX", strTexture);
		strTexture = DataDir + L"Result_UI_A.png";
		App::GetApp()->RegisterTexture(L"RESULT_A", strTexture);
		strTexture = DataDir + L"Result_UI_B.png";
		App::GetApp()->RegisterTexture(L"RESULT_B", strTexture);
		strTexture = DataDir + L"Result_UI_C.png";
		App::GetApp()->RegisterTexture(L"RESULT_C", strTexture);
		strTexture = DataDir + L"Result_UI_D.png";
		App::GetApp()->RegisterTexture(L"RESULT_D", strTexture);
		strTexture = DataDir + L"Result_UI_S.png";
		App::GetApp()->RegisterTexture(L"RESULT_S", strTexture);
		strTexture = DataDir + L"Result_UI_Clear.png";
		App::GetApp()->RegisterTexture(L"CLEAR_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_Miss.png";
		App::GetApp()->RegisterTexture(L"FAIL_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_frame.png";
		App::GetApp()->RegisterTexture(L"RESULT_FRAME", strTexture);
		strTexture = DataDir + L"Result_UI_Life.png";
		App::GetApp()->RegisterTexture(L"LIFE_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_BG.png";
		App::GetApp()->RegisterTexture(L"RESULT_BACK", strTexture);
		strTexture = DataDir + L"Result_UI_retry.png";
		App::GetApp()->RegisterTexture(L"RETRY_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_Play.png";
		App::GetApp()->RegisterTexture(L"TIME_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_Percet.png";
		App::GetApp()->RegisterTexture(L"PAERCENT_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_Life.png";
		App::GetApp()->RegisterTexture(L"LIFE_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_Rank.png";
		App::GetApp()->RegisterTexture(L"RANK_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_titel.png";
		App::GetApp()->RegisterTexture(L"TO_TITLE_LOGO", strTexture);
		strTexture = DataDir + L"Result_UI_Arrow.png";
		App::GetApp()->RegisterTexture(L"RESULT_ARROW", strTexture);

		strTexture = DataDir + L"space3.jpeg";
		App::GetApp()->RegisterTexture(L"TITLE_TX", strTexture);

		strTexture = DataDir + L"Boss_HP_Icon_R_01.png";
		App::GetApp()->RegisterTexture(L"Boss_HP_Icon_R_01_TX", strTexture);

		strTexture = DataDir + L"Boss_HP_Icon_R_02.png";
		App::GetApp()->RegisterTexture(L"Boss_HP_Icon_R_02_TX", strTexture);

		strTexture = DataDir + L"Boss_HP_Icon_G_01.png";
		App::GetApp()->RegisterTexture(L"Boss_HP_Icon_G_01_TX", strTexture);

		strTexture = DataDir + L"Boss_HP_Icon_G_02.png";
		App::GetApp()->RegisterTexture(L"Boss_HP_Icon_G_02_TX", strTexture);

		strTexture = DataDir + L"Boss_HP_Icon_B_01.png";
		App::GetApp()->RegisterTexture(L"Boss_HP_Icon_B_01_TX", strTexture);

		strTexture = DataDir + L"Boss_HP_Icon_B_02.png";
		App::GetApp()->RegisterTexture(L"Boss_HP_Icon_B_02_TX", strTexture);

		strTexture = DataDir + L"UI_Round_1.png";
		App::GetApp()->RegisterTexture(L"UI_Round_1_TX", strTexture);

		strTexture = DataDir + L"UI_Round_2.png";
		App::GetApp()->RegisterTexture(L"UI_Round_2_TX", strTexture);

		strTexture = DataDir + L"UI_Round_3.png";
		App::GetApp()->RegisterTexture(L"UI_Round_3_TX", strTexture);

		strTexture = DataDir + L"UI_Round_4.png";
		App::GetApp()->RegisterTexture(L"UI_Round_4_TX", strTexture);

		strTexture = DataDir + L"UI_Round_5.png";
		App::GetApp()->RegisterTexture(L"UI_Round_5_TX", strTexture);

		strTexture = DataDir + L"number.png";
		App::GetApp()->RegisterTexture(L"number_TX", strTexture);

		strTexture = DataDir + L"Transparent.png";
		App::GetApp()->RegisterTexture(L"TRANS_TX", strTexture);
		
		strTexture = DataDir + L"TitleLogoBack.png";
		App::GetApp()->RegisterTexture(L"TITLE_LOGO_BACK_TX", strTexture);

		//ボーンモデルのリソース
		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Kureha_fbx_v5.bmf");
		App::GetApp()->RegisterResource(L"KUREHA_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"E_NeedleV2.bmf");
		App::GetApp()->RegisterResource(L"NEEDLE_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"E_GunV2.bmf");
		App::GetApp()->RegisterResource(L"GUN_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"E_Angel.bmf");
		App::GetApp()->RegisterResource(L"ANGEL_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Large_Cannon_Blue_Blast.bmf");
		App::GetApp()->RegisterResource(L"CANNON_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Medium_Boss_Thor_AnimeAll.bmf");
		App::GetApp()->RegisterResource(L"THOR_BOSS_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Boss_Fbx_v02.bmf");
		App::GetApp()->RegisterResource(L"BOSS_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Yamato_Fbx_v05.bmf");
		App::GetApp()->RegisterResource(L"YAMATO_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Medium_Boss_Hand_Main.bmf");
		App::GetApp()->RegisterResource(L"HAND_BOSS_MESH", ModelMesh);


		//スタティックモデルのリソースとして読み込み
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Medium_Boss_Hand_Left.bmf");
		App::GetApp()->RegisterResource(L"LEFT_HAND_MESH", StaticModelMesh);
		StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Medium_Boss_Hand_Right.bmf");
		App::GetApp()->RegisterResource(L"RIGHT_HAND_MESH", StaticModelMesh);
		
		

		//エフェクト
		strTexture = DataDir + L"spark.png";
		App::GetApp()->RegisterTexture(L"SPARK_TX", strTexture);
		strTexture = DataDir + L"fire.png";
		App::GetApp()->RegisterTexture(L"FIRE_TX", strTexture);
		strTexture = DataDir + L"step.png";
		App::GetApp()->RegisterTexture(L"STEP_TX", strTexture);
		strTexture = DataDir + L"UI_Star.png";
		App::GetApp()->RegisterTexture(L"STAR_TX", strTexture);

		// BGM
		wstring strMusic = DataDir + L"BGM1.wav";
		App::GetApp()->RegisterWav(L"BGM_1", strMusic);

		wstring strMusic2 = DataDir + L"BGM2.wav";
		App::GetApp()->RegisterWav(L"BGM_2", strMusic2);
		// SE
		wstring panWav = DataDir + L"pan.wav";
		App::GetApp()->RegisterWav(L"Pan", panWav);

		wstring bossWav = DataDir + L"bossdamage.wav";
		App::GetApp()->RegisterWav(L"bossdamage", bossWav);

		wstring bukiWav = DataDir + L"buki.wav";
		App::GetApp()->RegisterWav(L"buki", bukiWav);

		wstring cannonWav = DataDir + L"cannon.wav";
		App::GetApp()->RegisterWav(L"cannon", cannonWav);

		wstring chainsawWav = DataDir + L"chainsaw.wav";
		App::GetApp()->RegisterWav(L"chainsaw", chainsawWav);

		wstring dagekiWav = DataDir + L"dageki.wav";
		App::GetApp()->RegisterWav(L"dageki", dagekiWav);

		wstring explosionWav = DataDir + L"explosion.wav";
		App::GetApp()->RegisterWav(L"explosion", explosionWav);

		wstring footstepWav = DataDir + L"footstep.wav";
		App::GetApp()->RegisterWav(L"footstep", footstepWav);

		wstring nakamaWav = DataDir + L"nakama.wav";
		App::GetApp()->RegisterWav(L"nakama", nakamaWav);

		wstring roboteyeflashWav = DataDir + L"roboteyeflash.wav";
		App::GetApp()->RegisterWav(L"eye_flash", roboteyeflashWav);

		wstring sakeruWav = DataDir + L"step.wav";
		App::GetApp()->RegisterWav(L"step_sound", sakeruWav);

		wstring DamageVoiceWav = DataDir + L"damage_voice.wav";
		App::GetApp()->RegisterWav(L"damage_voice", DamageVoiceWav);

		wstring shootWav = DataDir + L"shoot.wav";
		App::GetApp()->RegisterWav(L"shoot", shootWav);

		wstring AttackVoiceWav = DataDir + L"attack_voice.wav";
		App::GetApp()->RegisterWav(L"attack_voice", AttackVoiceWav);

		wstring MidBossDamageWav = DataDir + L"T_Bdamage.wav";
		App::GetApp()->RegisterWav(L"MidBossDamage", MidBossDamageWav);

		wstring SawBossDamageWav = DataDir + L"S_Bossdamage.wav";
		App::GetApp()->RegisterWav(L"SawBossDamage", SawBossDamageWav);

		wstring gunWav = DataDir + L"gun.wav";
		App::GetApp()->RegisterWav(L"gun", gunWav);

		wstring gameoverWav = DataDir + L"gameover.wav";
		App::GetApp()->RegisterWav(L"gameover", gameoverWav);

		wstring gameclear2Wav = DataDir + L"gameclear2.wav";
		App::GetApp()->RegisterWav(L"gameclear2", gameclear2Wav);

		wstring gameclearWav = DataDir + L"gameclear.wav";
		App::GetApp()->RegisterWav(L"gameclear", gameclearWav);

		wstring energyWav = DataDir + L"energy.wav";
		App::GetApp()->RegisterWav(L"energy", energyWav);

		wstring energy2Wav = DataDir + L"energy2.wav";
		App::GetApp()->RegisterWav(L"energy2", energy2Wav);

		wstring Shakeoff02Wav = DataDir + L"Shakeoff02.wav";
		App::GetApp()->RegisterWav(L"Shakeoff02", Shakeoff02Wav);

		wstring Shakeoff03Wav = DataDir + L"Shakeoff03.wav";
		App::GetApp()->RegisterWav(L"ShakeOff03", Shakeoff03Wav);

		wstring MissileWav = DataDir + L"Missile.wav";
		App::GetApp()->RegisterWav(L"Missile", MissileWav);

	}


	void Scene::OnCreate() {
		CreateResources();
		//自分自身にイベントを送る
		//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる]
		PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitle");
	}

	void Scene::OnUpdate() {
		GetActiveStage()->OnPreUpdateStage();
		GetActiveStage()->OnUpdateStage();
	}
	void Scene::OnDraw() {
		GetActiveStage()->OnDrawStage();
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToTitle") {
			//アクティブステージをGameStageに設定
			ResetActiveStage<TitleScene>();
		}
		if (event->m_MsgStr == L"ToGameStage") {
			//アクティブステージをGameStageに設定
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToEmptyStage") {
			//アクティブステージをEmptyStageに設定
			ResetActiveStage<EmptyStage>();
		}
	}
}
//end basecross
