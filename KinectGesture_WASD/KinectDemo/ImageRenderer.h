#pragma once
#include <d2d1.h>

class ImageRenderer
{
public:
    ImageRenderer(void);
    ~ImageRenderer(void);

    // ��ʼ��ͼƬ��Ⱦ������kinect��ȡ����Ƶ����ʾ����Ӧ�Ŀؼ��ϡ�
    HRESULT Initialize(HWND hwnd, ID2D1Factory* pD2DFactory, int sourceWidth, int sourceHeight, int sourceStride);

    // ����ͼƬ
    // pImage ��RGBX��ʽ��ͼƬ���ݣ�cbImageΪͼƬ���ݵ��ֽڴ�С
    HRESULT Draw(BYTE* pImage, unsigned long cbImage);

    void DrawCircle(int x, int y, int radius = 10);
private:
    // ͼƬ���ƵĿؼ�
    HWND m_hWnd;
    // ͼƬ�ĸ�
    UINT m_sourceHeight;
    // ͼƬ�Ŀ�
    UINT m_sourceWidth;
    // ͼƬ�߻��Ŀ�ȡ�һά�����ʾһ������ʱ����ֵ��ʾ�����������
    UINT m_sourceStride;

    // D2D����
    ID2D1Factory* m_pD2DFactory;
    // D2D��ȾĿ��
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1Bitmap* m_pBitmap;

    D2D1_ELLIPSE m_ellipse;
    ID2D1SolidColorBrush* m_pBrush;

    // ȷ����ص�D2D��Դ�Ѵ���
    HRESULT EnsureResources();

    D2D1_ELLIPSE m_Position;

    // �ͷ�D2D����Դ
    void DiscardResources();
};

