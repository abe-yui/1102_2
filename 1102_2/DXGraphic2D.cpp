#include "DXGraphic2D.h"

//Direct2D���g�����߂�d2d1.lib�̃����N���K�v
#pragma comment( lib, "d2d1.lib" )

//�֐��}�N��
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��



HRESULT DXGraphic2D::InitDX(HWND hwnd, UINT clientWidth, UINT clientHeight)
{
	//COM�̏�����
	CoInitialize(NULL);

	//�t�@�N�g���̏�����
	//�}���`�X���b�hOK�ŏ�����
	HRESULT hr =
		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_Factory);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�t�@�N�g���[�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}


	//�ݒ�f�[�^�쐬
	D2D1_PIXEL_FORMAT pixelFormat;
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

	//�����_�����O�ݒ�
	//UNKNOWN�ł��C���ݒ�
	pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;

	//�N���C�A���g�̈��f�B�X�v���C�ݒ�ɍ��킹��
	//�n�[�h�E�F�A�𗘗p���邩�ǂ���
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	//��ʂ̐F�ݒ�
	renderTargetProperties.pixelFormat = pixelFormat;
	renderTargetProperties.dpiX = 0;//1�C���`������̃h�b�g��
	renderTargetProperties.dpiY = 0;//0�ł��C��
	//GDI�ƘA�g����邩�A�����[�g���ɑΉ����邩�H
	renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	//�����Ŏg���@�\���x��
	renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	//�����_�����O�Ώۂ̐ݒ�
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties;
	//�\����̃E�C���h�E�n���h��
	hwndRenderTargetProperties.hwnd = hwnd;
	//�\���̈�̃T�C�Y
	hwndRenderTargetProperties.pixelSize = SizeU(clientWidth, clientHeight);
	//���j�^�[�Ɠ�������邩�̐ݒ�͂���(�񓯊��ݒ肵�Ă����ʂȂ�)
	hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

	//�ݒ�f�[�^��n���ă����_�����O�^�[�Q�b�g������
	hr = m_Factory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties, &m_RenderTarget);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�����_�[�^�[�Q�b�g�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}
	return hr;
}
void DXGraphic2D::Release()
{
	//Release�����o�֐����Ăяo���Č㏈�����s��
	SAFE_RELEASE(m_RenderTarget);
	SAFE_RELEASE(m_Factory);

	//COM�̌㏈��
	CoUninitialize();
}
void DXGraphic2D::DrawStart()
{
	//�\���J�n
	m_RenderTarget->BeginDraw();
	//��ʃN���A(�w��̐F�œh��Ԃ�)
	m_RenderTarget->Clear(ColorF(0, 0.5f, 1.0f));
}
void DXGraphic2D::DrawEnd()
{
	//�\���I��(���j�^�[�̍X�V��҂�)
	m_RenderTarget->EndDraw();
}