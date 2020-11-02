/*
Direct2D�ɂ��摜�̃��[�h�y�ѕ\����S������
���̃N���X�^�̕ϐ�1��1���̉摜�f�[�^��ۑ�����B
�����̉摜�𓯎��Ɏg���Ƃ��͂��̕��̕ϐ���p�ӂ���(�����ݒ�ňقȂ镶����\�����镪�ɂ�1��OK)

ver1.0�F�Ƃ肠�����쐬
*/
#pragma once

#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <wincodec.h>

using namespace D2D1;

class DXTexture2D
{
private:
	//���[�h������S������t�@�N�g���[
	static IWICImagingFactory* m_WICImagingFactory;
	static ID2D1HwndRenderTarget* m_RenderTarget;

	//�摜1��������
	ID2D1Bitmap* m_Bitmap = NULL;

	void Draw(D2D1_POINT_2F pos, D2D1_POINT_2F scale, float rotate, float alpha, bool center, D2D1_RECT_F useRect);
public:
	DXTexture2D() {};
	~DXTexture2D() {};

	//�N���X�S�̂̏���������
	//���̃N���X���g���O��1�x�����Ăяo���Ă���
	static HRESULT InitClass(ID2D1HwndRenderTarget* renderTarget);

	//�N���X�S�̂̌㏈��
	//�v���O�����I�����ɌĂяo��
	static void Cleanup();

	//�w�肵���p�X�̉摜�����[�h����
	//���[�h�ł���t�@�C���`����jpg�Apng�Abmp�Atga������
	//filePath�F���[�h����t�@�C���p�X���w�肷��(���C�h����)
	HRESULT Load(LPCWSTR filePath);

	//���[�h�����摜���J������
	void Release();

	//���[�h�����摜����ʂɕ\������
	//x�Ay�F�\���ʒu(�w��ʒu�ɉ摜�̒���������)
	void Draw(float x, float y);

	//���[�h�����摜�̕���Ԃ�
	float GetWidth();

	//���[�h�����摜�̍�����Ԃ�
	float GetHeight();
};
