#include "WindowClass.h"


HWND WindowClass::InitWindow(int width, int height, const wchar_t* ptitle, WNDPROC wndProc, HINSTANCE hinst)
{
	//ウインドウズプログラムやること
	//１．設定作成　２．ウインドウ表示　３．メッセージ処理作成
	WNDCLASS ws = { 0 };//0で初期化

	ws.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景色
	ws.hCursor = LoadCursor(NULL, IDC_ARROW);//マウスカーソル
	ws.hInstance = hinst;//インスタンスハンドル(WinMain第１引数)
	ws.lpfnWndProc = wndProc;//プロシージャ関数のポインタ(関数名を代入)
	ws.lpszClassName = L"設定";//設定に名前を付ける

	RegisterClass(&ws);//設定を登録

	//ウインドウ作成
	//ウインドウハンドルを返すので保存する
	m_Hwnd = CreateWindow(L"設定",//使用する設定名
		ptitle,//キャプションバーの文字列
		WS_OVERLAPPEDWINDOW,//ウインドウのレイアウト
		0, 0,//初期位置
		width, height,//初期サイズ
		NULL, NULL, hinst, NULL);//第10引数にインスタンスハンドル

	m_Width = width;
	m_Height = height;

	//ウインドウサイズを再調整
	RECT rect;
	GetClientRect(m_Hwnd, &rect);
	int tempWidth = width * 2 - (rect.right - rect.left);
	int tempHeight = height * 2 - (rect.bottom - rect.top);
	SetWindowPos(m_Hwnd, HWND_TOP, 0, 0, tempWidth, tempHeight, SWP_NOZORDER | SWP_NOMOVE);

	return m_Hwnd;
}


void WindowClass::DisplayWindow(int cmdshow)
{
	//ウインドウ表示(初期は非表示)
	//初期状態はOSから渡された値にする(WinMain第4引数)
	ShowWindow(m_Hwnd, cmdshow);//状態を設定
	UpdateWindow(m_Hwnd);//状態を更新
}