/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {



	//--------------------------------------------------------------------------------------
	///	�Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//�`��I�u�W�F�N�g�̒ǉ�
		void CreateDrawObjects();
		//�ȉ��A�ʂɕۑ����Ă���
		//������`��I�u�W�F�N�g
		shared_ptr<StringDrawObject> m_StringDrawObject;
		//�����g�p���鋅�̂̓o�^�i���\�[�X�o�^����j
		void RegisterDefaultSphere();
		weak_ptr<Player> m_PtrPlayer;
		shared_ptr<MultiAudioObject> m_AudioObjectPtr;

		Vec3 m_CameraPos;

		float m_FrameCount;
		float m_StartCameraY;
		bool m_FogActive;
		bool m_isClear;
		bool m_isFail;
		bool m_isGameStart;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		GameStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameStage() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�X�V�i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�`��i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

		void CannonStateStartBehaviour();
		void CannonStateEndBehaviour();

		void GameClearBehaviour();
		void GameOverBehaviour();

		void SetIsClear(bool value) {
			m_isClear = value;
			GameClearBehaviour();
		}
		bool GetIsClear() {
			return m_isClear;
		}

		bool GetFogActive() {
			return m_FogActive;
		}

		void SetIsFail(bool value) {
			m_isFail = value;
			GameOverBehaviour();
		}
		bool GetIsFail() {
			return m_isFail;
		}

		void SetActiveObjects(bool value);


		shared_ptr<Player> GetPlayerPtr()const {
			auto shptr = m_PtrPlayer.lock();
			if (shptr) {
				return shptr;
			}
			return nullptr;
		}
		void SetPlayerPtr(const shared_ptr<Player> ptr) {
			m_PtrPlayer = ptr;
		}

	};


	//--------------------------------------------------------------------------------------
	///	��̃X�e�[�W�i���b�Z�[�W�݂̂���j
	//--------------------------------------------------------------------------------------
	class EmptyStage : public Stage {
		shared_ptr<MessageSprite> m_MessageSprite;		///<���b�Z�[�W��\������X�v���C�g
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		EmptyStage() {}
		virtual ~EmptyStage() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�X�V�i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�`��i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};





}
//end basecross
