/*!
@file Player.h
@brief プレイヤー
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	球体のプレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject, public MatrixInterface {
		///メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		Mat4x4 m_MeshToTransformMatrix;
		///テクスチャリソース名
		wstring m_TextureResName;
		///スケーリングベースの最下地点
		float m_BaseY;
		///位置
		Vec3 m_Posision;
		///透明処理するかどうか
		bool m_Trace;
		///ジャンプのロック
		bool m_JumpLock;
		/// ステップ（回避行動）してるかどうか
		bool m_isStep;

		bool m_UpdateActive;

		bool m_CannonAnimation;
		///砲撃した場合のbool
		int m_NowCannonClass;
		//無敵かどうか
		bool m_isInvincible;

		shared_ptr<SoundObject> m_AttackSound;
		shared_ptr<SoundObject> m_StepSound;
		shared_ptr<SoundObject> m_DamageSound;
		shared_ptr<SoundObject> m_BombSound;

		//移動しているかどうか
		enum RunningMode {
			RunStart,Running,RunEnd
		};
		RunningMode isRunning;
		/// ステップする方向
		Vec3 m_StepVec;
		/// ノックバックする方向
		Vec3 m_KnockBackVec;
		/// フレームカウント用変数
		float m_FrameCount;
		//攻撃するときに補正をかける距離
		float m_AttackDis;
		//攻撃するときに補正をかける角度
		float m_FOV;

		float m_CannonHP;
		float m_DefaultCannonHP;
		//移動アニメーション用フレームカウント
		float m_RunAnimationFrameCount;
		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		///シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

		shared_ptr<GameObject> m_Sword;
		
		//ステートマシーン
		unique_ptr<StateMachine<Player>>  m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, bool Trace, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player();
		///描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 位置を得る
		@return	位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 位置を変える
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(Vec3 pos) {
			m_Rigidbody->m_Pos = pos;
		}

		float GetScale(){
			return m_Rigidbody->m_Scale.x;
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
		@brief ワールド行列の取得
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コントローラから方向ベクトルを得る
		@return	方向ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 入力関連の関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void MoveControll();

		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージ処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Damage(Vec3 EnemyPos);

		//--------------------------------------------------------------------------------------
		/*!
		@brief 通常のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DefaultBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 速度を大きく減らす処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InitVelocity();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 移動中のアにメーション変更
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RunningAnimation();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステップのエフェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void PlayerStepEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステップのベクトルの設定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetStepVec(Vec3 step_vec) {
			m_FrameCount = 0.0f;
			m_StepVec = step_vec;
			m_StepVec.normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 移動ベクトルの取得
		@return	移動ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetVelocity() {
			return m_Rigidbody->m_Velocity;
		}

		//void StepStartBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステップ中のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StepBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief 攻撃中のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AttackBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief つながり攻撃のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChainAttackBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief 大砲との当たり判定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CollisionWithCannon();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージを受けた時最初のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DamagedStartBehaviour(Vec3 StartPos);

		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージを受けた時のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DamagedBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ダメージを受け無敵状態のふるまい
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AfterDamagedBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief アニメーションの変更
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangeAnimation(wstring animation_key);

		//--------------------------------------------------------------------------------------
		/*!
		@brief ステートを変更
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangeDefaultState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 走るアニメーションをリセットする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InitRunFrameCount() {
			m_RunAnimationFrameCount = 0.0f;
			isRunning = RunEnd;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 砲撃してるかどうかのboolのゲッターセッター
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIsCannnon(int is_cannon) {
			m_NowCannonClass = is_cannon;
		}
		int GetIsCannon() {
			return m_NowCannonClass;
		}

		void SetInvincible(bool value) {
			m_isInvincible = value;
		}

		bool GetInvincible() {
			return m_isInvincible;
		}

		void SetKnockBackVec(Vec3 vec) {
			m_KnockBackVec = vec;
		}

		float GetDefaultBossHP() {
			return m_DefaultCannonHP;
		}

		void SetUpdateActive(bool value) {
			m_UpdateActive = value;
		}
		bool GetUpdateActive() {
			return m_UpdateActive;
		}

		void SetCannonHP(float value) {
			m_CannonHP = value;
		}
		float GetCannonHP() {
			return m_CannonHP;
		}
		void CannonDamage(float value) {
			m_CannonHP -= value;
		}

		bool GetCannonAnimation() {
			return m_CannonAnimation;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Player> >& GetStateMachine() {
			return m_StateMachine;
		}
	};

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class DefaultState : public ObjState<Player>
	{
		DefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(DefaultState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	攻撃ステート
	//--------------------------------------------------------------------------------------
	class PlayerAttackState : public ObjState<Player>
	{
		PlayerAttackState() {}
	private:
		float frame_count;
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerAttackState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	回避行動ステート
	//--------------------------------------------------------------------------------------
	class StepState : public ObjState<Player>
	{
		StepState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(StepState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ダメージを受けた際のステート
	//--------------------------------------------------------------------------------------
	class DamagedState : public ObjState<Player>
	{
		DamagedState() {}
	private:
		float frame_count;
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(DamagedState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	つながりを使った攻撃ステート
	//--------------------------------------------------------------------------------------
	class ChainAttackState : public ObjState<Player>
	{
		ChainAttackState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(ChainAttackState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤーの剣(α用）
	//--------------------------------------------------------------------------------------
	class Sword : public GameObject, public MatrixInterface {
		//テクスチャリソース名
		wstring m_TextureResName;
		//スケーリング
		Vec3 m_Scale;
		//回転
		Quat m_Qt;
		//位置
		Vec3 m_Pos;

		//親オブジェクト
		weak_ptr<GameObject> m_ParentPtr;
		//
		//Rigidbodyのshared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;

		//文字列描画オブジェクト
		shared_ptr<StringDrawObject> m_StringDrawObject;

		shared_ptr<SoundObject> m_AttackSound;

		///描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
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
		//後ろについてくるエネミー
		vector<weak_ptr<GameObject>> m_friends;
		//後ろについてくるエネミーの数
		int m_friends_num;
		//ステートマシーン
		unique_ptr<StateMachine<Sword>>  m_StateMachine;
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
		Sword(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Sword();
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
		unique_ptr< StateMachine<Sword> >& GetStateMachine() {
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
		@brief 追従する行動の開始
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
		@brief 攻撃処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AttackBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 後ろについてくる敵を増やす
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFriends(weak_ptr<GameObject> gameObject) {
			m_friends.push_back(gameObject);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 敵を大砲にセットする
		@return	セットする敵の数
		*/
		//--------------------------------------------------------------------------------------
		int SetEnemyToCannon(wstring tag_name);
		//--------------------------------------------------------------------------------------
		/*!
		@brief つながりを消す
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void DeleteChains(int num);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステートを変更する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetState(wstring state_name);

		wstring GetChainTX(int num);

		float GetScale() {
			return m_Scale.x;
		}
	};

	//--------------------------------------------------------------------------------------
	///	攻撃しないステート（Sword）
	//--------------------------------------------------------------------------------------
	class NonAttackState : public ObjState<Sword>
	{
		NonAttackState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(NonAttackState)
		virtual void Enter(const shared_ptr<Sword>& Obj)override;
		virtual void Execute(const shared_ptr<Sword>& Obj)override;
		virtual void Exit(const shared_ptr<Sword>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	攻撃ステート１（Sword）
	//--------------------------------------------------------------------------------------
	class Attack1State : public ObjState<Sword>
	{
		Attack1State() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Attack1State)
		virtual void Enter(const shared_ptr<Sword>& Obj)override;
		virtual void Execute(const shared_ptr<Sword>& Obj)override;
		virtual void Exit(const shared_ptr<Sword>& Obj)override;
	};

}
//end basecross
