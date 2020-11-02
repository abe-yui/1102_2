/*
Direct2Dによる画像のロード及び表示を担当する
このクラス型の変数1つで1枚の画像データを保存する。
複数の画像を同時に使うときはその分の変数を用意する(同じ設定で異なる文字を表示する分には1つでOK)

ver1.0：とりあえず作成
*/
#pragma once

#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <wincodec.h>

using namespace D2D1;

class DXTexture2D
{
private:
	//ロード処理を担当するファクトリー
	static IWICImagingFactory* m_WICImagingFactory;
	static ID2D1HwndRenderTarget* m_RenderTarget;

	//画像1枚を扱う
	ID2D1Bitmap* m_Bitmap = NULL;

	void Draw(D2D1_POINT_2F pos, D2D1_POINT_2F scale, float rotate, float alpha, bool center, D2D1_RECT_F useRect);
public:
	DXTexture2D() {};
	~DXTexture2D() {};

	//クラス全体の初期化処理
	//このクラスを使う前に1度だけ呼び出しておく
	static HRESULT InitClass(ID2D1HwndRenderTarget* renderTarget);

	//クラス全体の後処理
	//プログラム終了時に呼び出す
	static void Cleanup();

	//指定したパスの画像をロードする
	//ロードできるファイル形式はjpg、png、bmp、tgaあたり
	//filePath：ロードするファイルパスを指定する(ワイド文字)
	HRESULT Load(LPCWSTR filePath);

	//ロードした画像を開放する
	void Release();

	//ロードした画像を画面に表示する
	//x、y：表示位置(指定位置に画像の中央が来る)
	void Draw(float x, float y);

	//ロードした画像の幅を返す
	float GetWidth();

	//ロードした画像の高さを返す
	float GetHeight();
};
