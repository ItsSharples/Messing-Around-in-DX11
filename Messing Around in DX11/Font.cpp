Font::Font() :
    pD2DFactory_(NULL),
    pYellowBrush_(NULL),
    pDWriteFactory_(NULL),
    pTextFormat_(NULL)
{
    CreateDeviceIndependentResources();
}

Font::Font(HWND hwnd): Font()
{
    UseHWND(hwnd);
    CreateDeviceResources();
}

Font::~Font()
{
    DX::SafeRelease(&pD2DFactory_);
    DX::SafeRelease(&pYellowBrush_);
    DX::SafeRelease(&pDWriteFactory_);
    DX::SafeRelease(&pTextFormat_);
}


HRESULT Font::Render()
{
    HRESULT hr;

    //hr = CreateDeviceResources();

    if(m_pRenderTarget)
    {

        static const WCHAR sc_helloWorld[] = L"Hello, World!";

        // Retrieve the size of the render target.
        D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

        m_pRenderTarget->BeginDraw();

        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        m_pRenderTarget->Clear();

        m_pRenderTarget->DrawText(
            sc_helloWorld,
            ARRAYSIZE(sc_helloWorld) - 1,
            pTextFormat_,
            D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
            pYellowBrush_
        );

        hr = m_pRenderTarget->EndDraw();

        if(hr == D2DERR_RECREATE_TARGET)
        {
            hr = S_OK;
            DiscardDeviceResources();
        }
    }


    return hr;
}

HRESULT Font::CreateDeviceIndependentResources()
{
    HRESULT hr;

    // Create Direct2D factory.

    


    // Create a text format using Gabriola with a font size of 72.
    // This sets the default font, weight, stretch, style, and locale.
#if 0
    if (SUCCEEDED(hr))
    {
        hr = SetUpFont(L"Fira Code");
    }

    if (SUCCEEDED(hr))
    {
        hr = SetUpFont(L"Gabriola");
    }

    pTextFormat_ = TextFormatMap.at(L"Gabriola");
#endif
    return hr;
}


void Font::DiscardDeviceIndependentResources()
{}

HRESULT Font::CreateDeviceResources()
{
    HRESULT hr = S_OK;


    RECT rc;
    GetClientRect(m_Hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(
        rc.right - rc.left,
        rc.bottom - rc.top
    );

    // Create a Direct2D render target.





    return hr;
}

void Font::DiscardDeviceResources()
{
    DX::SafeRelease(&m_pRenderTarget);
    DX::SafeRelease(&pYellowBrush_);
}

#if 0
HRESULT Font::updateString(const std::wstring& new_string)
{
    stringText = new_string;
    cTextLength_ = (UINT32)stringText.length();

    D2D1_SIZE_F rt_size;
    if (pRT_)
    {
        rt_size = pRT_->GetSize();
    }
    else
    {
        if(renderer)
        {
            auto tmp = renderer->GetTargetSize();
            rt_size.width = tmp.x;
            rt_size.height = tmp.y;
        }
        else
        {
            rt_size = D2D1_SIZE_F(); 
            rt_size.width = 500;
            rt_size.height = 100;
        }
    }

    return pDWriteFactory_->CreateTextLayout(
        stringText.c_str(),      // The string to be laid out and formatted.
        cTextLength_,  // The length of the string.
        pTextFormat_,  // The text format to apply to the string (contains font information, etc).
        rt_size.width,         // The width of the layout box.
        rt_size.height,        // The height of the layout box.
        &pTextLayout_  // The IDWriteTextLayout interface pointer.
    );
}

HRESULT Font::SetUpFont(std::wstring font_name)
{
    HRESULT hr;


    IDWriteTextFormat* ptr;
    
    hr = pDWriteFactory_->CreateTextFormat(
        font_name.c_str(),          // Font family name.
        NULL,                       // Font collection (NULL sets it to use the system font collection).
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16.0f,
        L"en-gb",
        &ptr
    );


    // Center align (horizontally) the text.
    if (SUCCEEDED(hr))
    {
        hr = ptr->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
    }

    if (SUCCEEDED(hr))
    {
        hr = ptr->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }

    TextFormatMap.emplace(font_name, ptr);

    return hr;

}

    HRESULT hr = S_OK;

    // Escape early
    if(pRT_)
    {
        return hr;
    }

    if(!renderer)
    {
        return hr;
    }


    // Create a Direct2D render target.
    hr = pD2DFactory_->ID2D1Factory::CreateDxgiSurfaceRenderTarget(
        renderer->GetDXGISurface(),
        D2D1::RenderTargetProperties(),
        &pRT_
    );

    // Create a black brush.
    if(SUCCEEDED(hr))
    {
        hr = pRT_->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Yellow),
            &pYellowBrush_);
    }


    return hr;
}




HRESULT Font::DrawD2DContent()
{
    // TODO check if this is escaping as early as it can?

    HRESULT hr;

    hr = CreateDeviceResources();

    if(pRT_)
    {
        pRT_->BeginDraw();

        pRT_->SetTransform(D2D1::IdentityMatrix());

        pRT_->Clear(D2D1::ColorF(D2D1::ColorF::Black));

        pRT_->DrawTextLayout({ 1,1 }, pTextLayout_, pYellowBrush_);

        if(SUCCEEDED(hr))
        {
            hr = pRT_->EndDraw();
        }
    }



    if(FAILED(hr))
    {
        DiscardDeviceResources();
    }

    return hr;
}
#endif