#include "DXString2D.h"


#pragma comment( lib, "dwrite.lib" )

IDWriteFactory* DXString2D::m_WriteFactory;
ID2D1HwndRenderTarget* DXString2D::m_RenderTarget;

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


HRESULT DXString2D::InitClass(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr = S_OK;
	//�t�@�N�g���[�쐬
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,//�v���Z�X�ԂŃL���b�V���f�[�^�����L
							__uuidof(IDWriteFactory),	//�R���|�[�l���g��ID
							reinterpret_cast<IUnknown**>(&m_WriteFactory));
	if (FAILED(hr))
		MessageBox(NULL, L"�e�L�X�g�t�@�N�g���̍쐬���s", L"�G���[", MB_OK);

	//���낢��g���̂ŕۑ����Ă���
	m_RenderTarget = renderTarget;
	return hr;
}

void DXString2D::Cleanup()
{
	SAFE_RELEASE(m_WriteFactory);
}


HRESULT DXString2D::Initialize(float fontSize)
{
	//�e�L�X�g�t�H�[�}�b�g�쐬
	HRESULT hr = m_WriteFactory->CreateTextFormat(
		L"���C���I",				//�R���N�V�����ɂ���t�H���g��(�l�r �S�V�b�N�Ȃ�)
		NULL,						//�t�H���g�R���N�V����(NULL�ŃV�X�e���t�H���g)
		DWRITE_FONT_WEIGHT_NORMAL,	//����
		DWRITE_FONT_STYLE_NORMAL,	//�X�^�C��(�C�^���b�N�^�C�Ȃ�)
		DWRITE_FONT_STRETCH_NORMAL,	//��
		fontSize,					//�����̑傫��
		L"ja-jp",					//���P�[��(ja-jp��en-us)

		&m_Format);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�e�L�X�g�t�H�[�}�b�g�̍쐬���s", L"�G���[", MB_OK);
		return hr;
	}

	//�����̐F�����߂�u���V���쐬
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
	//�e�L�X�g�G���A
	//�������s������̂œK���ɑ傫���L���Ă���
	//���W�ϊ��̉e�����󂯂�̂ł����ł����W�ϊ��ňʒu����
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