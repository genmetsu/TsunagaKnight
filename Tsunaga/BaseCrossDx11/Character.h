/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	�Q�[���S�̂��Ǘ�����}�l�[�W���[�A�X�|�i�[
	//--------------------------------------------------------------------------------------
	class GameManager {
	private:
		GameManager() {
			m_friends_num = 0;
		}
		~GameManager() {}
		static GameManager* GM;

		float m_Player_HP;
		float m_Cannon_HP;
		float m_Boss_HP;
		float m_Default_Cannon_HP;
		float m_Default_Boss_HP;
		int m_friends_num;
		
	public:
		float m_camera_length;
		static GameManager* getInstance();

		void SetPlayerHP(float value) {
			m_Player_HP = value;
		}
		float GetPlayerHP() {
			return m_Player_HP;
		}
		void SetCannonHP(float value) {
			m_Cannon_HP = value;
		}
		float GetCannonHP() {
			return m_Cannon_HP;
		}
		void SetDefaultCannonHP(float value) {
			m_Default_Cannon_HP = value;
		}
		float GetDefaultCannonHP() {
			return m_Default_Cannon_HP;
		}

		void SetDefaultBossHP(float value) {
			m_Default_Boss_HP = value;
		}
		float GetDefaultBossHP() {
			return m_Default_Boss_HP;
		}

		void SetBossHP(float value) {
			m_Boss_HP = value;
		}
		float GetBossHP() {
			return m_Boss_HP;
		}

		void SetFriendsNum(int value) {
			m_friends_num = value;
		};
		int GetFriendsNum() {
			return m_friends_num;
		};
	};
	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	class SquareObject : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//�@���}�b�v�p�e�N�X�`�����\�[�X��
		wstring m_NormalTextureResName;
		Vec3 m_Scale;				///<�X�P�[�����O
		Quat m_Qt;			///<��]
		Vec3 m_Pos;				///<�ʒu
		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTnTStaticRenderer> m_Renderer;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̍쐬
		@param[in]	WrapX	X�����̃^�C�����O
		@param[in]	WrapY	Y�����̃^�C�����O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateBuffers(float WrapX, float WrapY);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	NormalTextureResName	�@���}�b�v�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const wstring& NormalTextureResName,
			const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
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

	//--------------------------------------------------------------------------------------
	///	�X�e�[�W�̃��}�g�N���X
	//--------------------------------------------------------------------------------------
	class YamatoStage : public GameObject{
	protected:
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;

		//���b�V���Ƃ̍����v�Z�p
		Mat4x4 m_MeshToTransformMatrix;
		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		YamatoStage(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos
			, bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~YamatoStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
	};

	//--------------------------------------------------------------------------------------
	///	�����ȕ�
	//--------------------------------------------------------------------------------------
	class TransparentWall : public GameObject {
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		TransparentWall(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TransparentWall();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {};
	};

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class AttackSpark : public MultiParticle;
	//�p�r: �U���̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	class AttackSpark : public MultiParticle {
	public:
		//�\�z�Ɣj��
		AttackSpark(shared_ptr<Stage>& StagePtr);
		virtual ~AttackSpark();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class MultiGuardEffect : public MultiParticle;
	//�p�r: �G���W�F���G�l�~�[�̖h��G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class MultiGuardEffect : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiGuardEffect(shared_ptr<Stage>& StagePtr);
		virtual ~MultiGuardEffect();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class BossEffect : public MultiParticle;
	//�p�r �{�X�̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class BossEffect : public MultiParticle {
	public:
		//�\�z�Ɣj��
		BossEffect(shared_ptr<Stage>& StagePtr);
		virtual ~BossEffect();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos,wstring name);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class EnemyMoveEffect : public MultiParticle;
	//�p�r: �G�l�~�[�̈ړ��G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class EnemyMoveEffect : public MultiParticle {
	public:
		//�\�z�Ɣj��
		EnemyMoveEffect(shared_ptr<Stage>& StagePtr);
		virtual ~EnemyMoveEffect();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class MultiFire : public MultiParticle;
	//�p�r: �����̉��N���X
	//--------------------------------------------------------------------------------------
	class MultiFire : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiFire(shared_ptr<Stage>& StagePtr);
		virtual ~MultiFire();
		//������
		virtual void OnCreate() override;
		void InsertFire(const Vec3& Pos,float Scale);
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//class AttackSigns : public MultiParticle;
	//�p�r: �G���̍U������������G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class AttackSigns : public MultiParticle {
	public:
		//�\�z�Ɣj��
		AttackSigns(shared_ptr<Stage>& StagePtr);
		virtual ~AttackSigns();
		//������
		virtual void OnCreate() override;
		void InsertSigns(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class BossAttackSigns : public MultiParticle;
	//�p�r: �{�X�̍U������������G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class BossAttackSigns : public MultiParticle {
	public:
		//�\�z�Ɣj��
		BossAttackSigns(shared_ptr<Stage>& StagePtr);
		virtual ~BossAttackSigns();
		//������
		virtual void OnCreate() override;
		void InsertSigns(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class CannonEffect : public MultiParticle;
	//�p�r: �C���̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class CannonEffect : public MultiParticle {
	public:
		//�\�z�Ɣj��
		CannonEffect(shared_ptr<Stage>& StagePtr);
		virtual ~CannonEffect();
		//������
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class StepEffect : public MultiParticle;
	//�p�r: �X�e�b�v�̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class StepEffect : public MultiParticle {
	public:
		//�\�z�Ɣj��
		StepEffect(shared_ptr<Stage>& StagePtr);
		virtual ~StepEffect();
		//������
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	//class ShootingEffect : public MultiParticle;
	//�p�r: ��C�Ō�����Ă���Ƃ��̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	class ShootingEffect : public MultiParticle {
	public:
		//�\�z�Ɣj��
		ShootingEffect(shared_ptr<Stage>& StagePtr);
		virtual ~ShootingEffect();
		//������
		virtual void OnCreate() override;
		void InsertEffect(const Vec3& Pos);
		virtual void OnUpdate() override;

	};


	//--------------------------------------------------------------------------------------
	///	���[���h�s����擾����C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class MatrixInterface {
	protected:
		MatrixInterface() {}
		virtual ~MatrixInterface() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾�i�������z�֐��j
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const = 0;
	};


	//--------------------------------------------------------------------------------------
	///	���b�s���O�������ꂽ�X�v���C�g�i�e�j
	//--------------------------------------------------------------------------------------
	class SpriteBase : public GameObject {
	public:
		//�u�����h���@
		enum class BlendState {
			Opaque,
			Trace,
			Additive
		};
	protected:
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		//�o�b�N�A�b�v���Ă������_�f�[�^
		vector<VertexPositionColorTexture> m_BackupVertices;
		wstring m_TextureResName;		///<�e�N�X�`�����\�[�X��
		Vec2 m_Scale;				///<�X�P�[�����O
		float m_Rot;				///<��]
		Vec2 m_Pos;				///<�ʒu
		UINT m_XWrap;		//X�����̃��b�v��
		UINT m_YWrap;		//Y�����̃��b�v��
		Col4 m_Emissive;	//�G�~�b�V�u�F
		BlendState m_BlendState;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		SpriteBase(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap,UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SpriteBase() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�ύX������
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime,VertexPositionColorTexture* vertices) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`�����\�[�X���̎擾
		@return	�e�N�X�`�����\�[�X��
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureResName() const {
			return m_TextureResName;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`�����\�[�X���̕ύX
		@param[in]	col	���\�[�X��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResName(const wstring& textureResName) {
			m_TextureResName = textureResName;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �u�����h���@�̎擾
		@return	�u�����h���@
		*/
		//--------------------------------------------------------------------------------------
		const BlendState& GetBlendState() const {
			return m_BlendState;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �u�����h���@�̕ύX
		@param[in]	col	�ύX�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(BlendState blendState) {
			m_BlendState = blendState;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�~�b�V�u�F�̎擾
		@return	�G�~�b�V�u�F
		*/
		//--------------------------------------------------------------------------------------
		const Col4& GetEmissive() const {
			return m_Emissive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�~�b�V�u�F�̕ύX
		@param[in]	col	�ύX�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const Col4& col) {
			m_Emissive = col;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
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


	//--------------------------------------------------------------------------------------
	///	��]����X�v���C�g
	//--------------------------------------------------------------------------------------
	class RotateSprite : public SpriteBase {
		float m_TotalTime;	//���_�ύX�Ɏg�p����^�C��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		RotateSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, 
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RotateSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	���b�Z�[�W��\������X�v���C�g
	//--------------------------------------------------------------------------------------
	class MessageSprite : public SpriteBase {
		float m_TotalTime;	//���_�ύX�Ɏg�p����^�C��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		MessageSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, 
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MessageSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	���b�Z�[�W��\������X�v���C�g
	//--------------------------------------------------------------------------------------
	class ResultSprite : public SpriteBase {
		float m_TotalTime;	//���_�ύX�Ɏg�p����^�C��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		ResultSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ResultSprite() {}
	};

	//--------------------------------------------------------------------------------------
	/// ��C�̃Q�[�W
	//--------------------------------------------------------------------------------------
	class CannonGauge : public SpriteBase {
		float m_TotalTime;	//���_�ύX�Ɏg�p����^�C��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		CannonGauge(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CannonGauge() {}
	};

	//--------------------------------------------------------------------------------------
	/// �{�X�̗̑̓Q�[�W
	//--------------------------------------------------------------------------------------
	class BossHPGauge : public SpriteBase {
		float m_TotalTime;	//���_�ύX�Ɏg�p����^�C��
		float m_DefalutHP;
		float m_DefaultSize;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		BossHPGauge(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BossHPGauge() {}
	};

	//--------------------------------------------------------------------------------------
	///	�w�i�X�v���C�g
	//--------------------------------------------------------------------------------------
	class MultiSprite : public SpriteBase {
		float m_TotalTime;	//���_�ύX�Ɏg�p����^�C��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �o�b�N�A�b�v���_�̏C��(���z�֐�)�B�h���N���X�͓Ǝ��̒��_�����C��������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���_�̕ύX
		@param[in]	ElapsedTime	�^�[������
		@param[out]	vertices	�}�b�v���ꂽ���_�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�����e�N�X�`�����\�[�X��
		@param[in]	StartScale	�����傫��
		@param[in]	StartRot	������]
		@param[in]	StartPos	�����ʒu
		@param[in]	XWrap	X�����̃��b�v��
		@param[in]	YWrap	Y�����̃��b�v��
		*/
		//--------------------------------------------------------------------------------------
		MultiSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	SkyBox
	//--------------------------------------------------------------------------------------
	class SkyBox : public GameObject {
	protected:
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;

		//�`��f�[�^
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<SimplePNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		SkyBox(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SkyBox();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
	};

	//--------------------------------------------------------------------------------------
	///	��C�N���X
	//--------------------------------------------------------------------------------------
	class Cannon : public GameObject, public MatrixInterface {
	protected:
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//��C���ʗp
		int myClass;

		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;

		//�ϋv�x
		float m_HP;
		//�ŏ��̑ϋv�x
		float m_DefaultHP;

		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;

		bool m_UpdateActive;

		//���b�V���Ƃ̍����v�Z�p
		Mat4x4 m_MeshToTransformMatrix;
		//�`��f�[�^
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<SimplePNTStaticRenderer2> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		Cannon(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const wstring& TagName,const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			int CannonClass,bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Cannon();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������HP�̏�ԃ`�F�b�N
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------

		void Rotation(Vec3 vec);
		void CheckHealth();

		float GetHP() {
			return m_HP;
		}
		void SetHP(float Value) {
			m_HP = Value;
		}
		float GetScale() {
			return m_Scale.x;
		}
		int GetCannonClass() {
			return myClass;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�G�I�u�W�F�N�g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class EnemyObject : public GameObject, public MatrixInterface {
	protected:
		//���b�V�����\�[�X��
		wstring m_MeshResName;
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//�f�t�H���g�̃A�j���[�V����
		wstring m_DefaultAnimation;
		//���b�V���Ƃ̍����v�Z�p
		Mat4x4 m_MeshToTransformMatrix;
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;
		//�ЂƂO�̈ʒu
		Vec3 m_BeforePos;
		// �ːi�̈ʒu���W
		Vec3 m_TargetPos;
		//�ǂ�������X�s�[�h
		float m_Speed;
		//�t���[���J�E���g
		float m_FrameCount;
		//�ˌ����邩�ǂ���
		bool m_Tackle;
		//�ˌ�����ۂɎ~�܂鎞��
		float m_StopTime;
		//�ǂꂮ�炢�߂Â�����ˌ����邩�̋���
		float m_SearchDis;
		//�ˌ��̃X�s�[�h
		float m_TackleSpeed;
		//�ːi����
		float m_TackleTime;
		//�U����̍d������
		float m_AfterAttackTime;
		//�G���W�F���G�l�~�[���牽�ԖڂɈʒu���邩
		int m_FollowingAngelNum;

		float m_HP;
		//�ŏ���HP
		float m_DefaultHP;

		float m_AttackPoint;
		//�������Œ肷��Y���̍���
		float m_BaseY;

		Vec3 m_CannonPos;
		Vec3 m_BossPos;
		Vec3 m_ToBossVec;

		//��C�ɃZ�b�g���ꂽ�Ƃ����Ԗڂɔ��˂���邩
		int m_ShootNumber;

		bool m_Bomb;

		bool m_UpdateActive;

		//���񂾂��ǂ���
		bool m_isDead;
		//�ːi����ۂ̍ŏ��̃|�W�V����
		Vec3 m_TackleStartPos;

		//�v���C���[�̃|�C���^
		weak_ptr<GameObject> m_PlayerPtr;
		//�e�I�u�W�F�N�g
		weak_ptr<GameObject> m_ParentPtr;
		//
		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		//�T�E���h�I�u�W�F�N�g
		shared_ptr<SoundObject> m_DeadSound;
		shared_ptr<SoundObject> m_BossDamageSound;
		shared_ptr<SoundObject> m_FriendsSound;
		shared_ptr<SoundObject> m_CannonSound;
		shared_ptr<SoundObject> m_EyeFlashSound;

		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;
		//���̃I�u�W�F�N�g�̃v���C���[���猩�����[�J���s��
		Mat4x4 m_PlayerLocalMatrix;
		//�v���C���[�̒���i�擪�j�̏ꍇ�̕�ԌW��
		float m_LerpToParent;
		//���̃I�u�W�F�N�g�̃`���C���h�I�u�W�F�N�g���猩�����[�J���s��
		Mat4x4 m_ChildLocalMatrix;
		//�`���C���h��̏ꍇ�̕�ԌW��
		float m_LerpToChild;
		//Attack1�̏ꍇ�̖ڕW�ƂȂ��]
		float m_Attack1ToRot;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<EnemyObject>>  m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		EnemyObject(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EnemyObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<EnemyObject> >& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(Vec3 Pos) {
			m_Rigidbody->m_Pos = Pos;
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Ǐ]����s���̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ComplianceStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �U���P�s���̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Attack1StartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �U���P�s���̌p��
		@return	�s�����I��������true
		*/
		//--------------------------------------------------------------------------------------
		bool Attack1ExcuteBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ��C�ɍ��߂��ă{�X�Ɍ������Ă����ŏ��ɌĂ΂�鏀������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void BulletStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ��C�ɍ��߂��ă{�X�Ɍ������Ă�������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void BulletExcuteBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G���X�|�[��������ۂ̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Spawn();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�g���ʏ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���ԂɂȂ��Ă��Ă���s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void FriendsBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �U����̍d������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AttackEndBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ��C�Ɍ��������̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ToCannonBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���C���[�̒e�Ƃ̏Փ˔���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CollisionBullet();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������HP�̏�ԃ`�F�b�N
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CheckHealth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�̐ݒ���s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CheckParent();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G���W�F���G�l�~�[���牽�Ԗڂɂ��邩�擾����
		@return	m_FollowingAngelNum
		*/
		//--------------------------------------------------------------------------------------
		int GetFollowingAngelNum() {
			return m_FollowingAngelNum;
		}
		void SetFollowingAngelNum(int value) {
			m_FollowingAngelNum = value;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�̊m�F���s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------

		void RotateToVelocity();

		void SetParent(weak_ptr<GameObject> gameObject) {
			m_ParentPtr = gameObject;
		};

		void ChangeState(wstring name);

		float GetHP(){
			return m_HP;
		}
		void SetHP(float Value){
			m_HP = Value;
		}

		void Damage(float value) {
			m_HP -= value;
		}

		void SetShootNumber(int value) {
			m_ShootNumber = value;
		}
		int GetShootNumber() {
			return m_ShootNumber;
		}

		float GetAttackPoint(){
			return m_AttackPoint;
		}
		void SetAttackPoint(float Value){
			m_AttackPoint = Value;
		}

		void SetUpdateActive(bool value) {
			m_UpdateActive = value;
		}
		bool GetUpdateActive() {
			return m_UpdateActive;
		}

		float GetScale(){
			return m_Scale.x;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�G�΃X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	class EnemyOppositionState : public ObjState<EnemyObject>
	{
		EnemyOppositionState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyOppositionState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�U����̍d���X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	class EnemyAttackEndState : public ObjState<EnemyObject>
	{
		EnemyAttackEndState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyAttackEndState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	��C�Ɍ������X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	class EnemyToCannonState : public ObjState<EnemyObject>
	{
		EnemyToCannonState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyToCannonState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�Ǐ]�X�e�[�g�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	class EnemyComplianceState : public ObjState<EnemyObject>
	{
		EnemyComplianceState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyComplianceState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	��C�Ō�����Ă�Ƃ��̃X�e�[�g
	//--------------------------------------------------------------------------------------
	class EnemyBulletState : public ObjState<EnemyObject>
	{
		EnemyBulletState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyBulletState)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�U���X�e�[�g�P�iEnemyObject�j
	//--------------------------------------------------------------------------------------
	class EnemyAttack1State : public ObjState<EnemyObject>
	{
		EnemyAttack1State() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyAttack1State)
		virtual void Enter(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Execute(const shared_ptr<EnemyObject>& Obj)override;
		virtual void Exit(const shared_ptr<EnemyObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�j�[�h���G�l�~�[�i�ߐڍU���j
	//--------------------------------------------------------------------------------------

	class NeedleEnemy : public EnemyObject
	{
	
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		NeedleEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~NeedleEnemy();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior() override;

		virtual void OnUpdate() override;


	};

	//--------------------------------------------------------------------------------------
	///	�e�����G
	//--------------------------------------------------------------------------------------
	class ShootEnemy : public EnemyObject
	{
		//�G�Ύ��̑ł��o���e�̑��x
		float m_EnemyShootSpeed;
		//�Ȃ����Ă��鎞�̌����o���e�̑��x
		float m_PlayerShootSpeed;
		//�Ȃ����Ă���Ƃ��̒e�̌����o���p�x
		float m_PlayerShootTime;

		shared_ptr<SoundObject> m_ShootSound;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		ShootEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShootEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�揈��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	};

	class BulletObject : public GameObject, public MatrixInterface {
	protected:
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;
		// �ːi�̈ʒu���W
		Vec3 m_TargetPos;
		//�ǂ�������X�s�[�h
		float m_Speed;
		//�t���[���J�E���g
		float m_FrameCount;

		wstring m_my_Tag;

		//�e�I�u�W�F�N�g
		weak_ptr<GameObject> m_ParentPtr;
		//
		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;

		//�`��f�[�^
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<SimplePNTStaticRenderer2> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;
		
		bool m_UpdateActive;
		// �e��������Ă��邩�ǂ���
		bool IsShoot;
		// �e�̑��ݎ���
		float m_BulletTime;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		BulletObject(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive, const wstring& Tag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BulletObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;

		virtual Vec3 GetPosition() override;

		void SetPosition(Vec3 pos);

		float GetScale(){
			return m_Scale.x;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��ł��o������
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void Wakeup(const Vec3& Position, const Vec3& Velocity);

		bool GetIsShoot()
		{
			return IsShoot;
		}
	};

	//--------------------------------------------------------------------------------------
	/// �G���W�F���G�l�~�[
	//--------------------------------------------------------------------------------------
	class AngelEnemy : public EnemyObject
	{

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		AngelEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AngelEnemy();
	};

	//--------------------------------------------------------------------------------------
	/// ���{�X�L�����@�ߋ��� 
	/// CR = Close Range
	//--------------------------------------------------------------------------------------

	class CR_BossEnemy : public EnemyObject
	{
		//�`��f�[�^
		shared_ptr<BcDrawObject> m_SimpleObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_StaticRenderer;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		CR_BossEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CR_BossEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() override;
	};

	//--------------------------------------------------------------------------------------
	/// ���{�X�L�����@�ߋ��� �� ��
	//--------------------------------------------------------------------------------------
	class BossHand : public GameObject, public MatrixInterface {
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;
		float m_FrameCount;
		//�U�������̎���
		float m_AttackSetupTime;
		//���U�艺�낷�O�̍d������
		float m_BeforeAttackTime;
		//�U������
		float m_AttackTime;
		//���̈ʒu�ɖ߂鎞��
		float m_ReturnDefaultTime;

		Vec3 m_AttackPos;
		Vec3 m_DefaultPos;
		Vec3 m_BeforePos;

		bool m_UpdateActive;
		//�e�I�u�W�F�N�g
		weak_ptr<GameObject> m_ParentPtr;
		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		//���b�V���Ƃ̍����v�Z�p
		Mat4x4 m_MeshToTransformMatrix;
		///�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;
		//���̃I�u�W�F�N�g���猩�����[�J���s��
		Mat4x4 m_LocalMatrix;
		//�e�Ƃ̕�ԌW��
		float m_LerpToParent;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<BossHand>>  m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		BossHand(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr,
			const wstring& TextureResName, const wstring& TagName,
			const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BossHand();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate2()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<BossHand> >& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�s���̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ComplianceStartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �U���P�s���̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Attack1StartBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �U���P�s���̌p��
		@return	�s�����I��������true
		*/
		//--------------------------------------------------------------------------------------
		bool Attack1ExcuteBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�g���ʏ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�g��ύX����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetState(wstring state_name);

		void SetUpdateActive(bool value) {
			m_UpdateActive = value;
		}
		bool GetUpdateActive() {
			return m_UpdateActive;
		}

		Vec3 GetPosition() {
			return m_Rigidbody->m_Pos;
		}
		float GetScale() {
			return m_Scale.y;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�U�����Ȃ��X�e�[�g�iBossHand�j
	//--------------------------------------------------------------------------------------
	class HandDefaultState : public ObjState<BossHand>
	{
		HandDefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(HandDefaultState)
		virtual void Enter(const shared_ptr<BossHand>& Obj)override;
		virtual void Execute(const shared_ptr<BossHand>& Obj)override;
		virtual void Exit(const shared_ptr<BossHand>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�U���X�e�[�g�iBossHand�j
	//--------------------------------------------------------------------------------------
	class HandAttackState : public ObjState<BossHand>
	{
		HandAttackState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(HandAttackState)
		virtual void Enter(const shared_ptr<BossHand>& Obj)override;
		virtual void Execute(const shared_ptr<BossHand>& Obj)override;
		virtual void Exit(const shared_ptr<BossHand>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	/// ���{�X�L�����@������
	/// LD = Long Distance
	//--------------------------------------------------------------------------------------

	class LD_BossEnemy : public EnemyObject
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		LD_BossEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const wstring& DefaultAnimation, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~LD_BossEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		//virtual void OppositionBehavior() override;

	};

	//--------------------------------------------------------------------------------------
	/// �G�̃{�X�D
	//--------------------------------------------------------------------------------------
	class Boss : public GameObject {
	protected:
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;

		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;
		//HP
		float m_HP;
		//�ŏ���HP
		float m_DefaultHP;

		bool m_isDead;

		int m_now_barrior;

		bool m_UpdateActive;

		float m_frame_count;

		bool m_isDamage;

		float m_DamageRate;

		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		//���b�V���Ƃ̍����v�Z�p
		Mat4x4 m_MeshToTransformMatrix;
		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		bool m_OwnShadowActive;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ParentPtr	�e�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		@param[in]	OwnShadowActive	�e�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		Boss(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos
			, bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Boss();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief
		@return
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(Vec3 Pos) {
			m_Rigidbody->m_Pos = Pos;
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief �_���[�W����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Damage(float value);

		int GetNowBarrior() {
			return m_now_barrior;
		}

		float GetHP() {
			return m_HP;
		}

		void SetIsDamage(bool value) {
			m_isDamage = value;
			
		}
		bool GetIsDamage() {
			return m_isDamage;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�j���[�V�����̕ύX
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangeAnimation(wstring anim);
	};

}
//end basecross
