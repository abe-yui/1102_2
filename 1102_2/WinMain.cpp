/*
ユーザー操作受付
メッセージ対応と状態をチェックしての対応の2種類がある
*/


#include <Windows.h>
#include "WindowClass.h"
#include "DXGraphic2D.h"
#include "DXTexture2D.h"//画像を扱うクラス
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
	//プログラムの初期化処理
	g_Window.InitWindow(1024, 768, L"Direct2D", WndProc, hinst);
	g_Window.DisplayWindow(cmdshow);

	g_Graphic.InitDX(g_Window.GetHwnd(), 1024, 768);
	DXTexture2D::InitClass(g_Graphic.GetRenderTarget());

	image.Load(L"ziki.png");

	//メッセージループ(メインループ)
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

	//後処理
	g_Graphic.Release();
	return 0;
}


//ユーザー操作専用のイベントがあるのでそれに対応処理を書く
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN://マウスの左ボタン押し
		MessageBox(NULL, L"左", L"押した", MB_OK);
		break;
	case WM_KEYDOWN://キーを押した(UP離したもある)
		//どのキーでも反応する
		//wparamにきーを表す値が入るのでチェックする
		//キー値はVK_～の定数値が大文字の文字コード(文字キー)
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