#include "stdafx.h"
#include "Project.h"

namespace basecross{

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
	AddGameObject<MultiSprite>(
		L"TsunagaKnight_TX",
		Vec2(1920 * 0.5f, 650 *0.5f),
		0.0f,
		Vec2(0, 200),
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
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}
}

}

