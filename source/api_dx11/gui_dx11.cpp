#include "shared.hpp"
#include "gui_dx11.hpp"
#include "render_api_dx11.hpp"
#include "i_input.hpp"
#include <imgui/imgui.h>

static AGN::GUIDX11* g_instance = nullptr; // TODO: refactor

void AGN::GUIDX11::render(ImDrawData* draw_data)
{
	// TODO:
}

#if defined(__GNUC__) || defined(__GNUG__)
// static friend functions are not supported in GCC
void renderDrawLists(ImDrawData* draw_data)
{
	// TODO:
}
#else
void AGN::renderDrawLists(ImDrawData* draw_data)
{
	// TODO:
}	
#endif

void AGN::GUIDX11::createDeviceObjects()
{
	// TODO:
}

void AGN::GUIDX11::createImGUIFont()
{
	// TODO:
}

void AGN::GUIDX11::invalidateDeviceObjects()
{
	// TODO:
}

AGN::GUIDX11::GUIDX11()
	: m_isEnabled(false)
{
	if (g_instance != nullptr) assert(false); // ensure only one gui can exist // TODO: refactor
	g_instance = this;
}

void AGN::GUIDX11::shutdown()
{
	invalidateDeviceObjects();
	ImGui::Shutdown();
}

void AGN::GUIDX11::update(float a_deltaTime)
{
	/*
	if (!m_fontTexture)
	{
		createDeviceObjects();
	}

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	// TODO: retrieve from window itself
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	io.DisplaySize = ImVec2((float)w, (float)h);

	// Setup time step
	io.DeltaTime = a_deltaTime;

	// Setup inputs
	// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
	int mx, my;
	Uint32 mouseMask = SDL_GetMouseState(&mx, &my);

	io.MousePos = ImVec2(	static_cast<float>(g_input.getMouseX()),
							static_cast<float>(g_input.getMouseY()));

	io.MouseDown[0] = g_input.getMouse(EMOUSECODE::LEFT);
	io.MouseDown[1] = g_input.getMouse(EMOUSECODE::RIGHT);
	io.MouseDown[2] = g_input.getMouse(EMOUSECODE::MIDDLE);

	if (g_input.getMouseScroll() > 0) io.MouseWheel = 1;
	else if (g_input.getMouseScroll() < 0) io.MouseWheel = -1;
	else io.MouseWheel = 0;
	
	// Hide OS mouse cursor if ImGui is drawing it
	// TODO:
	//SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

	// Start the frame
	ImGui::NewFrame();
	*/
}