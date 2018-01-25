#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void TitleScene::CreateViewLight()
	{
	}

	void TitleScene::CreateBackGround()
	{
	}

	TitleScene::~TitleScene()
	{
	}


	void TitleScene::OnUpdateStage() {
		//自分自身の更新
		this->OnUpdate();
	}

	void TitleScene::OnCreate() {
		//背景スプライトの作成
		//AddGameObject<MultiSprite>(
		//	L"TITLE_TX",
		//	Vec2(1280, 720),
		//	0.0f,
		//	Vec2(0, 0),
		//	1, 1
		//	);


		////背景スプライトの作成
		//AddGameObject<MultiSprite>(
		//	L"TsunagaKnight_TX",
		//	Vec2(1920 * 0.5f, 650 * 0.5f),
		//	0.0f,
		//	Vec2(0, 100),
		//	1, 1
		//	);
		AddGameObject<ScoreSprite>(
			L"number_TX",
			Vec2(512, 60),
			0.0f,
			Vec2(0, 0),
			1, 1
			);
		//背景スプライトの作成
		AddGameObject<MessageSprite>(
			L"PresAnyButton_TX",
			Vec2(256, 60),
			0.0f,
			Vec2(0, -300),
			1, 1
			);

		AddGameObject<DimSprite>(
			L"kuro_TX",
			Vec2(1280, 720),
			0.0f,
			Vec2(0, 0),
			1, 1
			);

		

	}
	void TitleScene::OnDraw() {

	}


	void TitleScene::OnDrawStage() {
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


	void TitleScene::OnUpdate()
	{

		//ターン毎の初期化
		GetRigidbodyManager()->InitRigidbody();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの更新
			v->OnUpdate();
		}



		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected)
		{
			//Bボタン
			if (CntlVec[0].wPressedButtons)
			{
				//PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");

			}
		}
	}

	DimSprite::DimSprite(const shared_ptr<Stage>& StagePtr, const wstring &
		TextureResName, const Vec2 & StartScale, float StartRot, const Vec2 &
		StartPos, UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
	}


	void DimSprite::AdjustVertex()
	{
	}

	void DimSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture * vertices)
	{

		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected)
		{
			//Bボタン
			if (CntlVec[0].wPressedButtons)
			{
				isButtonDown = true;

			}

		}

		if (isButtonDown == true) 
		{
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= 1.0f)
			{
				m_TotalTime = 1.0f;
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");

			}	
		}

		//float sin_val = sin(m_TotalTime) * 0.5f + 0.5f;
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, m_TotalTime);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++)
		{
			vertices[i] = VertexPositionColorTexture
			(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);
		}

	}


}

