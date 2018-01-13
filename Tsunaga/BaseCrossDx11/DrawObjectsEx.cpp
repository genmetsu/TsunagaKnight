/*!
@file DrawObjectsEx.cpp
@brief ����ȕ`��I�u�W�F�N�g�i�p�[�e�B�N���A������Ȃǁj����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	�p�[�e�B�N��
	//	*1�̃G�~�b�^�[�������A������ParticleSprite��ێ�����
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, ParticleDrawOption Option) :
		ObjectInterface(),
		m_DrawOption(Option),
		m_ParticleSpriteVec(Count),
		m_EmitterPos(0, 0, 0),
		m_TotalTime(0),
		m_MaxTime(0)
	{}
	Particle::~Particle() {}

	void Particle::Draw(const shared_ptr<ParticleDrawObject>& Manager) {
		for (auto Psp : m_ParticleSpriteVec) {
			if (Psp.m_Active && !m_TextureResource.expired()) {
				Manager->AddParticle(Psp, m_DrawOption,
					m_EmitterPos, m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	///	�}���`�p�[�e�B�N��(�p�[�e�B�N���G�t�F�N�g)
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_AddType(false)
	{}
	MultiParticle::~MultiParticle() {}

	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, ParticleDrawOption Option) {
		for (size_t i = 0; i < m_ParticleVec.size(); i++) {
			//������A�N�e�B�u�̃p�[�e�B�N��������Ώ��������ă��^�[��
			if (!m_ParticleVec[i]->IsActive()) {
				m_ParticleVec[i]->Reflesh(Count, Option);
				return m_ParticleVec[i];
			}
		}
		//�V�����p�[�e�B�N����ǉ�
		shared_ptr<Particle> ParticlePtr = ObjectFactory::Create<Particle>(Count, Option);
		m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}


	void MultiParticle::OnUpdate() {
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					//�ړ����x�ɏ]���Ĉړ�������
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->m_TotalTime >= ParticlePtr->m_MaxTime) {
						//�������ԂɂȂ�����
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto DrawPtrAdd = m_DrawPtrAdd.lock();
		if (!DrawPtrAdd) {
			DrawPtrAdd = PtrGameStage->FindTagGameObject<ParticleDrawObject>(L"ParticleDrawObjectAdd");
		}
		auto DrawPtrAlpha = m_DrawPtrAlpha.lock();
		if (!DrawPtrAlpha) {
			DrawPtrAlpha = PtrGameStage->FindTagGameObject<ParticleDrawObject>(L"ParticleDrawObjectAlpha");
		}
		if (m_ParticleVec.size() > 0) {
			for (auto Ptr : m_ParticleVec) {
				if (Ptr->IsActive()) {
					if (IsAddType()) {
						Ptr->Draw(DrawPtrAdd);
					}
					else {
						Ptr->Draw(DrawPtrAlpha);
					}
				}
			}
		}
	}



	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N���`��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	ParticleDrawObject::ParticleDrawObject(const shared_ptr<Stage>& StagePtr, bool AddType) :
		GameObject(StagePtr),
		m_MaxInstance(4000),
		m_ZBufferUse(true),
		m_SamplerWrap(false),
		m_AddType(AddType)
	{}
	ParticleDrawObject::~ParticleDrawObject() {}

	void ParticleDrawObject::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//���_�z��
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),  col,Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), col, Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0),  col,Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0),  col, Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);
			//�C���X�^���X�s��o�b�t�@�̍쐬
			//Max�l�ō쐬����
			vector<Mat4x4> matrices(m_MaxInstance, Mat4x4());
			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);
		}
		catch (...) {
			throw;
		}
	}


	//������
	void ParticleDrawObject::OnCreate() {
		CreateParticleBuffers();
		//�^�O�̒ǉ�
		if (m_AddType) {
			AddTag(L"ParticleDrawObjectAdd");
		}
		else {
			AddTag(L"ParticleDrawObjectAlpha");
		}
	}

	void ParticleDrawObject::AddParticle(const ParticleSprite& rParticleSprite, ParticleDrawOption Option,
		const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes) {
		if (m_DrawParticleSpriteVec.size() >= m_MaxInstance) {
			//�p�[�e�B�N���X�v���C�g������𒴂���
			return;
		}
		auto PtrStage = GetStage<Stage>();
		//�J�����̈ʒu
		Vec3 CameraEye = PtrStage->GetCamera().m_CamerEye;
		Vec3 CameraAt = PtrStage->GetCamera().m_CamerAt;
		Vec3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		float ToCaneraLength = bsm::length(CameraEye - WorldPos);
		Vec3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		Vec3 Temp;
		Quat Qt;
		Mat4x4 RotMatrix;
		Vec3 DefUp(0, 1.0f, 0);
		switch (Option) {
		case ParticleDrawOption::Billboard:
		{
			Temp = CameraAt - CameraEye;
			Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case ParticleDrawOption::Faceing:
		{
			Temp = WorldPos - CameraEye;
			Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case ParticleDrawOption::FaceingY:
			Temp = WorldPos - CameraEye;
			Temp.normalize();
			Qt = XMQuaternionRotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			Qt.normalize();
			break;
		case ParticleDrawOption::Normal:
			Qt = rParticleSprite.m_LocalQt;
			Qt.normalize();
			break;
		}

		bsm::Mat4x4 matrix;
		matrix.affineTransformation(
			LocalScale,
			Vec3(0, 0, 0),
			Qt,
			WorldPos
		);
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = matrix;
		Item.m_TextureRes = TextureRes;
		Item.m_Diffuse = rParticleSprite.m_Color;
		m_DrawParticleSpriteVec.push_back(Item);
	}

	void ParticleDrawObject::OnDraw() {
		auto PtrStage = GetStage<Stage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		if (m_DrawParticleSpriteVec.size() <= 0) {
			return;
		}

		//�J�����ʒu�Ń\�[�g
		auto func = [](DrawParticleSprite& Left, DrawParticleSprite& Right)->bool {
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(m_DrawParticleSpriteVec.begin(), m_DrawParticleSpriteVec.end(), func);
		//�擪�̃e�N�X�`���𓾂�
		auto  NowTexPtr = m_DrawParticleSpriteVec[0].m_TextureRes;
		size_t NowStartIndex = 0;
		size_t NowDrawCount = 0;

		shared_ptr<TextureResource> NowTexRes = m_DrawParticleSpriteVec[0].m_TextureRes;
		bsm::Col4 NowDiffuse = bsm::Col4(1, 1, 1, 1);
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			if (m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes ||
				m_DrawParticleSpriteVec[i].m_Diffuse != NowDiffuse) {
				m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));
				NowStartIndex = i;
				NowDrawCount = 0;
				NowTexRes = m_DrawParticleSpriteVec[i].m_TextureRes;
				NowDiffuse = m_DrawParticleSpriteVec[i].m_Diffuse;
			}
			else {
				NowDrawCount++;
			}
		}
		m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));


		//�C���X�^���X�o�b�t�@�Ƀ}�b�v
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//�s��̃}�b�v
		if (FAILED(pD3D11DeviceContext->Map(m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"�s���Map�Ɏ��s���܂����B",
				L"if(FAILED(pD3D11DeviceContext->Map()))",
				L"ParticleDrawObject::OnDraw()"
			);
		}
		//�s��̕ύX
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			World = m_DrawParticleSpriteVec[i].m_WorldMatrix;
			//�]�u����
			World.transpose();
			matrices[i] = World;
		}
		//�A���}�b�v
		pD3D11DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		//�`��̒��_�o�b�t�@�ƍs��o�b�t�@��ݒ�
		UINT stride[2] = { sizeof(VertexPositionColorTexture), sizeof(bsm::Mat4x4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { m_MeshResource->GetVertexBuffer().Get(), m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCTStaticInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCTStaticInstance::GetPtr()->GetInputLayout());

		//�u�����h�X�e�[�g
		if (m_AddType) {
			//���Z����
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
		}
		else {
			//��������
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthRead(), 0);
		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�r���[�s��̌���
		Mat4x4 View, Proj;
		PtrStage->GetCamera().GetViewProjMatrix(View, Proj);
		//�]�u����
		View.transpose();
		//�]�u����
		Proj.transpose();


		//�R���X�^���g�o�b�t�@�̏���
		SimpleConstants sb;
		sb.World = bsm::Mat4x4();	//���[���h�s��̓_�~�[
		sb.View = View;
		sb.Projection = Proj;
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = bsm::Col4(0, 0, 0, 0);

		for (auto& v : m_InstancVec) {
			sb.Diffuse = v.m_Diffuse;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.Tex->GetShaderResourceView().GetAddressOf());
			//�`��
			pD3D11DeviceContext->DrawIndexedInstanced(m_MeshResource->GetNumIndicis(), v.Count, 0, 0, v.Start);
		}
		//��n��
		Dev->InitializeStates();
		m_DrawParticleSpriteVec.clear();
		m_InstancVec.clear();
	}

	//--------------------------------------------------------------------------------------
	///	������`��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	StringDrawObject::StringDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_text(),
		m_FoneSize(16.0f),
		m_TextBlockWidth(128.0f),
		m_TextBlockHeight(32.0f),
		m_FontName(L"�l�r�S�V�b�N"),
		m_FontColor(bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_StartPosition{ 16.0f,16.0f },
		m_TextAlignment(TextAlignment::m_Left),
		m_BackColor(bsm::Col4(0.0f, 0.0f, 0.0f, 0.0f)),
		m_BackTextMargin(4.0f, 0.0f)
	{
		try {
			ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

			// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
			auto Dev = App::GetApp()->GetDeviceResources();
			auto D2DFactory = Dev->GetD2DFactory();
			auto DWriteFactory = Dev->GetDWriteFactory();
			auto D2DDeviceContext = Dev->GetD2DDeviceContext();

			ThrowIfFailed(
				DWriteFactory->CreateTextFormat(
					m_FontName.c_str(),
					nullptr,
					DWRITE_FONT_WEIGHT_LIGHT,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					m_FoneSize,
					L"ja",
					&m_textFormat
				),
				L"�t�H���g�쐬�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringDrawObject::StringDrawObject()"
			);

			ThrowIfFailed(
				m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringDrawObject::StringDrawObject()"
			);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&m_stateBlock),
				L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringDrawObject::StringDrawObject()"
			);

			auto Col = D2D1::ColorF(m_FontColor.x, m_FontColor.y, m_FontColor.z, m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
					Col,
					&m_Brush
				),
				L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringDrawObject::StringDrawObject()"
			);

			Col = D2D1::ColorF(m_BackColor.x, m_BackColor.y, m_BackColor.z, m_BackColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
					Col,
					&m_BackBrush
				),
				L"�o�b�N�O���E���h�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringDrawObject::StringDrawObject()"
			);
		}
		catch (...) {
			throw;
		}
	}
	StringDrawObject::~StringDrawObject() {}

	void StringDrawObject::OnCreate() {
		//�^�O�̒ǉ�
		AddTag(L"StringDrawObject");
	}

	void StringDrawObject::SetFont(const wstring& FontName, float FontSize) {
		ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DFactory = Dev->GetD2DFactory();
		auto DWriteFactory = Dev->GetDWriteFactory();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		ThrowIfFailed(
			DWriteFactory->CreateTextFormat(
				FontName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				FontSize,
				L"ja",
				&m_textFormat
			),
			L"�t�H���g�쐬�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringDrawObject::SetFont()"
		);
		//�t�H���g�̍쐬�ɐ��������̂Œl��ݒ�
		m_FontName = FontName;
		m_FoneSize = FontSize;

		ThrowIfFailed(
			m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
			L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringDrawObject::SetFont()"
		);

		ThrowIfFailed(
			D2DFactory->CreateDrawingStateBlock(&m_stateBlock),
			L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
			L"D2DFactory->CreateDrawingStateBlock()",
			L"StringDrawObject::SetFont()"
		);


	}
	TextAlignment StringDrawObject::GetTextAlignment() const {
		return m_TextAlignment;
	}
	void StringDrawObject::SetTextAlignment(TextAlignment Alignment) {
		m_TextAlignment = Alignment;
	}


	const wstring& StringDrawObject::GetText() const {
		return m_text;
	}
	void StringDrawObject::SetText(const wstring& str) {
		m_text = str;
	}


	//�Ō���ɒǉ�
	void StringDrawObject::AddText(const wstring& str) {
		wstring TempText = GetText();
		TempText += str;
		SetText(TempText);
	}


	const Col4& StringDrawObject::GetFontColor() const {
		return m_FontColor;
	}
	void StringDrawObject::SetFontColor(const Col4& Col) {
		m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(m_FontColor.x, m_FontColor.y, m_FontColor.z, m_FontColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
				ColBrush,
				&m_Brush
			),
			L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringDrawObject::SetFontColor()"
		);

	}

	const Col4& StringDrawObject::GetBackColor() const {
		return m_BackColor;
	}
	void StringDrawObject::SetBackColor(const Col4& Col) {
		m_BackColor = Col;
		auto ColBrush = D2D1::ColorF(m_BackColor.x, m_BackColor.y, m_BackColor.z,m_BackColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
				ColBrush,
				&m_BackBrush
			),
			L"�o�b�N�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringDrawObject::SetCaretColor()"
		);
	}

	Point2D<float> StringDrawObject::GetBackTextMargin() const {
		return m_BackTextMargin;
	}
	void StringDrawObject::SetBackTextMargin(Point2D<float> p) {
		m_BackTextMargin = p;
	}


	const wstring& StringDrawObject::GetFontName() const {
		return m_FontName;
	}
	float StringDrawObject::GetFontSize() const {
		return m_FoneSize;
	}

	float StringDrawObject::GetTextBlockWidth() const {
		return m_TextBlockWidth;
	}
	void StringDrawObject::SetTextBlockWidth(float f) {
		m_TextBlockWidth = f;
	}
	float StringDrawObject::GetTextBlockHeight() const {
		return m_TextBlockHeight;
	}
	void StringDrawObject::SetTextBlockHeight(float f) {
		m_TextBlockHeight = f;
	}




	ComPtr<IDWriteTextLayout> StringDrawObject::GetTextLayout()const {
		return m_textLayout;
	}
	ComPtr<IDWriteTextFormat>	StringDrawObject::GetTextFormat()const {
		return m_textFormat;
	}

	const DWRITE_TEXT_METRICS& StringDrawObject::GetDriteTextMetrics() const {
		return m_textMetrics;
	}

	const Point2D<float>& StringDrawObject::GetStartPosition() const {
		return m_StartPosition;
	}
	void StringDrawObject::SetStartPosition(const Point2D<float>& pos) {
		m_StartPosition = pos;
	}

	Rect2D<float> StringDrawObject::GetTextRect() const {
		Rect2D<float> ret;
		ret.left = GetStartPosition().x;
		ret.top = GetStartPosition().y;
		ret.right = ret.left + GetTextBlockWidth();
		ret.bottom = ret.top + GetTextBlockHeight();
		return ret;
	}
	void StringDrawObject::SetTextRect(const Rect2D<float>& rect) {
		SetStartPosition(Point2D<float>(rect.left, rect.top));
		SetTextBlockWidth(rect.Width());
		SetTextBlockHeight(rect.Height());
	}

	void StringDrawObject::OnUpdate() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		ThrowIfFailed(
			DWriteFactory->CreateTextLayout(
				m_text.c_str(),
				(uint32)m_text.length(),
				m_textFormat.Get(),
				m_TextBlockWidth, // ���̓e�L�X�g�̍ő啝�B
				m_TextBlockHeight, // ���̓e�L�X�g�̍ő卂���B
				&m_textLayout
			),
			L"�t�H���g���C�A�E�g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextLayout()",
			L"StringDrawObject::OnUpdate()"
		);
		ThrowIfFailed(
			m_textLayout->GetMetrics(&m_textMetrics),
			L"�e�L�X�g���g���N�X�擾�Ɏ��s���܂����B",
			L"pImpl->m_textLayout->GetMetrics()",
			L"StringDrawObject::OnUpdate()"
		);

	}



	void StringDrawObject::OnDraw() {

		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();
		float ElapsedTime = App::GetApp()->GetElapsedTime();


		D2D1_RECT_F BackRectangle = D2D1::RectF(
			m_StartPosition.x, m_StartPosition.y,
			m_StartPosition.x + m_TextBlockWidth,
			m_StartPosition.y + m_TextBlockHeight
		);
		BackRectangle.left -= m_BackTextMargin.x;
		BackRectangle.top -= m_BackTextMargin.y;
		BackRectangle.right += m_BackTextMargin.x;
		BackRectangle.bottom += m_BackTextMargin.y;

		D2DDeviceContext->SaveDrawingState(m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		//�o�b�N�O���E���h�̕`��
		D2DDeviceContext->FillRectangle(&BackRectangle, m_BackBrush.Get());


		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			m_StartPosition.x,
			m_StartPosition.y
		);

		D2DDeviceContext->SetTransform(screenTranslation);

		DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

		switch (m_TextAlignment) {
		case TextAlignment::m_Left:
			Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case TextAlignment::m_Center:
			Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case TextAlignment::m_Right:
			Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}


		ThrowIfFailed(
			m_textFormat->SetTextAlignment(Alignment),
			L"�e�L�X�g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"pImpl->m_textFormat->SetTextAlignment()",
			L"StringDrawObject::OnDraw()"
		);

		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			m_textLayout.Get(),
			m_Brush.Get()
		);

		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)) {
				throw BaseException(
					L"�������\���ł��܂���ł���",
					L"if (hr != D2DERR_RECREATE_TARGET)",
					L"StringDrawObject::OnDraw()"
				);
			}
		}
		D2DDeviceContext->RestoreDrawingState(m_stateBlock.Get());
	}





}
//end basecross
