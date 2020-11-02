/*
Direct2Dによる文字表示を担当する
このクラス型の変数1つで1つの表示設定を扱う
複数の設定を同時に使うときはその分の変数を用意する(同じ設定で異なる文字を表示する分には1つでOK)

ver1.0：とりあえず作成
*/
#pragma once

#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <dwrite.h>

using namespace D2D1;

class DXString2D
{
private:
	static IDWriteFactory* m_WriteFactory;
	static ID2D1HwndRenderTarget* m_RenderTarget;


	IDWriteTextFormat* m_Format = NULL;
	ID2D1SolidColorBrush* m_Brush = NULL;

	void Draw(LPCWSTR text, D2D1_POINT_2F pos, D2D1_POINT_2F scale, float rotate);
public:
	DXString2D() {}
	~DXString2D() {}

	//クラス全体の初期化処理
	//このクラスを使う前に1度だけ呼び出しておく
	static HRESULT InitClass(ID2D1HwndRenderTarget* renderTarget);

	//クラス全体の後処理
	//プログラム終了時に呼び出す
	static void Cleanup();

	//文字表示の準備処理
	//fontSize：表示する文字の大きさ
	HRESULT Initialize(float fontSize);

	//設定の解放処理
	void Release();

	//文字列の表示
	//text：表示したい文字列(ワイド文字)　x、y：表示位置(指定位置に文字列の左上が来る)
	void Draw(LPCWSTR text, float x, float y);
};
