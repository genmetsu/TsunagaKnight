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

		m_HP = 10.0f;
		AddTag(L"BossEnemy");

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

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void Boss::OnUpdate() {
		if (m_HP < 0.0f) {
			SetPosition(Vec3(100, 100, 100));
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
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	Vec3 Boss::GetPosition() {
		return m_Rigidbody->m_Pos;
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
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
				}
			}
		}
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

	void MultiFire::InsertFire(const Vec3& Pos) {
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
			rParticleSprite.m_LocalScale *= 5.0f;
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
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
		Vec3 MoveVec = Camera_Pos - Pos;
		MoveVec.normalize();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalScale = Vec2(0.05f, 0.05f);
			rParticleSprite.m_LocalPos += (MoveVec+Vec3(0,0.05f,0));
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
	{}
	void SpriteBase::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横指定数ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
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
		m_Rot += ElapsedTime;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}
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
		m_TackleSpeed(6.0f),
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

		m_isDead = false;

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
		for (auto cannon : CannonVec)
		{
			if (cannon)
			{
				auto Ptrcannon = dynamic_pointer_cast<Cannon>(cannon);
				m_CannonPos = Ptrcannon->GetPosition();
				return;
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

					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y -= 0.125f;
					//Sparkの送出
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
					SparkPtr->InsertSpark(Emitter);
					//弾を異次元に飛ばす
					PtrBullet->SetPosition(Vec3(0.0f, -10.0f, 0.0f));

					//体力を削る
					m_HP--;

					if (m_HP <= 0.0f) {
						
						//敵を異次元に飛ばす（仮倒し処理）
						SetPosition(Vec3(0, 0, 70));
						m_HP = 2.0f;
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
				m_Rigidbody->m_Velocity = Tag;
				m_FrameCount += ElapsedTime;
			}
			// エネミー移動処理
			else if (m_Tackle == false)
			{
				if (m_FrameCount > 0.0f)
				{
					ToPosVec.normalize();
					ToPosVec *= 0.01f;
					m_Rigidbody->m_Velocity = ToPosVec;
					m_Rigidbody->m_Pos.y = m_BaseY;
					m_FrameCount += ElapsedTime;
				}
				// プレイヤーとエネミーの距離が近くなった時の処理
				else if (dis <= m_SearchDis)
				{
					m_FrameCount += ElapsedTime;
					//fireの送出
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<AttackSigns>(L"AttackSigns");
					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y -= 0.125f;
					FirePtr->InsertSigns(Emitter);
				}
				// プレイヤーに向かう処理
				else
				{
					ToPosVec.normalize();
					ToPosVec *= m_Speed;
					m_Rigidbody->m_Velocity = ToPosVec;
					m_Rigidbody->m_Pos.y = m_BaseY;
				}
			}
		}
		Vec3 Temp = m_Rigidbody->m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
	}

	void EnemyObject::AttackEndBehavior() {
		if (m_FrameCount > m_AfterAttackTime) {
			m_FrameCount = 0.0f;
			m_StateMachine->ChangeState(EnemyOppositionState::Instance());
			return;
		}
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
				m_Rigidbody->m_Velocity = ToPosVec;
				m_Rigidbody->m_Pos.y = m_BaseY;
			}
		}
		Vec3 Temp = m_Rigidbody->m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
	}

	void EnemyObject::CheckHealth() {
		if (m_HP <= 0.0f) {
			if (m_isDead == false) {
				m_isDead = true;
				//m_Rigidbody->m_IsCollisionActive = false;
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

	void EnemyObject::BulletEndBehaviour() {
		AddTag(L"Zako");
		m_isDead = false;
		m_HP = 3.0f;
		m_Rigidbody->m_IsCollisionActive = true;
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

		if (FindTag(L"Green")) {
			auto PtrCannon = GetStage()->FindTagGameObject<Cannon>(L"GREEN_CANNON");
			Vec3 new_pos = PtrCannon->GetPosition();

			SetPosition(new_pos);
		}
		if (FindTag(L"Red")) {
			auto PtrCannon = GetStage()->FindTagGameObject<Cannon>(L"RED_CANNON");
			Vec3 new_pos = PtrCannon->GetPosition();
			SetPosition(new_pos);
		}
		if (FindTag(L"Blue")) {
			auto PtrCannon = GetStage()->FindTagGameObject<Cannon>(L"BLUE_CANNON");
			Vec3 new_pos = PtrCannon->GetPosition();
			SetPosition(new_pos);
		}

		m_Rigidbody->m_Velocity = Vec3(0);

		m_BossPos = GetStage()->FindTagGameObject<Boss>(L"BossEnemy")->GetPosition();
		m_ToBossVec = m_BossPos - m_Rigidbody->m_Pos;
		m_ToBossVec.normalize();
	}

	void EnemyObject::BulletExcuteBehavior() {

		m_Rigidbody->m_Velocity = m_ToBossVec * 15.0f;

		//ボスとの衝突判定
		float length = (m_BossPos - m_Rigidbody->m_Pos).length();
		if (length < 5.0f) {
			Vec3 Emitter = m_Rigidbody->m_Pos;
			Emitter.y -= 0.125f;
			//Sparkの送出
			auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
			SparkPtr->InsertFire(Emitter);
			SetPosition(Vec3(0, 0, 70));

			m_StateMachine->ChangeState(EnemyToCannonState::Instance());
			return;
		}
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
		Obj->BulletEndBehaviour();
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
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
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
		GetStage<GameStage>()->FindTagGameObjectVec(L"EnemyObject", EnemyVec);
		for (auto enemy : EnemyVec) {
			if (enemy) {
				if (!enemy->FindTag(L"LongBoss")) {
					auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);

					Vec3 EnemyPos = PtrEnemy->GetPosition();
					float length = (EnemyPos - m_Rigidbody->m_Pos).length();

					float EnemyRadius = PtrEnemy->GetScale() / 2.0f;
					float PlayerRadius = m_Rigidbody->m_Scale.x;

					if (length <= EnemyRadius + PlayerRadius) {
						if (PtrEnemy->GetHP() > 0) {
							Vec3 Emitter = m_Rigidbody->m_Pos;
							Emitter.y -= 0.125f;
							//Sparkの送出
							auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiSpark>(L"MultiSpark");
							SparkPtr->InsertSpark(Emitter);
							//敵を異次元に飛ばす（仮倒し処理）
							PtrEnemy->SetPosition(Vec3(0, 0, 70));
							return;
						}
					}
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
		m_EnemyShootSpeed = 2.0f;
		m_PlayerShootSpeed = 5.0f;
		m_PlayerShootTime = 1.0f;
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
			m_Rigidbody->m_Velocity = ToPosVec;
			m_Rigidbody->m_Pos.y = m_Scale.y / 2.0f;

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
							Ptr->Wakeup(m_Rigidbody->m_Pos + ToPosVec * 0.1f, ToPosVec.normalize() * m_EnemyShootSpeed);
							m_FrameCount = 0.0f;
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
				m_FrameCount += ElapsedTime;
				//サインの送出
				auto FirePtr = GetStage<GameStage>()->FindTagGameObject<AttackSigns>(L"AttackSigns");
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.y -= 0.125f;
				FirePtr->InsertSigns(Emitter);
			}

			//回転の処理
			Vec3 Temp = m_Rigidbody->m_Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在と目標を補間
			m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
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
							Ptr->Wakeup(force * 0.1f + m_Rigidbody->m_Pos, force * m_PlayerShootSpeed);
							m_FrameCount = 0.0f;
							return;
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
		m_BulletTime(20.0f),
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
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;
		if (m_my_Tag == L"Bullet") {
			m_PtrObj->m_Diffuse = Col4(0.2f, 0.2f, 0.2f, 1.0f);
			m_PtrObj->m_Emissive = Col4(0.2f, 0.2f, 0.2f, 1.0f);
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
		m_TackleSpeed = 4.0f;
		m_TackleTime = 1.7f;
		m_AfterAttackTime = 1.2f;
		AddTag(L"Green");
		AddTag(L"Zako");
		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
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
		m_Speed = 0.5f;
		m_HP = 15.0f;
		
		AddTag(L"CloseBoss");
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

		m_BaseY = m_Rigidbody->m_Scale.y / 2.0f + 1.0f;

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
		for (auto cannon : CannonVec)
		{
			if (cannon)
			{
				auto Ptrcannon = dynamic_pointer_cast<Cannon>(cannon);
				m_CannonPos = Ptrcannon->GetPosition();
				return;
			}
		}
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
		m_LerpToParent(0.2f),
		m_LerpToChild(0.2f),
		m_Attack1ToRot(0)
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
	}
	BossHand::~BossHand() {}

	void BossHand::OnCreate() {	

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

		//メッシュの取得
		shared_ptr<MeshResource> MeshPtr;

		if (FindTag(L"LEFT_HAND")) {
			MeshPtr = App::GetApp()->GetResource<MeshResource>(L"LEFT_HAND_MESH");
		}
		else {
			MeshPtr = App::GetApp()->GetResource<MeshResource>(L"RIGHT_HAND_MESH");
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
		//m_PtrObj->m_Alpha = 0.0f;
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

			World = m_PlayerLocalMatrix;
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

	void BossHand::AttackBehavior() {
		
	}

	void BossHand::ComplianceStartBehavior() {
		//ローカル行列の定義
		if (FindTag(L"LEFT_HAND")) {
			m_PlayerLocalMatrix.affineTransformation(
				m_Rigidbody->m_Scale,
				Vec3(0, 0, 0),
				Quat(),
				Vec3(-1.1f, -0.5f, 1.0f)
			);
		}
		else {
			m_PlayerLocalMatrix.affineTransformation(
				m_Rigidbody->m_Scale,
				Vec3(0, 0, 0),
				Quat(),
				Vec3(1.1f, -0.5f, 1.0f)
			);
		}
		//このステートではチャイルドの場合も同じ
		m_ChildLocalMatrix = m_PlayerLocalMatrix;
		m_LerpToParent = m_LerpToChild = 0.2f;
	}

	//攻撃１行動の開始
	void BossHand::Attack1StartBehavior() {
		m_Attack1ToRot = 0.1f;
		//ローカル行列の定義
		m_PlayerLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(Vec3(1.0, 0, 0), m_Attack1ToRot),
			Vec3(0, 0.5f, 0.0f)
		);
		m_ChildLocalMatrix.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			Quat(),
			Vec3(0, 0.5f, -0.5f)
		);
		m_LerpToParent = m_LerpToChild = 0.5f;

	}

	bool BossHand::Attack1ExcuteBehavior() {
		m_Attack1ToRot += 0.15f;
		if (m_Attack1ToRot >= (XM_PI + 2.0f)) {
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
			//auto PlayerPtr = Obj->GetStage()->FindTagGameObject<Player>(L"Player");
			//PlayerPtr->ChangeDefaultState();
			return;
		}
		Obj->UpdateBehavior();
		Obj->AttackBehavior();
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
		m_Speed = 0.5f;
		m_HP = 5.0f;
		AddTag(L"LongBoss");
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

}
//end basecross
