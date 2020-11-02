/*
ウインドウの作成および表示を担当する

ver1.0：とりあえず作成
*/
#pragma once

//WindowClassクラス：ウインドウの表示を担当する
#include <Windows.h>

class WindowClass
{
private:
	HWND m_Hwnd = 0;
	int m_Width = 0;//画面サイズを保持
	int m_Height = 0;

public:
	//ウインドウを作成する
	//width、height：表示領域の幅と高さを指定　ptitle：タイトルバーに表示する文字列(ワイド文字)
	//wndProc：ウインドウプロシージャ関数のアドレス　hinst：インスタンスハンドルを渡す(WinMainの第1引数をそのまま渡す)
	HWND InitWindow(int width, int height, const wchar_t* ptitle, WNDPROC wndProc, HINSTANCE hinst);

	//ウインドウの状態を設定する(表示させるために使う)
	//本来はSW_SHOWなどの定数を渡すが1回目の呼び出しは(WinMainの第4引数をそのまま渡す)
	void DisplayWindow(int cmdshow);

	//作成したウインドウのウインドウハンドルを返す
	HWND GetHwnd(){ return m_Hwnd; }

	//表示領域の初期幅を返す
	int GetWidth(){ return m_Width; }

	//表示領域の初期高さを返す
	int GetHeight(){ return m_Height; }
};