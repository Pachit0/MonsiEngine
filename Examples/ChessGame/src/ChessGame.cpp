#include "ChessGame.h"
#include <imgui.h>
#include <imgui_internal.h>

static bool CheckCollisionPointRec(const glm::vec2& point, const Rect& rec)
{
	bool collision = false;

	if ((point.x >= rec.x) && (point.x <= (rec.x + rec.width)) &&
		(point.y >= rec.y) && (point.y <= (rec.y + rec.height))) collision = true;

	return collision;
}

static glm::vec4 PhysicalToWorldCoords(const glm::vec2& mousePos, const Monsi::OrthographicControl& camera, float width, float height) {
	float x = (2.0f * mousePos.x) / width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / height;

	glm::vec4 mouseClip = { x, y, 0.0f, 1.0f };

	glm::mat4 invVP = glm::inverse(camera.GetCamera().GetViewProjectionMatrix());

	glm::vec4 worldPos = invVP * mouseClip;
	worldPos /= worldPos.w;

	return worldPos;
}

static Pos WorldToBoard(const glm::vec2& world)
{
	int x = (int)floor(world.x + 5.0f);
	int y = (int)floor(world.y + 2.5f);

	return { x, y };
}

static const char* s_MapTiles =
"BWBWBWBW"
"WBWBWBWB"
"BWBWBWBW"
"WBWBWBWB"
"BWBWBWBW"
"WBWBWBWB"
"BWBWBWBW"
"WBWBWBWB";

Chess::Chess() : Layer("Chess Game"),
	m_CameraControl(1280.0f / 720.0f, 4.5f, {-0.5f,1.5f}, false, false, false),
	m_ViewportSize{ 0.0f,0.0f }, m_ViewportFocused(false), m_CurrentTurn(WHITE)
{

}

void Chess::OnLayerAttach()
{
	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);

	m_Pieces = Monsi::Texture2D::Create(TEXTURE_PATH "Chess_Pieces_Sprite.png");

	m_BlackPawn = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 5,0 }, { 90.0f,90.0f });
	m_WhitePawn = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 5,1 }, { 90.0f,90.0f });

	m_WhiteQueen = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 1,1 }, { 90.0f,90.0f });
	m_BlackQueen = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 1,0 }, { 90.0f,90.0f });

	m_WhiteRook = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 4,1 }, { 90.0f,90.0f });
	m_BlackRook = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 4,0 }, { 90.0f,90.0f });

	m_WhiteBishop = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 2,1 }, { 90.0f,90.0f });
	m_BlackBishop = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 2,0 }, { 90.0f,90.0f });

	m_WhiteKing = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 0,1 }, { 90.0f,90.0f });
	m_BlackKing = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 0,0 }, { 90.0f,90.0f });

	m_WhiteKnight = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 3,1 }, { 90.0f,90.0f });
	m_BlackKnight = Monsi::SubTexture2D::CreateSubTexture(m_Pieces, { 3,0 }, { 90.0f,90.0f });

	s_TextureMap['W'] = { 1.0f,1.0f,1.0f,1.0f };
	s_TextureMap['B'] = { 0.5f,0.5f,0.5f,1.0f };
}

void Chess::OnLayerUpdate(Monsi::TimeStep timestep)
{
	m_CameraControl.OnLayerUpdate(timestep);
	
	Monsi::Renderer2D::ResetBatchStatistics();

	m_FrameBuffer->Bind();
	Monsi::RenderCommand::SetClearColor({ 0.2f, 0.5f, 0.3f, 0.5f });
	Monsi::RenderCommand::Clear();

	Monsi::Renderer2D::BeginScene2D(m_CameraControl.GetCamera());

	DrawBoard();

	DrawPiece();

	HandleInput();

	Monsi::Renderer2D::EndScene2D();
	m_FrameBuffer->Unbind();
}

void Chess::DrawBoard() {
	for (uint32_t y = 0; y < 8; y++) {
		for (uint32_t x = 0; x < 8; x++) {
			char tileType = s_MapTiles[x + y * 8];
			if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
				Monsi::Renderer2D::drawQuad({ x - 4.5f, y - 2.0f }, { 1.0f, 1.0f }, s_TextureMap[tileType]);
			}
			else {
				Monsi::Renderer2D::drawQuad({ x - 4.5f, y - 2.0f }, { 1.0f, 1.0f }, { 1.0f,0.0f,0.0f,1.0f });
			}
		}
	}
}

void Chess::DrawPiece() {
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			Figure* piece = m_Board.get(x, y);
			if (!piece) continue;

			auto texture = GetTexture(piece);
			if (!texture) continue;

			float worldX = x - 4.5f;
			float worldY = y - 2.0f;

			glm::vec4 tint = { 1.0f,1.0f,1.0f,1.0f };

			if (m_IsDragging && m_Selected.x == x && m_Selected.y == y)
				tint.a = 0.3f;

			Monsi::Renderer2D::drawQuad({ worldX, worldY, 0.1f }, { 1.0f, 1.0f }, texture, tint);
		}
	}
}

