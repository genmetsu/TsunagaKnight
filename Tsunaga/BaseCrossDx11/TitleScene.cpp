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

}
void TitleScene::OnDraw() {

}


void TitleScene::OnDrawStage() {

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

