/*!
@file SoundObject.h
@brief �T�E���h����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�T�E���h�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	SoundObject::SoundObject(const wstring& ResKey) :
		ObjectInterface(),
		ShapeInterface(),
		m_ResKey(ResKey)
	{
		try {
			if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
				return;
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"SSoundObject::SoundObject()"
			);
		}
		catch (...) {
			throw;
		}
	}
	SoundObject::~SoundObject() {}

	shared_ptr<AudioResource> SoundObject::GetAudioResource() const {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return nullptr;
		}
		auto shptr = m_AudioResource.lock();
		if (!shptr) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (!shptr)",
				L"SoundObject::GetAudioResource()"
			);

		}
		return shptr;
	}

	void SoundObject::Start(size_t LoopCount, float Volume) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (!m_pSourceVoice) {
			return;
		}
		auto shRes = m_AudioResource.lock();
		if (!shRes) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (!shRes)",
				L"SoundObject::Start()"
			);

		}
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = shRes->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &shRes->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//���d��`�֐����Ă�
		Start(buffer, Volume);

	}

	void SoundObject::Start(const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (!m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"m_pSourceVoice->Stop()",
			L"SoundObject::Start()"
		);
		ThrowIfFailed(
			m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundObject::Start()"
		);
		if (m_AudioResource.expired()) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundObject::Start()"
			);
		}
		ThrowIfFailed(
			m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"m_pSourceVoice->SetVolume()",
			L"SoundObject::Start()"
		);

		ThrowIfFailed(
			m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"SoundObject::Start()"
		);

		ThrowIfFailed(
			m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"m_pSourceVoice->Start()",
			L"SoundObject::Start()"
		);

	}

	void SoundObject::Stop() {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (!m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"m_pSourceVoice->Stop()",
			L"SoundObject::Stop()"
		);
		ThrowIfFailed(
			m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"m_pSourceVoice->FlushSourceBuffers()",
			L"SoundObject::Stop()"
		);

	}



}
// end basecross