/*
�E�C���h�E�̍쐬����ѕ\����S������

ver1.0�F�Ƃ肠�����쐬
*/
#pragma once

//WindowClass�N���X�F�E�C���h�E�̕\����S������
#include <Windows.h>

class WindowClass
{
private:
	HWND m_Hwnd = 0;
	int m_Width = 0;//��ʃT�C�Y��ێ�
	int m_Height = 0;

public:
	//�E�C���h�E���쐬����
	//width�Aheight�F�\���̈�̕��ƍ������w��@ptitle�F�^�C�g���o�[�ɕ\�����镶����(���C�h����)
	//wndProc�F�E�C���h�E�v���V�[�W���֐��̃A�h���X�@hinst�F�C���X�^���X�n���h����n��(WinMain�̑�1���������̂܂ܓn��)
	HWND InitWindow(int width, int height, const wchar_t* ptitle, WNDPROC wndProc, HINSTANCE hinst);

	//�E�C���h�E�̏�Ԃ�ݒ肷��(�\�������邽�߂Ɏg��)
	//�{����SW_SHOW�Ȃǂ̒萔��n����1��ڂ̌Ăяo����(WinMain�̑�4���������̂܂ܓn��)
	void DisplayWindow(int cmdshow);

	//�쐬�����E�C���h�E�̃E�C���h�E�n���h����Ԃ�
	HWND GetHwnd(){ return m_Hwnd; }

	//�\���̈�̏�������Ԃ�
	int GetWidth(){ return m_Width; }

	//�\���̈�̏���������Ԃ�
	int GetHeight(){ return m_Height; }
};