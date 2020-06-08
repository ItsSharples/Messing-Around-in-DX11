#include "pch.h"
#include "DeviceResources2D.h"
#include <Debugging\Logging.h>

using Microsoft::WRL::ComPtr;

#if 0
DeviceResources2D::~DeviceResources2D()
{
    // TODO Work out why these resources apparently "Leak" despite destruction
    writeFactory->Release();
    factory->Release();
    d2device->Release();
    d2DeviceContext->Release();

    brush_map.clear();
    format_map.clear();
}
#endif

DeviceResources2D::DeviceResources2D():
    writeFactory(),
    factory(),
    d2device(),
    d2DeviceContext(),

    brush_map(),
    format_map(),

    default_brush(),
    default_format(),

    window(),

    status()
{}

void DeviceResources2D::CreateDeviceResources()
{
    // create the DirectWrite factory
    if(FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory7), &writeFactory)))
        throw std::runtime_error("Critical error: Unable to create the DirectWrite factory!");

    // create WIC factory
    //if (FAILED(CoCreateInstance(CLSID_WICImagingFactory2, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory2, &WICFactory)))
    //	throw std::runtime_error("Critical error: Unable to create the WIC factory!");

    // create the Direct2D factory
    D2D1_FACTORY_OPTIONS options;
#ifndef NDEBUG
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
    options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
    if(FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory7), &options, &factory)))
        throw std::runtime_error("Critical error: Unable to create Direct2D Factory!");

    status |= loaded_devices;
}

void DeviceResources2D::CreateDeviceDependentResources(ID3D11Device* device)
{
    // get the dxgi device
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    
    if(FAILED(device->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice)))
        throw std::runtime_error("Critical error: Unable to get the DXGI device!");

    // create the Direct2D device
    if(FAILED(factory->CreateDevice(dxgiDevice.Get(), &d2device)))
        throw std::runtime_error("Critical error: Unable to create the Direct2D device!");

    // create its context
    if(FAILED(d2device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &d2DeviceContext)))
        throw std::runtime_error("Critical error: Unable to create the Direct2D device context!");

    status |= loaded_d3devices;
}

void DeviceResources2D::CreateDefaultItems()
{
    if(status >= loaded_defaults) return; // Avoid Creating Twice

    AddBrush(DefaultBrushName, DefaultBrushColor);
    AddTextFormat(DefaultFontName);

    default_format = GetTextFormat(DefaultFontName);
    default_brush = GetBrush(DefaultBrushName);

    status |= loaded_defaults;
}


HRESULT DeviceResources2D::AddBrush(std::wstring brush_name, D2D1::ColorF brush_color)
{
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> new_brush;

    HRESULT hr = CreateBrush(&new_brush, brush_color);

    brush_map.emplace(brush_name, new_brush);

    return hr;

}

HRESULT DeviceResources2D::AddTextFormat(std::wstring font_name)
{
    Microsoft::WRL::ComPtr<IDWriteTextFormat> new_format;

    HRESULT hr = CreateTextFormat(&new_format, font_name);

    format_map.emplace(font_name, new_format);

    return hr;
}

HRESULT DeviceResources2D::CreateBrush(ID2D1SolidColorBrush** brush, D2D1::ColorF brush_color)
{
    return d2DeviceContext->CreateSolidColorBrush(brush_color, brush);
}

HRESULT DeviceResources2D::CreateTextFormat(IDWriteTextFormat** format, std::wstring font_name)
{
    return writeFactory->CreateTextFormat(
        font_name.c_str(), 
        NULL, 
        DWRITE_FONT_WEIGHT_REGULAR, 
        DWRITE_FONT_STYLE_NORMAL, 
        DWRITE_FONT_STRETCH_NORMAL, 
        16.0f, 
        L"en-gb", 
        format
    );
}

