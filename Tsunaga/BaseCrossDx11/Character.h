/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;

	//--------------------------------------------------------------------------------------
	///	このプロジェクトで使うユーティリティネームスペース
	//--------------------------------------------------------------------------------------
	namespace ProjUtil {
		//--------------------------------------------------------------------------------------
		/*!
		@brief スライドさせる
		@param[in]	Base	元のベクトル
		@param[in]	Normal	計算に使う法線
		@return	スライドさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vec3 Slide(const Vec3& Base, const Vec3& Normal) {
			//Baseと法線から直行線の長さ（内積で求める）
			float Len = dot(Base, Normal);
			//その長さに伸ばす
			Vec3 Contact = Normal * Len;
			//スライドする方向は現在のベクトルから引き算
			return (Base - Contact);
		}
	}

	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	class SquareObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<シーン
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
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
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
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
	///	シリンダー
	//--------------------------------------------------------------------------------------
	class CylinderObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<シーン
											//メッシュ
		shared_ptr<MeshResource> m_CylinderMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		CylinderObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CylinderObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief CYLINDERを得る
		@return	CYLINDER
		*/
		//--------------------------------------------------------------------------------------
		CYLINDER GetCYLINDER()const;
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
	///	球
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<シーン
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec3 m_Scale;				///<スケーリング
		float m_BaseY;				///<スケーリングベースの最下地点
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		Vec3 m_Velocity;		//速度
		Vec3 m_Gravity;		//自由落下加速度
		Vec3 m_GravityVelocity;		//自由落下による速度
		bool m_JumpLock;	//ジャンプのロック
		Vec3 m_BeforePos;	//1つ前の位置
		float m_Mass;

		//--------------------------------------------------------------------------------------
		/*!
		@brief Boxとの衝突判定
		@param[in]	BeforePos	1つ前の場所
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CollisionWithBoxes(const Vec3& BeforePos);


		//--------------------------------------------------------------------------------------
		/*!
		@brief Cylinderとの衝突判定
		@param[in]	BeforePos	1つ前の場所
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CollisionWithCylinder(const Vec3& BeforePos);


		//--------------------------------------------------------------------------------------
		/*!
		@brief	コントローラから方向ベクトルを得る
		@return	方向ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 進行方向を向く
		@param[in]	LerpFact	補間係数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(float LerpFact);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	Division	分割数
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SphereObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief SPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetSPHERE()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 位置を得る
		@return	位置
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetPosition() const {
			return m_Pos;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前の位置を得る
		@return	1つ前の位置
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetBeforePos()const {
			return m_BeforePos;
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
		@brief 衝突判定をする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnCollision();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 回転処理をする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnRotation();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	ボックスの親
	//--------------------------------------------------------------------------------------
	class BoxBase : public ObjectInterface, public ShapeInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		BoxBase() :
			ObjectInterface(),
			ShapeInterface() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BoxBase() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief OBBを得る（仮想関数）
		@return	OBB
		*/
		//--------------------------------------------------------------------------------------
		virtual OBB GetOBB()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定をする（仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCollision() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 回転処理をする（仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnRotation() {}
	};



	//--------------------------------------------------------------------------------------
	///	固定のボックス
	//--------------------------------------------------------------------------------------
	class BoxObject : public BoxBase {
		weak_ptr<Scene> m_Scene;			///<シーン
											//メッシュ
		shared_ptr<MeshResource> m_BoxMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Scale	大きさ
		@param[in]	Qt	回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		BoxObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, bool Trace,
			const Vec3& Scale, 
			const Quat& Qt,
			const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BoxObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief OBBを得る
		@return	OBB
		*/
		//--------------------------------------------------------------------------------------
		virtual OBB GetOBB()const;
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
	///	移動ボックス
	//--------------------------------------------------------------------------------------
	class MoveBoxObject : public BoxBase {
		weak_ptr<Scene> m_Scene;			///<シーン
											//メッシュ
		shared_ptr<MeshResource> m_MoveBoxMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか

		Vec3 m_Velocity;			//速度
		float m_Mass;	//質量
		float m_Speed;	//追いかけるスピード
		Vec3 m_BeforePos;	//1つ前の位置
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プレイヤーを追いかける速度を設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVelosity();
		//--------------------------------------------------------------------------------------
		/*!
		@brief Boxとの衝突判定
		@param[in]	BeforePos	1つ前の場所
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CollisionWithBoxes(const Vec3& BeforePos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 進行方向を向く
		@param[in]	LerpFact	補間係数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(float LerpFact);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Scale	大きさ
		@param[in]	Qt	回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		MoveBoxObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, bool Trace,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MoveBoxObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief OBBを得る
		@return	OBB
		*/
		//--------------------------------------------------------------------------------------
		virtual OBB GetOBB()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 位置を得る
		@return	位置
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetPosition() const {
			return m_Pos;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前の位置を得る
		@return	1つ前の位置
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetBeforePos()const {
			return m_BeforePos;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 速度を設定する（プレイヤーとの衝突時の処理に使用）
		@param[in]	Velocity	速度
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetVelocity(const Vec3& Velocity) {
			m_Velocity = Velocity;
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
		@brief 衝突判定をする（仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCollision()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 回転処理をする（仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnRotation() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



	//--------------------------------------------------------------------------------------
	///	PNT頂点描画に使用する構造体
	//--------------------------------------------------------------------------------------
	struct DrawObject {
		shared_ptr<MeshResource> m_MeshRes;
		shared_ptr<TextureResource> m_TextureRes;
		Mat4x4 m_WorldMatrix;
		bool m_Trace;
		bool m_Wrap;
		DrawObject():
			m_MeshRes(nullptr),
			m_TextureRes(nullptr),
			m_WorldMatrix(),
			m_Trace(false),
			m_Wrap(false)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class PNTDrawObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<シーン
		vector<DrawObject> m_DrawObjectVec;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		*/
		//--------------------------------------------------------------------------------------
		PNTDrawObject(const shared_ptr<Scene> PtrScene);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	MeshRes	メッシュ
		@param[in]	TextureRes テクスチャ
		@param[in]	WorldMat ワールド行列
		@param[in]	Trace 透明処理するかどうか
		@param[in]	Wrap ラッピング処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes,
			const Mat4x4& WorldMat,
			bool Trace,bool Wrap = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
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
	///	ラッピング処理されたスプライト
	//--------------------------------------------------------------------------------------
	class WrappedSprite : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//バックアップしておく頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
		wstring m_TextureFileName;		///<テクスチャファイル名
		bool m_Trace;					///<透明処理するかどうか
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec2 m_Scale;				///<スケーリング
		float m_Rot;				///<回転
		Vec2 m_Pos;				///<位置
		UINT m_XWrap;		//X方向のラップ数
		UINT m_YWrap;		//Y方向のラップ数
		float m_TotalTime;	//頂点変更に使用するタイム
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の変更
		@param[in]	ElapsedTime	ターン時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertex(float ElapsedTime);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期大きさ
		@param[in]	StartPos	初期位置
		@param[in]	XWrap	X方向のラップ数
		@param[in]	YWrap	Y方向のラップ数
		*/
		//--------------------------------------------------------------------------------------
		WrappedSprite(const wstring& TextureFileName, bool Trace,
			const Vec2& StartScale,
			const Vec2& StartPos,
			UINT XWrap,UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WrappedSprite();
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



}
//end basecross