void Chess::HandleInput()
{
	if (!m_ViewportFocused && !m_ViewportHovered)
		return;

	ImVec2 mousePos = ImGui::GetMousePos();

	float relativeX = mousePos.x - m_ViewportPos.x;
	float relativeY = mousePos.y - m_ViewportPos.y;

	glm::vec2 worldPos = PhysicalToWorldCoords({ relativeX, relativeY }, m_CameraControl, m_ViewportSize.x, m_ViewportSize.y);

	Pos tile = WorldToBoard(worldPos);

	bool isInsideBoard = (tile.x >= 0 && tile.x < 8 && tile.y >= 0 && tile.y < 8);

	if (Monsi::Input::MouseButtonPressed(MONSI_MOUSE_BUTTON_LEFT))
	{
		if (!m_IsDragging && isInsideBoard)
		{
			Figure* piece = m_Board.get(tile.x, tile.y);

			if (piece && piece->getColor() == m_CurrentTurn)
			{
				m_Selected = tile;
				m_IsDragging = true;
				m_DragPos = worldPos;
			}
		}
	}

	if (m_IsDragging)
		m_DragPos = worldPos;

	if (Monsi::Input::MouseButtonReleased(MONSI_MOUSE_BUTTON_LEFT))
	{
		if (m_IsDragging)
		{
			Pos target = WorldToBoard(worldPos);

			if (isInsideBoard)
			{
				if (m_Board.move(m_Selected, target))
				{
					Figure* movedPiece = m_Board.get(target.x, target.y);

					Pawn* pawn = dynamic_cast<Pawn*>(movedPiece);

					if (pawn)
					{
						pawn->setFirstMove();
					}

					m_CurrentTurn = (m_CurrentTurn == WHITE) ? BLACK : WHITE;
				}
			}

			m_IsDragging = false;
		}
	}

	if (m_IsDragging)
	{
		Figure* dragged = m_Board.get(m_Selected.x, m_Selected.y);

		if (dragged)
		{
			auto texture = GetTexture(dragged);

			Monsi::Renderer2D::drawQuad({ m_DragPos.x, m_DragPos.y, 0.2f }, { 1.0f, 1.0f }, texture);
		}
	}

	Monsi::Renderer2D::drawQuad({ worldPos.x, worldPos.y, 0.5f }, { 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f });
}

Monsi::Reference<Monsi::SubTexture2D> Chess::GetTexture(Figure* piece)
{
	if (!piece) return nullptr;

	switch (piece->getType())
	{
	case PAWN:
		return (piece->getColor() == WHITE) ? m_WhitePawn : m_BlackPawn;

	case QUEEN:
		return (piece->getColor() == WHITE) ? m_WhiteQueen : m_BlackQueen;

	case ROOK:
		return (piece->getColor() == WHITE) ? m_WhiteRook : m_BlackRook;

	case BISHOP:
		return (piece->getColor() == WHITE) ? m_WhiteBishop : m_BlackBishop;

	case KNIGHT:
		return (piece->getColor() == WHITE) ? m_WhiteKnight : m_BlackKnight;

	case KING:
		return (piece->getColor() == WHITE) ? m_WhiteKing : m_BlackKing;
	}

	return nullptr;
}

void Chess::OnLayerDetach()
{

}

void Chess::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}

void Chess::OnImGuiDraw() {

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit")) { Monsi::Application::Get().CloseApp(); }
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");

	ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_None);

	static bool first_time = true;
	if (first_time)
	{
		first_time = false;

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		ImGuiID dock_id_main = dockspace_id;
		ImGuiID dock_id_left;

		ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);

		ImGuiID dock_id_left_top;
		ImGuiID dock_id_left_bottom;

		ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.5f, &dock_id_left_top, &dock_id_left_bottom);

		ImGui::DockBuilderDockWindow("Chess", dock_id_main);
		ImGui::DockBuilderDockWindow("Properties", dock_id_left_top);
		ImGui::DockBuilderDockWindow("Current Turn", dock_id_left_bottom);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::Begin("Properties");

	auto batchStats = Monsi::Renderer2D::GetBatchStatistics();

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", batchStats.DrawCalls);
	ImGui::Text("Quad Total Count: %d", batchStats.QuadCount);
	ImGui::Text("Quad Vertices: %d", batchStats.GetVertexCount());
	ImGui::Text("Quad Indices: %d", batchStats.GetIndexCount());

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f,0.0f });
	ImGui::Begin("Chess");

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	m_ViewportPos = { canvas_pos.x, canvas_pos.y };

	Monsi::Application::Get().GetImGuiLayer()->SetImGuiEventState(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 VpSize = ImGui::GetContentRegionAvail();
	if (VpSize.x > 0.0f && VpSize.y > 0.0f && (m_ViewportSize.x != VpSize.x || m_ViewportSize.y != VpSize.y))
	{
		m_ViewportSize = { VpSize.x, VpSize.y };
		m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		m_CameraControl.OnWindowResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	uint32_t textureID = m_FrameBuffer->GetColorAttachmentID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
	ImGui::PopStyleVar();


	ImGui::Begin("Current Turn");
	ImGui::SetWindowFontScale(2.0f);

	switch (m_CurrentTurn)
	{
	case WHITE: {
		ImGui::Text("White Moving");
		break;
	}
	case BLACK: {
		ImGui::Text("Black Moving");
		break;
	}
	default:
		break;
	}


	ImGui::SetWindowFontScale(1.0f);
	ImGui::End();
}