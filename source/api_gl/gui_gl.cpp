#include "shared.hpp"
#include "gui_gl.hpp"
#include "render_api_gl.hpp"

#include <imgui/imgui.h>
#include <gl/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_opengl.h>

// ImGui SDL2 binding with OpenGL
// You can copy and use unmodified imgui_impl_* files in your project. 
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// See main.cpp for an example of using this.
// https://github.com/ocornut/imgui

// Data
static double       g_time = 0.0f;
static bool         g_mousePressed[3] = { false, false, false };
static float        g_mouseWheel = 0.0f;
static uint32_t     g_fontTexture = 0;

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void renderDrawLists(ImDrawData* draw_data)
{
	// We are using the OpenGL fixed pipeline to make the example code simpler to read!
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

	// Handle cases of screen coordinates != from framebuffer coordinates (e.g. retina displays)
	ImGuiIO& io = ImGui::GetIO();
	float fb_height = io.DisplaySize.y * io.DisplayFramebufferScale.y;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Render command lists	
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))		// TODO: lolwut
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
		const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();
		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (uint32_t)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}
#undef OFFSETOF // TODO: lolwut

	// Restore modified state
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);

	AGN::RenderAPIGL::getOpenGLError();

	// TODO: properly restore state
}

static const char* ImGui_ImplSdl_GetClipboardText()
{
	return SDL_GetClipboardText();
}

static void ImGui_ImplSdl_SetClipboardText(const char* text)
{
	SDL_SetClipboardText(text);
}

bool AGN::GUIGL::processEvent(SDL_Event* event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
	case SDL_MOUSEWHEEL:
	{
		if (event->wheel.y > 0)
			g_mouseWheel = 1;
		if (event->wheel.y < 0)
			g_mouseWheel = -1;
		return true;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		if (event->button.button == SDL_BUTTON_LEFT) g_mousePressed[0] = true;
		if (event->button.button == SDL_BUTTON_RIGHT) g_mousePressed[1] = true;
		if (event->button.button == SDL_BUTTON_MIDDLE) g_mousePressed[2] = true;
		return true;
	}
	case SDL_TEXTINPUT:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(event->text.text);
		return true;
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
		io.KeysDown[key] = (event->type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		return true;
	}
	}
	return false;
}

void AGN::GUIGL::createDeviceObjects()
{
	createImGUIFont();
	createShaders();

}

void AGN::GUIGL::createShaders()
{
	// TODO:
}

void AGN::GUIGL::createImGUIFont()
{

	/*
	// OLD code; Build texture
	ImGuiIO& io = ImGui::GetIO();

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

	AGN::RenderAPIGL::getOpenGLError();

	// Create texture
	glGenTextures(1, &g_fontTexture);
	glBindTexture(GL_TEXTURE_2D, g_fontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	AGN::RenderAPIGL::getOpenGLError();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
	AGN::RenderAPIGL::getOpenGLError();

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_fontTexture;

	AGN::RenderAPIGL::getOpenGLError();

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();
	*/

	ImGuiIO& io = ImGui::GetIO();

	// Build texture atlas
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

	// Create OpenGL texture
	glGenTextures(1, &g_fontTexture);
	glBindTexture(GL_TEXTURE_2D, g_fontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_fontTexture;

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();


	AGN::RenderAPIGL::getOpenGLError();
}

void AGN::GUIGL::invalidateDeviceObjects()
{
	if (g_fontTexture)
	{
		glDeleteTextures(1, &g_fontTexture);
		ImGui::GetIO().Fonts->TexID = 0;
		g_fontTexture = 0;
	}
}

bool AGN::GUIGL::init(SDL_Window *a_window)
{
	m_window = a_window;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;                     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDLK_a;
	io.KeyMap[ImGuiKey_C] = SDLK_c;
	io.KeyMap[ImGuiKey_V] = SDLK_v;
	io.KeyMap[ImGuiKey_X] = SDLK_x;
	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	io.KeyMap[ImGuiKey_Z] = SDLK_z;

	io.RenderDrawListsFn = renderDrawLists;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.SetClipboardTextFn = ImGui_ImplSdl_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplSdl_GetClipboardText;

#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_window, &wmInfo);
	io.ImeWindowHandle = wmInfo.info.win.window;
#endif

	AGN::RenderAPIGL::getOpenGLError();

	return true;
}

void AGN::GUIGL::shutdown()
{
	invalidateDeviceObjects();
	ImGui::Shutdown();
}

void AGN::GUIGL::update(float a_deltaTime)
{
	if (!g_fontTexture)
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
	if (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MOUSE_FOCUS)
	{
		io.MousePos = ImVec2((float)mx, (float)my);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
	}
	else
	{
		io.MousePos = ImVec2(-1, -1);
	}

	io.MouseDown[0] = g_mousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
	io.MouseDown[1] = g_mousePressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
	io.MouseDown[2] = g_mousePressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
	g_mousePressed[0] = g_mousePressed[1] = g_mousePressed[2] = false;

	io.MouseWheel = g_mouseWheel;
	g_mouseWheel = 0.0f;

	// Hide OS mouse cursor if ImGui is drawing it
	SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

	AGN::RenderAPIGL::getOpenGLError();

	// Start the frame
	ImGui::NewFrame();
}