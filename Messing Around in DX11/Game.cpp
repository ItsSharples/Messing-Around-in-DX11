//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include <iostream>
#include <string>

#include "Debuggers.h"
#include <BaseObject.h>

#include <SimpleHelper.h>

#include "Random.h"

#include <strsafe.h>
#include <CPUUsageTimer.h>

#include "Performance.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

    m_d2deviceResources = std::make_unique<DeviceResources2D>();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);
    m_d2deviceResources->SetWindow(width, height);

    m_deviceResources->CreateDeviceResources();
    m_d2deviceResources->CreateDeviceResources();


    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    CreateDeviceIndependentResources();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    Time::start();
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });
    m_fixedTimer.Tick([&]()
    {
        FixedUpdate(m_fixedTimer);
    });

    m_frTimer.Tick([&]()
    {
        int current_fps = m_timer.GetFramesPerSecond();
        int current_fixed_fps = m_fixedTimer.GetFramesPerSecond();
        Logging::INFO("Normal fps: " + std::to_string(current_fps));
        Logging::INFO("Fixed fps: " + std::to_string(current_fixed_fps));

        auto total_util = update_counter + render_counter;
        auto update_util = 100 * update_counter / total_util;
        auto render_util = 100 * render_counter / total_util;

        Logging::INFO("Utilisation: " + std::to_string(total_util / 10) + "%");
        Logging::INFO("Update Share: " + std::to_string(update_util) + "%");
        Logging::INFO("Render Share: " + std::to_string(render_util) + "%");

        render_counter = 0;
        update_counter = 0;
        
        debug_str = std::wstring();
        debug_str += L"FPS: " + std::to_wstring(current_fps) + L"\n";
        debug_str += L"Fixed FPS: " + std::to_wstring(current_fixed_fps) + L"\n";
        debug_str += L"Update Share: " + std::to_wstring(update_util) + L"%\n";
        debug_str += L"Render Share: " + std::to_wstring(render_util) + L"%\n";
        debug_str += L"CPU Usage: " + std::to_wstring(Performance::CPU::GetUsage() * 100.f) + L"%\n";

        
        auto ram_bytes = Performance::RAM::GetUsage();
        auto ram_MBytes = ram_bytes / 1000000;
        debug_str += L"Ram Usage: " + std::to_wstring(ram_MBytes) + L"MB\n";

        Logging::Flush();
    });
    Time::stop();
    Time::start();
    Render();
    Time::stop();
    auto render_time = Time::getElapsedMilliseconds().count();
    auto update_time = Time::getElapsedMilliseconds().count();

    render_counter.store(render_counter.load() + render_time);
    update_counter.store(update_counter.load() + update_time);

    //Logging::INFO("Took " + std::to_string(update_time) + "ms to update");
    //Logging::INFO("Took " + std::to_string(render_time) + "ms to render");
    
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    m_deviceResources->PIXBeginEvent(L"Update");
    float elapsedTime = float(timer.GetElapsedSeconds());

    //Logging::INFO("Normal Update:" + std::to_string(elapsedTime) + "ms");

    // TODO: Add your game logic here.
    elapsedTime;
    m_deviceResources->PIXEndEvent();
}

void Game::FixedUpdate(DX::StepTimer const& timer)
{
    m_deviceResources->PIXBeginEvent(L"Fixed Update");
    float elapsedTime = float(timer.GetElapsedSeconds());

    //Logging::INFO("Fixed Update:" + std::to_string(elapsedTime) + "ms");

    // TODO: Add your game logic here.
    elapsedTime;
    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }


    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    m_d2deviceResources->BeginDraw();

    if(m_d2deviceResources->status >= m_d2deviceResources->able_to_draw)
    {
        // TODO Fix this mess ;-;
        Microsoft::WRL::ComPtr<IDWriteTextLayout> debug_layout;
        m_d2deviceResources->writeFactory->CreateTextLayout(debug_str.c_str(), debug_str.length(), m_d2deviceResources->default_format, m_d2deviceResources->window.width, m_d2deviceResources->window.height, &debug_layout);
        auto brush = m_d2deviceResources->GetBrush();
        m_d2deviceResources->DrawTextLayout({ 2, 2 }, debug_layout.Get(), brush);

        m_d2deviceResources->DrawGeometry(test_geometry, geometry_num);
    }
    

    m_d2deviceResources->EndDraw();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    Logging::INFO("Activated");
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    Logging::INFO("Deactivated");
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    Logging::INFO("Suspended");
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    Logging::INFO("Resumed");
    m_timer.ResetElapsedTime();
    m_fixedTimer.ResetElapsedTime();
    m_frTimer.ResetElapsedTime();
    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    Logging::INFO("Moved");


    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    m_d2deviceResources->WindowSizeChanged(width, height);

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
    Logging::INFO("Resized to size: {" + std::to_string(width) + "," + std::to_string(height) + "}");
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = default_width;
    height = default_height;
}
#pragma endregion


void Game::CreateDeviceIndependentResources()
{
    CreateLogger();

    CreateTimers();

    // Testing Stuff
    //Vector3 test{ 1, 2, 3 };
    //auto first = random_generator.rand();
    //auto second = random_generator.rand();
    //Logging::DEBUG("First Two 'Random' Values: "+ std::to_string(first) +"," + std::to_string(second));
    //Logging::DEBUG(SimpleHelpers::to_string(test));

    FLOAT current_height = mid_height;
    FLOAT width_segment = default_width / ((FLOAT)geometry_num - 1.f);
    FLOAT max_height = m_deviceResources->GetOutputSize().bottom;
    FLOAT min_height = current_height - 200.f;
    for(int i = 0; i < geometry_num; i++)
    {
        FLOAT up_down = random_generator.rand(0, 2) - 1.f;
        current_height = (current_height + up_down);
        if(current_height > max_height)
            current_height = max_height;
        if(current_height < min_height)
            current_height = min_height;
        test_geometry[i] = { i * width_segment,  current_height};
    };

}

void Game::CreateTimers()
{
    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    m_timer.SetTargetElapsedTicks(1);

    m_fixedTimer.SetFixedTimeStep(true);
    m_fixedTimer.SetTargetElapsedSeconds(1.0 / 60);

    m_frTimer.SetFixedTimeStep(true);
    m_frTimer.SetTargetElapsedSeconds(1.0);
}

void Game::CreateLogger()
{
    Logging::StartLogging("buffered std out", Logging::LogLevels::ALL);
}

void Game::CreateRandom()
{
    random_generator.seed(1);
}

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{

    auto device = m_deviceResources->GetD3DDevice();
   
    
    // TODO: Initialize device dependent objects here (independent of window size).
    device;

    
    m_d2deviceResources->CreateDeviceDependentResources(device);
    
    m_d2deviceResources->CreateDefaultItems();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto swapchain = m_deviceResources->GetSwapChain();
    m_d2deviceResources->UpdateSwapChain(swapchain);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
