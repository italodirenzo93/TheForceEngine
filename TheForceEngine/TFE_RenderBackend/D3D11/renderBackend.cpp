#include <TFE_RenderBackend/renderBackend.h>
#include <TFE_RenderBackend/dynamicTexture.h>
#include <TFE_RenderBackend/textureGpu.h>
#include <TFE_Settings/settings.h>
#include <TFE_Ui/ui.h>
#include <TFE_Asset/imageAsset.h>	// For image saving, this should be refactored...
#include <TFE_System/profiler.h>
#include <TFE_PostProcess/blit.h>
#include <TFE_PostProcess/bloomThreshold.h>
#include <TFE_PostProcess/bloomDownsample.h>
#include <TFE_PostProcess/bloomMerge.h>
#include <TFE_PostProcess/postprocess.h>
#include <SDL.h>
#include <SDL_syswm.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>
#undef min
#undef max

#include <d3d11_4.h>
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dcompiler.lib")

namespace TFE_RenderBackend
{
	static ID3D11Device* s_device = nullptr;
	static ID3D11DeviceContext* s_deviceContext = nullptr;
	static D3D_FEATURE_LEVEL s_featureLevel;

	SDL_Window* createWindow(const WindowState& state)
	{
		//u32 windowFlags = SDL_WINDOW_OPENGL;
		u32 windowFlags = 0;
		bool windowed = !(state.flags & WINFLAG_FULLSCREEN);

		TFE_Settings_Window* windowSettings = TFE_Settings::getWindowSettings();
		
		s32 x = windowSettings->x, y = windowSettings->y;
		s32 displayIndex = getDisplayIndex(x, y);
		assert(displayIndex >= 0);
		
		if (windowed)
		{
			y = std::max(32, y);
			windowSettings->y = y;
			windowFlags |= SDL_WINDOW_RESIZABLE;
		}
		else
		{
			MonitorInfo monitorInfo;
			getDisplayMonitorInfo(displayIndex, &monitorInfo);

			x = monitorInfo.x;
			y = monitorInfo.y;
			windowFlags |= SDL_WINDOW_BORDERLESS;
		}

		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

		TFE_System::logWrite(LOG_MSG, "RenderBackend", "SDL Videodriver: %s", SDL_GetCurrentVideoDriver());
		SDL_Window* window = SDL_CreateWindow(state.name, x, y, state.width, state.height, windowFlags);
		if (!window)
		{
			TFE_System::logWrite(LOG_ERROR, "RenderBackend", "SDL_CreateWindow() failed: %s", SDL_GetError());
			return nullptr;
		}

		//SDL_GLContext context = SDL_GL_CreateContext(window);
		//if (!context)
		//{
		//	SDL_DestroyWindow(window);
		//	TFE_System::logWrite(LOG_ERROR, "RenderBackend", "SDL_GL_CreateContext() failed: %s", SDL_GetError());
		//	return nullptr;
		//}

		//int glver = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
		//if (glver == 0)
		//{
		//	TFE_System::logWrite(LOG_ERROR, "RenderBackend", "cannot initialize GLAD");
		//	SDL_GL_DeleteContext(context);
		//	SDL_DestroyWindow(window);
		//	return nullptr;
		//}

		//OpenGL_Caps::queryCapabilities();
		//printGLInfo();
		//int tier = OpenGL_Caps::getDeviceTier();
		//TFE_System::logWrite(LOG_MSG, "RenderBackend", "OpenGL Device Tier: %d", tier);
		//if (tier < 2)
		//{
		//	TFE_System::logWrite(LOG_ERROR, "RenderBackend", "Insufficient GL capabilities!");
		//	SDL_GL_DeleteContext(context);
		//	SDL_DestroyWindow(window);
		//	return nullptr;
		//}

		//swap buffer at the monitors rate
		//SDL_GL_SetSwapInterval((state.flags & WINFLAG_VSYNC) ? 1 : 0);

		MonitorInfo monitorInfo;
		getDisplayMonitorInfo(displayIndex, &monitorInfo);
		// High resolution displays (> 1080p) tend to be very high density, so increase the scale somewhat.
		s32 uiScale = 100;
		if (monitorInfo.h >= 2160) // 4k+
		{
			uiScale = 125 * monitorInfo.h / 1080;	// scale based on 1080p being the base.
		}
		else if (monitorInfo.h >= 1440) // 1440p
		{
			uiScale = 150;
		}

	#ifndef _WIN32
		SDL_SetWindowFullscreen(window, windowed ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
	#endif

		// Direct3D stuff
		SDL_SysWMinfo wmInfo;
		SDL_GetWindowWMInfo(window, &wmInfo);

		if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 1, D3D11_SDK_VERSION, &s_device, &s_featureLevel, &s_deviceContext)))
		{
			TFE_System::logWrite(LOG_ERROR, "RenderBackend", "Could not create a D3D11 device");
			SDL_DestroyWindow(window);
			return nullptr;
		}

		TFE_Ui::init(window, s_deviceContext, uiScale);
		return window;
	}
}
