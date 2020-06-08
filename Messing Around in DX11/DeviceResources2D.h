#pragma once

#include <unordered_map>


namespace
{
#define DefaultBrushColor D2D1::ColorF(D2D1::ColorF::Yellow)
#define DefaultBrushName L"yellow"
#define DefaultFontName L"Fira Code"


}

class DeviceResources2D
{
public:
    DeviceResources2D();
    ~DeviceResources2D() = default;

    void SetWindow(int width, int height) { window = { (FLOAT)300, (FLOAT)300 }; }

    void CreateDeviceResources();
    void WindowSizeChanged(int width, int height) { SetWindow(width, height); };

    void CreateDeviceDependentResources(ID3D11Device* device);

    void CreateDefaultItems();

    HRESULT AddBrush(std::wstring brush_name, D2D1::ColorF brush_color);
    HRESULT AddTextFormat(std::wstring font_name);

    HRESULT CreateBrush(ID2D1SolidColorBrush** brush, D2D1::ColorF brush_color);
    HRESULT CreateTextFormat(IDWriteTextFormat** format, std::wstring font_name);

    IDWriteTextLayout* CreateLayout(const std::wstring& text, IDWriteTextFormat* format);

    IDWriteTextFormat* GetTextFormat(std::wstring font_name);
    ID2D1SolidColorBrush* GetBrush(std::wstring brush_name);

    IDWriteTextFormat* GetTextFormat() { return default_format; };
    ID2D1SolidColorBrush* GetBrush() { return default_brush; };

    void BeginDraw();
    HRESULT EndDraw();

    // Most basic form
    void DrawText(std::wstring text, D2D1_POINT_2F topLeft);


    // Using a Format instead of a layout
    void DrawText(std::wstring text, D2D1_POINT_2F topLeft, IDWriteTextFormat* format);
    void DrawText(std::wstring text, D2D1_POINT_2F topLeft, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush);

    // Using a specific Layout
    //void DrawText(std::wstring text, D2D1_POINT_2F topLeft, IDWriteTextLayout* layout);
    void DrawTextLayout(D2D1_POINT_2F topLeft, IDWriteTextLayout* layout, ID2D1SolidColorBrush* brush);




    void UpdateSwapChain(IDXGISwapChain* swap_chain) { CreateD2DTarget(swap_chain); }


    void CreateD2DTarget(IDXGISwapChain* swap_chain);

    Microsoft::WRL::ComPtr<IDWriteFactory7> writeFactory;
    Microsoft::WRL::ComPtr<ID2D1Factory7> factory;
    Microsoft::WRL::ComPtr<ID2D1Device6> d2device;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext6> d2DeviceContext;

    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> brush_map;
    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<IDWriteTextFormat>> format_map;

    ID2D1SolidColorBrush* default_brush;
    IDWriteTextFormat* default_format;

    D2D1_SIZE_F window{ 0,0 };

    

#define bitarray unsigned int
#define bitstatus static constexpr bitarray

    bitarray status;

    // Set up stages
    bitstatus unloaded = 0x00000000;
    bitstatus loaded_devices = 0x00000001;
    bitstatus loaded_d3devices = 0x00000010;
    bitstatus loaded_swapchain = 0x00000100;
    bitstatus loaded_defaults = 0x00001000;

    bitstatus is_drawing = 0x00010000;

    // Progress Markers
    bitstatus able_to_draw = 0x00011111;

    // As to not pollute the namespaces :p
#undef bitstatus
#undef bitarray


protected:
private:
};

