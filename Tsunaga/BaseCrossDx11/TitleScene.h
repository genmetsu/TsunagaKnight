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

	class DimSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
		bool isButtonDown;
		//--------------------------------------------------------------------------------------
		/*!
		@brief バックアップ頂点の修正(仮想関数)。派生クラスは独自の頂点初期修正を実装
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の変更
		@param[in]	ElapsedTime	ターン時間
		@param[out]	vertices	マップされた頂点データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	初期テクスチャリソース名
		@param[in]	StartScale	初期大きさ
		@param[in]	StartRot	初期回転
		@param[in]	StartPos	初期位置
		@param[in]	XWrap	X方向のラップ数
		@param[in]	YWrap	Y方向のラップ数
		*/
		//--------------------------------------------------------------------------------------
		DimSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DimSprite() {}
	};



}