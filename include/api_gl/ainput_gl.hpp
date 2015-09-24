#pragma once

#include "iainput.hpp"

namespace AGN
{
	class AInputGL : public IAInput
	{
	public:
		AInputGL();

		static AGN::AGN_SCANCODE getAGNKey(SDL_Scancode a_sdlkey);
		static AGN::AGN_MOUSECODE getAGNMouse(uint8_t a_sdlMouse);

		void registerMouseClick(AGN::AGN_MOUSECODE a_mouseButton) override;
		void registerMouseHold(AGN::AGN_MOUSECODE a_mouseButton, bool a_held) override;
		void registerHold(AGN::AGN_SCANCODE a_key, bool a_held) override;
		void registerFrameCompletion() override;
		void registerMouseScroll(int a_scrollAmount) override { m_scrollAmount = a_scrollAmount; }
		void registerMouseMotion(int a_mouseX, int a_mouseY) override;

		bool getKeyDown(AGN::AGN_SCANCODE a_key) override;
		bool getKeyUp(AGN::AGN_SCANCODE a_key) override;
		bool getKey(AGN::AGN_SCANCODE a_key) override;
		bool getMouseDown(AGN::AGN_MOUSECODE a_mouseButton) override;
		bool getMouseUp(AGN::AGN_MOUSECODE a_mouseButton) override;
		bool getMouse(AGN::AGN_MOUSECODE a_mouseButton) override;
		int getMouseX() override { return m_mouseX; }
		int getMouseY() override { return m_mouseY; }
		int getMouseXMove() override { return m_mouseX - m_oldMouseX; }
		int getMouseYMove() override { return m_mouseY - m_oldMouseY; }
		bool getCapslock() override;
		int getMouseScroll() override { return m_scrollAmount; };

		// callbacks
		void addMouseMotionHandler(class IAInputMouseMotionHandler* a_motionHandler) override;
		void removeMouseMotionHandler(class IAInputMouseMotionHandler* a_motionHandler) override;
		void addMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler) override;
		void removeMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler) override;


	private:
		//void onInputCharacter(SDL_Scancode a_key);
		//bool m_doTrackInput;
		
		bool m_held[(int)AGN::AGN_SCANCODE::NUM_SCANCODES];
		bool m_heldChange[(int)AGN::AGN_SCANCODE::NUM_SCANCODES];

		bool m_mouse[(int)AGN::AGN_MOUSECODE::NUM_MOUSECODES];
		bool m_click[(int)AGN::AGN_MOUSECODE::NUM_MOUSECODES];

		int m_oldMouseX;
		int m_oldMouseY;
		int m_mouseX;
		int m_mouseY;
		int m_scrollAmount;

		std::vector<class IAInputMouseMotionHandler*> m_mouseMotionHandlers;
		std::vector<class IAInputMouseClickHandler*> m_mouseClickHandlers;

		//std::string m_inputString;
		//int m_maxInputCharacters;
	};
}