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
			MeshUtill::CreateSphere(1.0f, 6, vertices, indices);
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE",
				MeshResource::CreateMeshResource(vertices, indices, false));
		}
		if (!App::GetApp()->CheckResource<MeshResource>(L"BOSS_BULLET")) {
			//���b�V�����o�^����ĂȂ���Γo�^
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(1.0f, 3, vertices, indices);
			App::GetApp()->RegisterResource(L"BOSS_BULLET",
				MeshResource::CreateMeshResource(vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {
		m_FrameCount = 0.0f;
		m_isClear = false;
		m_isFail = false;
		m_isGameStart = false;
		m_FogActive = false;
		m_CameraPos = Vec3(50, 10, 150);
		m_StartCameraY = 6.0f;
		
		//�V���h�E�}�b�v�̕`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

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

		AddGameObject<YamatoStage>(
			L"YAMATO_TX",
			Vec3(4.0f, 4.0f, 4.0f),
			Quat(),
			Vec3(0.0f, -12.74f, -18.2f - 1.5f),
			true);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 60.0f),
			Quat(Vec3(0, 1.0f, 0), -XM_PI / 180.0f * 4.18f),
			Vec3(15.15f, 0.25f, 14.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 60.0f),
			Quat(Vec3(0, 1.0f, 0), XM_PI / 180.0f * 4.18f),
			Vec3(-15.15f, 0.25f, 14.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 40.0f),
			Quat(Vec3(0, 1.0f, 0), -XM_PI / 180.0f * 17.0f),
			Vec3(10.0f, 0.25f, 54.0f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 40.0f),
			Quat(Vec3(0, 1.0f, 0), XM_PI / 180.0f * 17.0f),
			Vec3(-10.0f, 0.25f, 54.0f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 10.0f),
			Quat(Vec3(0, 1.0f, 0), -XM_PI / 180.0f * 62.0f),
			Vec3(3.0f, 0.25f, 72.9f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 10.0f),
			Quat(Vec3(0, 1.0f, 0), XM_PI / 180.0f * 62.0f),
			Vec3(-3.0f, 0.25f, 72.9f - 1.5f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 5.0f),
			Quat(),
			Vec3(-16.4f, 0.25f, -5.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(0.5f, 0.5f, 5.0f),
			Quat(),
			Vec3(16.4f, 0.25f, -5.0f)
			);

		AddGameObject<TransparentWall>(
			Vec3(50.0f, 0.5f, 0.5f),
			Quat(),
			Vec3(0.0f, 0.25f, -7.0f)
			);

		

		//�v���C���[�̍쐬
		auto Par =
			AddGameObject<Player>(
				L"KUREHA_TX",
				true,
				Vec3(0.0f, 0.5f, 11.0f)
				);

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
			wstring tag;
			switch (i) {
			case 0:
				texture = L"GREEN_CANNON_TX";
				tag = L"GREEN_CANNON";
				break;
			case 1:
				texture = L"RED_CANNON_TX";
				tag = L"RED_CANNON";
				break;
			case 2:
				texture = L"BLUE_CANNON_TX";
				tag = L"BLUE_CANNON";
				break;
			}
			AddGameObject<Cannon>(
				texture,
				tag,
				Vec3(5.0f, 5.0f, 5.0f),
				Quat(),
				Vec3(x * 9.0f - 9.0f, 0.0f, 0.0f),
				i, false);
		}

		AddGameObject<Boss>(
			L"BOSS_TX",
			Vec3(10.0f, 10.0f, 10.0f),
			Quat(),
			Vec3(0.0f, 12.0f, 60.0f),
			true);

		for (int i = 0; i < 20; i++) {
			float x = (float)(i + 1);
			AddGameObject<NeedleEnemy>(
				Par,
				L"NEEDLE_MESH",
				L"NEEDLE_TX",
				L"30frame",
				Vec3(0.25f, 0.25f, 0.25f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
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
				Vec3(0.21f, 0.21f, 0.21f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 20.0f - 10.0f, 0.125f, 60.0f - (float)rand() / 32767 * 20.0f),
				false);
			for (int j = 0; j < 5; j++) {
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

		for (int i = 0; i < 200; i++) {
			AddGameObject<BulletObject>(
				L"SPARK_TX",
				Vec3(0.5f, 0.5f, 0.5f),
				Quat(),
				Vec3(0.0f, -10.0f, 0.0f),
				false,
				L"BossBullet");
		}

		for (int i = 0; i < 10; i++) {
			float x = (float)(i + 1);
			AddGameObject<AngelEnemy>(
				Par,
				L"ANGEL_MESH",
				L"ANGEL_TX",
				L"30frame",
				Vec3(0.19f, 0.19f, 0.19f),
				Quat(Vec3(0, 1.0f, 0), XM_PI),
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
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 10.0f - 5.0f, 0.125f, 60.0f - (float)rand() / 32767 * 10.0f),
				false);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"LeftHand",
				Vec3(0.5f, 1.0f, 0.5f),
				Quat(),
				Vec3(0.0f, 0.5f, 0.0f),
				true);
			AddGameObject<BossHand>(
				ptr,
				L"HAND_TX",
				L"RightHand",
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
				Quat(Vec3(0, 1.0f, 0), XM_PI),
				Vec3((float)rand() / 32767 * 10.0f - 5.0f, 0.125f, 60.0f - (float)rand() / 32767 * 10.0f),
				false);
		}


		//�X�p�[�N�G�t�F�N�g
		AddGameObject<AttackSpark>();
		AddGameObject<MultiSpark>();
		//�t�@�C�A�G�t�F�N�g
		AddGameObject<MultiFire>();
		AddGameObject <ChainEffect>();
		AddGameObject<AttackSigns>();
		AddGameObject<HandBossSpark>();
		AddGameObject<BossAttackSigns>();
		AddGameObject<MiddleBossAttackSigns>();
		AddGameObject<StepEffect>();
		AddGameObject<ShootingEffect>();
		AddGameObject<CannonEffect>();
		AddGameObject<BossEffect>();
		AddGameObject<BossPrepareAttackEffect>();
		AddGameObject<BossBulletEffect>();
		AddGameObject<MultiGuardEffect>();
		AddGameObject<EnemyMoveEffect>();

		//�`��I�u�W�F�N�g�̒ǉ�
		CreateDrawObjects();

		AddGameObject<BossHPGauge>(
			L"BOSS_BAR_TX",
			Vec2(960, 24),
			0.0f,
			Vec2(-480, -330),
			1, 1
			);

		AddGameObject<UISprite>(
			L"BOSS_FRAME_TX",
			Vec2(1000, 50),
			0.0f,
			Vec2(0, -330),
			1, 1
			);
		AddGameObject<CannonGauge>(
			L"CANNON_BAR_TX",
			Vec2(300, 18),
			0.0f,
			Vec2(-555 + 64, 330),
			1, 1
			);
		AddGameObject<UISprite>(
			L"CANNON_FRAME_TX",
			Vec2(320, 40),
			0.0f,
			Vec2(-405 + 64, 330),
			1, 1
			);

		AddGameObject<UISprite>(
			L"BOSS_ICON_TX",
			Vec2(128.0f * 0.8f, 128.0f * 0.8f),
			0.0f,
			Vec2(-530, -320),
			1, 1
			);
		AddGameObject<UISprite>(
			L"KUREHA_ICON_TX",
			Vec2(64, 64),
			0.0f,
			Vec2(-600, 325),
			1, 1
			);
		AddGameObject<UISprite>(
			L"CANNON_ICON_TX",
			Vec2(52, 52),
			0.0f,
			Vec2(-530, 325),
			1, 1
			);

		for (int i = 0; i < 20; i++) {
			AddGameObject<ChainUISprite>(
				L"TRANS_TX",
				Vec2(30, 30),
				0.0f,
				Vec2(-600, 270 - i * 30),
				1, 1,
				i);
		}

		SetActiveObjects(false);
		//������`��I�u�W�F�N�g�̍쐬
		AddGameObject<StringDrawObject>();

		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"BGM_1");
		m_AudioObjectPtr->Start(L"BGM_1", XAUDIO2_LOOP_INFINITE, 0.4f);

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
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto& camera = GetCamera();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			auto GM = GameManager::getInstance();
			GM->m_camera_length = camera.m_CameraArmLen;
			auto PlayerPtr = FindTagGameObject<Player>(L"Player");
			auto boss = FindTagGameObject<Boss>(L"BossEnemy");

			//�`�F�C����������x�����Ƃ��͈ړ����Ă���Ƃ��J����������
			int now_friends_num = GM->GetFriendsNum();
			if (now_friends_num >= 3) {
				Vec3 PlayerVelo = PlayerPtr->GetVelocity();
				if (PlayerVelo.length() > 3.0f && camera.m_CameraArmLen < 5.0f) {
					camera.m_CameraArmLen += 0.02f;
				}
				else if (PlayerVelo.length() < 3.0f && camera.m_CameraArmLen > 2.6f) {
					camera.m_CameraArmLen -= 0.08f;
				}
			}

			int now_cannon = PlayerPtr->GetIsCannon();
			if (now_cannon < 3) {
				Vec3 boss_pos = boss->GetPosition();

				if (PlayerPtr->GetCannonAnimation()) {
					camera.m_CamerAt = PlayerPtr->GetPosition();
					camera.m_CamerAt.y += 0.7f;
					camera.m_CamerAt.z += 0.5f;
					camera.m_CamerEye = PlayerPtr->GetPosition();
					camera.m_CamerEye.x -= 1.8f;
					camera.m_CamerEye.z += 2.0f;
					camera.m_CamerEye.y += 0.5f;
				}
				else {
					if (now_cannon == 0) {
						vector<shared_ptr<GameObject>> ShootedEnemyVec;
						FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
						auto c = FindTagGameObject<Cannon>(L"GREEN_CANNON");
						int max_num = 0;
						Vec3 ShootPos;
						for (auto enemy : ShootedEnemyVec) {
							if (enemy) {
								auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
								if (PtrEnemy->GetShootNumber() > max_num) {
									max_num = PtrEnemy->GetShootNumber();
									ShootPos = PtrEnemy->GetPosition();
								}
							}
						}
						if (max_num == 0) {
							if (m_FrameCount > 1.0f) {
								CannonStateEndBehaviour();
								PlayerPtr->SetIsCannnon(3);
								m_FrameCount = 0.0f;
								if (boss->GetHP() > 0.0f) {
									boss->ChangeAnimation(L"Default");
								}
								return;
							}
							m_FrameCount += ElapsedTime;
							return;
						}
						c->Rotation(boss_pos);
						camera.m_CamerAt = boss_pos;
						camera.m_CameraArmLen = 30;
						camera.m_CamerEye = ShootPos;
						camera.m_CamerEye.x -= 4.0f;
						camera.m_CamerEye.z -= 7.0f;
					}
					if (now_cannon == 1) {
						vector<shared_ptr<GameObject>> ShootedEnemyVec;
						FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
						auto c = FindTagGameObject<Cannon>(L"RED_CANNON");
						int max_num = 0;
						Vec3 ShootPos;
						for (auto enemy : ShootedEnemyVec) {
							if (enemy) {
								auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
								if (PtrEnemy->GetShootNumber() > max_num) {
									max_num = PtrEnemy->GetShootNumber();
									ShootPos = PtrEnemy->GetPosition();
								}
							}
						}
						if (max_num == 0) {
							if (m_FrameCount > 1.0f) {
								CannonStateEndBehaviour();
								PlayerPtr->SetIsCannnon(3);
								m_FrameCount = 0.0f;
								if (boss->GetHP() > 0.0f) {
									boss->ChangeAnimation(L"Default");
								}
								return;
							}
							m_FrameCount += ElapsedTime;
							return;
						}

						c->Rotation(boss_pos);
						camera.m_CamerAt = boss_pos;
						camera.m_CameraArmLen = 30;
						camera.m_CamerEye = ShootPos;
						camera.m_CamerEye.x -= 3.0f;
						camera.m_CamerEye.z -= 7.0f;
					}
					if (now_cannon == 2) {
						vector<shared_ptr<GameObject>> ShootedEnemyVec;
						FindTagGameObjectVec(L"Shooted", ShootedEnemyVec);
						auto c = FindTagGameObject<Cannon>(L"BLUE_CANNON");
						int max_num = 0;
						Vec3 ShootPos;
						for (auto enemy : ShootedEnemyVec) {
							if (enemy) {
								auto PtrEnemy = dynamic_pointer_cast<EnemyObject>(enemy);
								if (PtrEnemy->GetShootNumber() > max_num) {
									max_num = PtrEnemy->GetShootNumber();
									ShootPos = PtrEnemy->GetPosition();
								}
							}
						}
						if (max_num == 0) {
							if (m_FrameCount > 1.0f) {
								CannonStateEndBehaviour();
								PlayerPtr->SetIsCannnon(3);
								m_FrameCount = 0.0f;
								if (boss->GetHP() > 0.0f) {
									boss->ChangeAnimation(L"Default");
								}
								return;
							}
							m_FrameCount += ElapsedTime;
							return;
						}
						c->Rotation(boss_pos);
						camera.m_CamerAt = boss_pos;
						camera.m_CameraArmLen = 30;
						camera.m_CamerEye = ShootPos;
						camera.m_CamerEye.x -= 2.0f;
						camera.m_CamerEye.z -= 7.0f;
					}
				}
			}
			else if (boss->GetIsLooked()) {

				Vec3 boss_pos = boss->GetPosition();
				camera.m_CamerAt = boss_pos;
				camera.m_CamerAt.y -= 2.0f;
				camera.m_CameraArmLen = 30;
				camera.m_CamerEye = Vec3(-5, 5, 50);
				camera.m_CamerEye.x -= 4.0f;
				camera.m_CamerEye.z -= 7.0f;

			}
			else if (m_isGameStart == false) {
				//�ŏ��̃J�������[�N
				if (m_CameraPos.z > -150) {
					camera.m_CamerAt = Vec3(0, 0, 30);
					camera.m_CameraArmLen = 100;
					//����
					m_CameraPos.z -= ElapsedTime * 500.0f;
					camera.m_CamerEye = m_CameraPos;
				}
				else if (m_CameraPos.z <= -150) {
					m_FogActive = true;
					camera.m_CameraArmLen = 2.6f;
					
					camera.m_CamerAt = PlayerPtr->GetPosition();
					camera.m_CamerAt.y += m_StartCameraY;
					if (m_StartCameraY > 0.5f) {
						m_StartCameraY -= ElapsedTime * 5.0f;
					}
					else {
						m_isGameStart = true;
						SetActiveObjects(true);
					}

					Vec3 CameraLocalEye =
						Vec3(
							sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
							cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
							-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
						);
					camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
				}
			}
			else {
				if (camera.m_CameraArmLen > 20) {
					camera.m_CameraArmLen = 2.6f;
				}

				if (!m_isClear) {
					if (CntlVec[0].fThumbRX != 0) {
						camera.m_CameraXZRad -= CntlVec[0].fThumbRX * 0.05f;
						if (abs(camera.m_CameraXZRad) >= XM_2PI) {
							camera.m_CameraXZRad = 0;
						}
					}
					if (CntlVec[0].fThumbRY != 0) {
						camera.m_CameraYRad += CntlVec[0].fThumbRY * 0.05f;
						if (camera.m_CameraYRad >= 2.0f) {
							camera.m_CameraYRad = 2.0f;
						}
						else if (camera.m_CameraYRad <= 0.5f) {
							camera.m_CameraYRad = 0.5f;
						}
						if (camera.m_CameraYRad >= XM_PIDIV2 - 0.3f) {
							if (CntlVec[0].fThumbRY > 0) {
								camera.m_CameraArmLen -= CntlVec[0].fThumbRY * 0.1f;
								if (GetCamera().m_CameraArmLen <= 1.5f) {
									camera.m_CameraArmLen = 1.5f;
								}
							}
							else if (CntlVec[0].fThumbRY < 0) {
								camera.m_CameraArmLen -= CntlVec[0].fThumbRY * 0.1f;
								if (GetCamera().m_CameraArmLen >= 50.0f) {
									GetCamera().m_CameraArmLen = 50.0f;
								}
							}
						}
					}
				}

				camera.m_CamerAt = PlayerPtr->GetPosition();
				camera.m_CamerAt.y += 0.5f;

				Vec3 CameraLocalEye =
					Vec3(
						sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
						cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
						-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
					);
				camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
			}
			//B�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				//PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
				//SetIsFail(true);
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

	void GameStage::SetActiveObjects(bool value) {
		vector<shared_ptr<GameObject>> ZakoVec;
		FindTagGameObjectVec(L"EnemyObject", ZakoVec);
		for (auto zako : ZakoVec) {
			if (zako) {
				auto PtrZako = dynamic_pointer_cast<EnemyObject>(zako);
				PtrZako->SetUpdateActive(value);
			}
		}
		vector<shared_ptr<GameObject>> BulletVec;
		FindTagGameObjectVec(L"AllBullet", BulletVec);
		for (auto bullet : BulletVec) {
			if (bullet) {
				auto PtrBullet = dynamic_pointer_cast<BulletObject>(bullet);
				PtrBullet->SetActive(value);
			}
		}
		vector<shared_ptr<GameObject>> HandVec;
		FindTagGameObjectVec(L"BossHand", HandVec);
		for (auto hand : HandVec) {
			if (hand) {
				auto PtrHand = dynamic_pointer_cast<BossHand>(hand);
				PtrHand->SetUpdateActive(value);
			}
		}
		auto player = FindTagGameObject<Player>(L"Player");
		if (player) {
			player->SetUpdateActive(value);
		}
	}

	//�C�����n�܂����Ƃ��G�̓������~�߂�
	void GameStage::CannonStateStartBehaviour() {
		SetActiveObjects(false);
	}

	//�C�����I�������̌Ă΂��֐�
	void GameStage::CannonStateEndBehaviour() {
		SetActiveObjects(true);
	}

	//�Q�[���N���A�������Ƃɂ�΂��
	void GameStage::GameClearBehaviour() {
		m_AudioObjectPtr->Stop(L"BGM_1");
		
		SetActiveObjects(false);

		AddGameObject<ResultSprite>(
			L"RESULT_BACK",
			Vec2(1280, 720),
			0.0f,
			Vec2(0, 0),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"CLEAR_LOGO",
			Vec2(600, 300),
			0.0f,
			Vec2(0, 250),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TIME_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-200, -30),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"LIFE_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-250, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"PAERCENT_LOGO",
			Vec2(80, 80),
			0.0f,
			Vec2(300, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RANK_LOGO",
			Vec2(400, 100),
			0.0f,
			Vec2(-100, 120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RESULT_S",
			Vec2(180, 180),
			0.0f,
			Vec2(110, 120),
			1, 1
			);

		AddGameObject<ResultCursorSprite>(
			L"RESULT_ARROW",
			Vec2(320, 80),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"RETRY_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TO_TITLE_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(130, -240),
			1, 1
			);
	}

	//�Q�[�����[�΁[�����Ƃ�
	void GameStage::GameOverBehaviour() {
		m_AudioObjectPtr->Stop(L"BGM_1");

		SetActiveObjects(false);

		AddGameObject<ResultSprite>(
			L"RESULT_BACK",
			Vec2(1280, 720),
			0.0f,
			Vec2(0, 0),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"FAIL_LOGO",
			Vec2(600, 300),
			0.0f,
			Vec2(0, 250),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TIME_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-200, -30),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"LIFE_LOGO",
			Vec2(320, 80),
			0.0f,
			Vec2(-250, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"PAERCENT_LOGO",
			Vec2(80, 80),
			0.0f,
			Vec2(300, -120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RANK_LOGO",
			Vec2(400, 100),
			0.0f,
			Vec2(-100, 120),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"RESULT_D",
			Vec2(180, 180),
			0.0f,
			Vec2(110, 120),
			1, 1
			);
		AddGameObject<ResultCursorSprite>(
			L"RESULT_ARROW",
			Vec2(320, 80),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);
		AddGameObject<ResultSprite>(
			L"RETRY_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(-130, -240),
			1, 1
			);

		AddGameObject<ResultSprite>(
			L"TO_TITLE_LOGO",
			Vec2(200, 50),
			0.0f,
			Vec2(130, -240),
			1, 1
			);
		
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
