/*!
@file Scene.h
@brief �V�[���Ȃ�
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	�J����
	//--------------------------------------------------------------------------------------
	struct Camera {
		Vec3 m_CamerEye;			///<�J�����ʒu
		Vec3 m_CamerAt;			///<�J�������_
		Vec3 m_CamerUp;			///<�J�����X��
		float m_FovY;				///<��p
		float m_CameraXZRad;			//�J������XZ���W�A��
		float m_CameraYRad;				//�J������Y���W�A��
		float m_CameraArmLen;		//�J�����̘r�̒���
		Camera():
			m_CamerEye(0, 5.0, -5.0f),
			m_CamerAt(0, 0, 0),
			m_CamerUp(0, 1.0f, 0),
			m_FovY(XM_PIDIV4),
			m_CameraXZRad(0.0f),
			m_CameraYRad(XM_PIDIV4 + 0.5f),
			m_CameraArmLen(3.0f)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �r���[�s��Ǝˉe�s��̎擾�i�e�Q�ƃp�����[�^�ɑ���j
		@param[out]	View	�r���[�s����󂯎��Q��
		@param[out]	Proj	�ˉe�s����󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetViewProjMatrix(Mat4x4& View, Mat4x4& Proj)const {
			View = XMMatrixLookAtLH(m_CamerEye, m_CamerAt, m_CamerUp);
			float w = static_cast<float>(App::GetApp()->GetGameWidth());
			float h = static_cast<float>(App::GetApp()->GetGameHeight());
			Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 5000.0f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �J������Eye��At���擾
		@param[out]	Eye	Eye���󂯎��Q��
		@param[out]	At	At���󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetCameraEyeAt(Vec3& Eye, Vec3& At)const {
			Eye = m_CamerEye;
			At = m_CamerAt;
		}
	};

	//--------------------------------------------------------------------------------------
	//	�V���O�����C�g
	//--------------------------------------------------------------------------------------
	struct SingleLight {
		Vec3 m_Directional;	//���C�g�̌���
		Col4 m_DiffuseColor;	//�f�B�t�B�[�Y�F
		Col4 m_SpecularColor;	//�X�y�L�����[�F
		Col4 m_AmbientLightColor;			//�A���r�G���g�F
		SingleLight()
		{
			SetDefaultLighting();
		}
		SingleLight(const Vec3& dir, const Col4& def, const Col4& sp, const Col4& am) :
			m_Directional(dir),
			m_DiffuseColor(def),
			m_SpecularColor(sp),
			m_AmbientLightColor(am)
		{
		}
		~SingleLight() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���C�g�ʒu����̌�����ݒ肷��
		@param[in]	Position	���C�g�̈ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(const Vec3& Position) {
			m_Directional = Position;
			m_Directional *= -1.0f;
			m_Directional.normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���C�g�ʒu����̌�����ݒ肷��
		@param[in]	x	���C�g�ʒux
		@param[in]	y	���C�g�ʒuy
		@param[in]	z	���C�g�ʒuz
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(float x, float y, float z) {
			m_Directional = bsm::Vec3(x, y, z);
			m_Directional *= -1.0f;
			m_Directional.normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�t�H���g�̃��C�e�B���O��ݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultLighting() {
			m_Directional = Vec3(0.4545195f, -0.7660444f,  0.4545195f);
			m_Directional.normalize();
			m_DiffuseColor = Col4(0.3231373f, 0.3607844f, 0.3937255f, 0.0f);
			m_SpecularColor = Col4(0.3231373f, 0.3607844f, 0.3937255f, 0.0f);
			m_AmbientLightColor = Col4( 0.05333332f, 0.09882354f, 0.1819608f ,0.0f );
		}
	};


	//--------------------------------------------------------------------------------------
	//	�}���`���C�g
	//--------------------------------------------------------------------------------------
	struct MultiLights {
		Vec3 m_Directional[3];	//���C�g�̌���
		Col4 m_DiffuseColor[3];	//�f�B�t�B�[�Y�F
		Col4 m_SpecularColor[3];	//�X�y�L�����[�F
		Col4 m_AmbientLightColor;			//�A���r�G���g�F
		size_t m_MainColorIndex;
		MultiLights()
		{
			SetDefaultLighting();
			m_MainColorIndex = 0;
		}
		~MultiLights() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�t�H���g�̃��C�e�B���O��ݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultLighting() {
			static const Vec3 defaultDirections[3] =
			{
				{ -0.5265408f, -0.5735765f, -0.6275069f },
				{ 0.7198464f,  0.3420201f,  0.6040227f },
				{ 0.4545195f, -0.7660444f,  0.4545195f },
			};
			static const Col4 defaultDiffuse[3] =
			{
				{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
				{ 0.9647059f, 0.7607844f, 0.4078432f,0.0f },
				{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
			};

			static const Col4 defaultSpecular[3] =
			{
				{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
				{ 0.0000000f, 0.0000000f, 0.0000000f,0.0f },
				{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
			};


			m_AmbientLightColor = Col4(0.05333332f, 0.09882354f, 0.1819608f, 0.0f);
			for (size_t i = 0; i < 3; i++) {
				m_Directional[i] = defaultDirections[i];
				m_Directional[i].normalize();
				m_DiffuseColor[i] = defaultDiffuse[i];
				m_SpecularColor[i] = defaultSpecular[i];
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���C���̃��C�g���擾����
		@param[out]	Light	�󂯎�郉�C�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetMainLight(SingleLight& light) {
			light.m_Directional = m_Directional[m_MainColorIndex];
			light.m_DiffuseColor = m_DiffuseColor[m_MainColorIndex];
			light.m_SpecularColor = m_SpecularColor[m_MainColorIndex];
			light.m_AmbientLightColor = m_AmbientLightColor;
		}
	};

	class RigidbodyManager;
	struct Rigidbody;
	struct CollisionState;

	//--------------------------------------------------------------------------------------
	///	�X�e�[�W�i�V�[���ŊǗ�����C���^�[�t�F�C�X�j
	//--------------------------------------------------------------------------------------
	class Stage : public ObjectInterface, public ShapeInterface {
		//�}���`���C�g
		MultiLights m_MultiLights;
		//�J����
		Camera m_Camera;
		//�I�u�W�F�N�g�̔z��
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//�r���ɃI�u�W�F�N�g���ǉ����ꂽ�ꍇ�A�^�[���̊J�n�܂ő҂z��
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;
		//�r���ɃI�u�W�F�N�g���폜���ꂽ�ꍇ�A�^�[���̊J�n�܂ő҂z��
		vector< shared_ptr<GameObject> > m_WaitRemoveObjectVec;
		//�ǉ��I�u�W�F�N�g�̎w��
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
		//�폜�I�u�W�F�N�g�̎w��
		void RemoveBackGameObject(const shared_ptr<GameObject>& Ptr);
		//�I�u�W�F�N�g�̍폜
		void RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj);
		//�ǉ���폜�҂��ɂȂ��Ă�I�u�W�F�N�g��ǉ��폜����
		void SetWaitToObjectVec();
	protected:
		//Rigidbody�}�l�[�W��
		shared_ptr<RigidbodyManager> m_RigidbodyManager;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���e�N�g�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Stage(){
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Stage() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �J�����̎擾
		@return	�J�����̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const Camera& GetCamera()const {
			return m_Camera;
		}
		Camera& GetCamera() {
			return m_Camera;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���C�����C�g�����̎擾
		@param[out]	LightDir	���C�g�����󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetLightDir(Vec4& LightDir)const {
			LightDir = m_MultiLights.m_Directional[m_MultiLights.m_MainColorIndex];
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �}���`���C�g�̎擾
		@return	�}���`���C�g�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const MultiLights& GetMultiLights()const {
			return m_MultiLights;
		}
		MultiLights& GetMultiLights() {
			return m_MultiLights;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �O������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�X�V�����i���z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreUpdateStage() {
			SetWaitToObjectVec();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�X�V�i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�`��i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g��ǉ�����
		@tparam	T	�쐬����^
		@tparam	Ts	�ϒ��p�����[�^�̌^
		@param[in]	params	�ϒ��p�����[�^
		@return	�쐬���ꂽ�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g�̔z��𓾂�
		@return	�Q�[���I�u�W�F�N�g�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector< shared_ptr<GameObject> >& GetGameObjectVec()const {
			return m_GameObjectVec;
		}
		vector< shared_ptr<GameObject> >& GetGameObjectVec() {
			return m_GameObjectVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C���X�^���X���炻�̃Q�[���I�u�W�F�N�g���w�肵���^���ǂ����𓾂�
		@tparam	T	���ׂ�^
		@param[in]	Obj	�C���X�^���X
		@return	�w�肵���^�ő��݂����true
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		bool FindGameObject(const shared_ptr<GameObject>& Obj) const{
			auto shptr = dynamic_pointer_cast<T>(Obj);
			if (shptr) {
				for (auto ptr : GetGameObjectVec()) {
					if (Obj == ptr) {
						return true;
					}
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�O�������̃Q�[���I�u�W�F�N�g�𓾂�
		@tparam	T	���ׂ�^
		@param[in]	TagName	�^�O��
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	 �w��̃^�O�����ŏ��̃I�u�W�F�N�g�i�ۑ����Ă͂����Ȃ��j
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> FindTagGameObject(const wstring& TagName,bool ExceptionActive = true) const {
			for (auto& v : GetGameObjectVec()) {
				if (v->FindTag(TagName)) {
					auto shptr = dynamic_pointer_cast<T>(v);
					if (shptr) {
						return shptr;
					}
				}
			}
			if (ExceptionActive) {
				throw BaseException(
					L"�I�u�W�F�N�g��������܂���",
					TagName,
					L"Stage::FindGameObject()"
				);
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�O���炻�̃Q�[���I�u�W�F�N�g�𓾂�
		@param[in]	TagName	�^�O��
		@param[out]	Ret GameObject�̔z��(shared_ptr�̔z��)
		@return	 �w��̃^�O�����I�u�W�F�N�g�̔z��𓾂�B�g�p��͑��₩�ɉ�����邱��
		*/
		//--------------------------------------------------------------------------------------
		void FindTagGameObjectVec(const wstring& TagName, vector<shared_ptr<GameObject>>& Ret) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g���폜����
		@tparam	T	�폜����^
		@param[in]	Obj	�C���X�^���X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void RemoveGameObject(const shared_ptr<GameObject>& Obj) {
			if (FindGameObject<T>(Obj)) {
				RemoveBackGameObject(Obj);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rigidbody�}�l�[�W���𓾂�
		@return	Rigidbody�}�l�[�W��
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RigidbodyManager> GetRigidbodyManager() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbody��o�^����
		@return	�ǉ�����Rigidbody��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> AddRigidbody(const Rigidbody& body);
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
		@brief Rigidbody�̔z��𓾂�
		@return	Rigidbody�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<shared_ptr<Rigidbody>>& GetRigidbodyVec()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Փˏ��̔z��𓾂�
		@return	�Փˏ��̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionState>& GetCollisionStateVec()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w��̃I�[�i�[��Rigidbody�𓾂�
		@param[in]	OwnerPtr	�I�[�i�[�̃|�C���^
		@return	�w��̃I�[�i�[��Rigidbody��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
	};

	//--------------------------------------------------------------------------------------
	///	�Q�[���I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class GameObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Stage> m_Stage;	//�����X�e�[�W
		set<wstring> m_Tag;	//�^�O
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���e�N�g�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const {
			auto shptr = m_Stage.lock();
			if (!shptr) {
				throw BaseException(
					L"�����X�e�[�W��null�ł��B",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			return shptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�(�^�`�F�b�N����)
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetStage() const {
			auto StagePtr = GetStage();
			auto TargetPtr = dynamic_pointer_cast<T>(StagePtr);
			if (!TargetPtr) {
				throw BaseException(
					L"�X�e�[�W������܂������A�^�L���X�g�ł��܂���",
					Util::GetWSTypeName<T>(),
					L"GameObject::GetStage<T>()"
				);
			}
			return TargetPtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�O�̃Z�b�g�𓾂�
		@return	�^�O�̃Z�b�g
		*/
		//--------------------------------------------------------------------------------------
		const set<wstring>& GetTagSet()const {
			return m_Tag;
		}
		set<wstring>& GetTagSet() {
			return m_Tag;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肷��^�O�����݂��邩�ǂ����𓾂�
		@param[in]	tagstr	���؂���^�O
		@return	���݂����true
		*/
		//--------------------------------------------------------------------------------------
		bool FindTag(const wstring& tagstr) const {
			if (m_Tag.find(tagstr) == m_Tag.end()) {
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肷��^�O��ǉ�����
		@param[in]	tagstr	�ǉ�����^�O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  AddTag(const wstring& tagstr) {
			if (tagstr == L"") {
				//�󔒂Ȃ��O
				throw BaseException(
					L"�ݒ肷��^�O����ł�",
					L"if (tagstr == L"")",
					L"GameObject::AddTag()"
				);
			}
			m_Tag.insert(tagstr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肷��^�O�����݂�����폜����i���݂��Ȃ��ꍇ�͉������Ȃ��j
		@param[in]	tagstr	�폜����^�O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveTag(const wstring& tagstr) {
			m_Tag.erase(tagstr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I�u�W�F�N�g�̈ʒu�𓾂�i���z�֐��j
		@return	�I�u�W�F�N�g�̈ʒu
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() {
			return Vec3(0, 0, 0);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��(���z�֐�)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() {}
	};


	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		//�A�N�e�B�u�ȃX�e�[�W
		shared_ptr<Stage> m_ActiveStage;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�N�e�B�u�ȃX�e�[�W��ݒ肵�ď���������
		@tparam	T	�擾����^�iStage�Ɍ^�ϊ��ł�����́j
		@tparam	Ts	�ϒ��ϐ��̌^
		@param[in]	params	���̃X�e�[�W���\�z����̂Ɏg�p����p�����[�^�B
		@return	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params) {
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"�ȉ���Stage�Ɍ^�L���X�g�ł��܂���B",
					Util::GetWSTypeName<T>(),
					L"Scene::ResetActiveStage<T>()"
				);
			}
			m_ActiveStage = StagePtr;
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���\�[�X�̍쐬
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateResources();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�N�e�B�u�ȃX�e�[�W�̎擾
		@return	�A�N�e�B�u�ȃX�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetActiveStage() const {
			if (!m_ActiveStage) {
				throw BaseException(
					L"�A�N�e�B�u�ȃX�e�[�W�͂���܂���",
					L"f (!m_ActiveStage)",
					L"Scene::GetActiveStage()"
				);
			}
			return m_ActiveStage;
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	};


}
// end basecross
