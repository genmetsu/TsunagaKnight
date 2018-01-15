/*!
@file RigidbodyManager.h
@brief ���̂ƏՓ˔���
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	inline Vec3 SlideEx(const Vec3& Base, const Vec3& Norm) {
		//vec�Ɩ@�����璼�s���̒����i���ςŋ��߂�j
		float Len = bsm::dot(Base, Norm);
		//���̒����ɐL�΂�
		Vec3 Contact = Norm * Len * 0.5f;
		//�X���C�h��������͌��݂̃x�N�g����������Z
		return (Base - Contact);
	}

	//--------------------------------------------------------------------------------------
	///	�R���W�����^�C�v
	//--------------------------------------------------------------------------------------
	enum class CollType {
		typeNone,
		typeSPHERE,
		typeCAPSULE,
		typeOBB,
	};

	struct Rigidbody;
	//--------------------------------------------------------------------------------------
	///	�Փˏ��
	//--------------------------------------------------------------------------------------
	struct CollisionState {
		Rigidbody* m_Src;
		Vec3 m_SrcHitNormal;
		Rigidbody* m_Dest;
		Vec3 m_DestHitNormal;
		float m_HitTime;
	};

	class GameObject;
	struct Camera;

	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	struct Rigidbody {
		//�`��p�̃��b�V��
		static shared_ptr<MeshResource> m_SPHEREMesh;
		static shared_ptr<MeshResource> m_CAPSULEMesh;
		static shared_ptr<MeshResource> m_OBBMesh;
		//�I�[�i�[
		weak_ptr<GameObject> m_Owner;
		//�d�͉����x
		Vec3 m_Gravity;
		//����
		float m_Mass;
		//���݂̃t�H�[�X
		Vec3 m_Force;
		//���x
		Vec3 m_Velocity;
		//�R���W�����̃^�C�v
		CollType m_CollType;
		//Fixed���ǂ���
		bool m_IsFixed;
		//�Փ˔�����s�����ǂ���
		bool m_IsCollisionActive;
		//�f�o�b�O�p�`����s�����ǂ���
		bool m_IsDrawActive;
		//�X�P�[��
		Vec3 m_Scale;
		//��]
		Quat m_Quat;
		//�ʒu
		Vec3 m_Pos;
		//1�O�̃X�P�[��
		Vec3 m_BeforeScale;
		//1�O�̉�]
		Quat m_BeforeQuat;
		//1�O�̈ʒu
		Vec3 m_BeforePos;
		void SetToBefore() {
			m_BeforeScale = m_Scale;
			m_BeforeQuat = m_Quat;
			m_BeforePos = m_Pos;
		}
		Rigidbody() :
			m_Gravity(0, -9.8f, 0),
			m_Mass(0),
			m_Force(0),
			m_Velocity(0),
			m_CollType(CollType::typeNone),
			m_IsFixed(false),
			m_IsCollisionActive(true),
			m_IsDrawActive(false),
			m_Scale(1.0f),
			m_Quat(),
			m_Pos(0),
			m_BeforeScale(1.0f),
			m_BeforeQuat(),
			m_BeforePos(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief SPHERE�𓾂�
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetSPHERE()const {
			SPHERE sp;
			sp.m_Center = m_Pos;
			sp.m_Radius = m_Scale.y * 0.5f;
			return sp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CAPSULE�𓾂�
		@return	CAPSULE
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE GetCAPSULE()const {
			Mat4x4 mat;
			mat.affineTransformation(m_Scale, Vec3(0, 0, 0),
				m_Quat, m_Pos);
			CAPSULE cap(0.5f, Vec3(0, -0.5f, 0), Vec3(0, 0.5f, 0), mat);
			return cap;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief OBB�𓾂�
		@return	OBB
		*/
		//--------------------------------------------------------------------------------------
		OBB GetOBB()const {
			Mat4x4 mat;
			mat.affineTransformation(m_Scale, Vec3(0, 0, 0),
				m_Quat, m_Pos);
			OBB ob(Vec3(1.0f, 1.0f, 1.0f), mat);
			return ob;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief Velocity�ɏ]���āASpantime�����ʒu��ύX����
		@param[in]	Spantime	�ʒu�ύX�^�C��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Move(float Spantime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Փ˔���(Src��SPHERE)
		@param[in]	Dest	����̎Q��
		@param[in]	ElapsedTime	�^�[������
		@param[out]	state	�Փ˃X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		bool CollisionSPHERE(Rigidbody& Dest, float ElapsedTime, CollisionState& state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Փ˔���(Src��CAPSULE)
		@param[in]	Dest	����̎Q��
		@param[in]	ElapsedTime	�^�[������
		@param[out]	state	�Փ˃X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		bool CollisionCAPSULE(Rigidbody& Dest, float ElapsedTime, CollisionState& state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Փ˔���(Src��OBB)
		@param[in]	Dest	����̎Q��
		@param[in]	ElapsedTime	�^�[������
		@param[out]	state	�Փ˃X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		bool CollisionOBB(Rigidbody& Dest, float ElapsedTime, CollisionState& state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�X�P�[�v(Src��SPHERE)
		@param[in]	Dest	����̃|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void EscapeSPHERE(Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�X�P�[�v(Src��CAPSULE)
		@param[in]	Dest	����̃|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void EscapeCAPSULE(Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�X�P�[�v(Src��OBB)
		@param[in]	Dest	����̃|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void EscapeOBB(Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���C�A�t���[���`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DrawWireFlame(const Camera& camera);
	};


	//--------------------------------------------------------------------------------------
	///	Rigidbody�}�l�[�W��
	//--------------------------------------------------------------------------------------
	class RigidbodyManager : public GameObject {
		//Rigidbody�̔z��
		vector<shared_ptr<Rigidbody>> m_RigidbodyVec;
		//�Փ˔���
		void CollisionDest(Rigidbody& Src);
		bool CollisionStateChk(Rigidbody* p1, Rigidbody* p2);
		bool CollisionTest(Rigidbody& Src, Rigidbody& Dest, CollisionState& state);
		//�Փ˃X�e�[�g�̔z��
		vector<CollisionState> m_CollisionStateVec;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		RigidbodyManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RigidbodyManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbody�̔z��𓾂�
		@return	Rigidbody�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<shared_ptr<Rigidbody>>& GetRigidbodyVec()const {
			return m_RigidbodyVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Փˏ��̔z��𓾂�
		@return	�Փˏ��̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionState>& GetCollisionStateVec()const {
			return m_CollisionStateVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w��̃I�[�i�[��Rigidbody�𓾂�
		@param[in]	OwnerPtr	�I�[�i�[�̃|�C���^
		@return	�w��̃I�[�i�[��Rigidbody��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w��̃I�[�i�[��Rigidbody���폜����i������Ȃ���Ή������Ȃ��j
		@param[in]	OwnerPtr	�I�[�i�[�̃|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �t�H�[�X������������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InitRigidbody();
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbody��o�^����
		@return	�ǉ�����Rigidbody��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> AddRigidbody(const Rigidbody& body);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Src��Dest����̃G�X�P�[�v
		@param[in]	Src		Src�̃|�C���^
		@param[in]	Dest	Dest�̃|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Escape(Rigidbody* Src, Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ŏI�X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate2()override;
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
