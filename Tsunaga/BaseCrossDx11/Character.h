/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	ゲーム全体を管理するマネージャー、スポナー
	//--------------------------------------------------------------------------------------
	class GameManager {
	private:
		GameManager() {
			m_friends_num = 0;
		}
		~GameManager() {}
		static GameManager* GM;

		float m_Player_HP;
		float m_Cannon_HP;
		float m_Boss_HP;
		float m_Default_Cannon_HP;
		float m_Default_Boss_HP;
		int m_friends_num;
		
	public:
		float m_camera_length;
		static GameManager* getInstance();

		void SetPlayerHP(float value) {
			m_Player_HP = value;
		}
		float GetPlayerHP() {
			return m_Player_HP;
		}
		void SetCannonHP(float value) {
			m_Cannon_HP = value;
		}
		float GetCannonHP() {
			return m_Cannon_HP;
		}
		void SetDefaultCannonHP(float value) {
			m_Default_Cannon_HP = value;
		}
		float GetDefaultCannonHP() {
			return m_Default_Cannon_HP;
		}

		void SetDefaultBossHP(float value) {
			m_Default_Boss_HP = value;
		}
		float GetDefaultBossHP() {
			return m_Default_Boss_HP;
		}

		void SetBossHP(float value) {
			m_Boss_HP = value;
		}
		float GetBossHP() {
			return m_Boss_HP;
		}

		void SetFriendsNum(int value) {
			m_friends_num = value;
		};
		int GetFriendsNum() {
			return m_friends_num;
		};
	};
	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	class SquareObject : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//テクスチャリソース名
		wstring m_TextureResName;
		//法線マップ用テクスチャリソース名
		wstring m_NormalTextureResName;
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTnTStaticRenderer> m_Renderer;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の作成
		@param[in]	WrapX	X方向のタイリング
		@param[in]	WrapY	Y方向のタイリング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateBuffers(float WrapX, float WrapY);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	NormalTextureResName	法線マップテクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const wstring& NormalTextureResName,
			const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	ステージのヤマトクラス
	//--------------------------------------------------------------------------------------
	class YamatoStage : public GameObject{
	protected:
		//テクスチャリソース名
		wstring m_TextureResName;
		
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;

		//メッシュとの差分計算用
		Mat4x4 m_MeshToTransformMatrix;
		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTnTStaticModelRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		YamatoStage(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos
			, bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~YamatoStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
	};

	//--------------------------------------------------------------------------------------
	///	透明な壁
	//--------------------------------------------------------------------------------------
	class TransparentWall : public GameObject {
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		TransparentWall(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TransparentWall();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {};
	};

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//構築と破棄
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class AttackSpark : public MultiParticle;
	//用途: 攻撃が当たった時のスパーククラス
	//--------------------------------------------------------------------------------------
	class AttackSpark : public MultiParticle {
	public:
		//構築と破棄
		AttackSpark(shared_ptr<Stage>& StagePtr);
		virtual ~AttackSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class HandBossSpark : public MultiParticle;
	//用途: HandBossの振り下ろした拳のパーティクル
	//--------------------------------------------------------------------------------------
	class HandBossSpark : public MultiParticle {
	public:
		//構築と破棄
		HandBossSpark(shared_ptr<Stage>& StagePtr);
		virtual ~HandBossSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class ChainEffect : public MultiParticle;
	//用途: 繋げているときのエフェクト
	//--------------------------------------------------------------------------------------
	class ChainEffect : public MultiParticle {
	public:
		//構築と破棄
		ChainEffect(shared_ptr<Stage>& StagePtr);
		virtual ~ChainEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos1, const Vec3& Pos2);
		virtual void OnUpdate() override;
		void UpdatePosition(const Vec3& Pos1, const Vec3& Pos2);
	};

	//--------------------------------------------------------------------------------------
	//class MultiGuardEffect : public MultiParticle;
	//用途: エンジェルエネミーの防御エフェクト
	//--------------------------------------------------------------------------------------
	class MultiGuardEffect : public MultiParticle {
	public:
		//構築と破棄
		MultiGuardEffect(shared_ptr<Stage>& StagePtr);
		virtual ~MultiGuardEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class BossPrepareAttackEffect : public MultiParticle;
	//用途 ボスの攻撃準備エフェクト
	//--------------------------------------------------------------------------------------
	class BossPrepareAttackEffect : public MultiParticle {
	public:
		//構築と破棄
		BossPrepareAttackEffect(shared_ptr<Stage>& StagePtr);
		virtual ~BossPrepareAttackEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class BossEffect : public MultiParticle;
	//用途 ボスのバリアエフェクト
	//--------------------------------------------------------------------------------------
	class BossEffect : public MultiParticle {
	public:
		//構築と破棄
		BossEffect(shared_ptr<Stage>& StagePtr);
		virtual ~BossEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos,wstring name);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class BossBulletEffect : public MultiParticle;
	//用途: ボスエネミーの弾のエフェクト
	//--------------------------------------------------------------------------------------
	class BossBulletEffect : public MultiParticle {
	public:
		//構築と破棄
		BossBulletEffect(shared_ptr<Stage>& StagePtr);
		virtual ~BossBulletEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class EnemyMoveEffect : public MultiParticle;
	//用途: エネミーの移動エフェクト
	//--------------------------------------------------------------------------------------
	class EnemyMoveEffect : public MultiParticle {
	public:
		//構築と破棄
		EnemyMoveEffect(shared_ptr<Stage>& StagePtr);
		virtual ~EnemyMoveEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class MultiFire : public MultiParticle;
	//用途: 複数の炎クラス
	//--------------------------------------------------------------------------------------
	class MultiFire : public MultiParticle {
	public:
		//構築と破棄
		MultiFire(shared_ptr<Stage>& StagePtr);
		virtual ~MultiFire();
		//初期化
		virtual void OnCreate() override;
		void InsertFire(const Vec3& Pos,float Scale);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class AttackSigns : public MultiParticle;
	//用途: 雑魚の攻撃を示唆するエフェクト
	//--------------------------------------------------------------------------------------
	class AttackSigns : public MultiParticle {
	public:
		//構築と破棄
		AttackSigns(shared_ptr<Stage>& StagePtr);
		virtual ~AttackSigns();
		//初期化
		virtual void OnCreate() override;
		void InsertSigns(const Vec3& Pos,float Size);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class MiddleBossAttackSigns : public MultiParticle;
	//用途: ボスの攻撃を示唆するエフェクト
	//--------------------------------------------------------------------------------------
	class MiddleBossAttackSigns : public MultiParticle {
	public:
		//構築と破棄
		MiddleBossAttackSigns(shared_ptr<Stage>& StagePtr);
		virtual ~MiddleBossAttackSigns();
		//初期化
		virtual void OnCreate() override;
		void InsertSigns(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class BossAttackSigns : public MultiParticle;
	//用途: ボスの攻撃を示唆するエフェクト
	//--------------------------------------------------------------------------------------
	class BossAttackSigns : public MultiParticle {
	public:
		//構築と破棄
		BossAttackSigns(shared_ptr<Stage>& StagePtr);
		virtual ~BossAttackSigns();
		//初期化
		virtual void OnCreate() override;
		void InsertSigns(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class CannonEffect : public MultiParticle;
	//用途: 砲撃のエフェクト
	//--------------------------------------------------------------------------------------
	class CannonEffect : public MultiParticle {
	public:
		//構築と破棄
		CannonEffect(shared_ptr<Stage>& StagePtr);
		virtual ~CannonEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class StepEffect : public MultiParticle;
	//用途: ステップのエフェクト
	//--------------------------------------------------------------------------------------
	class StepEffect : public MultiParticle {
	public:
		//構築と破棄
		StepEffect(shared_ptr<Stage>& StagePtr);
		virtual ~StepEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class SwordEffect : public MultiParticle;
	//用途: 剣のエフェクト
	//--------------------------------------------------------------------------------------
	class SwordEffect : public MultiParticle {
	public:
		//構築と破棄
		SwordEffect(shared_ptr<Stage>& StagePtr);
		virtual ~SwordEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class ShootingEffect : public MultiParticle;
	//用途: 大砲で撃たれているときのエフェクト
	//--------------------------------------------------------------------------------------
	class ShootingEffect : public MultiParticle {
	public:
		//構築と破棄
		ShootingEffect(shared_ptr<Stage>& StagePtr);
		virtual ~ShootingEffect();
		//初期化
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};


	//--------------------------------------------------------------------------------------
	///	ワールド行列を取得するインターフェイス
	//--------------------------------------------------------------------------------------
	class MatrixInterface {
	protected:
		MatrixInterface() {}
		virtual ~MatrixInterface() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワールド行列の取得（純粋仮想関数）
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const = 0;
	};


	//--------------------------------------------------------------------------------------
	///	ラッピング処理されたスプライト（親）
	//--------------------------------------------------------------------------------------
	class SpriteBase : public GameObject {
	public:
		//ブレンド方法
		enum class BlendState {
			Opaque,
			Trace,
			Additive
		};
	protected:
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//バックアップしておく頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
		wstring m_TextureResName;		///<テクスチャリソース名
		Vec2 m_Scale;				///<スケーリング
		float m_Rot;				///<回転
		Vec2 m_Pos;				///<位置
		UINT m_XWrap;		//X方向のラップ数
		UINT m_YWrap;		//Y方向のラップ数
		Col4 m_Emissive;	//エミッシブ色
		BlendState m_BlendState;
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
		SpriteBase(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap,UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SpriteBase() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief バックアップ頂点の修正(仮想関数)。派生クラスは独自の頂点初期修正を実装
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の変更(仮想関数)。派生クラスは独自の頂点変更を実装
		@param[in]	ElapsedTime	ターン時間
		@param[out]	vertices	マップされた頂点データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime,VertexPositionColorTexture* vertices) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソース名の取得
		@return	テクスチャリソース名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureResName() const {
			return m_TextureResName;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソース名の変更
		@param[in]	col	リソース名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResName(const wstring& textureResName) {
			m_TextureResName = textureResName;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ブレンド方法の取得
		@return	ブレンド方法
		*/
		//--------------------------------------------------------------------------------------
		const BlendState& GetBlendState() const {
			return m_BlendState;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ブレンド方法の変更
		@param[in]	col	変更色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(BlendState blendState) {
			m_BlendState = blendState;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief エミッシブ色の取得
		@return	エミッシブ色
		*/
		//--------------------------------------------------------------------------------------
		const Col4& GetEmissive() const {
			return m_Emissive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief エミッシブ色の変更
		@param[in]	col	変更色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const Col4& col) {
			m_Emissive = col;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	回転するスプライト
	//--------------------------------------------------------------------------------------
	class RotateSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
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
		RotateSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~RotateSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	class MessageSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム

	public:
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
		MessageSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~MessageSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	class TutorialMessageSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		wstring my_tag;
		bool m_isPressed;
	public:
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
		TutorialMessageSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap,
			wstring tag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TutorialMessageSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	class ResultSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
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
		ResultSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~ResultSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	class TutorialSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		float m_FrameCount;
	public:
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
		TutorialSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~TutorialSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	暗転を作るスプライト
	//--------------------------------------------------------------------------------------
	class DimSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		wstring m_scene;
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
			UINT XWrap, UINT YWrap,
			wstring name);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DimSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	class ResultCursorSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		bool m_Moved;
	public:
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
		ResultCursorSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~ResultCursorSprite() {}
	};
	//--------------------------------------------------------------------------------------
	///	ポーズカーソル
	//--------------------------------------------------------------------------------------
	class PauseCursorSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		bool m_isPause;
		bool m_Moved;

	public:
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
		PauseCursorSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~PauseCursorSprite() {}
	};

	//--------------------------------------------------------------------------------------
	/// 大砲のゲージ
	//--------------------------------------------------------------------------------------
	class CannonGauge : public SpriteBase {
		float m_TotalTime;
		//色を変える時間
		float m_ChangeColorTime;
		float m_DefaultSize;
	public:
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
		CannonGauge(const shared_ptr<Stage>& StagePtr,
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
		virtual ~CannonGauge() {}
	};

	//--------------------------------------------------------------------------------------
	/// ボスの体力ゲージ
	//--------------------------------------------------------------------------------------
	class BossHPGauge : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		float m_DefalutHP;
		float m_DefaultSize;
	public:
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
		BossHPGauge(const shared_ptr<Stage>& StagePtr,
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
		virtual ~BossHPGauge() {}
	};

	//--------------------------------------------------------------------------------------
	/// スコア
	//--------------------------------------------------------------------------------------
	class ScoreSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		float m_DefalutHP;
		float m_DefaultSize;

		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float m_Score;
		//桁数
		UINT m_NumberOfDigits;
		//バックアップ頂点データ
		vector<VertexPositionTexture> m_BackupVertices;
	public:
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
		ScoreSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~ScoreSprite() {}

		void SetScore(float f) {
			m_Score = f;
		}
	};
	//--------------------------------------------------------------------------------------
	///	スプライト
	//--------------------------------------------------------------------------------------
	class MultiSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
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
		MultiSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~MultiSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	UIスプライト
	//--------------------------------------------------------------------------------------
	class UISprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
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
		UISprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~UISprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	CannonUIスプライト、色が変わ
	//--------------------------------------------------------------------------------------
	class CannonUISprite : public SpriteBase {
		float m_TotalTime;
		//色の変わる時間
		float m_ChangeColorTime;
	public:
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
		CannonUISprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~CannonUISprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	チェインスプライト
	//--------------------------------------------------------------------------------------
	class ChainUISprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		int m_MyNumber;
	public:
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
		ChainUISprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap,
			int MyNum);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ChainUISprite() {}
	};
	//--------------------------------------------------------------------------------------
	///	ポーズ
	//--------------------------------------------------------------------------------------
	class PauseSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
		bool m_isPause;
	public:
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
		PauseSprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~PauseSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	SkyBox
	//--------------------------------------------------------------------------------------
	class SkyBox : public GameObject {
	protected:
		//テクスチャリソース名
		wstring m_TextureResName;
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;

		//描画データ
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<SimplePNTStaticRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		SkyBox(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SkyBox();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワールド行列の取得
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
	};

	//--------------------------------------------------------------------------------------
	///	大砲クラス
	//--------------------------------------------------------------------------------------
	class Cannon : public GameObject, public MatrixInterface {
	protected:
		//テクスチャリソース名
		wstring m_TextureResName;
		//大砲判別用
		int myClass;

		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;

		//耐久度
		float m_HP;
		//最初の耐久度
		float m_DefaultHP;

		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;

		bool m_UpdateActive;

		//メッシュとの差分計算用
		Mat4x4 m_MeshToTransformMatrix;
		//描画データ
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<SimplePNTStaticRenderer2> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		Cannon(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const wstring& TagName,const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			int CannonClass,bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Cannon();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワールド行列の取得
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 自分のHPの状態チェック
		@return	なし
		*/
		//--------------------------------------------------------------------------------------

		void Rotation(Vec3 vec);
		void CheckHealth();

		float GetHP() {
			return m_HP;
		}
		void SetHP(float Value) {
			m_HP = Value;
		}
		float GetScale() {
			return m_Scale.x;
		}
		int GetCannonClass() {
			return myClass;
		}
	};

	//--------------------------------------------------------------------------------------
	///	敵オブジェクトの親クラス
	//--------------------------------------------------------------------------------------
	class EnemyObject : public GameObject, public MatrixInterface {
	protected:
		//メッシュリソース名
		wstring m_MeshResName;
		//テクスチャリソース名
		wstring m_TextureResName;
		//デフォルトのアニメーション
		wstring m_DefaultAnimation;
		//メッシュとの差分計算用
		Mat4x4 m_MeshToTransformMatrix;
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;
		//ひとつ前の位置
		Vec3 m_BeforePos;
		// 突進の位置座標
		Vec3 m_TargetPos;
		//追いかけるスピード
		float m_Speed;
		//フレームカウント
		float m_FrameCount;
		//突撃するかどうか
		bool m_Tackle;
		//突撃する際に止まる時間
		float m_StopTime;
		//どれぐらい近づいたら突撃するかの距離
		float m_SearchDis;
		//突撃のスピード
		float m_TackleSpeed;
		//突進時間
		float m_TackleTime;
		//攻撃後の硬直時間
		float m_AfterAttackTime;
		//エンジェルエネミーから何番目に位置するか
		int m_FollowingAngelNum;

		float m_HP;
		//最初のHP
		float m_DefaultHP;

		Vec3 m_DefaultScale;

		float m_AttackPoint;
		//自分を固定するY軸の高さ
		float m_BaseY;

		Vec3 m_CannonPos;
		Vec3 m_BossPos;
		Vec3 m_ToBossVec;

		//大砲にセットされたとき何番目に発射されるか
		int m_ShootNumber;

		bool m_Bomb;

		bool m_UpdateActive;
		//リスポーン待ちかどうか
		bool m_isWaiting;
		//描画するかどうか
		bool m_isDraw;

		//死んだかどうか
		bool m_isDead;
		//突進する際の最初のポジション
		Vec3 m_TackleStartPos;

		//プレイヤーのポインタ
		weak_ptr<GameObject> m_PlayerPtr;
		//親オブジェクト
		weak_ptr<GameObject> m_ParentPtr;
		//
		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		//サウンドオブジェクト
		shared_ptr<SoundObject> m_DeadSound;
		shared_ptr<SoundObject> m_BossDamageSound;
		shared_ptr<SoundObject> m_FriendsSound;
		shared_ptr<SoundObject> m_CannonSound;
		shared_ptr<SoundObject> m_EyeFlashSound;

		shared_ptr<SoundObject> m_DamageSound;
		shared_ptr<SoundObject> m_SawDamageSound;

		shared_ptr<SoundObject> m_SawSound[5];

		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;
		//このオブジェクトのプレイヤーから見たローカル行列
		Mat4x4 m_PlayerLocalMatrix;
		//プレイヤーの直後（先頭）の場合の補間係数
		float m_LerpToParent;
		//このオブジェクトのチャイルドオブジェクトから見たローカル行列
		Mat4x4 m_ChildLocalMatrix;
		//チャイルド後の場合の補間係数
		float m_LerpToChild;
		//Attack1の場合の目標となる回転
		float m_Attack1ToRot;
		//ステートマシーン
		unique_ptr<StateMachine<EnemyObject>>  m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		EnemyObject(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EnemyObject();
		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<EnemyObject> >& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(Vec3 Pos) {
			m_Rigidbody->m_Pos = Pos;
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワールド行列の取得
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 仲間になる演出の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CompliancePerformanceStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 仲間になる演出の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CompliancePerformanceBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 追従する行動の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ComplianceStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃１行動の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Attack1StartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃１行動の継続
		@return	行動が終了したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool Attack1ExcuteBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に込められるときの準備処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void BulletPrepareStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に込めらていく処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void BulletPrepareExcuteBehavior();



		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に込められてボスに向かっていく最初に呼ばれる準備処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void BulletStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に込められてボスに向かっていく処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void BulletExcuteBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵をスポーンさせる際の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Spawn();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステート共通処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 仲間になってついてくる行動
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void FriendsBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵対中の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃後の硬直処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AttackEndBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に向かう時の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ToCannonBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージ受けているときの行動
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DamageBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief スポーン待ちの最初にする行動
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void WaitingStartBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージ受けたときの最初にする行動
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DamageStartBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief スポーン待ちが終わった時にする行動
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void WaitingEndBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief プレイヤーの弾との衝突判定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CollisionBullet();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 自分のHPの状態チェック
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CheckHealth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 親の設定を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CheckParent();
		//--------------------------------------------------------------------------------------
		/*!
		@brief エンジェルエネミーから何番目にいるか取得する
		@return	m_FollowingAngelNum
		*/
		//--------------------------------------------------------------------------------------
		int GetFollowingAngelNum() {
			return m_FollowingAngelNum;
		}
		void SetFollowingAngelNum(int value) {
			m_FollowingAngelNum = value;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 親の確認を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetParent(weak_ptr<GameObject> gameObject) {
			m_ParentPtr = gameObject;
		};

		void MidBossCheckHealth();

		void RotateToVelocity();
		
		void ChangeState(wstring name);

		void ChangeAnimation(wstring name);

		float GetHP(){
			return m_HP;
		}
		void SetHP(float Value){
			m_HP = Value;
		}

		void Damage(float value) {
			m_HP -= value;
		}

		void SetShootNumber(int value) {
			m_ShootNumber = value;
		}
		int GetShootNumber() {
			return m_ShootNumber;
		}

		float GetAttackPoint(){
			return m_AttackPoint;
		}
		void SetAttackPoint(float Value){
			m_AttackPoint = Value;
		}

		void SetUpdateActive(bool value) {
			m_UpdateActive = value;
		}
		bool GetUpdateActive() {
			return m_UpdateActive;
		}

		float GetScale(){
			return m_Scale.x;
		}

		bool GetIsDraw() {
			return m_isDraw;
		}
		void SetIsDraw(bool value) {
			m_isDraw = value;
		}

	};

	//--------------------------------------------------------------------------------------
	///	敵対ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyOppositionState : public ObjState<EnemyObject>
	{
		EnemyOppositionState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyOppositionState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	攻撃後の硬直ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyAttackEndState : public ObjState<EnemyObject>
	{
		EnemyAttackEndState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyAttackEndState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	大砲に向かうステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyToCannonState : public ObjState<EnemyObject>
	{
		EnemyToCannonState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyToCannonState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	仲間になった演出ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyComplianceStartState : public ObjState<EnemyObject>
	{
		EnemyComplianceStartState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyComplianceStartState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	追従ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyComplianceState : public ObjState<EnemyObject>
	{
		EnemyComplianceState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyComplianceState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	大砲で撃たれる前の準備ステート
	//--------------------------------------------------------------------------------------
	class EnemyBulletPrepareState : public ObjState<EnemyObject>
	{
		EnemyBulletPrepareState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyBulletPrepareState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	大砲で撃たれてるときのステート
	//--------------------------------------------------------------------------------------
	class EnemyBulletState : public ObjState<EnemyObject>
	{
		EnemyBulletState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyBulletState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	攻撃ステート１（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyAttack1State : public ObjState<EnemyObject>
	{
		EnemyAttack1State() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyAttack1State)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ダメージステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyDamageState : public ObjState<EnemyObject>
	{
		EnemyDamageState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyDamageState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	スポーン待ちステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	class EnemyWaitingState : public ObjState<EnemyObject>
	{
		EnemyWaitingState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyWaitingState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ニードルエネミー（近接攻撃）
	//--------------------------------------------------------------------------------------

	class NeedleEnemy : public EnemyObject
	{
	
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		NeedleEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~NeedleEnemy();

		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵対中の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior() override;

		virtual void OnUpdate() override;


	};

	//--------------------------------------------------------------------------------------
	///	弾をうつ敵
	//--------------------------------------------------------------------------------------
	class ShootEnemy : public EnemyObject
	{
		//敵対時の打ち出す弾の速度
		float m_EnemyShootSpeed;
		//つながっている時の撃ち出す弾の速度
		float m_PlayerShootSpeed;
		//つながっているときの弾の撃ち出し頻度
		float m_PlayerShootTime;

		shared_ptr<SoundObject> m_ShootSound;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		ShootEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShootEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵対中の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵対中の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	};

	class BulletObject : public GameObject, public MatrixInterface {
	protected:
		//テクスチャリソース名
		wstring m_TextureResName;
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;
		// 突進の位置座標
		Vec3 m_TargetPos;
		//追いかけるスピード
		float m_Speed;
		//フレームカウント
		float m_FrameCount;

		//現在の速度を保存しておく
		Vec3 m_NowVelocity;

		wstring m_my_Tag;

		//サウンドオブジェクト
		shared_ptr<SoundObject> m_CollisionSound;

		//親オブジェクト
		weak_ptr<GameObject> m_ParentPtr;
		
		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;

		//描画データ
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<SimplePNTStaticRenderer2> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

		bool m_OwnShadowActive;
		
		bool m_UpdateActive;

		// 弾が撃たれているかどうか
		bool m_isShoot;
		// 弾の存在時間
		float m_BulletTime;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		BulletObject(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive, const wstring& Tag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BulletObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワールド行列の取得
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;

		virtual Vec3 GetPosition() override;

		void SetPosition(Vec3 pos);

		float GetScale(){
			return m_Scale.x;
		}

		void SetActive(bool value) {
			if (value == false) {
				m_NowVelocity = m_Rigidbody->m_Velocity;
				m_Rigidbody->m_Velocity = Vec3(0);
			}
			else {
				m_Rigidbody->m_Velocity = m_NowVelocity;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	弾を打ち出す処理
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void Wakeup(const Vec3& Position, const Vec3& Velocity);

		void SetSleep();

		bool GetIsShoot()
		{
			return m_isShoot;
		}
	};

	//--------------------------------------------------------------------------------------
	/// エンジェルエネミー
	//--------------------------------------------------------------------------------------
	class AngelEnemy : public EnemyObject
	{

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		AngelEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AngelEnemy();
	};

	//--------------------------------------------------------------------------------------
	/// 中ボスキャラ　近距離 
	/// CR = Close Range
	//--------------------------------------------------------------------------------------

	class CR_BossEnemy : public EnemyObject
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		CR_BossEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CR_BossEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵対中の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に向かう処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ToCannonBehavior() override;
	};

	//--------------------------------------------------------------------------------------
	/// 中ボスキャラ　近距離 の 手
	//--------------------------------------------------------------------------------------
	class BossHand : public GameObject, public MatrixInterface {
		//テクスチャリソース名
		wstring m_TextureResName;
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;
		float m_FrameCount;
		//攻撃準備の時間
		float m_AttackSetupTime;
		//手を振り下ろす前の硬直時間
		float m_BeforeAttackTime;
		//攻撃時間
		float m_AttackTime;
		//元の位置に戻る時間
		float m_ReturnDefaultTime;

		Vec3 m_AttackPos;
		Vec3 m_DefaultPos;
		Vec3 m_BeforePos;

		bool m_AttackEnd;

		bool m_UpdateActive;

		shared_ptr<SoundObject> m_HandSound;

		Col4 m_DefaultEmissive;
		Col4 m_DefaultDiffuse;

		//親オブジェクト
		weak_ptr<GameObject> m_ParentPtr;
		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		//メッシュとの差分計算用
		Mat4x4 m_MeshToTransformMatrix;
		
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;
		//このオブジェクトから見たローカル行列
		Mat4x4 m_LocalMatrix;
		//親との補間係数
		float m_LerpToParent;
		//ステートマシーン
		unique_ptr<StateMachine<BossHand>>  m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		BossHand(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr,
			const wstring& TextureResName, const wstring& TagName,
			const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BossHand();
		///描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate2()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<BossHand> >& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワールド行列の取得
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 通常行動の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ComplianceStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃１行動の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Attack1StartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃１行動の継続
		@return	行動が終了したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool Attack1ExcuteBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステート共通処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステートを変更する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetState(wstring state_name);


		void ChangeDamageColor();

		void SetUpdateActive(bool value) {
			m_UpdateActive = value;
		}
		bool GetUpdateActive() {
			return m_UpdateActive;
		}

		Vec3 GetPosition() {
			return m_Rigidbody->m_Pos;
		}
		float GetScale() {
			return m_Scale.y;
		}
	};

	//--------------------------------------------------------------------------------------
	///	攻撃しないステート（BossHand）
	//--------------------------------------------------------------------------------------
	class HandDefaultState : public ObjState<BossHand>
	{
		HandDefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(HandDefaultState)
		virtual void Enter(const shared_ptr<BossHand>& Obj)override;
		virtual void Execute(const shared_ptr<BossHand>& Obj)override;
		virtual void Exit(const shared_ptr<BossHand>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	攻撃ステート（BossHand）
	//--------------------------------------------------------------------------------------
	class HandAttackState : public ObjState<BossHand>
	{
		HandAttackState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(HandAttackState)
		virtual void Enter(const shared_ptr<BossHand>& Obj)override;
		virtual void Execute(const shared_ptr<BossHand>& Obj)override;
		virtual void Exit(const shared_ptr<BossHand>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	/// 中ボスキャラ　遠距離
	/// LD = Long Distance
	//--------------------------------------------------------------------------------------

	class LD_BossEnemy : public EnemyObject
	{
		int m_SoundCount;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		LD_BossEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~LD_BossEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵対中の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃が終わった後の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AttackEndBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲に向かう処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ToCannonBehavior() override;
	};

	//--------------------------------------------------------------------------------------
	/// 敵のボス船
	//--------------------------------------------------------------------------------------
	class Boss : public GameObject {
	protected:
		//テクスチャリソース名
		wstring m_TextureResName;

		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;
		//HP
		float m_HP;

		//バリアを張っているか
		bool m_isBarrior;

		//カメラがボスに注目するかどうかのflg
		bool m_isLooked;

		//最初のHP
		float m_DefaultHP;
		//バリアの耐久力
		float m_BarrierHP;
		float m_DefalutBarrierHP;

		Vec3 m_DefaultPos;

		//攻撃の準備時間
		float m_PrepareAttackTime;
		//バリアを変える時間
		float m_BarriorChangeTime;

		bool m_isDead;

		int m_now_barrior;

		bool m_UpdateActive;

		float m_frame_count;

		float m_DamageFrameCount;

		bool m_isDamage;

		float m_DamageRate;

		float m_SpawnCount;

		float m_SpawnTime;

		float m_AttackFrameCount;
		float m_AttackRate;

		//飛ばすバレットの数
		float m_AttackBulletNum;
		//現在飛ばしたバレットの数
		float m_NowAttackBulletNum;

		float m_BeforeAttackTime;
		float m_BulletSpeed;

		float m_TotalTime;

		Vec3 m_CannonPos[3];

		shared_ptr<SoundObject> m_AttackSignSound;
		shared_ptr<SoundObject> m_AttackSound;

		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		//メッシュとの差分計算用
		Mat4x4 m_MeshToTransformMatrix;
		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ParentPtr	親のポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		@param[in]	OwnShadowActive	影描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		Boss(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos
			, bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Boss();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(Vec3 Pos) {
			m_Rigidbody->m_Pos = Pos;
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージ処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Damage(float value);

		bool GetIsbarrior() {
			return m_isBarrior;
		}

		int GetNowBarrior() {
			return m_now_barrior;
		}

		float GetHP() {
			return m_HP;
		}

		void SetIsDamage(bool value) {
			m_isDamage = value;
		}
		bool GetIsDamage() {
			return m_isDamage;
		}

		void SetIsLooked(bool value) {
			m_isLooked = value;
		}

		bool GetIsLooked() {
			return m_isLooked;
		}

		void AttackMove(int player_num);

		//--------------------------------------------------------------------------------------
		/*!
		@brief アニメーションの変更
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangeAnimation(wstring anim);

		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵をスポーンさせる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SpawnEnemy(wstring tag);
	};


}
//end basecross
