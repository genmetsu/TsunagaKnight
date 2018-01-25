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
		//�������g�̍X�V
		this->OnUpdate();
	}

	void TitleScene::OnCreate() {
		//�w�i�X�v���C�g�̍쐬
		//AddGameObject<MultiSprite>(
		//	L"TITLE_TX",
		//	Vec2(1280, 720),
		//	0.0f,
		//	Vec2(0, 0),
		//	1, 1
		//	);


		////�w�i�X�v���C�g�̍쐬
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
		//�w�i�X�v���C�g�̍쐬
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
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));

		//�V���h�I�E�}�b�v�`��̊J�n
		Dev->ClearShadowmapViews();
		Dev->StartShadowmapDraw();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̕`��
			v->OnDrawShadowmap();
		}
		Dev->EndShadowmapDraw();

		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̕`��
			v->OnDraw();
		}
		//�������g�̕`��
		this->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}


	void TitleScene::OnUpdate()
	{

		//�^�[�����̏�����
		GetRigidbodyManager()->InitRigidbody();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̍X�V
			v->OnUpdate();
		}



		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected)
		{
			//B�{�^��
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
			//B�{�^��
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

