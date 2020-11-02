#include "DXTexture2D.h"

//IWICImagingFactory���g�����߂ɕK�v
#pragma comment( lib, "WindowsCodecs.lib" )


IWICImagingFactory* DXTexture2D::m_WICImagingFactory;
ID2D1HwndRenderTarget* DXTexture2D::m_RenderTarget;

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

HRESULT DXTexture2D::InitClass(ID2D1HwndRenderTarget* renderTarget)
{
	//COM���g���ăt�@�N�g���[��������
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void **>(&m_WICImagingFactory));
	if (FAILED(hr))
		MessageBox(NULL, L"�C���[�W�t�@�N�g���̍쐬���s", L"�G���[", MB_OK);

	//���낢��g���̂ŕۑ����Ă���
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

	//�f�R�[�_�[(���[�h�p�̃f�[�^)���쐬
	HRESULT hr = m_WICImagingFactory->
		CreateDecoderFromFilename(
		filePath,						//�t�@�C���p�X(UNICODE������)
		NULL,							//�f�R�[�_�[��GUID�A���ɂȂ����NULL
		GENERIC_READ,					//�t�@�C���ɑ΂���A�N�Z�X�A�ǂݍ��݂Ȃ̂�GENERIC_READ
		WICDecodeMetadataCacheOnLoad,	//���^�f�[�^�̃��[�h�^�C�~���O
		&pdecoder);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�f�R�[�_�[�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}

	//�摜�f�[�^�����o��
	hr = pdecoder->GetFrame(0, &pbitmapFrame);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�t���[���̎擾���s", L"�G���[", MB_OK);
		return hr;
	}

	//�摜���g�p����`�ɕϊ�����R���o�[�^�[���쐬
	hr = m_WICImagingFactory->CreateFormatConverter(&pconverter);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�R���o�[�^�[�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}

	//�r�b�g�}�b�v�쐬���̃t�H�[�}�b�g��PRGBA�AAlpha�ABGR�APBGR��4���ۂ�
	//PRGBA�Ȃǂ�P���̓J���[�l���A���t�@�l�ŏ�Z�ς݂ɂȂ��Ă���
	//�����_�����O�^�[�Q�b�g�̃A���t�@���[�h��D2D1_ALPHA_MODE_PREMULTIPLIED�Ɠ���
	//�r�b�g�[�x�̏k���`���ɕϊ�����ꍇ�̓p���b�g�f�[�^��ݒ肷��A���Ȃ��ꍇ��NULL��OK
	hr = pconverter->Initialize(
		pbitmapFrame,					// BitmapSource
		GUID_WICPixelFormat32bppPRGBA,	// �s�N�Z���t�H�[�}�b�g
		WICBitmapDitherTypeNone,		// �f�B�U�����O����
		NULL,							// �ϊ��Ɏg�p����p���b�g(�ϊ��Ȃ��Ȃ̂�NULL)
		0.0f,							// �A���t�@��臒l(�ϊ��Ȃ��Ȃ̂�0)
		WICBitmapPaletteTypeCustom);	// �p���b�g�^�C�v(�ϊ��Ȃ��Ȃ̂�Custom)
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�R���o�[�^�[�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}

	//�r�b�g�}�b�v�쐬
	hr = m_RenderTarget->CreateBitmapFromWicBitmap(pconverter, &m_Bitmap);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�r�b�g�}�b�v�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}

	//�K�v�Ȃ��̂ŕێ����Ȃ��ŉ��
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

	//�s��ňʒu�w�肷��̂ŕ\����ł͈ʒu��ς��Ȃ�
	D2D1_RECT_F drawRect;
	if(center==false)
		drawRect = RectF(0, 0, size.width, size.height);
	else
		drawRect = RectF(-size.width*0.5f, -size.height*0.5f, size.width*0.5f, size.height*0.5f);

	//�\����̈�̒����Œ��S�\����Ή�����̂ŉ�]�A�g��̊�ʒu��0,0�ɂ��Ă���
	//�s����g�������W�ϊ��ɂ��p������
	//�ʒu�A��]�A�g����w��

	//�ʒu�f�[�^
	Matrix3x2F matTrans =
		Matrix3x2F::Translation(pos.x, pos.y);

	//��](�P�ʂ͊p�x)
	Matrix3x2F matRotate =
		Matrix3x2F::Rotation(rotate, Point2F(0, 0));

	//�g��
	Matrix3x2F matScale =
		Matrix3x2F::Scale(scale.x, scale.y, Point2F(0,0));

	//�|���Z�ōs�������
	//�s��͊|���鏇�ԂŌ��ʂ��ς��
	//���̃f�[�^���珇�ɓK�p�����`�ɂȂ�
	//�ʏ�͊g��A��]�A�ʒu�̏��ō�������
	Matrix3x2F matWorld = matScale*matRotate*matTrans;

	//����f�[�^��K�p
	m_RenderTarget->SetTransform(matWorld);
	//m_RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	//�����_�[�^�[�Q�b�g���\�����߂�����
	m_RenderTarget->DrawBitmap(m_Bitmap,//�g���摜�f�[�^
		drawRect,//�\����̈ʒu�Ȃ�
		alpha,	//���l
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,//�g��k������Ƃ��̏���
		useRect);//�摜���̎g�p����̈�
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