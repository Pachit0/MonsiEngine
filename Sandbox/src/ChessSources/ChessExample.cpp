#include "ChessModules/ChessExample.h"

static const char* s_MapTiles =
"WBWBWBWB"
"BWBWBWBW"
"WBWBWBWB"
"BWBWBWBW"
"WBWBWBWB"
"BWBWBWBW"
"WBWBWBWB"
"BWBWBWBW";

Chess::Chess() : Layer("Chess Game"), m_CameraControl(1280.0f / 720.0f, 7.0f)
{

}

void Chess::OnLayerAttach()
{
	m_Pieces = Monsi::Texture2D::Create(TEXTURE_PATH "Chess_Pieces_Sprite.png");
	m_BalckPawn = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 5,0 }, { 90.0f,90.0f });
	m_WhitePawn = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 5,1 }, { 90.0f,90.0f });

	s_TextureMap['W'] = { 1.0f,1.0f,1.0f,1.0f };
	s_TextureMap['B'] = { 0.5f,0.5f,0.5f,1.0f };
}

void Chess::OnLayerUpdate(Monsi::TimeStep timestep)
{
	m_CameraControl.OnLayerUpdate(timestep);

	Monsi::Renderer2D::ResetBatchStatistics();
	{
		Monsi::RenderCommand::SetClearColor({ 0.2f, 0.5f, 0.3f, 0.5f });
		Monsi::RenderCommand::Clear();
	}

	{

		Monsi::Renderer2D::BeginScene2D(m_CameraControl.GetCamera());


		for (uint32_t y = 0; y < 8; y++) {
			for (uint32_t x = 0; x < 8; x++) {
				char tileType = s_MapTiles[x + y * 8];
				if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
					Monsi::Renderer2D::drawQuad({ x - 5.0f, y - 2.5f }, { 1.0f, 1.0f }, s_TextureMap[tileType]);
				}
				else {
					Monsi::Renderer2D::drawQuad({ x - 5.0f, y - 2.5f }, { 1.0f, 1.0f }, {1.0f,0.0f,0.0f,1.0f});
				}
			}
		}

		for (int i = 0; i < 8; i++) {
			Monsi::Renderer2D::drawQuad({ i - 5, 3.5f, 0.1f }, { 1.0f, 1.0f }, m_BalckPawn);
			Monsi::Renderer2D::drawQuad({ i - 5, -1.5f, 0.1f }, { 1.0f, 1.0f }, m_WhitePawn);
		}

		Monsi::Renderer2D::EndScene2D();
	}
}

void Chess::OnLayerDetach()
{

}

void Chess::OnLayerEvent(Monsi::Event& event)
{

}

void Chess::OnImGuiDraw()
{

}
