/*
���[�U�[�����t
���b�Z�[�W�Ή��Ə�Ԃ��`�F�b�N���Ă̑Ή���2��ނ�����
*/


#include <Windows.h>
#include "WindowClass.h"
#include "DXGraphic2D.h"
#include "DXTexture2D.h"//�摜�������N���X
#include "DXString2D.h"

LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

bool g_Loop = true;
WindowClass g_Window;
DXGraphic2D g_Graphic;

DXTexture2D image;
float posx;
float posy;


int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR cmdline, int cmdshow)
{
	//�v���O�����̏���������
	g_Window.InitWindow(1024, 768, L"Direct2D", WndProc, hinst);
	g_Window.DisplayWindow(cmdshow);

	g_Graphic.InitDX(g_Window.GetHwnd(), 1024, 768);
	DXTexture2D::InitClass(g_Graphic.GetRenderTarget());

	image.Load(L"ziki.png");

	//���b�Z�[�W���[�v(���C�����[�v)
	MSG msg;
	while (g_Loop==true)
	{
		int hoge = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (hoge != 0)
			DispatchMessage(&msg);

		g_Graphic.DrawStart();

		image.Draw(posx, posy);

		g_Graphic.DrawEnd();

	}

	//�㏈��
	g_Graphic.Release();
	return 0;
}


//���[�U�[�����p�̃C�x���g������̂ł���ɑΉ�����������
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN://�}�E�X�̍��{�^������
		MessageBox(NULL, L"��", L"������", MB_OK);
		break;
	case WM_KEYDOWN://�L�[��������(UP������������)
		//�ǂ̃L�[�ł���������
		//wparam�ɂ��[��\���l������̂Ń`�F�b�N����
		//�L�[�l��VK_�`�̒萔�l���啶���̕����R�[�h(�����L�[)
		if (wparam==VK_RIGHT)
		posx += 10;
		if (wparam == VK_F1)
			posx = 0;
		break;
	case WM_DESTROY:
		g_Loop = false;
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}