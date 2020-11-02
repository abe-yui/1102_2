#include "DXGraphic2D.h"

//Direct2Dを使うためにd2d1.libのリンクが必要
#pragma comment( lib, "d2d1.lib" )

//関数マクロ
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ



HRESULT DXGraphic2D::InitDX(HWND hwnd, UINT clientWidth, UINT clientHeight)
{
	//COMの初期化
	CoInitialize(NULL);

	//ファクトリの初期化
	//マルチスレッドOKで初期化
	HRESULT hr =
		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_Factory);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"ファクトリーの作成失敗", L"エラー", MB_OK);
		return hr;
	}


	//設定データ作成
	D2D1_PIXEL_FORMAT pixelFormat;
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

	//レンダリング設定
	//UNKNOWNでお任せ設定
	pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;

	//クライアント領域やディスプレイ設定に合わせる
	//ハードウェアを利用するかどうか
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	//画面の色設定
	renderTargetProperties.pixelFormat = pixelFormat;
	renderTargetProperties.dpiX = 0;//1インチ当たりのドット数
	renderTargetProperties.dpiY = 0;//0でお任せ
	//GDIと連携を取るか、リモート環境に対応するか？
	renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	//内部で使う機能レベル
	renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	//レンダリング対象の設定
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties;
	//表示先のウインドウハンドル
	hwndRenderTargetProperties.hwnd = hwnd;
	//表示領域のサイズ
	hwndRenderTargetProperties.pixelSize = SizeU(clientWidth, clientHeight);
	//モニターと同期を取るかの設定はここ(非同期設定しても効果なし)
	hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

	//設定データを渡してレンダリングターゲット初期化
	hr = m_Factory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties, &m_RenderTarget);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"レンダーターゲットの作成失敗", L"エラー", MB_OK);
		return hr;
	}
	return hr;
}
void DXGraphic2D::Release()
{
	//Releaseメンバ関数を呼び出して後処理を行う
	SAFE_RELEASE(m_RenderTarget);
	SAFE_RELEASE(m_Factory);

	//COMの後処理
	CoUninitialize();
}
void DXGraphic2D::DrawStart()
{
	//表示開始
	m_RenderTarget->BeginDraw();
	//画面クリア(指定の色で塗りつぶす)
	m_RenderTarget->Clear(ColorF(0, 0.5f, 1.0f));
}
void DXGraphic2D::DrawEnd()
{
	//表示終了(モニターの更新を待つ)
	m_RenderTarget->EndDraw();
}