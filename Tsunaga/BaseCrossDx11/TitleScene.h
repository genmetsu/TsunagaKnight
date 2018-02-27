#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class TitleScene : public Stage {
		shared_ptr<MultiAudioObject> m_AudioObjectPtr;
		//ビューの作成
		void CreateViewLight();
		void CreateBackGround();


	public:
		//構築と破棄
		TitleScene() :Stage() {}
		virtual ~TitleScene();
		//初期化
		virtual void OnUpdateStage() override;

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;


		virtual void OnDrawStage()override;

	};

}