/*
Direct2D�ɂ�镶���\����S������
���̃N���X�^�̕ϐ�1��1�̕\���ݒ������
�����̐ݒ�𓯎��Ɏg���Ƃ��͂��̕��̕ϐ���p�ӂ���(�����ݒ�ňقȂ镶����\�����镪�ɂ�1��OK)

ver1.0�F�Ƃ肠�����쐬
*/
#pragma once

#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <dwrite.h>

using namespace D2D1;

class DXString2D
{
private:
	static IDWriteFactory* m_WriteFactory;
	static ID2D1HwndRenderTarget* m_RenderTarget;


	IDWriteTextFormat* m_Format = NULL;
	ID2D1SolidColorBrush* m_Brush = NULL;

	void Draw(LPCWSTR text, D2D1_POINT_2F pos, D2D1_POINT_2F scale, float rotate);
public:
	DXString2D() {}
	~DXString2D() {}

	//�N���X�S�̂̏���������
	//���̃N���X���g���O��1�x�����Ăяo���Ă���
	static HRESULT InitClass(ID2D1HwndRenderTarget* renderTarget);

	//�N���X�S�̂̌㏈��
	//�v���O�����I�����ɌĂяo��
	static void Cleanup();

	//�����\���̏�������
	//fontSize�F�\�����镶���̑傫��
	HRESULT Initialize(float fontSize);

	//�ݒ�̉������
	void Release();

	//������̕\��
	//text�F�\��������������(���C�h����)�@x�Ay�F�\���ʒu(�w��ʒu�ɕ�����̍��オ����)
	void Draw(LPCWSTR text, float x, float y);
};
