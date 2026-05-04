#pragma once

#include <Monsi.h>
#include "ChessCore.h"

struct Rect {
	float x;
	float y;
	float width;
	float height;
};

class Chess : public Monsi::Layer {
public:

	Chess();
	virtual ~Chess() = default;

	void OnLayerAttach() override;
	void OnLayerUpdate(Monsi::TimeStep timestep) override;
	void OnLayerDetach() override;
	void OnLayerEvent(Monsi::Event& event) override;
	void OnImGuiDraw() override;

	void DrawBoard();
	void DrawPiece();
	void HandleInput();
	Monsi::Reference<Monsi::SubTexture2D> GetTexture(Figure* piece);

private:
	Monsi::Reference<Monsi::FrameBuffer> m_FrameBuffer;
	glm::vec2 m_ViewportPos;
	glm::vec2 m_ViewportSize;
	bool m_ViewportFocused;
	bool m_ViewportHovered;

private:
	std::unordered_map<char, glm::vec4> s_TextureMap;
	
	Monsi::Reference<Monsi::Texture2D> m_Pieces;

	Monsi::Reference<Monsi::SubTexture2D> m_WhitePawn;
	Monsi::Reference<Monsi::SubTexture2D> m_WhiteKing;
	Monsi::Reference<Monsi::SubTexture2D> m_WhiteQueen;
	Monsi::Reference<Monsi::SubTexture2D> m_WhiteRook;
	Monsi::Reference<Monsi::SubTexture2D> m_WhiteKnight;
	Monsi::Reference<Monsi::SubTexture2D> m_WhiteBishop;

	Monsi::Reference<Monsi::SubTexture2D> m_BlackPawn;
	Monsi::Reference<Monsi::SubTexture2D> m_BlackQueen;
	Monsi::Reference<Monsi::SubTexture2D> m_BlackKing;
	Monsi::Reference<Monsi::SubTexture2D> m_BlackRook;
	Monsi::Reference<Monsi::SubTexture2D> m_BlackKnight;
	Monsi::Reference<Monsi::SubTexture2D> m_BlackBishop;

	FigureColor m_CurrentTurn;

	Monsi::OrthographicControl m_CameraControl;

	Pos m_Selected;
	bool m_HasSelection = false;

	bool m_IsDragging = false;
	glm::vec2 m_DragPos;

	Board m_Board;

};