#include "DXTexture2D.h"

//IWICImagingFactoryを使うために必要
#pragma comment( lib, "WindowsCodecs.lib" )


IWICImagingFactory* DXTexture2D::m_WICImagingFactory;
ID2D1HwndRenderTarget* DXTexture2D::m_RenderTarget;

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

HRESULT DXTexture2D::InitClass(ID2D1HwndRenderTarget* renderTarget)
{
	//COMを使ってファクトリーを初期化
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void **>(&m_WICImagingFactory));
	if (FAILED(hr))
		MessageBox(NULL, L"イメージファクトリの作成失敗", L"エラー", MB_OK);

	//いろいろ使うので保存しておく
	m_RenderTarget = renderTarget;

	return hr;
}
void DXTexture2D::Cleanup()
{
	SAFE_RELEASE(m_WICImagingFactory);
}


HRESULT DXTexture2D::Load(LPCWSTR filePath)
{
	IWICBitmapDecoder* pdecoder = NULL;
	IWICBitmapFrameDecode* pbitmapFrame = NULL;
	IWICFormatConverter* pconverter = NULL;

	//デコーダー(ロード用のデータ)を作成
	HRESULT hr = m_WICImagingFactory->
		CreateDecoderFromFilename(
		filePath,						//ファイルパス(UNICODE文字列)
		NULL,							//デコーダーのGUID、特になければNULL
		GENERIC_READ,					//ファイルに対するアクセス、読み込みなのでGENERIC_READ
		WICDecodeMetadataCacheOnLoad,	//メタデータのロードタイミング
		&pdecoder);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"デコーダーの作成失敗", L"エラー", MB_OK);
		return hr;
	}

	//画像データを取り出す
	hr = pdecoder->GetFrame(0, &pbitmapFrame);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"フレームの取得失敗", L"エラー", MB_OK);
		return hr;
	}

	//画像を使用する形に変換するコンバーターを作成
	hr = m_WICImagingFactory->CreateFormatConverter(&pconverter);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンバーターの作成失敗", L"エラー", MB_OK);
		return hr;
	}

	//ビットマップ作成時のフォーマットはPRGBA、Alpha、BGR、PBGRの4つっぽい
	//PRGBAなどのPつきはカラー値がアルファ値で乗算済みになっている
	//レンダリングターゲットのアルファモードのD2D1_ALPHA_MODE_PREMULTIPLIEDと同じ
	//ビット深度の縮小形式に変換する場合はパレットデータを設定する、しない場合はNULLでOK
	hr = pconverter->Initialize(
		pbitmapFrame,					// BitmapSource
		GUID_WICPixelFormat32bppPRGBA,	// ピクセルフォーマット
		WICBitmapDitherTypeNone,		// ディザリング処理
		NULL,							// 変換に使用するパレット(変換なしなのでNULL)
		0.0f,							// アルファの閾値(変換なしなので0)
		WICBitmapPaletteTypeCustom);	// パレットタイプ(変換なしなのでCustom)
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンバーターの作成失敗", L"エラー", MB_OK);
		return hr;
	}

	//ビットマップ作成
	hr = m_RenderTarget->CreateBitmapFromWicBitmap(pconverter, &m_Bitmap);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"ビットマップの作成失敗", L"エラー", MB_OK);
		return hr;
	}

	//必要ないので保持しないで解放
	pdecoder->Release();
	pbitmapFrame->Release();
	pconverter->Release();

	return hr;
}
void DXTexture2D::Release()
{
	SAFE_RELEASE(m_Bitmap);
}

void DXTexture2D::Draw(float x, float y)
{
	D2D_POINT_2F pos = Point2F(x, y);
	D2D_POINT_2F scale = Point2F(1,1);
	D2D1_RECT_F useRect = RectF(0,0,m_Bitmap->GetSize().width, m_Bitmap->GetSize().height);
	Draw(pos, scale, 0, 1, true, useRect);
}


void DXTexture2D::Draw(D2D1_POINT_2F pos, D2D1_POINT_2F scale, float rotate, float alpha, bool center, D2D1_RECT_F useRect)
{
	D2D1_SIZE_F size;

	size.width = useRect.right - useRect.left;
	size.height = useRect.bottom - useRect.top;

	//行列で位置指定するので表示先では位置を変えない
	D2D1_RECT_F drawRect;
	if(center==false)
		drawRect = RectF(0, 0, size.width, size.height);
	else
		drawRect = RectF(-size.width*0.5f, -size.height*0.5f, size.width*0.5f, size.height*0.5f);

	//表示先領域の調整で中心表示を対応するので回転、拡大の基準位置は0,0にしておく
	//行列を使った座標変換による姿勢制御
	//位置、回転、拡大を指定

	//位置データ
	Matrix3x2F matTrans =
		Matrix3x2F::Translation(pos.x, pos.y);

	//回転(単位は角度)
	Matrix3x2F matRotate =
		Matrix3x2F::Rotation(rotate, Point2F(0, 0));

	//拡大
	Matrix3x2F matScale =
		Matrix3x2F::Scale(scale.x, scale.y, Point2F(0,0));

	//掛け算で行列を合成
	//行列は掛ける順番で結果が変わる
	//左のデータから順に適用される形になる
	//通常は拡大、回転、位置の順で合成する
	Matrix3x2F matWorld = matScale*matRotate*matTrans;

	//制御データを適用
	m_RenderTarget->SetTransform(matWorld);
	//m_RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	//レンダーターゲットが表示命令を持つ
	m_RenderTarget->DrawBitmap(m_Bitmap,//使う画像データ
		drawRect,//表示先の位置など
		alpha,	//α値
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,//拡大縮小するときの処理
		useRect);//画像内の使用する領域
}


float DXTexture2D::GetWidth()
{
	if (m_Bitmap != NULL)
		return m_Bitmap->GetSize().width;
	return 0;
}
float DXTexture2D::GetHeight()
{
	if (m_Bitmap != NULL)
		return m_Bitmap->GetSize().height;
	return 0;
}