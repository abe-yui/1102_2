/*
ユーザー操作受付
メッセージ対応と状態をチェックしての対応の2種類がある
メッセージ対応
押したときの処理ができる、押し続けて動作する処理ができない

状態をチェックして対応
押したときの処理ができない、押し続けての動作はできる
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

//キャラクターデータは無いので必要な変数を集めて
//キャラクターデータを作る
//変化する、個体ごとに異なるデータは変数で扱っていく
DXTexture2D image;
float posx;//位置を保存
float posy;
float speedx;//1フレームの移動量を保存
float speedy;


//押しているフレーム数をカウント
//押している間1ずつ増やす、そうでないときは0にする
int pressCount;

//押しているか状態を保存
bool buttonPress;//Downでtrue、Upでfalseにする


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

		//状態を調べての処理はループ内に書く
		//ループ内で状態をチェック、対応処理を行う


		//関数を使ってキーの状態をチェック
		//GetKeyState(キー値)
		//押されていたら戻り値の最上位ビットが1になる(& 0x80 で調べる)
		if (GetKeyState(VK_DOWN) & 0x80)
			posy += 1;

		//まとめてチェック
		BYTE kb[256];//256は固定 BYTEはunsigned char型
		GetKeyboardState(kb);//全てのキーをチェック

		//キー値を要素番号として扱う
		if (kb[VK_UP] & 0x80)
			posy -= 1;

		if ((kb[VK_RBUTTON] & 0x80)!=0)
		{
			//押してもカウント0＝押した瞬間
			if (pressCount==0)//押した瞬間
			posx = rand() % 1024;
			pressCount++;
		}
		else
		{
			pressCount = 0;
		}

		if (posx>512)//画面右半分に入ったら
		{
			g_Loop = false;
		}

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
		//マウス関連のイベントはウインドウ上にマウスがある時のみ発生する
		//マウス操作はすべて位置が渡される
		//lparamに入っているがxとyに分類して扱う
		//LOWORDでx、HIWORDでyを取り出すことができる
	case WM_LBUTTONDOWN://マウスの左ボタン押し
		//MessageBox(NULL, L"左", L"押した", MB_OK);
		//posx = LOWORD(lparam);
		//posy = HIWORD(lparam);
		buttonPress = true;
		break;
	case WM_LBUTTONUP:
		buttonPress = false;
		break;
	case WM_MOUSEMOVE:
		//左ボタンが押されている間
		if (buttonPress == true)
		{
			posx = LOWORD(lparam);
			posy = HIWORD(lparam);
		}
		break;
	case WM_MOUSEWHEEL://ホイール回した
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			posy -= 10;
		break;
	case WM_KEYDOWN://キーを押した(UP離したもある)
		//どのキーでも反応する
		//wparamにきーを表す値が入るのでチェックする
		//キー値はVK_～の定数値が大文字の文字コード(文字キー)
		if (wparam==VK_RIGHT)
			posx += 10;
		if (wparam == VK_F1)
			posx = 0;
		if (wparam == 'A')//大文字
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