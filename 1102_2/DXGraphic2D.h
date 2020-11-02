/*
Direct2Dの初期化および表示の前後処理を担当する

ver1.0：とりあえず作成
*/
#pragma once

#include <d2d1_2.h>//Direct2D本体
#include <d2d1_2helper.h>

//d2d1_3helperにある要素はD2D1空間に属しており主に行列、色の機能拡張版が定義されている
//行列はMatrix3x2F、色はColorFが定義されている。
//それ以外は初期化用の関数が用意されているのみ
using namespace D2D1;

class DXGraphic2D
{
private:
	//レンダーターゲットが必要でそのためにファクトリがある
	ID2D1Factory* m_Factory = NULL;//ファクトリ
	ID2D1HwndRenderTarget* m_RenderTarget = NULL;//レンダーターゲット

public:
	DXGraphic2D() {}
	~DXGraphic2D() {}

	//Direct2Dの初期化を行う
	//hwnd：表示先のウインドウハンドル　clientWidth：表示領域の幅　clientHeight：表示領域の高さ
	HRESULT InitDX(HWND hwnd, UINT clientWidth, UINT clientHeight);

	//Direct2Dの後処理
	void Release();

	//表示開始処理
	//表示開始と画面の消去処理を行う
	void DrawStart();

	//表示の後処理
	//ここで表示内容が画面に反映+モニターのリフレッシュと同期を取る
	void DrawEnd();

	//レンダーターゲットを返す
	//DXTexture2DとDXString2Dの準備に使う
	ID2D1HwndRenderTarget* GetRenderTarget() { return m_RenderTarget; }
};
