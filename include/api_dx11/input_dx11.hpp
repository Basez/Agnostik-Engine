#pragma once
#include "i_input.hpp"

namespace AGN
{
	class InputDX11 : public IInput
	{
	public:
		InputDX11();

		static AGN::ESCANCODE getAGNScanCode(uint32_t a_dx11scancode);

		void registerMouseClick(AGN::EMOUSECODE a_mouseButton) override;
		void registerMouseHold(AGN::EMOUSECODE a_mouseButton, bool a_held) override;
		void registerHold(AGN::ESCANCODE a_key, bool a_held) override;
		void registerFrameCompletion() override;
		void registerMouseScroll(int a_scrollAmount) override { m_scrollAmount = a_scrollAmount; }
		void registerMouseMotion(int a_mouseX, int a_mouseY) override;

		bool getKeyDown(AGN::ESCANCODE a_key) override;
		bool getKeyUp(AGN::ESCANCODE a_key) override;
		bool getKey(AGN::ESCANCODE a_key) override;
		bool getMouseDown(AGN::EMOUSECODE a_mouseButton) override;
		bool getMouseUp(AGN::EMOUSECODE a_mouseButton) override;
		bool getMouse(AGN::EMOUSECODE a_mouseButton) override;
		int getMouseX() override { return m_mouseX; }
		int getMouseY() override { return m_mouseY; }
		int getMouseXMove() override { return m_mouseX - m_oldMouseX; }
		int getMouseYMove() override { return m_mouseY - m_oldMouseY; }
		bool getCapslock() override;
		int getMouseScroll() override { return m_scrollAmount; };

		// callbacks
		void addMouseMotionHandler(class IInputMouseMotionHandler* a_motionHandler) override;
		void removeMouseMotionHandler(class IInputMouseMotionHandler* a_motionHandler) override;
		void addMouseClickHandler(class IInputMouseClickHandler* a_clickHandler) override;
		void removeMouseClickHandler(class IInputMouseClickHandler* a_clickHandler) override;


	private:
		//void onInputCharacter(SDL_Scancode a_key);
		//bool m_doTrackInput;
		
		bool m_held[(int)AGN::ESCANCODE::NUM_SCANCODES];
		bool m_heldChange[(int)AGN::ESCANCODE::NUM_SCANCODES];

		bool m_mouse[(int)AGN::EMOUSECODE::NUM_MOUSECODES];
		bool m_click[(int)AGN::EMOUSECODE::NUM_MOUSECODES];

		int m_oldMouseX;
		int m_oldMouseY;
		int m_mouseX;
		int m_mouseY;
		int m_scrollAmount;

		std::vector<class IInputMouseMotionHandler*> m_mouseMotionHandlers;
		std::vector<class IInputMouseClickHandler*> m_mouseClickHandlers;

		//std::string m_inputString;
		//int m_maxInputCharacters;
	};
}