IDWriteTextLayout* DeviceResources2D::CreateLayout(const std::wstring& text, IDWriteTextFormat* format)
{
    if(status >= loaded_devices)
    {
        Microsoft::WRL::ComPtr<IDWriteTextLayout> newLayout;
        writeFactory->CreateTextLayout(text.c_str(), text.length(), format, window.width, window.height, &newLayout);
        return newLayout.Get();
    }
    return nullptr;
}

IDWriteTextFormat* DeviceResources2D::GetTextFormat(std::wstring font_name)
{
    // If the font name is not valid
    if(format_map.find(font_name) == format_map.end())
    {
        return nullptr;
    }

    // Yay ^-^
    return format_map.at(font_name).Get();
}

ID2D1SolidColorBrush* DeviceResources2D::GetBrush(std::wstring brush_name)
{
    // If the brush name is not valid
    if(brush_map.find(brush_name) == brush_map.end())
    {
        return nullptr;
    }

    // Yay ^-^
    return brush_map.at(brush_name).Get();
}

void DeviceResources2D::BeginDraw()
{
    if(status >= loaded_defaults)
    {
        d2DeviceContext->BeginDraw();

        d2DeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

        //d2DeviceContext->Clear(D2D1::ColorF(0, 0, 0, 0));

        status |= is_drawing;
    }
}



void DeviceResources2D::DrawTextLayout(D2D1_POINT_2F topLeft, IDWriteTextLayout* layout, ID2D1SolidColorBrush* brush)
{
    if(status >= able_to_draw)
    {
        d2DeviceContext->DrawTextLayout(topLeft, layout, brush);
    }
}

HRESULT DeviceResources2D::EndDraw()
{
    if(status >= able_to_draw)
    {
        status ^= is_drawing; // Un draw
        return d2DeviceContext->EndDraw();
    }
}

#pragma region DrawText
void DeviceResources2D::DrawText(std::wstring text, D2D1_POINT_2F topLeft)
{
    DrawText(text, topLeft, default_format, default_brush);
}

void DeviceResources2D::DrawText(std::wstring text, D2D1_POINT_2F topLeft, IDWriteTextFormat* format)
{

    DrawText(text, topLeft, format, default_brush);
}

void DeviceResources2D::DrawText(std::wstring text, D2D1_POINT_2F topLeft, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush)
{
    if(status >= able_to_draw)
    {
        //format->GetFontSize();
        
        d2DeviceContext->DrawText(text.c_str(), text.length(), format, { topLeft.x, window.width, topLeft.y, window.height }, brush);
        //Logging::DEBUG("Drew: \"" + std::string(text.begin(), text.end()) + "\" at: {" + std::to_string(topLeft.x) + "," + std::to_string(topLeft.y) + "}");
    }
}

#pragma endregion

void Null()
{
#if 0
    if(factory)
    {
        d2DeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Yellow),
            &pYellowBrush_);
    }
#endif
}




// TODO Plug Into DR2D
void DeviceResources2D::CreateD2DTarget(IDXGISwapChain* swap_chain)
{
    // specify the desired bitmap properties
    D2D1_BITMAP_PROPERTIES1 bp;
    bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    bp.dpiX = 96.0f;
    bp.dpiY = 96.0f;
    bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    bp.colorContext = nullptr;

    // Direct2D needs the DXGI version of the back buffer
    Microsoft::WRL::ComPtr<IDXGISurface> dxgiBuffer;

    if(FAILED(swap_chain->GetBuffer(0, __uuidof(IDXGISurface), &dxgiBuffer)))
        throw std::runtime_error("Critical error: Unable to retrieve the back buffer!");

    // create the bitmap
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;

    if(FAILED(d2DeviceContext->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, &targetBitmap)))
        throw std::runtime_error("Critical error: Unable to create the Direct2D bitmap from the DXGI surface!");

    // set the newly created bitmap as render target
    d2DeviceContext->SetTarget(targetBitmap.Get());

    // set antialiasing for text to grayscale
    d2DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

    status |= loaded_swapchain;
}

