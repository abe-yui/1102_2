#include "DXString2D.h"


#pragma comment( lib, "dwrite.lib" )

IDWriteFactory* DXString2D::m_WriteFactory;
ID2D1HwndRenderTarget* DXString2D::m_RenderTarget;

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


HRESULT DXString2D::InitClass(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr = S_OK;
	//ファクトリー作成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,//プロセス間でキャッシュデータを共有
							__uuidof(IDWriteFactory),	//コンポーネントのID
							reinterpret_cast<IUnknown**>(&m_WriteFactory));
	if (FAILED(hr))
		MessageBox(NULL, L"テキストファクトリの作成失敗", L"エラー", MB_OK);

	//いろいろ使うので保存しておく
	m_RenderTarget = renderTarget;
	return hr;
}

void DXString2D::Cleanup()
{
	SAFE_RELEASE(m_WriteFactory);
}


HRESULT DXString2D::Initialize(float fontSize)
{
	//テキストフォーマット作成
	HRESULT hr = m_WriteFactory->CreateTextFormat(
		L"メイリオ",				//コレクションにあるフォント名(ＭＳ ゴシックなど)
		NULL,						//フォントコレクション(NULLでシステムフォント)
		DWRITE_FONT_WEIGHT_NORMAL,	//太さ
		DWRITE_FONT_STYLE_NORMAL,	//スタイル(イタリックタイなど)
		DWRITE_FONT_STRETCH_NORMAL,	//幅
		fontSize,					//文字の大きさ
		L"ja-jp",					//ロケール(ja-jpやen-us)

		&m_Format);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"テキストフォーマットの作成失敗", L"エラー", MB_OK);
		return hr;
	}

	//文字の色を決めるブラシを作成
	m_RenderTarget->CreateSolidColorBrush(ColorF(1, 0, 0, 1), &m_Brush);

	return hr;
}


void DXString2D::Release()
{
	SAFE_RELEASE(m_Brush);
	SAFE_RELEASE(m_Format);
}



void DXString2D::Draw(LPCWSTR text, D2D1_POINT_2F pos, D2D1_POINT_2F scale, float rotate)
{
	//テキストエリア
	//自動改行があるので適当に大きく広げておく
	//座標変換の影響を受けるのでここでも座標変換で位置調整
	D2D1_RECT_F rect = RectF(0, 0, 2000, 2000);

	Matrix3x2F matTrans = Matrix3x2F::Translation(pos.x, pos.y);
	Matrix3x2F matRotate = Matrix3x2F::Rotation(rotate, Point2F(0,0));
	Matrix3x2F matScale = Matrix3x2F::Scale(scale.x, scale.y, Point2F(0, 0));
	Matrix3x2F matWorld = matScale * matRotate * matTrans;

	m_RenderTarget->SetTransform(matWorld);

	m_RenderTarget->DrawText(text, wcslen(text), m_Format, rect, m_Brush);
}



void DXString2D::Draw(LPCWSTR text, float x, float y)
{
	D2D1_POINT_2F pos = Point2F(x, y);
	D2D1_POINT_2F scale = Point2F(1,1);
	Draw(text, pos, scale, 0);
}