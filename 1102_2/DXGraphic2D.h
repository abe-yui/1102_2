/*
Direct2D�̏���������ѕ\���̑O�㏈����S������

ver1.0�F�Ƃ肠�����쐬
*/
#pragma once

#include <d2d1_2.h>//Direct2D�{��
#include <d2d1_2helper.h>

//d2d1_3helper�ɂ���v�f��D2D1��Ԃɑ����Ă����ɍs��A�F�̋@�\�g���ł���`����Ă���
//�s���Matrix3x2F�A�F��ColorF����`����Ă���B
//����ȊO�͏������p�̊֐����p�ӂ���Ă���̂�
using namespace D2D1;

class DXGraphic2D
{
private:
	//�����_�[�^�[�Q�b�g���K�v�ł��̂��߂Ƀt�@�N�g��������
	ID2D1Factory* m_Factory = NULL;//�t�@�N�g��
	ID2D1HwndRenderTarget* m_RenderTarget = NULL;//�����_�[�^�[�Q�b�g

public:
	DXGraphic2D() {}
	~DXGraphic2D() {}

	//Direct2D�̏��������s��
	//hwnd�F�\����̃E�C���h�E�n���h���@clientWidth�F�\���̈�̕��@clientHeight�F�\���̈�̍���
	HRESULT InitDX(HWND hwnd, UINT clientWidth, UINT clientHeight);

	//Direct2D�̌㏈��
	void Release();

	//�\���J�n����
	//�\���J�n�Ɖ�ʂ̏����������s��
	void DrawStart();

	//�\���̌㏈��
	//�����ŕ\�����e����ʂɔ��f+���j�^�[�̃��t���b�V���Ɠ��������
	void DrawEnd();

	//�����_�[�^�[�Q�b�g��Ԃ�
	//DXTexture2D��DXString2D�̏����Ɏg��
	ID2D1HwndRenderTarget* GetRenderTarget() { return m_RenderTarget; }
};
