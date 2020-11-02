#include "WindowClass.h"


HWND WindowClass::InitWindow(int width, int height, const wchar_t* ptitle, WNDPROC wndProc, HINSTANCE hinst)
{
	//�E�C���h�E�Y�v���O������邱��
	//�P�D�ݒ�쐬�@�Q�D�E�C���h�E�\���@�R�D���b�Z�[�W�����쐬
	WNDCLASS ws = { 0 };//0�ŏ�����

	ws.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�w�i�F
	ws.hCursor = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\��
	ws.hInstance = hinst;//�C���X�^���X�n���h��(WinMain��P����)
	ws.lpfnWndProc = wndProc;//�v���V�[�W���֐��̃|�C���^(�֐�������)
	ws.lpszClassName = L"�ݒ�";//�ݒ�ɖ��O��t����

	RegisterClass(&ws);//�ݒ��o�^

	//�E�C���h�E�쐬
	//�E�C���h�E�n���h����Ԃ��̂ŕۑ�����
	m_Hwnd = CreateWindow(L"�ݒ�",//�g�p����ݒ薼
		ptitle,//�L���v�V�����o�[�̕�����
		WS_OVERLAPPEDWINDOW,//�E�C���h�E�̃��C�A�E�g
		0, 0,//�����ʒu
		width, height,//�����T�C�Y
		NULL, NULL, hinst, NULL);//��10�����ɃC���X�^���X�n���h��

	m_Width = width;
	m_Height = height;

	//�E�C���h�E�T�C�Y���Ē���
	RECT rect;
	GetClientRect(m_Hwnd, &rect);
	int tempWidth = width * 2 - (rect.right - rect.left);
	int tempHeight = height * 2 - (rect.bottom - rect.top);
	SetWindowPos(m_Hwnd, HWND_TOP, 0, 0, tempWidth, tempHeight, SWP_NOZORDER | SWP_NOMOVE);

	return m_Hwnd;
}


void WindowClass::DisplayWindow(int cmdshow)
{
	//�E�C���h�E�\��(�����͔�\��)
	//������Ԃ�OS����n���ꂽ�l�ɂ���(WinMain��4����)
	ShowWindow(m_Hwnd, cmdshow);//��Ԃ�ݒ�
	UpdateWindow(m_Hwnd);//��Ԃ��X�V
}