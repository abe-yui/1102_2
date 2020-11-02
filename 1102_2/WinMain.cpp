/*
���[�U�[�����t
���b�Z�[�W�Ή��Ə�Ԃ��`�F�b�N���Ă̑Ή���2��ނ�����
���b�Z�[�W�Ή�
�������Ƃ��̏������ł���A���������ē��삷�鏈�����ł��Ȃ�

��Ԃ��`�F�b�N���đΉ�
�������Ƃ��̏������ł��Ȃ��A���������Ă̓���͂ł���
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

//�L�����N�^�[�f�[�^�͖����̂ŕK�v�ȕϐ����W�߂�
//�L�����N�^�[�f�[�^�����
//�ω�����A�̂��ƂɈقȂ�f�[�^�͕ϐ��ň����Ă���
DXTexture2D image;
float posx;//�ʒu��ۑ�
float posy;
float speedx;//1�t���[���̈ړ��ʂ�ۑ�
float speedy;


//�����Ă���t���[�������J�E���g
//�����Ă����1�����₷�A�����łȂ��Ƃ���0�ɂ���
int pressCount;

//�����Ă��邩��Ԃ�ۑ�
bool buttonPress;//Down��true�AUp��false�ɂ���


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

		//��Ԃ𒲂ׂĂ̏����̓��[�v���ɏ���
		//���[�v���ŏ�Ԃ��`�F�b�N�A�Ή��������s��


		//�֐����g���ăL�[�̏�Ԃ��`�F�b�N
		//GetKeyState(�L�[�l)
		//������Ă�����߂�l�̍ŏ�ʃr�b�g��1�ɂȂ�(& 0x80 �Œ��ׂ�)
		if (GetKeyState(VK_DOWN) & 0x80)
			posy += 1;

		//�܂Ƃ߂ă`�F�b�N
		BYTE kb[256];//256�͌Œ� BYTE��unsigned char�^
		GetKeyboardState(kb);//�S�ẴL�[���`�F�b�N

		//�L�[�l��v�f�ԍ��Ƃ��Ĉ���
		if (kb[VK_UP] & 0x80)
			posy -= 1;

		if ((kb[VK_RBUTTON] & 0x80)!=0)
		{
			//�����Ă��J�E���g0���������u��
			if (pressCount==0)//�������u��
			posx = rand() % 1024;
			pressCount++;
		}
		else
		{
			pressCount = 0;
		}

		if (posx>512)//��ʉE�����ɓ�������
		{
			g_Loop = false;
		}

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
		//�}�E�X�֘A�̃C�x���g�̓E�C���h�E��Ƀ}�E�X�����鎞�̂ݔ�������
		//�}�E�X����͂��ׂĈʒu���n�����
		//lparam�ɓ����Ă��邪x��y�ɕ��ނ��Ĉ���
		//LOWORD��x�AHIWORD��y�����o�����Ƃ��ł���
	case WM_LBUTTONDOWN://�}�E�X�̍��{�^������
		//MessageBox(NULL, L"��", L"������", MB_OK);
		//posx = LOWORD(lparam);
		//posy = HIWORD(lparam);
		buttonPress = true;
		break;
	case WM_LBUTTONUP:
		buttonPress = false;
		break;
	case WM_MOUSEMOVE:
		//���{�^����������Ă����
		if (buttonPress == true)
		{
			posx = LOWORD(lparam);
			posy = HIWORD(lparam);
		}
		break;
	case WM_MOUSEWHEEL://�z�C�[���񂵂�
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			posy -= 10;
		break;
	case WM_KEYDOWN://�L�[��������(UP������������)
		//�ǂ̃L�[�ł���������
		//wparam�ɂ��[��\���l������̂Ń`�F�b�N����
		//�L�[�l��VK_�`�̒萔�l���啶���̕����R�[�h(�����L�[)
		if (wparam==VK_RIGHT)
			posx += 10;
		if (wparam == VK_F1)
			posx = 0;
		if (wparam == 'A')//�啶��
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