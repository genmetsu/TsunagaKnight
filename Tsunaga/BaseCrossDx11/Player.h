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
		/// �X�e�b�v�������
		Vec3 m_StepVec;
		/// �m�b�N�o�b�N�������
		Vec3 m_KnockBackVec;
		/// �t���[���J�E���g�p�ϐ�
		float m_FrameCount;
		//Rigidbody��shared_ptr
		shared_ptr<Rigidbody> m_Rigidbody;
		
		///�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTBoneModelRenderer> m_Renderer;
		///�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���g���[����������x�N�g���𓾂�
		@return	�����x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector();
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʒu�𓾂�
		@return	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override;
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
		@brief ����s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Step();

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
	///	�U���X�e�[�g
	//--------------------------------------------------------------------------------------
	class AttackState : public ObjState<Player>
	{
		AttackState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(AttackState)
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
