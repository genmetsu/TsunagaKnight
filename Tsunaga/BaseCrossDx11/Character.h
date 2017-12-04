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

		float m_player_HP;
		int m_friends_num;
		
	public:
		static GameManager* getInstance();
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
		void InsertFire(const Vec3& Pos);
	};

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
	///	�w�i�X�v���C�g
	//--------------------------------------------------------------------------------------
	class SkySprite : public SpriteBase {
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
		SkySprite(const shared_ptr<Stage>& StagePtr,
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
		virtual ~SkySprite() {}
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
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;

		//�ϋv�x
		float m_HP;

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
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
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
		float m_TackleDis;
		//�ˌ��̃X�s�[�h
		float m_TackleSpeed;
		float m_HP;
		float m_AttackPoint;

		//���񂾂��ǂ���
		bool m_isDead;

		Vec3 m_TackleStart;

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
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
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
		@brief �X�e�[�g���ʏ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior();
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
		void SetParent(weak_ptr<GameObject> gameObject) {
			m_ParentPtr = gameObject;
		};

		float GetHP(){
			return m_HP;
		}
		void SetHP(float Value){
			m_HP = Value;
		}

		float GetAttackPoint(){
			return m_AttackPoint;
		}
		void SetAttackPoint(float Value){
			m_AttackPoint = Value;
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
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
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
	};

	//--------------------------------------------------------------------------------------
	///	�e�����G
	//--------------------------------------------------------------------------------------
	class ShootEnemy : public EnemyObject
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
		ShootEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShootEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
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
		float m_TackleDis;
		//�ːi�̃X�s�[�h
		float m_TackleSpeed;
		//�e�̃X�s�[�h
		float m_ShootSpeed;
		float m_HP;
		float m_AttackPoint;

		Vec3 m_TackleStart;

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
		// �e��������Ă��邩�ǂ���
		bool IsShoot;

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
			bool OwnShadowActive,const wstring& Tag);
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
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;

		virtual Vec3 GetPosition() override;
		 void SetPosition(Vec3 pos);

		 void Wakeup(const Vec3& Position, const Vec3& Velocity);

		 bool GetIsShoot()
		 {
			 return IsShoot;
		 }

		 


	};

	//--------------------------------------------------------------------------------------
	/// �{�X�L�����@�ł���
	//--------------------------------------------------------------------------------------

	class BossEnemy : public EnemyObject
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
		BossEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr, const wstring& MeshResName,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BossEnemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�Β��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OppositionBehavior() override;
		
	};

}
//end basecross
