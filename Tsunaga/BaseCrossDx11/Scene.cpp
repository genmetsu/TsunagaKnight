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
		wstring strTexture = DataDir + L"space3.jpeg";
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

		//ボーンモデルのリソース
		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Kureha_fbx_v2.bmf");
		App::GetApp()->RegisterResource(L"KUREHA_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"E_Needle.bmf");
		App::GetApp()->RegisterResource(L"NEEDLE_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"E_Gun.bmf");
		App::GetApp()->RegisterResource(L"GUN_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"E_Angel.bmf");
		App::GetApp()->RegisterResource(L"ANGEL_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Large_Cannon_Blue_Blast.bmf");
		App::GetApp()->RegisterResource(L"CANNON_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Medium_Boss_Thor_Move.bmf");
		App::GetApp()->RegisterResource(L"THOR_BOSS_MESH", ModelMesh);
		ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Boss_Fbx_v02.bmf");
		App::GetApp()->RegisterResource(L"BOSS_MESH", ModelMesh);


		//スタティックモデルのリソースとして読み込み
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Medium_Boss_Hand_Left.bmf");
		App::GetApp()->RegisterResource(L"LEFT_HAND_MESH", StaticModelMesh);
		StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Medium_Boss_Hand_Right.bmf");
		App::GetApp()->RegisterResource(L"RIGHT_HAND_MESH", StaticModelMesh);
		StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Medium_Boss_Hand_Main.bmf");
		App::GetApp()->RegisterResource(L"HAND_BOSS_MESH", StaticModelMesh);
		StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Yamato_Fbx_v03.bmf");
		App::GetApp()->RegisterResource(L"YAMATO_MESH", StaticModelMesh);

		//エフェクト
		strTexture = DataDir + L"spark.png";
		App::GetApp()->RegisterTexture(L"SPARK_TX", strTexture);
		strTexture = DataDir + L"fire.png";
		App::GetApp()->RegisterTexture(L"FIRE_TX", strTexture);
		strTexture = DataDir + L"step.png";
		App::GetApp()->RegisterTexture(L"STEP_TX", strTexture);

		// BGM
		wstring strMusic = DataDir + L"BGM1.wav";
		App::GetApp()->RegisterWav(L"Nanika", strMusic);
		// SE
		wstring panWav = DataDir + L"pan.wav";
		App::GetApp()->RegisterWav(L"Pan", panWav);

		wstring bossWav = DataDir + L"bossdamage.wav";
		App::GetApp()->RegisterWav(L"bossdamage", bossWav);

		wstring bukiWav = DataDir + L"buki.wav";
		App::GetApp()->RegisterWav(L"Buki", bukiWav);

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
		App::GetApp()->RegisterWav(L"roboteyeflash", roboteyeflashWav);

		wstring sakeruWav = DataDir + L"sakeru.wav";
		App::GetApp()->RegisterWav(L"sakeru", sakeruWav);

		wstring ShakeoffWav = DataDir + L"Shakeoff.wav";
		App::GetApp()->RegisterWav(L"Shakeoff", ShakeoffWav);

		wstring shootWav = DataDir + L"shoot.wav";
		App::GetApp()->RegisterWav(L"shoot", shootWav);

		wstring hazureruWav = DataDir + L"hazureru.wav";
		App::GetApp()->RegisterWav(L"hazureru", hazureruWav);

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
