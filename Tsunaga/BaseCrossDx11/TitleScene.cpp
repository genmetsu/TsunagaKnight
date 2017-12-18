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
		L"SKY_TX",
		Vec2(1280, 800),
		0.0f,
		Vec2(0, 0),
		1, 1
		);
	//�w�i�X�v���C�g�̍쐬
	AddGameObject<MultiSprite>(
		L"KUREHA_TX",
		Vec2(500, 500),
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
	auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
	if (CntlVec[0].bConnected)
	{
		//B�{�^��
		if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) 
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}
}

}

