/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲーム全体を管理するマネージャー、スポナー
	//--------------------------------------------------------------------------------------
	GameManager* GameManager::GM = nullptr;

	GameManager* GameManager::getInstance() {
		if (GM == nullptr) {
			GM = new GameManager();
		}
		return GM;
	}

	//--------------------------------------------------------------------------------------
	///	平面実体
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const wstring& NormalTextureResName,
		const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_NormalTextureResName(NormalTextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers(float WrapX, float WrapY) {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vec3(-HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, WrapY)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, WrapY)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		vector<VertexPositionNormalTangentTexture> new_vertices;
		BcRenderer::ConvertToNormalVertex(vertices, new_vertices);
		//メッシュの作成（変更できない）
		m_SquareMesh = MeshResource::CreateMeshResource(new_vertices, indices, false);
	}


	void SquareObject::OnCreate() {

		CreateBuffers(m_Scale.x, m_Scale.y);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto NormTexPtr = App::GetApp()->GetResource<TextureResource>(m_NormalTextureResName);

		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SquareMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_NormalTextureRes = NormTexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_SamplerState = SamplerState::LinearWrap;
		m_PtrObj->m_ShadowmapUse = false;
		m_PtrObj->m_FogEnabled = true;
		//フォグはきつめに
		m_PtrObj->m_FogColor = Col4(0.3f, 0.3f, 0.3f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;
	}

	void SquareObject::OnUpdate() {
	}

	void SquareObject::OnDraw() {
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///　ステージの大和オブジェクト
	//--------------------------------------------------------------------------------------

	YamatoStage::YamatoStage(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive)
	{}
	YamatoStage::~YamatoStage() {}

	void YamatoStage::OnCreate() {
		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"YAMATO_MESH");

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -XM_PIDIV2, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		m_PtrObj->m_FogEnabled = true;
		m_PtrObj->m_FogColor = Col4(0.07f, 0.0f, 0.09f, 1.0f);
		m_PtrObj->m_FogStart = -3.0f;
		m_PtrObj->m_FogEnd = -100.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void YamatoStage::OnUpdate() {

	}

	void YamatoStage::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void YamatoStage::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	Vec3 YamatoStage::GetPosition() {
		return m_Pos;
	}

	//--------------------------------------------------------------------------------------
	///	透明な壁
	//--------------------------------------------------------------------------------------
	TransparentWall::TransparentWall(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos) {}
	TransparentWall::~TransparentWall() {}
	//初期化
	void TransparentWall::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeOBB;
		body.m_IsFixed = true;
		//body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			body.m_Scale,
			Vec3(0, 0, 0),
			body.m_Quat,
			body.m_Pos
		);
	}

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//初期化
	void MultiSpark::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"MultiSpark");
	}

	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(64);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 0.5f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalScale = Vec2(0.2, 0.2);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 30.0f,
				rParticleSprite.m_LocalPos.y * 30.0f,
				rParticleSprite.m_LocalPos.z * 30.0f
			);
			//色の指定
			int r = (int)((Util::RandZeroToOne() * 0.5f + 0.3f) * 256.0f);
			int g = (int)((Util::RandZeroToOne() * 0.5f + 0.3f) * 256.0f);
			int b = (int)((Util::RandZeroToOne() * 0.5f + 0.3f) * 256.0f);
			r = r >> 7;
			g = g >> 7;
			b = b >> 7;
			rParticleSprite.m_Color = Col4((float)r, (float)g, (float)b, 1.0f);
		}
	}

	void MultiSpark::OnUpdate() {
		MultiParticle::OnUpdate();
		/*float ElapsedTime = App::GetApp()->GetElapsedTime();

		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
				}
			}
		}*/
	}

	//--------------------------------------------------------------------------------------
	//class AttackSpark : public MultiParticle;
	//用途: 攻撃のスパーククラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	AttackSpark::AttackSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	AttackSpark::~AttackSpark() {}

	//初期化
	void AttackSpark::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"AttackSpark");
	}

	void AttackSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(128);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 0.3f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalScale = Vec2(0.1, 0.1);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 30.0f,
				rParticleSprite.m_LocalPos.y * 30.0f,
				rParticleSprite.m_LocalPos.z * 30.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 0.4f, 0.0f, 1.0f);
		}
	}

	void AttackSpark::OnUpdate() {
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.w -= 0.06f;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//class MultiGuardEffect : public MultiParticle;
	//用途: エンジェルエネミーの防御エフェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiGuardEffect::MultiGuardEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiGuardEffect::~MultiGuardEffect() {}

	//初期化
	void MultiGuardEffect::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"MultiGuardEffect");
	}

	void MultiGuardEffect::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(8);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 0.08f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalScale = Vec2(0.5, 0.5);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(0.0f, 1.0f, 0.0f, 0.1f);
		}
	}

	void MultiGuardEffect::OnUpdate() {
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		/*for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
				}
			}
		}*/
	}

	//--------------------------------------------------------------------------------------
	//class BossEffect : public MultiParticle;
	//用途: ボスの防御エフェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BossEffect::BossEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	BossEffect::~BossEffect() {}

	//初期化
	void BossEffect::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"BossEffect");
	}

	void BossEffect::InsertSpark(const Vec3& Pos,wstring name) {
		auto ParticlePtr = InsertParticle(8);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 0.08f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 5.0f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 5.0f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 5.0f;
			rParticleSprite.m_LocalScale = Vec2(20.0, 20.0);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//色の指定
			if (name == L"Red") {
				rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 0.1f);
			}
			if (name == L"Green") {
				rParticleSprite.m_Color = Col4(0.0f, 1.0f, 0.0f, 0.1f);
			}
			if (name == L"Blue") {
				rParticleSprite.m_Color = Col4(0.0f, 0.0f, 1.0f, 0.1f);
			}
		}
	}

	void BossEffect::OnUpdate() {
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		/*for (auto ParticlePtr : GetParticleVec()) {
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
		if (rParticleSprite.m_Active) {
		}
		}
		}*/
	}

	//--------------------------------------------------------------------------------------
	//class EnemyMoveEffect : public MultiParticle;
	//用途: エネミーの移動エフェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	EnemyMoveEffect::EnemyMoveEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	EnemyMoveEffect::~EnemyMoveEffect() {}

	//初期化
	void EnemyMoveEffect::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"EnemyMoveEffect");
		
	}

	void EnemyMoveEffect::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(3);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 0.4f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalScale = Vec2(0.2, 0.2);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(0.5f, 0.5f, 0.5f, 0.1f);
		}
	}

	void EnemyMoveEffect::OnUpdate() {
		MultiParticle::OnUpdate();
		/*float ElapsedTime = App::GetApp()->GetElapsedTime();

		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
				}
			}
		}*/
	}

	//--------------------------------------------------------------------------------------
	//class MultiFire : public MultiParticle;
	//用途: 複数の炎クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//初期化
	void MultiFire::OnCreate() {
		//タグの追加
		AddTag(L"MultiFire");
	}

	void MultiFire::InsertFire(const Vec3& Pos ,float Scale) {
		auto ParticlePtr = InsertParticle(16);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"FIRE_TX");
		ParticlePtr->m_MaxTime = 1.0f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			rParticleSprite.m_LocalScale *= Scale;
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	void MultiFire::OnUpdate()
	{
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.w -= 0.03f;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	//class AttackSigns : public MultiParticle;
	//用途:敵の攻撃する前兆エフェクト
	//--------------------------------------------------------------------------------------
	AttackSigns::AttackSigns(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	AttackSigns::~AttackSigns() {}

	//初期化
	void AttackSigns::OnCreate() {
		//加算描画処理をする
		SetAddType(false);
		//タグの追加
		AddTag(L"AttackSigns");
	}


	void AttackSigns::InsertSigns(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(2);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 1.25f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		//カメラの方向に少し移動させる
		auto &Camera = GetStage()->GetCamera();
		Vec3 Camera_Pos = Camera.m_CamerEye;
		Camera_Pos.y = 0.0f;
		Vec3 MoveVec = Camera_Pos - Pos;
		MoveVec.normalize();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalScale = Vec2(0.05f, 0.05f);
			rParticleSprite.m_LocalPos += MoveVec * 0.15f;
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	void AttackSigns::OnUpdate()
	{
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_LocalScale.x += 0.15f;
					rParticleSprite.m_Color.w -= 0.025f;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	//class BossAttackSigns : public MultiParticle;
	//用途:ボスの攻撃する前兆エフェクト
	//--------------------------------------------------------------------------------------
	BossAttackSigns::BossAttackSigns(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	BossAttackSigns::~BossAttackSigns() {}

	//初期化
	void BossAttackSigns::OnCreate() {
		//加算描画処理をする
		SetAddType(false);
		//タグの追加
		AddTag(L"BossAttackSigns");
	}

	void BossAttackSigns::InsertSigns(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(5);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 1.25f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		//カメラの方向に少し移動させる
		auto &Camera = GetStage()->GetCamera();
		Vec3 Camera_Pos = Camera.m_CamerEye;
		Camera_Pos.y = 0.0f;
		Vec3 MoveVec = Camera_Pos - Pos;
		MoveVec.normalize();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalScale = Vec2(0.1f, 0.1f);
			rParticleSprite.m_LocalPos += (MoveVec * 1.5f);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 0.0f, 1.0f);
		}
	}

	void BossAttackSigns::OnUpdate()
	{
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_LocalScale.x += 0.15f;
					rParticleSprite.m_Color.w -= 0.025f;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//class CannonEffect : public MultiParticle;
	//用途: 大砲のエフェクト
	//--------------------------------------------------------------------------------------
	CannonEffect::CannonEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	CannonEffect::~CannonEffect() {}

	//初期化
	void CannonEffect::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"CannonEffect");
	}


	void CannonEffect::InsertEffect(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(2);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"STEP_TX");
		ParticlePtr->m_MaxTime = 0.5f;

		auto &camera = GetStage()->GetCamera();
		Vec3 camera_pos = camera.m_CamerEye;

		Vec3 MoveVec = camera_pos - Pos;
		MoveVec.normalize();

		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(0.3f, 0.3f, 0.3f, 1.0f);
			rParticleSprite.m_LocalScale *= 3.0f;
		}
	}

	void CannonEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.w -= 0.03f;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//class StepEffect : public MultiParticle;
	//用途: ステップエフェクト
	//--------------------------------------------------------------------------------------
	StepEffect::StepEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	StepEffect::~StepEffect() {}

	//初期化
	void StepEffect::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"StepEffect");
	}


	void StepEffect::InsertEffect(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(2);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"STEP_TX");
		ParticlePtr->m_MaxTime = 0.5f;

		auto &camera = GetStage()->GetCamera();
		Vec3 camera_pos = camera.m_CamerEye;

		Vec3 MoveVec = camera_pos - Pos;
		MoveVec.normalize();

		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	void StepEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.w -= 0.03f;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//class ShootingEffect : public MultiParticle;
	//用途: 大砲で撃たれているときのエフェクト
	//--------------------------------------------------------------------------------------
	ShootingEffect::ShootingEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	ShootingEffect::~ShootingEffect() {}

	//初期化
	void ShootingEffect::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"ShootingEffect");
	}


	void ShootingEffect::InsertEffect(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(2);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"STEP_TX");
		ParticlePtr->m_MaxTime = 0.3f;

		auto &camera = GetStage()->GetCamera();
		Vec3 camera_pos = camera.m_CamerEye;

		Vec3 MoveVec = camera_pos - Pos;
		MoveVec.normalize();

		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 0.7f);
		}
	}

	void ShootingEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.w -= 0.1f;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	///	ラッピング処理されたスプライト
	//--------------------------------------------------------------------------------------
	SpriteBase::SpriteBase(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, 
		const Vec2& StartScale, 
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(StartScale),
		m_Rot(StartRot),
		m_Pos(StartPos),
		m_XWrap(XWrap),
		m_YWrap(YWrap),
		m_Emissive(0,0,0,0),
		m_BlendState(BlendState::Opaque)
	{
		float HelfSize = 0.5f;
		//頂点配列(縦横指定数ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
	}
	void SpriteBase::OnCreate() {
		//頂点の初期修正（仮想関数呼びだし）
		AdjustVertex();
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できる）
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
	}

	void SpriteBase::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		//仮想関数呼び出し
		UpdateVertex(ElapsedTime, vertices);
		//アンマップ
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);
	}



	void SpriteBase::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation2D(
			m_Scale,			//スケーリング
			Vec2(0, 0),		//回転の中心（重心）
			m_Rot,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Mat4x4 Proj(XMMatrixOrthographicLH(w, h, -1.0, 1.0f));
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ加算。
		sb.Emissive = m_Emissive;
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());
		//ブレンドステート
		switch (m_BlendState) {
		case BlendState::Opaque:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			break;
		case BlendState::Trace:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			break;
		case BlendState::Additive:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
			break;
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, TexPtr->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//描画
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}
	//--------------------------------------------------------------------------------------
	///	回転するスプライト
	//--------------------------------------------------------------------------------------
	RotateSprite::RotateSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, 
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap):
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Additive);
		float HelfSize = 0.5f;
		//頂点配列(縦横指定数ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(0, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(0, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
	}

	void RotateSprite::AdjustVertex() {
		//頂点色を変更する
		for (size_t i = 0; i < m_BackupVertices.size();i++) {
			switch (i) {
			case 0:
				m_BackupVertices[i].color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 1:
				m_BackupVertices[i].color = Col4(0.0f, 1.0f, 0.0f, 1.0f);
				break;
			case 2:
				m_BackupVertices[i].color = Col4(0.0f, 0.0f, 1.0f, 1.0f);
				break;
			case 3:
				m_BackupVertices[i].color = Col4(1.0f, 1.0f, 0, 1.0);
				break;
			}
		}
	}

	void RotateSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		/*m_Rot += ElapsedTime;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}*/
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 1.0f) {
			m_TotalTime = 0;
		}
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vec2 UV(m_BackupVertices[i].textureCoordinate);
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);
		}
		m_Scale.x -= 1.0f;

	}

	//--------------------------------------------------------------------------------------
	///	大砲のゲージ
	//--------------------------------------------------------------------------------------
	CannonGauge::CannonGauge(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
		float HelfSize = 0.5f;
		//頂点配列(縦横指定数ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(0, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(0, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
		m_DefaultSize = StartScale.x;
	}

	void CannonGauge::AdjustVertex() {
	}

	void CannonGauge::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, 1.0f);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);
		}
		auto player = GetStage()->FindTagGameObject<Player>(L"Player");
		float HP = player->GetCannonHP();
		float ratio = HP / player->GetDefaultBossHP();
		m_Scale.x = m_DefaultSize * ratio;
	}

	//--------------------------------------------------------------------------------------
	///	ボスのHPゲージ
	//--------------------------------------------------------------------------------------
	BossHPGauge::BossHPGauge(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
		float HelfSize = 0.5f;
		//頂点配列(縦横指定数ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(0, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(0, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
		m_DefaultSize = StartScale.x;
	}

	void BossHPGauge::AdjustVertex() {
	}

	void BossHPGauge::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, 1.0f);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);
		}
		auto GM = GameManager::getInstance();
		float bossHP = GM->GetBossHP();
		float ratio = bossHP / GM->GetDefaultBossHP();
		m_Scale.x = m_DefaultSize * ratio;
	}

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	MessageSprite::MessageSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
	}

	void MessageSprite::AdjustVertex() {
		//ここでは何もしない
	}

	void  MessageSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		m_TotalTime += (ElapsedTime * 5.0f);
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		float sin_val = sin(m_TotalTime) * 0.5f + 0.5f;
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, sin_val);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);

		}
	}

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	ResultSprite::ResultSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
	}

	void ResultSprite::AdjustVertex() {
		//頂点色を変更する
		
	}

	void  ResultSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		if (GetStage<GameStage>()->GetIsClear() || GetStage<GameStage>()->GetIsFail()) {
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= 1.0f) {
				m_TotalTime = 1.0f;
			}
			//float sin_val = sin(m_TotalTime) * 0.5f + 0.5f;
			Col4 UpdateCol(1.0f, 1.0f, 1.0f, m_TotalTime);
			for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
				vertices[i] = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					UpdateCol,
					m_BackupVertices[i].textureCoordinate
				);

			}
		}
		else {
			
			Col4 UpdateCol(1.0f, 1.0f, 1.0f, 0.0f);
			for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
				vertices[i] = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					UpdateCol,
					m_BackupVertices[i].textureCoordinate
				);

			}
		}
	}

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	ResultCursorSprite::ResultCursorSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
		m_Moved = false;
	}

	void ResultCursorSprite::AdjustVertex() {
		//頂点色を変更する

	}

	void  ResultCursorSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		if (GetStage<GameStage>()->GetIsClear() || GetStage<GameStage>()->GetIsFail()) {
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= 1.0f) {
				m_TotalTime = 1.0f;
			}
			//float sin_val = sin(m_TotalTime) * 0.5f + 0.5f;
			Col4 UpdateCol(1.0f, 1.0f, 1.0f, m_TotalTime);
			for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
				vertices[i] = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					UpdateCol,
					m_BackupVertices[i].textureCoordinate
				);

			}
		}
		else {

			Col4 UpdateCol(1.0f, 1.0f, 1.0f, 0.0f);
			for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
				vertices[i] = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					UpdateCol,
					m_BackupVertices[i].textureCoordinate
				);

			}
		}
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0) {
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				if (MoveX > 0) {
					if (MoveX > 0.8f && m_Moved == false) {
						m_Pos.x *= -1.0f;
						m_Moved = true;
					}
					else if (MoveX < 0.7f && m_Moved == true) {
						m_Moved = false;
					}
				}
				else if (MoveX < 0) {
					if (MoveX < -0.8f && m_Moved == false) {
						m_Pos.x *= -1.0f;
						m_Moved = true;
					}
					else if (MoveX > -0.7f && m_Moved == true) {
						m_Moved = false;
					}
				}
			}
			if ((CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A)
				|| (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B)) {
				if (m_Pos.x < 0) {
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
				}
				else {
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitle");
				}
			}
			
		}
	}

	//--------------------------------------------------------------------------------------
	///	背景スプライト
	//--------------------------------------------------------------------------------------
	MultiSprite::MultiSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
	}

	void MultiSprite::AdjustVertex() {
		
	}

	void MultiSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, 1.0f);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);
		}

	}

	//--------------------------------------------------------------------------------------
	///　SkyBox
	//--------------------------------------------------------------------------------------

	SkyBox::SkyBox(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive)
	{}
	SkyBox::~SkyBox() {}

	void SkyBox::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();

		//メッシュの取得

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(-1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		auto MeshPtr = MeshResource::CreateMeshResource(vertices, indices, false);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = false;
		m_PtrObj->m_UsedModelColor = false;

		m_PtrObj->m_Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		m_PtrObj->m_Emissive = Col4(1.0f, 1.0f, 1.0f, 1.0f);

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void SkyBox::OnUpdate() {

	}

	void SkyBox::OnDrawShadowmap() {
		
	}

	void SkyBox::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<SimplePNTStaticRenderer>(L"SimplePNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///　大砲オブジェクト
	//--------------------------------------------------------------------------------------

	Cannon::Cannon(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const wstring& TagName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		int CannonClass, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive),
		myClass(CannonClass),
		m_HP(100.0f)
	{
		AddTag(TagName);
	}
	Cannon::~Cannon() {}

	void Cannon::OnCreate() {
		//タグの追加
		AddTag(L"Cannon");

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsFixed = true;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"CANNON_MESH");

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.5f, 1.5f, 1.5f),
			Vec3(0.0f, 0.0f, 0.3f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void Cannon::OnUpdate() {
	}

	void Cannon::Rotation(Vec3 vec) {
		Vec3 Temp = vec - m_Rigidbody->m_Pos;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
	}

	void Cannon::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void Cannon::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	Vec3 Cannon::GetPosition() {
		return m_Rigidbody->m_Pos;
	}

	void Cannon::GetWorldMatrix(Mat4x4& m) const {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m = World;
	}

	//--------------------------------------------------------------------------------------
	///	敵の親オブジェクト
	//--------------------------------------------------------------------------------------

	EnemyObject::EnemyObject(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
		const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_ParentPtr(ParentPtr),
		m_MeshResName(MeshResName),
		m_TextureResName(TextureResName),
		m_DefaultAnimation(DefaultAnimation),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_BeforePos(Pos),
		m_FrameCount(0.0f),
		m_Speed(0.5f),
		m_Tackle(false),
		m_StopTime(1.5f),
		m_SearchDis(3.0f),
		m_TackleTime(0.75f),
		m_ShootNumber(0),
		m_TackleSpeed(3.0f),
		m_AfterAttackTime(2.0f),
		m_TackleStartPos(Vec3(0.0f, 0.0f, 0.0f)),
		m_OwnShadowActive(OwnShadowActive),
		m_LerpToParent(0.2f),
		m_LerpToChild(0.2f),
		m_Attack1ToRot(0),
		m_HP(1.0f),
		m_AttackPoint(100.0f)
	{
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
		m_BaseY = m_Scale.y / 2.0f;
	}
	EnemyObject::~EnemyObject() {}

	void EnemyObject::OnCreate() {
		//タグの追加
		AddTag(L"EnemyObject");

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = true;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		m_UpdateActive = true;
		m_isDead = false;
		m_Bomb = false;

		m_DefaultHP = m_HP;

		m_PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");

		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(m_MeshResName);
		//サウンドオブジェクトの初期化
		m_DeadSound = ObjectFactory::Create<SoundObject>(L"Pan");
		m_BossDamageSound = ObjectFactory::Create<SoundObject>(L"bossdamage");
		m_FriendsSound = ObjectFactory::Create<SoundObject>(L"nakama");
		m_CannonSound = ObjectFactory::Create<SoundObject>(L"cannon");
		m_EyeFlashSound = ObjectFactory::Create<SoundObject>(L"eye_flash");

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//ターゲット座標の初期化
		m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);

		m_FollowingAngelNum = 0;

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;
		//m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		//m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;
		m_PtrObj->m_FogEnabled = true;
		m_PtrObj->m_FogColor = Col4(0.07f, 0.0f, 0.09f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -100.0f;

		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"30frame", 0, 30, true, 30.0f);
		m_PtrObj->AddAnimation(L"NonMove", 0, 1, true, 30.0f);
		m_PtrObj->AddAnimation(L"Attack", 8, 20, false, 45.0f);
		m_PtrObj->ChangeCurrentAnimation(m_DefaultAnimation);

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<EnemyObject>(GetThis<EnemyObject>()));
		//ステート初期値設定
		m_StateMachine->ChangeState(EnemyToCannonState::Instance());

		vector<shared_ptr<GameObject>> CannonVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Cannon", CannonVec);

		int num = rand() % 3;
		for (auto cannon : CannonVec)
		{
			if (cannon)
			{
				auto Ptrcannon = dynamic_pointer_cast<Cannon>(cannon);
				if (num == Ptrcannon->GetCannonClass()) {
					m_CannonPos = Ptrcannon->GetPosition();
					break;
				}
			}
		}
	}

	void EnemyObject::CollisionBullet() {
		//弾との衝突判定
		vector<shared_ptr<GameObject>> BulletVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"PlayerBullet", BulletVec);
		for (auto bullet : BulletVec) {
			if (bullet) {
				auto PtrBullet = dynamic_pointer_cast<BulletObject>(bullet);

				Vec3 BulletPos = PtrBullet->GetPosition();
				float length = (BulletPos - m_Rigidbody->m_Pos).length();

				float Radius = PtrBullet->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

				if (length < Radius + PlayerRadius) {

					Vec3 Emitter = BulletPos;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);
					//弾を異次元に飛ばす
					PtrBullet->SetPosition(Vec3(0.0f, -100.0f, 0.0f));

					//体力を削る
					m_HP--;

					if (m_HP <= 0.0f) {
						Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
						float p_dis = (p_pos - GetPosition()).length();
						if (p_dis < 1.0f) {
							p_dis = 1.0f;
						}
						//サウンドの発行
						if (FindTag(L"Zako")) {
							m_DeadSound->Start(0, 1.0f / p_dis);
						}
						else {
							m_CannonSound->Start(0, 1.0f / p_dis);
						}

						Vec3 Emitter = GetPosition();
						//Fireの送出
						auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
						SparkPtr->InsertFire(Emitter, m_Scale.x * 3.0f);
						
						m_HP = m_DefaultHP;
						m_StateMachine->ChangeState(EnemyWaitingState::Instance());
					}
					return;
				}
			}
		}
	}

	void EnemyObject::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);
		//ステートマシン更新
		m_StateMachine->Update();
	}

	void EnemyObject::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void EnemyObject::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	Vec3 EnemyObject::GetPosition() {
		return m_Rigidbody->m_Pos;
	}

	void EnemyObject::WaitingStartBehaviour() {
		SetPosition(Vec3(0, -100, 0));
		m_Rigidbody->m_Velocity = Vec3(0, 0, 0);
		m_Tackle = false;
		m_FrameCount = 0.0f;
		AddTag(L"WaitingSpawn");
		m_Rigidbody->m_IsCollisionActive = false;
	}

	void EnemyObject::WaitingEndBehaviour() {
		m_Rigidbody->m_Velocity = Vec3(0, 0, 0);
		RemoveTag(L"WaitingSpawn");
		m_Rigidbody->m_IsCollisionActive = true;
	}

	void EnemyObject::GetWorldMatrix(Mat4x4& m) const {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m = World;
	}

	enum class ParentFlg {
		NoParent,
		Player,
		Child
	};

	void EnemyObject::FriendsBehavior() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_ParentPtr.lock();
		//親のワールド行列を取得する変数
		Mat4x4 ParMat;
		if (shptr) {
			ParentFlg flg = ParentFlg::NoParent;
			//行列取得用のインターフェイスを持ってるかどうか
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
				if (shptr->FindTag(L"Player")) {
					flg = ParentFlg::Player;
				}
				else if (shptr->FindTag(L"Chain")) {
					flg = ParentFlg::Child;
				}
			}
			Mat4x4 World;
			World.identity();
			float LerpNum = 0.2f;
			switch (flg) {
			case ParentFlg::Player:
				//行列の定義
				World = m_PlayerLocalMatrix;
				LerpNum = m_LerpToParent;
				break;
			case ParentFlg::Child:
				//行列の定義
				World = m_ChildLocalMatrix;
				LerpNum = m_LerpToChild;
				break;
			default:
				break;
			}
			if (flg != ParentFlg::NoParent) {
				//スケーリングを1.0にした行列に変換
				ParMat.scaleIdentity();
				//行列の反映
				World *= ParMat;
				//この時点でWorldは目標となる位置
				Vec3 toPos = World.transInMatrix();
				//補間処理で移動位置を決定
				auto CalcPos = Lerp::CalculateLerp(m_Rigidbody->m_BeforePos, toPos, 0, 1.0f, LerpNum, Lerp::rate::Linear);
				Vec3 DammiPos;
				World.decompose(m_Rigidbody->m_Scale, m_Rigidbody->m_Quat, DammiPos);
				Vec3 Velo = CalcPos - m_Rigidbody->m_BeforePos;
				Velo /= ElapsedTime;
				m_Rigidbody->m_Velocity = Velo;
			}
		}
		if (m_FollowingAngelNum) {
			Vec3 Emitter = m_Rigidbody->m_Pos;
			//Sparkの送出
			auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiGuardEffect>(L"MultiGuardEffect");
			SparkPtr->InsertSpark(Emitter);
		}
		else if (m_FollowingAngelNum == 0 && m_Rigidbody->m_Velocity.length() > 1.0f) {
			Vec3 Emitter = m_Rigidbody->m_Pos;
			Emitter.y -= 0.125f;
			//Sparkの送出
			auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<EnemyMoveEffect>(L"EnemyMoveEffect");
			SparkPtr->InsertSpark(Emitter);
		}
	}

	void EnemyObject::UpdateBehavior() {
		FriendsBehavior();
	}

	void EnemyObject::OppositionBehavior() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		//親のワールド行列を取得する変数
		Mat4x4 ParMat;
		if (shptr) {
			//行列取得用のインターフェイスを持ってるかどうか
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
			}

			Mat4x4 World;
			World.identity();
			//行列の定義
			World = m_PlayerLocalMatrix;
			//スケーリングを1.0にした行列に変換
			ParMat.scaleIdentity();
			//行列の反映
			World *= ParMat;
			//この時点でWorldは目標となる位置
			Vec3 toPos = World.transInMatrix();
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//距離を求める
			float dis = ToPosVec.length();

			// 突進してしばらくたったら
			if (m_FrameCount > m_StopTime + m_TackleTime)
			{
				m_Tackle = false;
				m_FrameCount = 0.0f;
				m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);
				m_Rigidbody->m_Velocity *= 0.01f;
				m_StateMachine->ChangeState(EnemyAttackEndState::Instance());
				return;
			}
			// 止まりはじめ
			else if (m_FrameCount > m_StopTime && m_Tackle == false)
			{
				m_Tackle = true;
				if (m_TargetPos == Vec3(0.0f, 0.0f, 0.0f)) {
					m_TargetPos = toPos;
					m_TackleStartPos = m_Rigidbody->m_Pos;
				}
			}

			// 突進の処理
			if (m_Tackle == true)
			{
				Vec3 Tag = m_TargetPos - m_TackleStartPos;
				Tag.normalize();
				Tag *= m_TackleSpeed;
				m_Rigidbody->m_Pos.y = m_BaseY;
				if (m_UpdateActive == false) {
					Tag *= 0.0f;
				}
				else {
					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y -= 0.125f;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<EnemyMoveEffect>(L"EnemyMoveEffect");
					SparkPtr->InsertSpark(Emitter);
					m_FrameCount += ElapsedTime;
				}
				m_Rigidbody->m_Velocity = Tag;
			}
			else if (m_Tackle == false)
			{
				if (m_FrameCount > 0.0f && m_UpdateActive)
				{
					ToPosVec.normalize();
					ToPosVec *= 0.01f;
					m_Rigidbody->m_Velocity = ToPosVec;
					m_Rigidbody->m_Pos.y = m_BaseY;
					m_FrameCount += ElapsedTime;
				}
				// プレイヤーとエネミーの距離が近くなった時の処理
				else if (dis <= m_SearchDis && m_UpdateActive)
				{
					m_FrameCount += ElapsedTime;
					if (dis < 1.0f) {
						dis = 1.0f;
					}
					m_EyeFlashSound->Start(0, 0.2f / dis);
					//fireの送出
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<AttackSigns>(L"AttackSigns");
					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y += 0.125f;
					FirePtr->InsertSigns(Emitter);
				}
				// プレイヤーとエネミーの距離が遠くなったら再び大砲に向かう
				else if (dis >= m_SearchDis * 3.0f && m_UpdateActive)
				{
					m_StateMachine->ChangeState(EnemyToCannonState::Instance());
					return;
				}
				// プレイヤーに向かう処理
				else
				{
					ToPosVec.normalize();
					ToPosVec *= m_Speed;
					if (m_UpdateActive == false) {
						ToPosVec *= 0.0f;
					}
					m_Rigidbody->m_Velocity = ToPosVec;
					m_Rigidbody->m_Pos.y = m_BaseY;
				}
			}
		}
		RotateToVelocity();
	}

	void EnemyObject::AttackEndBehavior() {
		if (m_FrameCount > m_AfterAttackTime) {
			m_FrameCount = 0.0f;
			m_StateMachine->ChangeState(EnemyOppositionState::Instance());
			return;
		}
		//高さを固定
		m_Rigidbody->m_Pos.y = m_BaseY;
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_FrameCount += ElapsedTime;
	}

	void EnemyObject::ToCannonBehavior() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		if (shptr) {
			Vec3 toPos = m_CannonPos;
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//プレイヤーとの距離を求める
			Vec3 PlayerPos = shptr->GetPosition();
			float dis = (PlayerPos - m_Rigidbody->m_Pos).length();
			// プレイヤーとエネミーの距離が近くなった時の処理
			if (dis <= (m_SearchDis - 1.0f))
			{
				m_StateMachine->ChangeState(EnemyOppositionState::Instance());
				return;
			}
			// 大砲に向かう処理
			else
			{
				ToPosVec.normalize();
				ToPosVec *= m_Speed;
				if (m_UpdateActive == false) {
					ToPosVec *= 0.0f;
				}
				m_Rigidbody->m_Velocity = ToPosVec;
				m_Rigidbody->m_Pos.y = m_BaseY;
			}
		}
		//大砲との衝突
		vector<shared_ptr<GameObject>> CannonVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Cannon", CannonVec);
		for (auto cannon : CannonVec) {
			if (cannon) {
				auto PtrCannon = dynamic_pointer_cast<Cannon>(cannon);

				Vec3 CannonPos = PtrCannon->GetPosition();
				float length = (CannonPos - m_Rigidbody->m_Pos).length();

				float CannonRadius = PtrCannon->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

				if (length < CannonRadius + PlayerRadius + 1.0f) {
					Vec3 Emitter = m_Rigidbody->m_Pos;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);
					//Fireの送出
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
					FirePtr->InsertFire(Emitter, 1.0f);

					Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
					float p_dis = (p_pos - GetPosition()).length();
					if (p_dis < 1.0f) {
						p_dis = 1.0f;
					}
					//サウンドの発行
					m_DeadSound->Start(0, (0.7f / p_dis) + 0.3f);

					auto p_ptr = dynamic_pointer_cast<Player>(shptr);
					p_ptr->CannonDamage(1.0f);
					if (p_ptr->GetCannonHP() <= 0.0f ) {
						GetStage<GameStage>()->SetIsFail(true);
					}

					m_StateMachine->ChangeState(EnemyWaitingState::Instance());

					break;
				}
			}
		}
		RotateToVelocity();
	}

	void EnemyObject::DamageBehaviour() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		if (m_FrameCount > 1.0f) {
			m_FrameCount = 0;
			m_StateMachine->ChangeState(EnemyOppositionState::Instance());
			return;
		}
		else if (m_FrameCount > 0.3f) {
			m_Rigidbody->m_Velocity = Vec3(0);
		}
		else if (m_FrameCount < 0.3f) {
			auto player = dynamic_pointer_cast<Player>(m_PlayerPtr.lock());
			Vec3 MoveVec = m_Rigidbody->m_Pos - player->GetPosition();
			MoveVec.y = 0.0f;
			MoveVec.normalize();
			m_Rigidbody->m_Velocity = MoveVec * 4.0f;
		}

		m_FrameCount += ElapsedTime;
	}

	void EnemyObject::CheckHealth() {
		if (m_HP <= 0.0f) {
			if (m_isDead == false) {
				m_isDead = true;
				m_StateMachine->ChangeState(EnemyComplianceState::Instance());
				return;
			}
		}
	}

	void EnemyObject::CheckParent() {
		
	}

	void EnemyObject::ComplianceStartBehavior() {
		//ローカル行列の定義
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, -0.05f, -0.3f)
		);
		m_ChildLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, 0, -0.3f)
		);
		m_LerpToParent = m_LerpToChild = 0.2f;
		m_Rigidbody->m_CollType = CollType::typeCAPSULE;
		m_FriendsSound->Start(0, 0.15f);
	}

	//攻撃１行動の開始
	void EnemyObject::Attack1StartBehavior() {
		m_Attack1ToRot = 0.1f;
		//ローカル行列の定義
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(Vec3(1.0, 0, 0), m_Attack1ToRot),
			Vec3(0, 0.4f, 0.0f)
		);
		m_ChildLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, 0.4f, -0.5f)
		);
		m_LerpToParent = m_LerpToChild = 0.5f;

	}

	void EnemyObject::RotateToVelocity() {
		//回転の処理
		Vec3 Temp = m_Rigidbody->m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		//移動しないときは回転しない
		if (m_Rigidbody->m_Velocity.length() > 0.0f) {
			m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
		}
	}

	void EnemyObject::ChangeState(wstring name) {
		if (name == L"ToCannon") {
			m_StateMachine->ChangeState(EnemyToCannonState::Instance());
			return;
		}
		else if (name == L"Damage") {
			m_StateMachine->ChangeState(EnemyDamageState::Instance());
			return;
		}
		else if (name == L"Waiting") {
			m_StateMachine->ChangeState(EnemyWaitingState::Instance());
			return;
		}
	}

	void EnemyObject::Spawn() {
		AddTag(L"Zako");
		m_isDead = false;
		m_Bomb = false;
		m_HP = m_DefaultHP;
		m_FrameCount = 0.0f;
		m_ShootNumber = 0;
		m_Rigidbody->m_CollType = CollType::typeSPHERE;

		if (FindTag(L"Blue")) {
			//メッシュとトランスフォームの差分の設定
			m_MeshToTransformMatrix.affineTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);
		}
	}

	bool EnemyObject::Attack1ExcuteBehavior() {
		m_Attack1ToRot += 0.1f;
		if (m_Attack1ToRot >= (XM_PI + 0.5f)) {
			m_Attack1ToRot = 0.0f;
			return true;
		}
		//ローカル行列の定義
		Vec3 Pos(0, sin(m_Attack1ToRot) * 0.5f, -cos(m_Attack1ToRot) * 0.5f);
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(Vec3(1.0, 0, 0), m_Attack1ToRot),
			Pos
		);
		return false;
	}

	void EnemyObject::BulletStartBehavior() {
		m_FrameCount = 0.0f;
		AddTag(L"Shooted");

		if (FindTag(L"Green")) {
			auto PtrCannon = GetStage()->FindTagGameObject<Cannon>(L"GREEN_CANNON");
			Vec3 new_pos = PtrCannon->GetPosition();
			new_pos.y += 1.0f;
			SetPosition(new_pos);
		}
		if (FindTag(L"Red")) {
			auto PtrCannon = GetStage()->FindTagGameObject<Cannon>(L"RED_CANNON");
			Vec3 new_pos = PtrCannon->GetPosition();
			new_pos.y += 1.0f;
			SetPosition(new_pos);
		}
		if (FindTag(L"Blue")) {
			auto PtrCannon = GetStage()->FindTagGameObject<Cannon>(L"BLUE_CANNON");
			Vec3 new_pos = PtrCannon->GetPosition();
			new_pos.y += 1.0f;
			SetPosition(new_pos);
		}

		m_Rigidbody->m_Velocity = Vec3(0);

		m_BossPos = GetStage()->FindTagGameObject<Boss>(L"BossEnemy")->GetPosition();
		m_ToBossVec = m_BossPos - m_Rigidbody->m_Pos;
		m_ToBossVec.normalize();
	}

	void EnemyObject::BulletExcuteBehavior() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (m_FrameCount > m_ShootNumber) {
			if (m_ShootNumber == 1 && m_Bomb == false && m_FrameCount >= 2.5f) {
				//Sparkの送出
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.z += 5.0f;
				//Sparkの送出
				auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
				SparkPtr->InsertFire(Emitter, 6.0f);

				m_CannonSound->Start(0, 1.0f);
				m_Bomb = true;
			}
			m_Rigidbody->m_Velocity = m_ToBossVec * 15.0f;

			//ボスとの衝突判定
			float length = (m_BossPos - m_Rigidbody->m_Pos).length();
			if (length < 5.0f) {
				Vec3 Emitter = m_Rigidbody->m_Pos;
				//Sparkの送出
				auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
				SparkPtr->InsertFire(Emitter, 8.0f);

				m_BossDamageSound->Start(0, 0.8f);

				auto BossPtr = GetStage()->FindTagGameObject<Boss>(L"BossEnemy");
				BossPtr->Damage(1.0f);
				if (BossPtr->GetHP() >= 0.0f) {
					BossPtr->ChangeAnimation(L"Damage");
				}

				//SetPosition(Vec3(0, 0, 70));

				m_StateMachine->ChangeState(EnemyWaitingState::Instance());
				return;
			}
		}
		RotateToVelocity();
		//エフェクトの再生
		auto FirePtr = GetStage<GameStage>()->FindTagGameObject<ShootingEffect>(L"ShootingEffect");
		Vec3 Emitter = m_Rigidbody->m_Pos;
		FirePtr->InsertEffect(Emitter);

		m_FrameCount += ElapsedTime * 5.0f;
	}

	//--------------------------------------------------------------------------------------
	///	敵対ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyOppositionState)
	void EnemyOppositionState::Enter(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	void EnemyOppositionState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->OppositionBehavior();
		Obj->CollisionBullet();
	}

	void EnemyOppositionState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	攻撃後の硬直ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyAttackEndState)
	void EnemyAttackEndState::Enter(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	void EnemyAttackEndState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->AttackEndBehavior();
		Obj->CollisionBullet();
	}

	void EnemyAttackEndState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	大砲に向かうステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyToCannonState)
	void EnemyToCannonState::Enter(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	void EnemyToCannonState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->ToCannonBehavior();
		Obj->CollisionBullet();
	}

	void EnemyToCannonState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	追従ステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyComplianceState)

	void EnemyComplianceState::Enter(const shared_ptr<EnemyObject>& Obj) {
		Obj->ComplianceStartBehavior();
		Obj->RemoveTag(L"Zako");
		Obj->AddTag(L"Chain");
	}

	void EnemyComplianceState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->UpdateBehavior();
	}

	void EnemyComplianceState::Exit(const shared_ptr<EnemyObject>& Obj) {
		Obj->RemoveTag(L"Chain");
	}

	//--------------------------------------------------------------------------------------
	///	大砲で撃たれてる時のステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyBulletState)

	void EnemyBulletState::Enter(const shared_ptr<EnemyObject>& Obj) {
		Obj->BulletStartBehavior();
	}

	void EnemyBulletState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->BulletExcuteBehavior();
	}

	void EnemyBulletState::Exit(const shared_ptr<EnemyObject>& Obj) {
		Obj->RemoveTag(L"Shooted");
	}

	//--------------------------------------------------------------------------------------
	///	チェインをぶん回すステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyAttack1State)

	void EnemyAttack1State::Enter(const shared_ptr<EnemyObject>& Obj) {
		Obj->Attack1StartBehavior();
	}

	void EnemyAttack1State::Execute(const shared_ptr<EnemyObject>& Obj) {
		if (Obj->Attack1ExcuteBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyComplianceState::Instance());
			return;
		}
		Obj->UpdateBehavior();
	}

	void EnemyAttack1State::Exit(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	ダメージを受けたときのステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDamageState)

	void EnemyDamageState::Enter(const shared_ptr<EnemyObject>& Obj) {
		
	}

	void EnemyDamageState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->DamageBehaviour();
	}

	void EnemyDamageState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	スポーン街のステート（EnemyObject）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyWaitingState)

	void EnemyWaitingState::Enter(const shared_ptr<EnemyObject>& Obj) {
		Obj->WaitingStartBehaviour();
	}

	void EnemyWaitingState::Execute(const shared_ptr<EnemyObject>& Obj) {
		
	}

	void EnemyWaitingState::Exit(const shared_ptr<EnemyObject>& Obj) {
		Obj->WaitingEndBehaviour();
		Obj->Spawn();
	}

	//--------------------------------------------------------------------------------------
	///	ニードルエネミー（近接）
	//--------------------------------------------------------------------------------------

	NeedleEnemy::NeedleEnemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& ParentPtr, 
		const wstring& MeshResName,
		const wstring & TextureResName, const wstring& DefaultAnimation, const Vec3 & Scale,
		const Quat & Qt, const Vec3 & Pos, bool OwnShadowActive) :
		EnemyObject(StagePtr, ParentPtr,MeshResName ,TextureResName, DefaultAnimation, Scale,Qt,Pos, OwnShadowActive)
	{
		AddTag(L"Red");
		AddTag(L"Zako");
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.1f, 1.1f, 1.1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.2f, 0.0f)
		);
		m_AfterAttackTime = 1.5f;
	}

	NeedleEnemy::~NeedleEnemy()
	{
	}

	void NeedleEnemy::UpdateBehavior() {
		FriendsBehavior();
		//敵との当たり判定
		vector<shared_ptr<GameObject>> EnemyVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Zako", EnemyVec);
		for (auto enemy : EnemyVec) {
			if (enemy) {

				auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);

				Vec3 EnemyPos = PtrEnemy->GetPosition();
				float length = (EnemyPos - m_Rigidbody->m_Pos).length();

				float EnemyRadius = PtrEnemy->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x;

				if (length <= EnemyRadius + PlayerRadius) {
					if (PtrEnemy->GetHP() > 0) {
						Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
						float p_dis = (p_pos - GetPosition()).length();
						if (p_dis < 1.0f) {
							p_dis = 1.0f;
						}
						//サウンドの発行
						m_DeadSound->Start(0, 1.0f / p_dis);

						Vec3 Emitter = PtrEnemy->GetPosition();
						//Fireの送出
						auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
						SparkPtr->InsertFire(Emitter, 1.0f);
					
						PtrEnemy->ChangeState(L"Waiting");
						return;
					}
				}
			}
		}
		//弾との衝突判定
		vector<shared_ptr<GameObject>> BulletVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Bullet", BulletVec);
		for (auto bullet : BulletVec) {
			if (bullet) {
				auto PtrBullet = dynamic_pointer_cast<BulletObject>(bullet);

				Vec3 BulletPos = PtrBullet->GetPosition();
				float length = (BulletPos - m_Rigidbody->m_Pos).length();

				float Radius = PtrBullet->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

				if (length < Radius + PlayerRadius) {
					PtrBullet->SetPosition(Vec3(0, -100, 0));
					Vec3 Emitter = m_Rigidbody->m_Pos;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);

					return;
				}
			}
		}
		//ボスハンドとの衝突判定
		vector<shared_ptr<GameObject>> HandVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"BossHand", HandVec);
		for (auto hand : HandVec) {
			if (hand) {
				auto PtrHand = dynamic_pointer_cast<BossHand>(hand);

				Vec3 HandPos = PtrHand->GetPosition();
				float length = (HandPos - m_Rigidbody->m_Pos).length();

				float Radius = PtrHand->GetScale() / 2.0f;
				float PlayerRadius = m_Rigidbody->m_Scale.x / 2.0f;

				if (length < Radius + PlayerRadius) {

					auto PtrBoss = GetStage()->FindTagGameObject<EnemyObject>(L"HandBoss");
				
					Vec3 Emitter = m_Rigidbody->m_Pos;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);

					PtrBoss->Damage(0.1f);
					PtrBoss->ChangeState(L"Damage");
					if (PtrBoss->GetHP() <= 0.0f) {
						Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
						float p_dis = (p_pos - GetPosition()).length();
						if (p_dis < 1.0f) {
							p_dis = 1.0f;
						}
						
						m_CannonSound->Start(0, 1.0f / p_dis);
						
						Vec3 Emitter = PtrBoss->GetPosition();
						//Fireの送出
						auto FirePtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
						FirePtr->InsertFire(Emitter, PtrBoss->GetScale() * 3.0f);
						
						PtrBoss->ChangeState(L"Waiting");
					}
					//ノックバック方向の設定
					return;
				}
			}
		}
	}

	void NeedleEnemy::OnUpdate() {
		EnemyObject::OnUpdate();
	}

	//--------------------------------------------------------------------------------------
	///	銃をうつエネミー（遠距離）
	//--------------------------------------------------------------------------------------
	ShootEnemy::ShootEnemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& ParentPtr, 
		const wstring& MeshResName,
		const wstring & TextureResName, const wstring& DefaultAnimation,
		const Vec3 & Scale, const Quat & Qt, 
		const Vec3 & Pos, bool OwnShadowActive):
		EnemyObject(StagePtr, ParentPtr, MeshResName,TextureResName, DefaultAnimation, Scale, Qt, Pos, OwnShadowActive)
	{
		m_Speed = 0.3f;
		m_SearchDis = 5.0;
		m_EnemyShootSpeed = 1.2f;
		m_PlayerShootSpeed = 5.0f;
		m_PlayerShootTime = 1.8f;
		AddTag(L"Blue");
		AddTag(L"Zako");
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
	}

	ShootEnemy::~ShootEnemy()
	{
	}

	void ShootEnemy::OnCreate() {
		EnemyObject::OnCreate();
		m_ShootSound = ObjectFactory::Create<SoundObject>(L"shoot");
	}

	void ShootEnemy::OppositionBehavior() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		if (shptr) {
			Vec3 toPos = shptr->GetPosition();
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//距離を求める
			float dis = ToPosVec.length();

			//移動する処理
			ToPosVec.normalize();
			ToPosVec *= m_Speed;
			if (m_UpdateActive == false) {
				ToPosVec *= 0.0f;
			}
			m_Rigidbody->m_Velocity = ToPosVec;
			m_Rigidbody->m_Pos.y = m_BaseY;

			if (m_UpdateActive) {
				if (m_FrameCount > m_StopTime) {
					//球を飛ばす処理
					vector<shared_ptr<GameObject>> ShootVec;
					GetStage<GameStage>()->FindTagGameObjectVec(L"Bullet", ShootVec);
					for (auto v : ShootVec) {
						if (v) {
							auto Ptr = dynamic_pointer_cast<BulletObject>(v);
							bool nowShooting = Ptr->GetIsShoot();
							if (nowShooting == false)
							{
								m_PtrObj->ChangeCurrentAnimation(L"Attack");
								Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
								float p_dis = (p_pos - GetPosition()).length();
								if (p_dis < 1.0f) {
									p_dis = 1.0f;
								}
								//サウンドの発行
								m_ShootSound->Start(0, 0.1f / p_dis);
								ToPosVec.y = 0.0f;
								Vec3 ShootPos = m_Rigidbody->m_Pos + ToPosVec * 1.3f;
								ShootPos.y += 0.1f;
								Ptr->Wakeup(ShootPos, ToPosVec.normalize() * m_EnemyShootSpeed);
								m_FrameCount = 0.0f;
								m_Rigidbody->m_Velocity *= 0.01f;
								m_StateMachine->ChangeState(EnemyAttackEndState::Instance());
								return;
							}
						}
					}
				}
				//遅くする
				if (m_FrameCount > 0.0f) {
					m_FrameCount += ElapsedTime;
					m_Rigidbody->m_Velocity *= 0.01f;
				}
				// プレイヤーとエネミーの距離が近くなった時の処理
				else if (dis <= m_SearchDis)
				{
					if (dis < 1.0f) {
						dis = 1.0f;
					}
					m_EyeFlashSound->Start(0, 0.2f / dis);
					m_FrameCount += ElapsedTime;
					//サインの送出
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<AttackSigns>(L"AttackSigns");
					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y += 0.15f;
					FirePtr->InsertSigns(Emitter);
				}
			}
			RotateToVelocity();
		}
	}

	void ShootEnemy::OnUpdate() {
		EnemyObject::OnUpdate();
	}

	void ShootEnemy::UpdateBehavior() {
		FriendsBehavior();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		if (shptr) {
			//プレイヤー方向への二つのベクトルを作り、そこから外積を作って側面に弾を飛ばすようにする
			Vec3 toPos = shptr->GetPosition();
			Vec3 toPos2 = toPos;

			toPos.y = 1.0f;
			toPos2.y = -1.0f;

			Vec3 force1 = toPos - m_Rigidbody->m_Pos;
			Vec3 force2 = toPos2 - m_Rigidbody->m_Pos;

			Vec3 force = cross(force1, force2);
			force.y = 0;
			force.normalize();
			
			//回転の向きは90度
			float angle = -XM_PIDIV2;

			//常にステージの奥に向かって弾を撃つようにする
			if (force.z < 0)
			{
				force.z *= -1.0f;
				force.x *= -1.0f;
				angle *= -1.0f;
			}			

			//メッシュとトランスフォームの差分の設定
			m_MeshToTransformMatrix.affineTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, angle, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

			if (m_FrameCount > m_PlayerShootTime) {
				vector<shared_ptr<GameObject>> ShootVec;
				GetStage<GameStage>()->FindTagGameObjectVec(L"PlayerBullet", ShootVec);
				for (auto v : ShootVec) {
					if (v) {
						auto Ptr = dynamic_pointer_cast<BulletObject>(v);
						bool nowShooting = Ptr->GetIsShoot();
						if (nowShooting == false)
						{
							m_PtrObj->ChangeCurrentAnimation(L"Attack");
							Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
							float p_dis = (p_pos - GetPosition()).length();
							if (p_dis < 1.0f) {
								p_dis = 1.0f;
							}
							//サウンドの発行
							m_ShootSound->Start(0, 0.1f / p_dis);
							Vec3 ShootPos = m_Rigidbody->m_Pos + force * 0.1f;
							ShootPos.y += 0.1f;
							Ptr->Wakeup(ShootPos, force * m_PlayerShootSpeed);
							m_FrameCount = 0.0f;
							break;
						}
					}
				}
			}
		}
		m_FrameCount += ElapsedTime;
	}

	void ShootEnemy::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///	ShootEnemyの撃ち出す弾クラス
	//--------------------------------------------------------------------------------------

	BulletObject::BulletObject(const shared_ptr<Stage>& StagePtr,
		const wstring & TextureResName,
		const Vec3 & Scale,
		const Quat & Qt,
		const Vec3 & Pos, bool OwnShadowActive,
		const wstring & Tag) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive),
		m_my_Tag(Tag),
		m_FrameCount(0.0f),
		m_BulletTime(15.0f),
		IsShoot(false)
	{
	}
	BulletObject::~BulletObject()
	{														
	}
	void BulletObject::OnCreate()
	{
		AddTag(m_my_Tag);

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = true;
		body.m_IsFixed = true;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");

		//行列の定義
		Mat4x4 World;
		World.affineTransformation
		(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//ターゲット座標の初期化
		m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;
		if (m_my_Tag == L"Bullet") {
			m_PtrObj->m_Diffuse = Col4(0.2f, 0.2f, 0.2f, 1.0f);
			m_PtrObj->m_Emissive = Col4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else {

			m_PtrObj->m_Diffuse = Col4(0.0f, 0.3f, 1.0f, 1.0f);
			m_PtrObj->m_Emissive = Col4(0.0f, 0.3f, 1.0f, 1.0f);
		}

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
	}
	void BulletObject::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (m_FrameCount > m_BulletTime)
		{
			m_FrameCount = 0.0f;
			IsShoot = false;
		}
		if (IsShoot == true)
		{
			m_FrameCount += ElapsedTime;
		}

	}
	void BulletObject::OnDrawShadowmap()
	{
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}
	void BulletObject::OnDraw()
	{
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	void BulletObject::GetWorldMatrix(Mat4x4& m) const {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m = World;
	}

	Vec3 BulletObject::GetPosition() 
	{
		return m_Rigidbody->m_Pos;
	}

	void BulletObject::SetPosition(Vec3 pos)
	{
		m_Rigidbody->m_Pos = pos;	
	}

	void BulletObject::Wakeup(const Vec3 & Position, const Vec3 & Velocity)
	{
		SetPosition(Position);
		m_Rigidbody->m_Velocity = Velocity;
		IsShoot = true;
	}

	//--------------------------------------------------------------------------------------
	/// エンジェルエネミー
	//--------------------------------------------------------------------------------------
	AngelEnemy::AngelEnemy(const shared_ptr<Stage>& StagePtr, 
		const shared_ptr<GameObject>& ParentPtr, const wstring & MeshResName, 
		const wstring & TextureResName, const wstring& DefaultAnimation, const Vec3 & Scale, const Quat & Qt,
		const Vec3 & Pos, bool OwnShadowActive) :
		EnemyObject(StagePtr, ParentPtr, MeshResName, TextureResName, DefaultAnimation, Scale, Qt, Pos, OwnShadowActive)
	{
		m_Speed = 0.6f;
		m_SearchDis = 2.5f;
		m_TackleSpeed = 2.0f;
		m_TackleTime = 1.7f;
		m_AfterAttackTime = 1.2f;
		AddTag(L"Green");
		AddTag(L"Zako");
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.25f, 0.0f)
		);
	}

	AngelEnemy::~AngelEnemy()
	{
	}
	
	//--------------------------------------------------------------------------------------
	/// 中ボスエネミー近距離
	//--------------------------------------------------------------------------------------
	CR_BossEnemy::CR_BossEnemy(const shared_ptr<Stage>& StagePtr, 
		const shared_ptr<GameObject>& ParentPtr, const wstring & MeshResName, 
		const wstring & TextureResName, const wstring& DefaultAnimation, const Vec3 & Scale, const Quat & Qt,
		const Vec3 & Pos, bool OwnShadowActive) :
		EnemyObject(StagePtr, ParentPtr, MeshResName, TextureResName, DefaultAnimation, Scale, Qt, Pos, OwnShadowActive)
	{
		m_Speed = 1.5f;
		m_HP = 20.0f;
		m_DefaultHP = m_HP;
		m_TackleSpeed = 0.0f;
		
		AddTag(L"HandBoss");
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(0.8f, 0.8f, 0.8f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);
	}

	CR_BossEnemy::~CR_BossEnemy()
	{
	}

	void CR_BossEnemy::OnCreate() {
		//タグの追加
		AddTag(L"EnemyObject");

		m_EyeFlashSound = ObjectFactory::Create<SoundObject>(L"eye_flash");

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = true;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		m_isDead = false;
		m_UpdateActive = true;

		m_PlayerPtr = GetStage()->FindTagGameObject<Player>(L"Player");

		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(m_MeshResName);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//ターゲット座標の初期化
		m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);

		m_BaseY = m_Rigidbody->m_Scale.y / 2.0f + 0.6f;

		//サウンドオブジェクトの初期化
		m_DeadSound = ObjectFactory::Create<SoundObject>(L"Pan");
		m_BossDamageSound = ObjectFactory::Create<SoundObject>(L"bossdamage");
		m_FriendsSound = ObjectFactory::Create<SoundObject>(L"nakama");
		m_CannonSound = ObjectFactory::Create<SoundObject>(L"cannon");
		m_EyeFlashSound = ObjectFactory::Create<SoundObject>(L"eye_flash");

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);

		//描画データの構築
		m_SimpleObj = make_shared<BcDrawObject>();
		m_SimpleObj->m_MeshRes = MeshPtr;
		m_SimpleObj->m_TextureRes = TexPtr;
		m_SimpleObj->m_WorldMatrix = World;
		m_SimpleObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_SimpleObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_SimpleObj->m_ShadowmapUse = true;
		m_SimpleObj->m_FogEnabled = true;
		m_SimpleObj->m_FogColor = Col4(0.07f, 0.0f, 0.09f, 1.0f);
		m_SimpleObj->m_FogStart = -10.0f;
		m_SimpleObj->m_FogEnd = -100.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<EnemyObject>(GetThis<EnemyObject>()));
		//ステート初期値設定
		m_StateMachine->ChangeState(EnemyOppositionState::Instance());

		vector<shared_ptr<GameObject>> CannonVec;
		GetStage<GameStage>()->FindTagGameObjectVec(L"Cannon", CannonVec);
		int num = rand() % 3;
		for (auto cannon : CannonVec)
		{
			if (cannon)
			{
				auto Ptrcannon = dynamic_pointer_cast<Cannon>(cannon);
				if (num == Ptrcannon->GetCannonClass()) {
					m_CannonPos = Ptrcannon->GetPosition();
					break;
				}
			}
		}
	}

	void CR_BossEnemy::OppositionBehavior() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		//親のワールド行列を取得する変数
		Mat4x4 ParMat;
		if (shptr) {
			//行列取得用のインターフェイスを持ってるかどうか
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
			}

			Mat4x4 World;
			World.identity();
			//行列の定義
			World = m_PlayerLocalMatrix;
			//スケーリングを1.0にした行列に変換
			ParMat.scaleIdentity();
			//行列の反映
			World *= ParMat;
			//この時点でWorldは目標となる位置
			Vec3 toPos = World.transInMatrix();
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//距離を求める
			float dis = ToPosVec.length();

			// 突進してしばらくたったら
			if (m_FrameCount > m_StopTime + m_TackleTime)
			{
				m_Tackle = false;
				m_FrameCount = 0.0f;
				m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);
				m_Rigidbody->m_Velocity *= 0.01f;
				m_StateMachine->ChangeState(EnemyAttackEndState::Instance());
				return;
			}
			// 止まりはじめ
			else if (m_FrameCount > m_StopTime && m_Tackle == false)
			{
				m_Tackle = true;
				if (m_TargetPos == Vec3(0.0f, 0.0f, 0.0f)) {
					m_TargetPos = toPos;
					m_TackleStartPos = m_Rigidbody->m_Pos;
				}
			}

			// 突進の処理
			if (m_Tackle == true && m_UpdateActive)
			{
				m_FrameCount += ElapsedTime;
			}
			// エネミー移動処理
			else if (m_Tackle == false)
			{
				if (m_FrameCount > 0.0f && m_UpdateActive)
				{
					ToPosVec.normalize();
					ToPosVec *= 0.01f;
					m_Rigidbody->m_Velocity = ToPosVec;
					m_Rigidbody->m_Pos.y = m_BaseY;
					m_FrameCount += ElapsedTime;
				}
				// プレイヤーとエネミーの距離が近くなった時の処理
				else if (dis <= m_SearchDis && m_UpdateActive)
				{
					m_FrameCount += ElapsedTime;
					//fireの送出
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<BossAttackSigns>(L"BossAttackSigns");
					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y += 0.35f;
					FirePtr->InsertSigns(Emitter);

					if (dis < 1.0f) {
						dis = 1.0f;
					}
					m_EyeFlashSound->Start(0, 0.2f / dis);

					vector<shared_ptr<GameObject>> HandVec;
					GetStage<GameStage>()->FindTagGameObjectVec(L"BossHand", HandVec);
					for (auto hand : HandVec) {
						if (hand) {
							auto PtrHand = dynamic_pointer_cast<BossHand>(hand);
							PtrHand->SetState(L"Attack");
						}
					}
				}
				// プレイヤーに向かう処理
				else
				{
					ToPosVec.normalize();
					ToPosVec *= m_Speed;
					if (!m_UpdateActive) {
						ToPosVec *= 0.0f;
					}
					m_Rigidbody->m_Velocity = ToPosVec;
					m_Rigidbody->m_Pos.y = m_BaseY;
				}
			}
		}
		RotateToVelocity();
	}

	void CR_BossEnemy::OnUpdate() {
		//ステートマシン更新
		m_StateMachine->Update();
	}

	void CR_BossEnemy::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_SimpleObj->m_WorldMatrix = World;
		m_SimpleObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_StaticRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_StaticRenderer = shptr;
		}
		shptr->AddDrawObject(m_SimpleObj);
	}


	//--------------------------------------------------------------------------------------
	/// ボスの手
	//--------------------------------------------------------------------------------------
	BossHand::BossHand(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<GameObject>& ParentPtr,
		const wstring& TextureResName, const wstring& TagName,
		const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_ParentPtr(ParentPtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive),
		m_BeforeAttackTime(1.0f),
		m_AttackSetupTime(1.0f),
		m_AttackTime(1.5f),
		m_ReturnDefaultTime(0.5f),
		m_LerpToParent(0.2f)
	{
		//タグの追加
		AddTag(TagName);
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(3.0f, 1.5f, 3.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);
		m_FrameCount = 0.0f;
	}
	BossHand::~BossHand() {}

	void BossHand::OnCreate() {	
		AddTag(L"BossHand");

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = false;
		body.m_IsFixed = false;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);


		m_UpdateActive = true;

		//メッシュの取得
		shared_ptr<MeshResource> MeshPtr;

		if (FindTag(L"LeftHand")) {
			MeshPtr = App::GetApp()->GetResource<MeshResource>(L"LEFT_HAND_MESH");
			m_DefaultPos = Vec3(-1.1f, -0.5f, 1.0f);
			m_AttackPos = Vec3(-0.3f, 0.3f, 2.5f);
		}
		else {
			MeshPtr = App::GetApp()->GetResource<MeshResource>(L"RIGHT_HAND_MESH");
			m_DefaultPos = Vec3(1.1f, -0.5f, 1.0f);
			m_AttackPos = Vec3(0.3f, 0.3f, 2.5f);
		}

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = false;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;
		m_PtrObj->m_FogEnabled = true;
		m_PtrObj->m_FogColor = Col4(0.07f, 0.0f, 0.09f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -100.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<BossHand>(GetThis<BossHand>()));
		//ステート初期値設定
		m_StateMachine->ChangeState(HandDefaultState::Instance());

	}

	void BossHand::OnUpdate() {
		//ステートマシン更新
		m_StateMachine->Update();
	}

	void BossHand::OnUpdate2() {
	}

	void BossHand::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void BossHand::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	void BossHand::GetWorldMatrix(Mat4x4& m) const {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		m = World;
	}

	void BossHand::UpdateBehavior() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_ParentPtr.lock();
		//親のワールド行列を取得する変数
		Mat4x4 ParMat;
		if (shptr) {
			ParentFlg flg = ParentFlg::NoParent;
			//行列取得用のインターフェイスを持ってるかどうか
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
			}
			Mat4x4 World;
			World.identity();
			float LerpNum = 0.2f;

			World = m_LocalMatrix;
			LerpNum = m_LerpToParent;

			//スケーリングを1.0にした行列に変換
			ParMat.scaleIdentity();
			//行列の反映
			World *= ParMat;
			//この時点でWorldは目標となる位置
			Vec3 toPos = World.transInMatrix();
			Vec3 DammiPos;
			World.decompose(m_Rigidbody->m_Scale, m_Rigidbody->m_Quat, DammiPos);
			Vec3 Velo = toPos - m_Rigidbody->m_Pos;
			Velo /= ElapsedTime;
			m_Rigidbody->m_Velocity = Velo;
		}
	}

	void BossHand::ComplianceStartBehavior() {
		//ローカル行列の定義
		if (FindTag(L"LeftHand")) {
			m_LocalMatrix.affineTransformation(
				m_Rigidbody->m_Scale,
				Vec3(0, 0, 0),
				Quat(),
				Vec3(-1.1f, -0.5f, 1.0f)
			);
		}
		else {
			m_LocalMatrix.affineTransformation(
				m_Rigidbody->m_Scale,
				Vec3(0, 0, 0),
				Quat(),
				Vec3(1.1f, -0.5f, 1.0f)
			);
		}
		m_LerpToParent = 0.2f;
	}

	//攻撃１行動の開始
	void BossHand::Attack1StartBehavior() {
	}

	bool BossHand::Attack1ExcuteBehavior() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (m_UpdateActive) {
			if (m_FrameCount > m_AttackSetupTime + m_BeforeAttackTime + m_AttackTime + m_ReturnDefaultTime) {
				m_FrameCount = 0.0f;
				return true;
			}
			if (m_FrameCount > m_AttackSetupTime + m_BeforeAttackTime + m_AttackTime) {
				//ローカル行列の定義
				Vec3 NewPos = (m_DefaultPos - m_BeforePos) * ((m_FrameCount - m_AttackSetupTime - m_BeforeAttackTime - m_AttackTime) / m_ReturnDefaultTime);
				m_LocalMatrix.affineTransformation(
					m_Rigidbody->m_Scale,
					Vec3(0, 0, 0),
					Quat(),
					m_BeforePos + NewPos
				);
			}
			else if (m_FrameCount > m_AttackSetupTime + m_BeforeAttackTime) {
				if (m_Rigidbody->m_Pos.y < m_Rigidbody->m_Scale.y / 2.0f) {

				}
				else {
					//ローカル行列の定義
					Vec3 NewPos = m_BeforePos;
					NewPos.y -= 0.1f;
					m_LocalMatrix.affineTransformation(
						m_Rigidbody->m_Scale,
						Vec3(0, 0, 0),
						Quat(),
						NewPos
					);
					m_BeforePos = NewPos;
				}
			}
			else if (m_FrameCount < m_AttackSetupTime) {
				//ローカル行列の定義
				Vec3 NewPos = (m_AttackPos - m_DefaultPos) * (m_FrameCount / m_AttackSetupTime);
				m_LocalMatrix.affineTransformation(
					m_Rigidbody->m_Scale,
					Vec3(0, 0, 0),
					Quat(),
					m_DefaultPos + NewPos
				);
				m_BeforePos = m_DefaultPos + NewPos;
			}
			m_FrameCount += ElapsedTime;
		}
		return false;
	}

	void BossHand::SetState(wstring state_name) {
		if (state_name == L"Attack") {
			m_StateMachine->ChangeState(HandAttackState::Instance());
		}
		else if (state_name == L"Default") {
			m_StateMachine->ChangeState(HandDefaultState::Instance());
		}
	}


	//--------------------------------------------------------------------------------------
	///	追従ステート（BossHand）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(HandDefaultState)

	void HandDefaultState::Enter(const shared_ptr<BossHand>& Obj) {
		Obj->ComplianceStartBehavior();
		//何もしない
	}

	void HandDefaultState::Execute(const shared_ptr<BossHand>& Obj) {
		Obj->UpdateBehavior();
	}

	void HandDefaultState::Exit(const shared_ptr<BossHand>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	攻撃ステート１（BossHand）
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(HandAttackState)

	void HandAttackState::Enter(const shared_ptr<BossHand>& Obj) {
		Obj->Attack1StartBehavior();
	}

	void HandAttackState::Execute(const shared_ptr<BossHand>& Obj) {
		if (Obj->Attack1ExcuteBehavior()) {
			Obj->GetStateMachine()->ChangeState(HandDefaultState::Instance());
			return;
		}
		Obj->UpdateBehavior();
	}

	void HandAttackState::Exit(const shared_ptr<BossHand>& Obj) {

	}

	//--------------------------------------------------------------------------------------
	/// 中ボスエネミー遠距離
	//--------------------------------------------------------------------------------------
	LD_BossEnemy::LD_BossEnemy(const shared_ptr<Stage>& StagePtr, 
		const shared_ptr<GameObject>& ParentPtr, const wstring & MeshResName, 
		const wstring & TextureResName, const wstring& DefaultAnimation, const Vec3 & Scale, const Quat & Qt,
		const Vec3 & Pos, bool OwnShadowActive) :
		EnemyObject(StagePtr, ParentPtr, MeshResName, TextureResName,DefaultAnimation, Scale, Qt, Pos, OwnShadowActive)
	{
		m_Speed = 1.0f;
		m_HP = 15.0f;
		m_DefaultHP = m_HP;
		AddTag(L"SawBoss");
		m_TackleSpeed = 1.3f;
		m_SearchDis = 6.0f;
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(0.8f, 3.0f, 0.8f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);
	}

	LD_BossEnemy::~LD_BossEnemy()
	{
	}

	//--------------------------------------------------------------------------------------
	/// 敵のボス
	//--------------------------------------------------------------------------------------

	Boss::Boss(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive)
	{}
	Boss::~Boss() {}

	void Boss::OnCreate() {
		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"BOSS_MESH");

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(0.3f, 0.3f, 0.3f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.7f, 0.0f)
		);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		m_frame_count = 0.0f;
		m_DamageRate = 0.0f;
		m_now_barrior = 0;

		m_HP = 75.0f;
		m_DefaultHP = m_HP;
		AddTag(L"BossEnemy");

		m_isDead = false;
		m_isDamage = false;

		auto GM = GameManager::getInstance();
		GM->SetDefaultBossHP(m_DefaultHP);

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsCollisionActive = true;
		//		body.m_IsDrawActive = true;
		body.SetToBefore();
		m_Rigidbody = PtrGameStage->AddRigidbody(body);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 30, true, 30.0f);
		m_PtrObj->AddAnimation(L"Damage", 30, 90, false, 60.0f);
		m_PtrObj->AddAnimation(L"Dead", 90, 170, false, 60.0f);
		m_PtrObj->ChangeCurrentAnimation(L"Default");

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void Boss::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);



		if (m_HP <= 0.0f && m_isDead == false) {
			ChangeAnimation(L"Dead");
			m_isDead = true;
			m_frame_count = 0.0f;
		}
		else if (m_isDead) {
			if (m_frame_count >= 2.0f) {
				GetStage<GameStage>()->SetIsClear(true);
				m_isDead = false;
				m_Rigidbody->m_Pos = Vec3(100, 100, 100);
				m_HP = 0.0001f;
			}
			m_frame_count += ElapsedTime;
			return;
		}
		auto GM = GameManager::getInstance();
		GM->SetBossHP(m_HP);


		if (m_isDamage) {
			m_frame_count += ElapsedTime;
		}
		if (m_frame_count >= 2.0f && m_isDamage == true) {
			m_isDamage = false;
			m_frame_count = 0.0f;
			m_DamageRate = 0.0f;
		}



		if (m_HP > 0.0f) {
			auto player = GetStage()->FindTagGameObject<Player>(L"Player");
			int now_num = player->GetIsCannon();
			if (now_num == 3) {
				m_frame_count += ElapsedTime;
			}
			if (m_frame_count >= 20.0f && m_now_barrior == 0) {
				m_now_barrior = 1;
				m_frame_count = 0.0f;
			}
			if (m_frame_count >= 20.0f && m_now_barrior == 1) {
				m_now_barrior = 2;
				m_frame_count = 0.0f;
			}
			if (m_frame_count >= 20.0f && m_now_barrior == 2) {
				m_now_barrior = 0;
				m_frame_count = 0.0f;
			}
			
			Vec3 Emitter = m_Rigidbody->m_Pos;
			Emitter.x -= 3.0f;
			Emitter.y -= 3.0f;
			//Sparkの送出
			auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<BossEffect>(L"BossEffect");
			if (m_now_barrior == 0) {
				SparkPtr->InsertSpark(Emitter, L"Green");
			}
			else if (m_now_barrior == 1) {
				SparkPtr->InsertSpark(Emitter, L"Red");
			}
			else if (m_now_barrior == 2) {
				SparkPtr->InsertSpark(Emitter, L"Blue");
			}
		}

	}

	void Boss::Damage(float value) {
		auto player = GetStage()->FindTagGameObject<Player>(L"Player");
		if (player->GetIsCannon() == m_now_barrior) {
			if (m_isDamage) {
				m_DamageRate += 0.75f;
				m_HP -= (value + m_DamageRate) * 1.7f;
			}
			else {
				m_HP -= value * 1.7f;
				SetIsDamage(true);
				m_DamageRate++;
			}
		}
		else {
			if (m_isDamage) {
				m_DamageRate += 0.75f;
				m_HP -= (value + m_DamageRate);
			}
			else {
				m_HP -= value;
				SetIsDamage(true);
				m_DamageRate++;
			}
		}
	}

	void Boss::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void Boss::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//差分を計算
		World = m_MeshToTransformMatrix * World;

		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	Vec3 Boss::GetPosition() {
		return m_Rigidbody->m_Pos;
	}

	void Boss::ChangeAnimation(wstring anim) {
		m_PtrObj->ChangeCurrentAnimation(anim);
	}

}
//end basecross
