#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class TitleScene : public Stage {
		shared_ptr<MultiAudioObject> m_AudioObjectPtr;
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateBackGround();


	public:
		//�\�z�Ɣj��
		TitleScene() :Stage() {}
		virtual ~TitleScene();
		//������
		virtual void OnUpdateStage() override;

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;


		virtual void OnDrawStage()override;

	};

}