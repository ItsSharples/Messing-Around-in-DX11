//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "DeviceResources2D.h"


#include "StepTimer.h"
#include <atomic>

#include "Random.h"
#include <RenderTarget.h>
#include <Debug_Graph.h>


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void FixedUpdate(DX::StepTimer const& timer);
    void Render();

    void Clear();

    
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    void CreateDeviceIndependentResources();
#pragma region IndependentResources
    void CreateTimers();
    void CreateLogger();
    void CreateRandom();
#pragma endregion



    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;
    std::unique_ptr<DeviceResources2D>      m_d2deviceResources;

    // Timers.
    DX::StepTimer                           m_timer;// Normal Update Timer
    DX::StepTimer                           m_fixedTimer;// Fixed Update Timer
    DX::StepTimer                           m_frTimer;// Frame Rate Timer

    // Debug counters
    std::atomic<double> update_counter = 0;
    std::atomic<double> render_counter = 0;
    std::atomic<int> move_count = 0;

    // Randoms
    Random::xorshift128_state random_state;
    Random::xor128 random_generator;

    // Text Support

    std::wstring debug_str;

    std::unique_ptr<RenderTarget> debug_target;
    //ID2D1DeviceContext* text_context = nullptr;
    //ID2D1Factory* pD2DFactory_ = nullptr;
    //IDWriteFactory* pDWriteFactory_ = nullptr;
	//IDWriteTextFormat* pTextFormat_ = nullptr;
    //ID2D1SolidColorBrush* pYellowBrush_ = nullptr;
    //std::thread m_updatePool[2];


    static constexpr int default_width = 800;
    static constexpr int default_height = 600;

    //RenderTarget* text_renderer = nullptr;
    void GenerateTerrain();

    Debug_Graph fps_graph;

};
