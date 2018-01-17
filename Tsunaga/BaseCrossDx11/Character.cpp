/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���S�̂��Ǘ�����}�l�[�W���[�A�X�|�i�[
	//--------------------------------------------------------------------------------------
	GameManager* GameManager::GM = nullptr;

	GameManager* GameManager::getInstance() {
		if (GM == nullptr) {
			GM = new GameManager();
		}
		return GM;
	}

	//--------------------------------------------------------------------------------------
	///	���ʎ���
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
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SquareMesh = MeshResource::CreateMeshResource(new_vertices, indices, false);
	}


	void SquareObject::OnCreate() {

		CreateBuffers(m_Scale.x, m_Scale.y);
		//�s��̒�`
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
		//�t�H�O�͂��߂�
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
	///�@�X�e�[�W�̑�a�I�u�W�F�N�g
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
		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"YAMATO_MESH");

		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -XM_PIDIV2, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void YamatoStage::OnUpdate() {

	}

	void YamatoStage::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//�������v�Z
		World = m_MeshToTransformMatrix * World;
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		//�������v�Z
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
	///	�����ȕ�
	//--------------------------------------------------------------------------------------
	TransparentWall::TransparentWall(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos) {}
	TransparentWall::~TransparentWall() {}
	//������
	void TransparentWall::OnCreate() {

		//Rigidbody�̏�����
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

		//�s��̒�`
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
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//������
	void MultiSpark::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 30.0f,
				rParticleSprite.m_LocalPos.y * 30.0f,
				rParticleSprite.m_LocalPos.z * 30.0f
			);
			//�F�̎w��
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
	//�p�r: �U���̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	AttackSpark::AttackSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	AttackSpark::~AttackSpark() {}

	//������
	void AttackSpark::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 30.0f,
				rParticleSprite.m_LocalPos.y * 30.0f,
				rParticleSprite.m_LocalPos.z * 30.0f
			);
			//�F�̎w��
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
	//�p�r: �G���W�F���G�l�~�[�̖h��G�t�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiGuardEffect::MultiGuardEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiGuardEffect::~MultiGuardEffect() {}

	//������
	void MultiGuardEffect::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//�F�̎w��
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
	//�p�r: �{�X�̖h��G�t�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BossEffect::BossEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	BossEffect::~BossEffect() {}

	//������
	void BossEffect::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//�F�̎w��
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
	//�p�r: �G�l�~�[�̈ړ��G�t�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EnemyMoveEffect::EnemyMoveEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	EnemyMoveEffect::~EnemyMoveEffect() {}

	//������
	void EnemyMoveEffect::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//�F�̎w��
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
	//�p�r: �����̉��N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//������
	void MultiFire::OnCreate() {
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			rParticleSprite.m_LocalScale *= Scale;
			//�F�̎w��
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
	//�p�r:�G�̍U������O���G�t�F�N�g
	//--------------------------------------------------------------------------------------
	AttackSigns::AttackSigns(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	AttackSigns::~AttackSigns() {}

	//������
	void AttackSigns::OnCreate() {
		//���Z�`�揈��������
		SetAddType(false);
		//�^�O�̒ǉ�
		AddTag(L"AttackSigns");
	}


	void AttackSigns::InsertSigns(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(2);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 1.25f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		//�J�����̕����ɏ����ړ�������
		auto &Camera = GetStage()->GetCamera();
		Vec3 Camera_Pos = Camera.m_CamerEye;
		Camera_Pos.y = 0.0f;
		Vec3 MoveVec = Camera_Pos - Pos;
		MoveVec.normalize();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalScale = Vec2(0.05f, 0.05f);
			rParticleSprite.m_LocalPos += MoveVec * 0.15f;
			//�F�̎w��
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
	//�p�r:�{�X�̍U������O���G�t�F�N�g
	//--------------------------------------------------------------------------------------
	BossAttackSigns::BossAttackSigns(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	BossAttackSigns::~BossAttackSigns() {}

	//������
	void BossAttackSigns::OnCreate() {
		//���Z�`�揈��������
		SetAddType(false);
		//�^�O�̒ǉ�
		AddTag(L"BossAttackSigns");
	}

	void BossAttackSigns::InsertSigns(const Vec3& Pos)
	{
		auto ParticlePtr = InsertParticle(5);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 1.25f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		//�J�����̕����ɏ����ړ�������
		auto &Camera = GetStage()->GetCamera();
		Vec3 Camera_Pos = Camera.m_CamerEye;
		Camera_Pos.y = 0.0f;
		Vec3 MoveVec = Camera_Pos - Pos;
		MoveVec.normalize();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalScale = Vec2(0.1f, 0.1f);
			rParticleSprite.m_LocalPos += (MoveVec * 1.5f);
			//�F�̎w��
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
	//�p�r: ��C�̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	CannonEffect::CannonEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	CannonEffect::~CannonEffect() {}

	//������
	void CannonEffect::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
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
	//�p�r: �X�e�b�v�G�t�F�N�g
	//--------------------------------------------------------------------------------------
	StepEffect::StepEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	StepEffect::~StepEffect() {}

	//������
	void StepEffect::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
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
	//�p�r: ��C�Ō�����Ă���Ƃ��̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	ShootingEffect::ShootingEffect(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	ShootingEffect::~ShootingEffect() {}

	//������
	void ShootingEffect::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
		//�^�O�̒ǉ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
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
	///	���b�s���O�������ꂽ�X�v���C�g
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
		//���_�z��(�c���w�萔���\��)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
	}
	void SpriteBase::OnCreate() {
		//���_�̏����C���i���z�֐��Ăт����j
		AdjustVertex();
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬�i�ύX�ł���j
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
	}

	void SpriteBase::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�̕ύX
		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//���_�̕ύX
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		//���z�֐��Ăяo��
		UpdateVertex(ElapsedTime, vertices);
		//�A���}�b�v
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);
	}



	void SpriteBase::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//���[���h�s��̌���
		Mat4x4 World;
		World.affineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vec2(0, 0),		//��]�̒��S�i�d�S�j
			m_Rot,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Mat4x4 Proj(XMMatrixOrthographicLH(w, h, -1.0, 1.0f));
		//�s��̍���
		World *= Proj;
		//�R���X�^���g�o�b�t�@�̏���
		SpriteConstantBuffer sb;
		//�G�~�b�V�u���Z�B
		sb.Emissive = m_Emissive;
		//�s��̐ݒ�
		sb.World = World;
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());
		//�u�����h�X�e�[�g
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
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//���X�^���C�U�X�e�[�g
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, TexPtr->GetShaderResourceView().GetAddressOf());
		//���b�s���O�T���v���[
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//�`��
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}
	//--------------------------------------------------------------------------------------
	///	��]����X�v���C�g
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
		//���_�z��(�c���w�萔���\��)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(0, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(0, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize*2.0f, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
	}

	void RotateSprite::AdjustVertex() {
		//���_�F��ύX����
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
	///	��C�̃Q�[�W
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
		//���_�z��(�c���w�萔���\��)
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
	///	�{�X��HP�Q�[�W
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
		//���_�z��(�c���w�萔���\��)
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
	///	���b�Z�[�W��\������X�v���C�g
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
		//�����ł͉������Ȃ�
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
	///	���b�Z�[�W��\������X�v���C�g
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
		//���_�F��ύX����
		
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
	///	���b�Z�[�W��\������X�v���C�g
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
		//���_�F��ύX����

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
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0) {
				//�R���g���[���̌����v�Z
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
	///	�w�i�X�v���C�g
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
	///�@SkyBox
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

		//Rigidbody�̏�����
		auto PtrGameStage = GetStage<GameStage>();

		//���b�V���̎擾

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(-1.0f, 18, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		auto MeshPtr = MeshResource::CreateMeshResource(vertices, indices, false);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

	}

	void SkyBox::OnUpdate() {

	}

	void SkyBox::OnDrawShadowmap() {
		
	}

	void SkyBox::OnDraw() {
		//�s��̒�`
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
	///�@��C�I�u�W�F�N�g
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
		//�^�O�̒ǉ�
		AddTag(L"Cannon");

		//Rigidbody�̏�����
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

		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"CANNON_MESH");

		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.5f, 1.5f, 1.5f),
			Vec3(0.0f, 0.0f, 0.3f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
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
		//���݂ƖڕW����
		m_Rigidbody->m_Quat = XMQuaternionSlerp(m_Rigidbody->m_Quat, Qt, 0.1f);
	}

	void Cannon::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�������v�Z
		World = m_MeshToTransformMatrix * World;
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//�������v�Z
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
		//�s��̒�`
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
	///	�G�̐e�I�u�W�F�N�g
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
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
		//�^�O�̒ǉ�
		AddTag(L"EnemyObject");

		//Rigidbody�̏�����
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

		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(m_MeshResName);
		//�T�E���h�I�u�W�F�N�g�̏�����
		m_DeadSound = ObjectFactory::Create<SoundObject>(L"Pan");
		m_BossDamageSound = ObjectFactory::Create<SoundObject>(L"bossdamage");
		m_FriendsSound = ObjectFactory::Create<SoundObject>(L"nakama");
		m_CannonSound = ObjectFactory::Create<SoundObject>(L"cannon");
		m_EyeFlashSound = ObjectFactory::Create<SoundObject>(L"eye_flash");

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//�^�[�Q�b�g���W�̏�����
		m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);

		m_FollowingAngelNum = 0;

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<EnemyObject>(GetThis<EnemyObject>()));
		//�X�e�[�g�����l�ݒ�
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
		//�e�Ƃ̏Փ˔���
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
					//Spark�̑��o
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);
					//�e���َ����ɔ�΂�
					PtrBullet->SetPosition(Vec3(0.0f, -100.0f, 0.0f));

					//�̗͂����
					m_HP--;

					if (m_HP <= 0.0f) {
						Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
						float p_dis = (p_pos - GetPosition()).length();
						if (p_dis < 1.0f) {
							p_dis = 1.0f;
						}
						//�T�E���h�̔��s
						if (FindTag(L"Zako")) {
							m_DeadSound->Start(0, 1.0f / p_dis);
						}
						else {
							m_CannonSound->Start(0, 1.0f / p_dis);
						}

						Vec3 Emitter = GetPosition();
						//Fire�̑��o
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
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
	}

	void EnemyObject::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//�������v�Z
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
		//�s��̒�`
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
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_ParentPtr.lock();
		//�e�̃��[���h�s����擾����ϐ�
		Mat4x4 ParMat;
		if (shptr) {
			ParentFlg flg = ParentFlg::NoParent;
			//�s��擾�p�̃C���^�[�t�F�C�X�������Ă邩�ǂ���
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
				//�s��̒�`
				World = m_PlayerLocalMatrix;
				LerpNum = m_LerpToParent;
				break;
			case ParentFlg::Child:
				//�s��̒�`
				World = m_ChildLocalMatrix;
				LerpNum = m_LerpToChild;
				break;
			default:
				break;
			}
			if (flg != ParentFlg::NoParent) {
				//�X�P�[�����O��1.0�ɂ����s��ɕϊ�
				ParMat.scaleIdentity();
				//�s��̔��f
				World *= ParMat;
				//���̎��_��World�͖ڕW�ƂȂ�ʒu
				Vec3 toPos = World.transInMatrix();
				//��ԏ����ňړ��ʒu������
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
			//Spark�̑��o
			auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiGuardEffect>(L"MultiGuardEffect");
			SparkPtr->InsertSpark(Emitter);
		}
		else if (m_FollowingAngelNum == 0 && m_Rigidbody->m_Velocity.length() > 1.0f) {
			Vec3 Emitter = m_Rigidbody->m_Pos;
			Emitter.y -= 0.125f;
			//Spark�̑��o
			auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<EnemyMoveEffect>(L"EnemyMoveEffect");
			SparkPtr->InsertSpark(Emitter);
		}
	}

	void EnemyObject::UpdateBehavior() {
		FriendsBehavior();
	}

	void EnemyObject::OppositionBehavior() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		//�e�̃��[���h�s����擾����ϐ�
		Mat4x4 ParMat;
		if (shptr) {
			//�s��擾�p�̃C���^�[�t�F�C�X�������Ă邩�ǂ���
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
			}

			Mat4x4 World;
			World.identity();
			//�s��̒�`
			World = m_PlayerLocalMatrix;
			//�X�P�[�����O��1.0�ɂ����s��ɕϊ�
			ParMat.scaleIdentity();
			//�s��̔��f
			World *= ParMat;
			//���̎��_��World�͖ڕW�ƂȂ�ʒu
			Vec3 toPos = World.transInMatrix();
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//���������߂�
			float dis = ToPosVec.length();

			// �ːi���Ă��΂炭��������
			if (m_FrameCount > m_StopTime + m_TackleTime)
			{
				m_Tackle = false;
				m_FrameCount = 0.0f;
				m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);
				m_Rigidbody->m_Velocity *= 0.01f;
				m_StateMachine->ChangeState(EnemyAttackEndState::Instance());
				return;
			}
			// �~�܂�͂���
			else if (m_FrameCount > m_StopTime && m_Tackle == false)
			{
				m_Tackle = true;
				if (m_TargetPos == Vec3(0.0f, 0.0f, 0.0f)) {
					m_TargetPos = toPos;
					m_TackleStartPos = m_Rigidbody->m_Pos;
				}
			}

			// �ːi�̏���
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
					//Spark�̑��o
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
				// �v���C���[�ƃG�l�~�[�̋������߂��Ȃ������̏���
				else if (dis <= m_SearchDis && m_UpdateActive)
				{
					m_FrameCount += ElapsedTime;
					if (dis < 1.0f) {
						dis = 1.0f;
					}
					m_EyeFlashSound->Start(0, 0.2f / dis);
					//fire�̑��o
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<AttackSigns>(L"AttackSigns");
					Vec3 Emitter = m_Rigidbody->m_Pos;
					Emitter.y += 0.125f;
					FirePtr->InsertSigns(Emitter);
				}
				// �v���C���[�ƃG�l�~�[�̋����������Ȃ�����Ăё�C�Ɍ�����
				else if (dis >= m_SearchDis * 3.0f && m_UpdateActive)
				{
					m_StateMachine->ChangeState(EnemyToCannonState::Instance());
					return;
				}
				// �v���C���[�Ɍ���������
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
		//�������Œ�
		m_Rigidbody->m_Pos.y = m_BaseY;
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_FrameCount += ElapsedTime;
	}

	void EnemyObject::ToCannonBehavior() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		if (shptr) {
			Vec3 toPos = m_CannonPos;
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//�v���C���[�Ƃ̋��������߂�
			Vec3 PlayerPos = shptr->GetPosition();
			float dis = (PlayerPos - m_Rigidbody->m_Pos).length();
			// �v���C���[�ƃG�l�~�[�̋������߂��Ȃ������̏���
			if (dis <= (m_SearchDis - 1.0f))
			{
				m_StateMachine->ChangeState(EnemyOppositionState::Instance());
				return;
			}
			// ��C�Ɍ���������
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
		//��C�Ƃ̏Փ�
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
					//Spark�̑��o
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);
					//Fire�̑��o
					auto FirePtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
					FirePtr->InsertFire(Emitter, 1.0f);

					Vec3 p_pos = m_PlayerPtr.lock()->GetPosition();
					float p_dis = (p_pos - GetPosition()).length();
					if (p_dis < 1.0f) {
						p_dis = 1.0f;
					}
					//�T�E���h�̔��s
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
		//���[�J���s��̒�`
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

	//�U���P�s���̊J�n
	void EnemyObject::Attack1StartBehavior() {
		m_Attack1ToRot = 0.1f;
		//���[�J���s��̒�`
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
		//��]�̏���
		Vec3 Temp = m_Rigidbody->m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//���݂ƖڕW����
		//�ړ����Ȃ��Ƃ��͉�]���Ȃ�
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
			//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
		//���[�J���s��̒�`
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
				//Spark�̑��o
				Vec3 Emitter = m_Rigidbody->m_Pos;
				Emitter.z += 5.0f;
				//Spark�̑��o
				auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
				SparkPtr->InsertFire(Emitter, 6.0f);

				m_CannonSound->Start(0, 1.0f);
				m_Bomb = true;
			}
			m_Rigidbody->m_Velocity = m_ToBossVec * 15.0f;

			//�{�X�Ƃ̏Փ˔���
			float length = (m_BossPos - m_Rigidbody->m_Pos).length();
			if (length < 5.0f) {
				Vec3 Emitter = m_Rigidbody->m_Pos;
				//Spark�̑��o
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
		//�G�t�F�N�g�̍Đ�
		auto FirePtr = GetStage<GameStage>()->FindTagGameObject<ShootingEffect>(L"ShootingEffect");
		Vec3 Emitter = m_Rigidbody->m_Pos;
		FirePtr->InsertEffect(Emitter);

		m_FrameCount += ElapsedTime * 5.0f;
	}

	//--------------------------------------------------------------------------------------
	///	�G�΃X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyOppositionState)
	void EnemyOppositionState::Enter(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	void EnemyOppositionState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->OppositionBehavior();
		Obj->CollisionBullet();
	}

	void EnemyOppositionState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�U����̍d���X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyAttackEndState)
	void EnemyAttackEndState::Enter(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	void EnemyAttackEndState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->AttackEndBehavior();
		Obj->CollisionBullet();
	}

	void EnemyAttackEndState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	��C�Ɍ������X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyToCannonState)
	void EnemyToCannonState::Enter(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	void EnemyToCannonState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->ToCannonBehavior();
		Obj->CollisionBullet();
	}

	void EnemyToCannonState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�Ǐ]�X�e�[�g�iEnemyObject�j
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
	///	��C�Ō�����Ă鎞�̃X�e�[�g�iEnemyObject�j
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
	///	�`�F�C�����Ԃ�񂷃X�e�[�g�iEnemyObject�j
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
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�_���[�W���󂯂��Ƃ��̃X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDamageState)

	void EnemyDamageState::Enter(const shared_ptr<EnemyObject>& Obj) {
		
	}

	void EnemyDamageState::Execute(const shared_ptr<EnemyObject>& Obj) {
		Obj->DamageBehaviour();
	}

	void EnemyDamageState::Exit(const shared_ptr<EnemyObject>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�X�|�[���X�̃X�e�[�g�iEnemyObject�j
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
	///	�j�[�h���G�l�~�[�i�ߐځj
	//--------------------------------------------------------------------------------------

	NeedleEnemy::NeedleEnemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& ParentPtr, 
		const wstring& MeshResName,
		const wstring & TextureResName, const wstring& DefaultAnimation, const Vec3 & Scale,
		const Quat & Qt, const Vec3 & Pos, bool OwnShadowActive) :
		EnemyObject(StagePtr, ParentPtr,MeshResName ,TextureResName, DefaultAnimation, Scale,Qt,Pos, OwnShadowActive)
	{
		AddTag(L"Red");
		AddTag(L"Zako");
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
		//�G�Ƃ̓����蔻��
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
						//�T�E���h�̔��s
						m_DeadSound->Start(0, 1.0f / p_dis);

						Vec3 Emitter = PtrEnemy->GetPosition();
						//Fire�̑��o
						auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
						SparkPtr->InsertFire(Emitter, 1.0f);
					
						PtrEnemy->ChangeState(L"Waiting");
						return;
					}
				}
			}
		}
		//�e�Ƃ̏Փ˔���
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
					//Spark�̑��o
					auto SparkPtr = GetStage<GameStage>()->FindTagGameObject<AttackSpark>(L"AttackSpark");
					SparkPtr->InsertSpark(Emitter);

					return;
				}
			}
		}
		//�{�X�n���h�Ƃ̏Փ˔���
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
					//Spark�̑��o
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
						//Fire�̑��o
						auto FirePtr = GetStage<GameStage>()->FindTagGameObject<MultiFire>(L"MultiFire");
						FirePtr->InsertFire(Emitter, PtrBoss->GetScale() * 3.0f);
						
						PtrBoss->ChangeState(L"Waiting");
					}
					//�m�b�N�o�b�N�����̐ݒ�
					return;
				}
			}
		}
	}

	void NeedleEnemy::OnUpdate() {
		EnemyObject::OnUpdate();
	}

	//--------------------------------------------------------------------------------------
	///	�e�����G�l�~�[�i�������j
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
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
			//���������߂�
			float dis = ToPosVec.length();

			//�ړ����鏈��
			ToPosVec.normalize();
			ToPosVec *= m_Speed;
			if (m_UpdateActive == false) {
				ToPosVec *= 0.0f;
			}
			m_Rigidbody->m_Velocity = ToPosVec;
			m_Rigidbody->m_Pos.y = m_BaseY;

			if (m_UpdateActive) {
				if (m_FrameCount > m_StopTime) {
					//�����΂�����
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
								//�T�E���h�̔��s
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
				//�x������
				if (m_FrameCount > 0.0f) {
					m_FrameCount += ElapsedTime;
					m_Rigidbody->m_Velocity *= 0.01f;
				}
				// �v���C���[�ƃG�l�~�[�̋������߂��Ȃ������̏���
				else if (dis <= m_SearchDis)
				{
					if (dis < 1.0f) {
						dis = 1.0f;
					}
					m_EyeFlashSound->Start(0, 0.2f / dis);
					m_FrameCount += ElapsedTime;
					//�T�C���̑��o
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
			//�v���C���[�����ւ̓�̃x�N�g�������A��������O�ς�����đ��ʂɒe���΂��悤�ɂ���
			Vec3 toPos = shptr->GetPosition();
			Vec3 toPos2 = toPos;

			toPos.y = 1.0f;
			toPos2.y = -1.0f;

			Vec3 force1 = toPos - m_Rigidbody->m_Pos;
			Vec3 force2 = toPos2 - m_Rigidbody->m_Pos;

			Vec3 force = cross(force1, force2);
			force.y = 0;
			force.normalize();
			
			//��]�̌�����90�x
			float angle = -XM_PIDIV2;

			//��ɃX�e�[�W�̉��Ɍ������Ēe�����悤�ɂ���
			if (force.z < 0)
			{
				force.z *= -1.0f;
				force.x *= -1.0f;
				angle *= -1.0f;
			}			

			//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
							//�T�E���h�̔��s
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//�������v�Z
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
	///	ShootEnemy�̌����o���e�N���X
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

		//Rigidbody�̏�����
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

		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation
		(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//�^�[�Q�b�g���W�̏�����
		m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
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
	/// �G���W�F���G�l�~�[
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
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
	/// ���{�X�G�l�~�[�ߋ���
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
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
		//�^�O�̒ǉ�
		AddTag(L"EnemyObject");

		m_EyeFlashSound = ObjectFactory::Create<SoundObject>(L"eye_flash");

		//Rigidbody�̏�����
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

		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(m_MeshResName);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//�^�[�Q�b�g���W�̏�����
		m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);

		m_BaseY = m_Rigidbody->m_Scale.y / 2.0f + 0.6f;

		//�T�E���h�I�u�W�F�N�g�̏�����
		m_DeadSound = ObjectFactory::Create<SoundObject>(L"Pan");
		m_BossDamageSound = ObjectFactory::Create<SoundObject>(L"bossdamage");
		m_FriendsSound = ObjectFactory::Create<SoundObject>(L"nakama");
		m_CannonSound = ObjectFactory::Create<SoundObject>(L"cannon");
		m_EyeFlashSound = ObjectFactory::Create<SoundObject>(L"eye_flash");

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);

		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<EnemyObject>(GetThis<EnemyObject>()));
		//�X�e�[�g�����l�ݒ�
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
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_PlayerPtr.lock();
		//�e�̃��[���h�s����擾����ϐ�
		Mat4x4 ParMat;
		if (shptr) {
			//�s��擾�p�̃C���^�[�t�F�C�X�������Ă邩�ǂ���
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
			}

			Mat4x4 World;
			World.identity();
			//�s��̒�`
			World = m_PlayerLocalMatrix;
			//�X�P�[�����O��1.0�ɂ����s��ɕϊ�
			ParMat.scaleIdentity();
			//�s��̔��f
			World *= ParMat;
			//���̎��_��World�͖ڕW�ƂȂ�ʒu
			Vec3 toPos = World.transInMatrix();
			Vec3 ToPosVec = toPos - m_Rigidbody->m_Pos;
			//���������߂�
			float dis = ToPosVec.length();

			// �ːi���Ă��΂炭��������
			if (m_FrameCount > m_StopTime + m_TackleTime)
			{
				m_Tackle = false;
				m_FrameCount = 0.0f;
				m_TargetPos = Vec3(0.0f, 0.0f, 0.0f);
				m_Rigidbody->m_Velocity *= 0.01f;
				m_StateMachine->ChangeState(EnemyAttackEndState::Instance());
				return;
			}
			// �~�܂�͂���
			else if (m_FrameCount > m_StopTime && m_Tackle == false)
			{
				m_Tackle = true;
				if (m_TargetPos == Vec3(0.0f, 0.0f, 0.0f)) {
					m_TargetPos = toPos;
					m_TackleStartPos = m_Rigidbody->m_Pos;
				}
			}

			// �ːi�̏���
			if (m_Tackle == true && m_UpdateActive)
			{
				m_FrameCount += ElapsedTime;
			}
			// �G�l�~�[�ړ�����
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
				// �v���C���[�ƃG�l�~�[�̋������߂��Ȃ������̏���
				else if (dis <= m_SearchDis && m_UpdateActive)
				{
					m_FrameCount += ElapsedTime;
					//fire�̑��o
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
				// �v���C���[�Ɍ���������
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
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
	}

	void CR_BossEnemy::OnDraw() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//�������v�Z
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
	/// �{�X�̎�
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
		//�^�O�̒ǉ�
		AddTag(TagName);
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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

		//Rigidbody�̏�����
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

		//���b�V���̎擾
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

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<BossHand>(GetThis<BossHand>()));
		//�X�e�[�g�����l�ݒ�
		m_StateMachine->ChangeState(HandDefaultState::Instance());

	}

	void BossHand::OnUpdate() {
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
	}

	void BossHand::OnUpdate2() {
	}

	void BossHand::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�������v�Z
		World = m_MeshToTransformMatrix * World;
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//�������v�Z
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
		//�s��̒�`
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
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto shptr = m_ParentPtr.lock();
		//�e�̃��[���h�s����擾����ϐ�
		Mat4x4 ParMat;
		if (shptr) {
			ParentFlg flg = ParentFlg::NoParent;
			//�s��擾�p�̃C���^�[�t�F�C�X�������Ă邩�ǂ���
			auto matintptr = dynamic_pointer_cast<MatrixInterface>(shptr);
			if (matintptr) {
				matintptr->GetWorldMatrix(ParMat);
			}
			Mat4x4 World;
			World.identity();
			float LerpNum = 0.2f;

			World = m_LocalMatrix;
			LerpNum = m_LerpToParent;

			//�X�P�[�����O��1.0�ɂ����s��ɕϊ�
			ParMat.scaleIdentity();
			//�s��̔��f
			World *= ParMat;
			//���̎��_��World�͖ڕW�ƂȂ�ʒu
			Vec3 toPos = World.transInMatrix();
			Vec3 DammiPos;
			World.decompose(m_Rigidbody->m_Scale, m_Rigidbody->m_Quat, DammiPos);
			Vec3 Velo = toPos - m_Rigidbody->m_Pos;
			Velo /= ElapsedTime;
			m_Rigidbody->m_Velocity = Velo;
		}
	}

	void BossHand::ComplianceStartBehavior() {
		//���[�J���s��̒�`
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

	//�U���P�s���̊J�n
	void BossHand::Attack1StartBehavior() {
	}

	bool BossHand::Attack1ExcuteBehavior() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (m_UpdateActive) {
			if (m_FrameCount > m_AttackSetupTime + m_BeforeAttackTime + m_AttackTime + m_ReturnDefaultTime) {
				m_FrameCount = 0.0f;
				return true;
			}
			if (m_FrameCount > m_AttackSetupTime + m_BeforeAttackTime + m_AttackTime) {
				//���[�J���s��̒�`
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
					//���[�J���s��̒�`
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
				//���[�J���s��̒�`
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
	///	�Ǐ]�X�e�[�g�iBossHand�j
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(HandDefaultState)

	void HandDefaultState::Enter(const shared_ptr<BossHand>& Obj) {
		Obj->ComplianceStartBehavior();
		//�������Ȃ�
	}

	void HandDefaultState::Execute(const shared_ptr<BossHand>& Obj) {
		Obj->UpdateBehavior();
	}

	void HandDefaultState::Exit(const shared_ptr<BossHand>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�U���X�e�[�g�P�iBossHand�j
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
	/// ���{�X�G�l�~�[������
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
		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
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
	/// �G�̃{�X
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
		//���b�V���̎擾
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"BOSS_MESH");

		//���b�V���ƃg�����X�t�H�[���̍����̐ݒ�
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(0.3f, 0.3f, 0.3f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.7f, 0.0f)
		);

		//�s��̒�`
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

		//Rigidbody�̏�����
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
		//�`��f�[�^�̍\�z
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

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//�`��f�[�^�̍s����R�s�[
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
			//Spark�̑��o
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);
		//�������v�Z
		World = m_MeshToTransformMatrix * World;
		//�`��f�[�^�̍s����R�s�[
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
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Rigidbody->m_Scale,
			Vec3(0, 0, 0),
			m_Rigidbody->m_Quat,
			m_Rigidbody->m_Pos
		);

		//�������v�Z
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
