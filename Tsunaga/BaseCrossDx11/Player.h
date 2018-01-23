/*!
@file Player.h
@brief �v���C���[
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	���̂̃v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject, public MatrixInterface {
		///���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		Mat4x4 m_MeshToTransformMatrix;
		///�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		///�X�P�[�����O�x�[�X�̍ŉ��n�_
		float m_BaseY;
		///�ʒu
		Vec3 m_Posision;
		///�����������邩�ǂ���
		bool m_Trace;
		///�W�����v�̃��b�N
		bool m_JumpLock;
		/// �X�e�b�v�i����s���j���Ă邩�ǂ���
		bool m_isStep;

		bool m_UpdateActive;

		bool m_CannonAnimation;
		///�C�������ꍇ��bool
		int m_NowCannonClass;
		//���G���ǂ���
		bool m_isInvincible;

		shared_ptr<SoundObject> m_AttackSound;
		shared_ptr<SoundObject> m_StepSound;
		shared_ptr<SoundObject> m_DamageSound;
		shared_ptr<SoundObject> m_BombSound;

		//�ړ����Ă��邩�ǂ���
		enum RunningMode {
			RunStart,Running,RunEnd
		};
		RunningMode isRunning;
		/// �X�e�b�v�������
		Vec3 m_StepVec;
		/// �m�b�N�o�b�N�������
		Vec3 m_KnockBackVec;
		/// �t���[���J�E���g�p�ϐ�
		float m_FrameCount;
		//�U������Ƃ��ɕ␳�������鋗��
		float m_AttackDis;
		//�U������Ƃ��ɕ␳��������p�x
		float m_FOV;

		float m_CannonHP;
		float m_DefaultCannonHP;
		//�ړ��A�j���[�V�����p�t���[���J�E���g
		float m_RunAnimationFrameCount;
		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		///�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

		shared_ptr<GameObject> m_Sword;
		
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Player>>  m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	TextureResName	�e�N�X�`�����\�[�X��
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, bool Trace, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player();
		///�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʒu�𓾂�
		@return	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʒu��ς���
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(Vec3 pos) {
			m_Rigidbody->m_Pos = pos;
		}

		float GetScale(){
			return m_Rigidbody->m_Scale.x;
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
		@brief ���[���h�s��̎擾
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetWorldMatrix(Mat4x4& m) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���g���[����������x�N�g���𓾂�
		@return	�����x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���͊֘A�̊֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void MoveControll();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �_���[�W����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Damage(Vec3 EnemyPos);

		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DefaultBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���x��傫�����炷����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InitVelocity();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ړ����̃A�Ƀ��[�V�����ύX
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RunningAnimation();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�b�v�̃G�t�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void PlayerStepEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�b�v�̃x�N�g���̐ݒ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetStepVec(Vec3 step_vec) {
			m_FrameCount = 0.0f;
			m_StepVec = step_vec;
			m_StepVec.normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ړ��x�N�g���̎擾
		@return	�ړ��x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetVelocity() {
			return m_Rigidbody->m_Velocity;
		}

		//void StepStartBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�b�v���̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void StepBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �U�����̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AttackBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �Ȃ���U���̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChainAttackBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief ��C�Ƃ̓����蔻��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CollisionWithCannon();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �_���[�W���󂯂����ŏ��̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DamagedStartBehaviour(Vec3 StartPos);

		//--------------------------------------------------------------------------------------
		/*!
		@brief �_���[�W���󂯂����̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DamagedBehaviour();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �_���[�W���󂯖��G��Ԃ̂ӂ�܂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AfterDamagedBehaviour();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�j���[�V�����̕ύX
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangeAnimation(wstring animation_key);

		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�g��ύX
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangeDefaultState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ����A�j���[�V���������Z�b�g����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InitRunFrameCount() {
			m_RunAnimationFrameCount = 0.0f;
			isRunning = RunEnd;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�����Ă邩�ǂ�����bool�̃Q�b�^�[�Z�b�^�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetIsCannnon(int is_cannon) {
			m_NowCannonClass = is_cannon;
		}
		int GetIsCannon() {
			return m_NowCannonClass;
		}

		void SetInvincible(bool value) {
			m_isInvincible = value;
		}

		bool GetInvincible() {
			return m_isInvincible;
		}

		void SetKnockBackVec(Vec3 vec) {
			m_KnockBackVec = vec;
		}

		float GetDefaultBossHP() {
			return m_DefaultCannonHP;
		}

		void SetUpdateActive(bool value) {
			m_UpdateActive = value;
		}
		bool GetUpdateActive() {
			return m_UpdateActive;
		}

		void SetCannonHP(float value) {
			m_CannonHP = value;
		}
		float GetCannonHP() {
			return m_CannonHP;
		}
		void CannonDamage(float value) {
			m_CannonHP -= value;
		}

		bool GetCannonAnimation() {
			return m_CannonAnimation;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Player> >& GetStateMachine() {
			return m_StateMachine;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class DefaultState : public ObjState<Player>
	{
		DefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(DefaultState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�U���X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerAttackState : public ObjState<Player>
	{
		PlayerAttackState() {}
	private:
		float frame_count;
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerAttackState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	����s���X�e�[�g
	//--------------------------------------------------------------------------------------
	class StepState : public ObjState<Player>
	{
		StepState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(StepState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�_���[�W���󂯂��ۂ̃X�e�[�g
	//--------------------------------------------------------------------------------------
	class DamagedState : public ObjState<Player>
	{
		DamagedState() {}
	private:
		float frame_count;
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(DamagedState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�Ȃ�����g�����U���X�e�[�g
	//--------------------------------------------------------------------------------------
	class ChainAttackState : public ObjState<Player>
	{
		ChainAttackState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(ChainAttackState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�v���C���[�̌�(���p�j
	//--------------------------------------------------------------------------------------
	class Sword : public GameObject, public MatrixInterface {
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		//�X�P�[�����O
		Vec3 m_Scale;
		//��]
		Quat m_Qt;
		//�ʒu
		Vec3 m_Pos;

		//�e�I�u�W�F�N�g
		weak_ptr<GameObject> m_ParentPtr;
		//
		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;

		//������`��I�u�W�F�N�g
		shared_ptr<StringDrawObject> m_StringDrawObject;

		shared_ptr<SoundObject> m_AttackSound;

		///�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
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
		//���ɂ��Ă���G�l�~�[
		vector<weak_ptr<GameObject>> m_friends;
		//���ɂ��Ă���G�l�~�[�̐�
		int m_friends_num;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Sword>>  m_StateMachine;
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
		Sword(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& ParentPtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
			bool OwnShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Sword();
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
		unique_ptr< StateMachine<Sword> >& GetStateMachine() {
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
		@brief �Ǐ]����s���̊J�n
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
		@brief �U������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AttackBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���ɂ��Ă���G�𑝂₷
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFriends(weak_ptr<GameObject> gameObject) {
			m_friends.push_back(gameObject);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G���C�ɃZ�b�g����
		@return	�Z�b�g����G�̐�
		*/
		//--------------------------------------------------------------------------------------
		int SetEnemyToCannon(wstring tag_name);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Ȃ��������
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void DeleteChains(int num);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�g��ύX����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetState(wstring state_name);

		wstring GetChainTX(int num);

		float GetScale() {
			return m_Scale.x;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�U�����Ȃ��X�e�[�g�iSword�j
	//--------------------------------------------------------------------------------------
	class NonAttackState : public ObjState<Sword>
	{
		NonAttackState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(NonAttackState)
		virtual void Enter(const shared_ptr<Sword>& Obj)override;
		virtual void Execute(const shared_ptr<Sword>& Obj)override;
		virtual void Exit(const shared_ptr<Sword>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�U���X�e�[�g�P�iSword�j
	//--------------------------------------------------------------------------------------
	class Attack1State : public ObjState<Sword>
	{
		Attack1State() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Attack1State)
		virtual void Enter(const shared_ptr<Sword>& Obj)override;
		virtual void Execute(const shared_ptr<Sword>& Obj)override;
		virtual void Exit(const shared_ptr<Sword>& Obj)override;
	};

}
//end basecross
