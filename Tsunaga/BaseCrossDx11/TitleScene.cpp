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
	//�������g�̍X�V
	this->OnUpdate();
}

void TitleScene::OnCreate() {

	//�w�i�X�v���C�g�̍쐬
	AddGameObject<MultiSprite>(
		L"TsunagaKnight_TX",
		Vec2(1920 * 0.5f, 650 *0.5f),
		0.0f,
		Vec2(0, 200),
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
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}
}

}

