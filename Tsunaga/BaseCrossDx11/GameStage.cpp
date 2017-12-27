/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	GameManager;

	//--------------------------------------------------------------------------------------
	///	�Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	GameStage::GameStage()
	{
	}

	void GameStage::RegisterDefaultSphere() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"DEFAULT_SPHERE")) {
			//���b�V�����o�^����ĂȂ���Γo�^
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE",
				MeshResource::CreateMeshResource(vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {

		//�V���h�E�}�b�v�̕`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

		//�w�i�X�v���C�g�̍쐬
		/*AddGameObject<MultiSprite>(
			L"SKY_TX",
			Vec2(1280, 800),
			0.0f,
			Vec2(0, 0),
			1, 1
			);*/
		AddGameObject<SkyBox>(
			L"SKY_TX",
			Vec3(2000.0f, 2000.0f, 2000.0f),
			Quat(),
			Vec3(0.0f),
			false);

		//�����g�p���鋅�̂̓o�^�i���\�[�X�o�^����j
		RegisterDefaultSphere();

		//���ʂ̍쐬
		Quat Qt;
		Qt.rotationX(XM_PIDIV2);
		/*AddGameObject<SquareObject>(
			L"WALL_TX",
			L"WALL_NORMAL_TX",
			Vec3(20.0f, 100.0f, 1.0f),
			Qt,
			Vec3(0.0f, 0.0f, 40.0f)
			);*/

		AddGameObject<YamatoStage>(
			L"YAMATO_TX",
			Vec3(4.0f, 4.0f, 4.0f),
			Quat(),
			Vec3(0.0f, -12.8f, -20.0f),
			true);

		//�v���C���[�̍쐬
		//shared_ptr<GameObject> 
		auto Par =
			AddGameObject<Player>(
				L"KUREHA_TX",
				true,
				Vec3(0.0f, 0.5f, 10.0f)
				);
		//SetPlayerPtr(Par);

		//���̍쐬
		AddGameObject<Sword>(
			Par,
			L"SPARK_TX",
			Vec3(0.5f, 0.5f, 0.8f),
			Quat(),
			Vec3(0.0f, 0.5f, 0.0f),
			true);

		for (int i = 0; i < 3; i++) {
			float x = (float)(i);
			wstring texture;
			switch (i) {
			case 0:
				texture = L"GREEN_CANNON_TX";
				break;
			case 1:
				texture = L"RED_CANNON_TX";
				break;
			case 2:
				texture = L"BLUE_CANNON_TX";
				break;
			}
			AddGameObject<Cannon>(
				texture,
				Vec3(4.0f, 4.0f, 4.0f),
				Quat(),
				Vec3(x * 6.0f - 6.0f, 0.0f, -3.0f),
				i, false);
		}

		for (int i = 0; i < 20; i++) {
			float x = (float)(i + 1);
			AddGameObject<NeedleEnemy>(
				Par,
				L"NEEDLE_MESH",
				L"NEEDLE_TX",
				L"30frame",
				Vec3(0.3f, 0.3f, 0.3f),
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 15.0f),
				false);
		}

		for (int i = 0; i < 20; i++) {
			float x = (float)(i + 1);
			AddGameObject<ShootEnemy>(
				Par,
				L"GUN_MESH",
				L"GUN_TX",
				L"NonMove",
				Vec3(0.25f, 0.25f, 0.25f),
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
			for (int j = 0; j < 2; j++) {
				AddGameObject<BulletObject>(
					L"SPARK_TX",
					Vec3(0.125f, 0.125f, 0.125f),
					Quat(),
					Vec3(0.0f, -10.0f, 0.0f),
					false,
					L"Bullet");
				AddGameObject<BulletObject>(
					L"SPARK_TX",
					Vec3(0.125f, 0.125f, 0.125f),
					Quat(),
					Vec3(0.0f, -10.0f, 0.0f),
					false,
					L"PlayerBullet");
			}
		}
		for (int i = 0; i < 1; i++) {
			float x = (float)(i + 1);
			AddGameObject<BossEnemy>(
				Par,
				L"NEEDLE_MESH",
				L"NEEDLE_TX",
				L"30frame",
				Vec3(10.0f, 10.0f, 10.0f),
				Quat(),
				Vec3(0.0f, 3.0f, 80.0f),
				false);
			for (int j = 0; j < 3; j++) {
				AddGameObject<BulletObject>(
					L"RED_TX",
					Vec3(0.5f, 0.5f, 0.5f),
					Quat(),
					Vec3(0.0f, -10.0f, 0.0f),
					false,
					L"BossBullet");
			}
		}

		for (int i = 0; i < 10; i++) {
			float x = (float)(i + 1);
			AddGameObject<AngelEnemy>(
				Par,
				L"ANGEL_MESH",
				L"ANGEL_TX",
				L"30frame",
				Vec3(0.25f, 0.25f, 0.25f),
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
		}


		for (int i = 0; i < 1; i++) {
			float x = (float)(i + 1);
			auto ptr = AddGameObject<CR_BossEnemy>(
				Par,
				L"HAND_BOSS_MESH",
				L"HAND_BOSS_TX",
				L"30frame",
				Vec3(1.8f, 1.8f, 1.8f),
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"LEFT_HAND",
				Vec3(0.5f, 1.0f, 0.5f),
				Quat(),
				Vec3(0.0f, 0.5f, 0.0f),
				true);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"RIGHT_HAND",
				Vec3(0.5f, 1.0f, 0.5f),
				Quat(),
				Vec3(0.0f, 0.5f, 0.0f),
				true);
		}


		for (int i = 0; i < 1; i++) {
			float x = (float)(i + 1);
			AddGameObject<LD_BossEnemy>(
				Par,
				L"THOR_BOSS_MESH",
				L"THOR_BOSS_TX",
				L"30frame",
				Vec3(3.0f, 0.8f, 3.0f),
				Quat(),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
		}


		//�X�p�[�N�G�t�F�N�g
		AddGameObject<MultiSpark>();
		//�t�@�C�A�G�t�F�N�g
		AddGameObject<MultiFire>();
		AddGameObject<AttackSigns>();
		AddGameObject<StepEffect>();

		//�`��I�u�W�F�N�g�̒ǉ�
		CreateDrawObjects();


		//��]����X�v���C�g�̍쐬
		/*AddGameObject<RotateSprite>(
			L"TRACE_TX",
			Vec2(160, 160),
			0.0f,
			Vec2(-480, 260),
			4, 4
			);*/


			//���b�Z�[�W��\������X�v���C�g�̍쐬
			/*AddGameObject<MessageSprite>(
				L"MESSAGE_TX",
				Vec2(256, 64),
				0.0f,
				Vec2(480, 260),
				1, 1
				);*/

				//������`��I�u�W�F�N�g�̍쐬
		AddGameObject<StringDrawObject>();

		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"Nanika");
		m_AudioObjectPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.0f);

	}

	//�`��I�u�W�F�N�g�̒ǉ�
	void GameStage::CreateDrawObjects() {
		//�V���h�E�}�b�v�`��I�u�W�F�N�g�̍쐬
		AddGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");

		//SimplePCTStaticRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePCTStaticRenderer>(L"SimplePCTStaticRenderer");

		//SimplePNTStaticRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticRenderer>(L"SimplePNTStaticRenderer");
		//SimplePNTStaticRenderer2�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");

		//SimplePNTStaticModelRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticModelRenderer>(L"SimplePNTStaticModelRenderer");
		//SimplePNTStaticModelRenderer2�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticModelRenderer2>(L"SimplePNTStaticModelRenderer2");

		//SimplePNTBoneModelRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTBoneModelRenderer>(L"SimplePNTBoneModelRenderer");
		//SimplePNTBoneModelRenderer2�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTBoneModelRenderer2>(L"SimplePNTBoneModelRenderer2");


		//BcPNTStaticDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
		//BcPNTnTStaticDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");

		//BcPNTStaticModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTStaticModelRenderer>(L"BcPNTStaticModelRenderer");
		//BcPNTnTStaticModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTnTStaticModelRenderer>(L"BcPNTnTStaticModelRenderer");

		//BcPNTBoneModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
		//BcPNTnTBoneModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTnTBoneModelRenderer>(L"BcPNTnTBoneModelRenderer");

		//Particle�`��I�u�W�F�N�g�̍쐬(���Z�������Ȃ�)
		AddGameObject<ParticleDrawObject>(false);
		//Particle�`��I�u�W�F�N�g�̍쐬(���Z��������)
		AddGameObject<ParticleDrawObject>(true);

	}


	void GameStage::OnUpdateStage() {
		//�^�[�����̏�����
		GetRigidbodyManager()->InitRigidbody();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̍X�V
			v->OnUpdate();
		}
		//Rigidbody�}�l�[�W���̍X�V�i�Փ˔���Ȃǁj
		GetRigidbodyManager()->OnUpdate();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̍ŏI�X�V
			v->OnUpdate2();
		}
		//�������g�̍X�V(�J����)
		this->OnUpdate();
		//Rigidbody�}�l�[�W���̍ŏI�X�V�i�Փ˔�����̃N���A�j
		GetRigidbodyManager()->OnUpdate2();
	}


	void GameStage::OnUpdate() {
		auto& camera = GetCamera();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//D�p�b�h��
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//�J�����ʒu������
				camera.m_CameraArmLen += 0.3f;
				if (GetCamera().m_CameraArmLen >= 50.0f) {
					GetCamera().m_CameraArmLen = 50.0f;
				}
			}
			//D�p�b�h��
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//�J�����ʒu�����
				camera.m_CameraArmLen -= 0.3f;
				if (GetCamera().m_CameraArmLen <= 2.0f) {
					camera.m_CameraArmLen = 2.0f;
				}
			}

			if (CntlVec[0].fThumbRX != 0) {
				camera.m_CameraXZRad -= CntlVec[0].fThumbRX * 0.05f;
				if (abs(camera.m_CameraXZRad) >= XM_2PI) {
					camera.m_CameraXZRad = 0;
				}
			}

			if (CntlVec[0].fThumbRY != 0) {
				camera.m_CameraYRad += CntlVec[0].fThumbRY * 0.05f;
				if (camera.m_CameraYRad >= XM_PIDIV2 - 0.05f) {
					camera.m_CameraYRad = XM_PIDIV2 - 0.05f;
				}
				else if (camera.m_CameraYRad <= 0.5f) {
					camera.m_CameraYRad = 0.5f;
				}
			}

			/*auto GM = GameManager::getInstance();
			int now_friends_num = GM->GetFriendsNum();
			if (now_friends_num >= 6) {
				camera.m_CameraArmLen = (float)now_friends_num * 0.5f;
				if (GetCamera().m_CameraArmLen >= 50.0f) {
					GetCamera().m_CameraArmLen = 50.0f;
				}
			}*/

			camera.m_CamerAt = FindTagGameObject<GameObject>(L"Player")->GetPosition();
			camera.m_CamerAt.y += 0.5f;
			Vec3 CameraLocalEye =
				Vec3(
					sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
					cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
					-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
				);
			camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
			//B�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
			}
		}
	}

	void GameStage::OnDrawStage() {
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


	void GameStage::OnDraw() {
		m_RigidbodyManager->OnDraw();
	}

	//--------------------------------------------------------------------------------------
	///	��̃X�e�[�W�i���b�Z�[�W�݂̂���j
	//--------------------------------------------------------------------------------------
	void EmptyStage::OnCreate() {
		//���b�Z�[�W�X�v���C�g
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(0, 0),
			1, 1);
	}
	void EmptyStage::OnUpdateStage() {
		//�X�v���C�g�̍X�V
		m_MessageSprite->OnUpdate();
		//�������g�̍X�V
		this->OnUpdate();
	}
	void EmptyStage::OnUpdate() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//B�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void EmptyStage::OnDrawStage() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		//�X�v���C�g�`��
		m_MessageSprite->OnDraw();
		//�������g�̕`��
		this->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}


	void EmptyStage::OnDraw() {
		//�������Ȃ�
	}




}
//end basecross
