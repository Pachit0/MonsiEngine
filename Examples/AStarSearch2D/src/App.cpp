#include "AStarSearch2D.h"
#include <cmath>
#include <algorithm>

AStarSearch2D::AStarSearch2D()
	: Layer("AStarSearch2D"),
	m_CameraControl(1280.0f / 720.0f, 16.0f),
	m_CurrentMap(FIRST),
	m_Col(11),
	m_Row(10),
	m_Start({ 9,1 }),
	m_End({ 0,8 }),
	m_Timer(0.0f),
	m_StepDelay(0.1f)
{
	m_Map.resize(MapVariation::COUNT);
	for (int var = 0; var < MapVariation::COUNT; var++) {
		m_Map[var].resize(m_Col);
		for (int i = 0; i < m_Col; i++) {
			m_Map[var][i].resize(m_Row);
		}
	}
}

void AStarSearch2D::OnLayerAttach()
{
	//11x10
	m_Map[FIRST] = {
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,PATH,WALL},
	{WALL,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,WALL},
	{WALL,PATH,WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL},
	{WALL,PATH,WALL,PATH,PATH,PATH,PATH,WALL,PATH,WALL},
	{WALL,PATH,WALL,WALL,WALL,WALL,PATH,WALL,PATH,WALL},
	{WALL,PATH,PATH,PATH,PATH,WALL,PATH,PATH,PATH,WALL},
	{WALL,WALL,WALL,WALL,PATH,WALL,PATH,WALL,PATH,WALL},
	{WALL,PATH,PATH,PATH,PATH,PATH,PATH,WALL,PATH,WALL},
	{WALL,PATH,WALL,WALL,WALL,WALL,PATH,WALL,PATH,WALL},
	{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
	};
	//15x12
	m_Map[SECOND] = {
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,WALL,PATH,PATH,PATH,WALL,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,PATH,WALL,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
	};

	//15x15
	m_Map[THIRD] = {
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,PATH,WALL,PATH,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
	};

	//11x20
	m_Map[FOURTH] = {
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,PATH,PATH,WALL,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
	};

	//18x18
	m_Map[FIFTH] = {
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL,PATH,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL},
		{WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,WALL,PATH,WALL,WALL,PATH,WALL},
		{WALL,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,PATH,WALL}
	};
	//5x5
	m_Map[SIX] = {
			{PATH,PATH,PATH,WALL,PATH},
			{PATH,WALL,PATH,PATH,PATH},
			{PATH,WALL,PATH,WALL,PATH},
			{PATH,PATH,PATH,WALL,PATH},
			{PATH,WALL,PATH,PATH,PATH},
	};

	InitAStar();
}

void AStarSearch2D::InitAStar()
{
	m_ClosedList.clear();
	m_Nodes.clear();

	m_ClosedList.resize(m_Col, std::vector<bool>(m_Row, false));
	m_Nodes.resize(m_Col, std::vector<Node>(m_Row));

	m_ClosedList = std::vector(m_Col, std::vector<bool>(m_Row, false));
	m_Nodes = std::vector(m_Col, std::vector<Node>(m_Row));

	for (int i = 0; i < m_Col; i++) {
		for (int j = 0; j < m_Row; j++) {
			m_Nodes[i][j] = { i,j,FLT_MAX,FLT_MAX,FLT_MAX,-1,-1 };
		}
	}

	int sx = m_Start.first;
	int sy = m_Start.second;

	m_Nodes[sx][sy] = { sx,sy,0,0,0,sx,sy };

	m_OpenList.clear();
	m_OpenList.insert({ 0.0f,{sx,sy} });

	m_Path.clear();
	m_Finished = false;
}

void AStarSearch2D::StepAStar()
{
	if (m_OpenList.empty() || m_Finished)
		return;

	auto current = *m_OpenList.begin();
	m_OpenList.erase(m_OpenList.begin());

	int x = current.second.first;
	int y = current.second.second;

	m_ClosedList[x][y] = true;

	int dx[8] = { -1,1,0,0,-1,-1,1,1 };
	int dy[8] = { 0,0,1,-1,1,-1,1,-1 };
	float cost[8] = { 1,1,1,1,1.414f,1.414f,1.414f,1.414f };

	auto heuristic = [&](int x, int y) {
		return std::sqrt((x - m_End.first) * (x - m_End.first) +
			(y - m_End.second) * (y - m_End.second));
		};

	for (int d = 0; d < 8; d++) {
		int nx = x + dx[d];
		int ny = y + dy[d];

		if (nx < 0 || nx >= m_Col || ny < 0 || ny >= m_Row) {
			continue;
		}

		if (nx == m_End.first && ny == m_End.second) {
			m_Nodes[nx][ny].parentX = x;
			m_Nodes[nx][ny].parentY = y;

			int cx = nx, cy = ny;
			while (!(m_Nodes[cx][cy].parentX == cx &&
				m_Nodes[cx][cy].parentY == cy)) {
				m_Path.push_back({ cx, cy });
				int px = m_Nodes[cx][cy].parentX;
				int py = m_Nodes[cx][cy].parentY;
				cx = px;
				cy = py;
			}

			m_Path.push_back({ cx, cy });
			std::reverse(m_Path.begin(), m_Path.end());

			m_Finished = true;
			return;
		}

		if (!m_ClosedList[nx][ny] && m_Map[m_CurrentMap][nx][ny] == PATH) {

			float gNew = m_Nodes[x][y].g + cost[d];
			float hNew = heuristic(nx, ny);
			float fNew = gNew + hNew;

			if (m_Nodes[nx][ny].f == FLT_MAX || m_Nodes[nx][ny].f > fNew) {
				m_OpenList.insert({ fNew, {nx, ny} });
				m_Nodes[nx][ny] = { nx, ny, fNew, gNew, hNew, x, y };
			}
		}
	}
}

void AStarSearch2D::OnLayerUpdate(Monsi::TimeStep timestep)
{
	m_CameraControl.OnLayerUpdate(timestep);


	m_Timer += timestep;
	if (m_Timer >= m_StepDelay) {
		StepAStar();
		m_Timer = 0.0f;
	}

	Monsi::RenderCommand::SetClearColor({ 0.5f,0.5f,0.5f,1.0f });
	Monsi::RenderCommand::Clear();

	Monsi::Renderer2D::BeginScene2D(m_CameraControl.GetCamera());

	for (int i = 0; i < m_Col; i++) {
		for (int j = 0; j < m_Row; j++) {

			bool isPath = std::find(m_Path.begin(), m_Path.end(), std::make_pair(i, j)) != m_Path.end();

			bool isOpen = false;
			for (auto& o : m_OpenList) {
				if (o.second.first == i && o.second.second == j) {
					isOpen = true;
				}
			}

			if (isPath) {
				Monsi::Renderer2D::drawQuad({ i - 4,j - 4 }, { 0.9f,0.9f }, { 1.0f,0.0f,0.0f,1.0f });
			}
			else if (m_ClosedList[i][j]) {
				Monsi::Renderer2D::drawQuad({ i - 4,j - 4 }, { 0.9f,0.9f }, { 0.0f,0.0f,1.0f,1.0f });
			}
			else if (isOpen) {
				Monsi::Renderer2D::drawQuad({ i - 4,j - 4 }, { 0.9f,0.9f }, { 1.0f,1.0f,0.0f,1.0f });
			}
			else {
				if (m_Map[m_CurrentMap][i][j] == PATH) {
					Monsi::Renderer2D::drawQuad({ m_End.first - 4,m_End.second - 4 }, { 0.9f,0.9f }, { 0.0f,1.0f,0.0f,1.0f });
					Monsi::Renderer2D::drawQuad({ i - 4,j - 4 }, { 0.9f,0.9f }, { 0.5f,0,0.5f,1 });
				}
				else {
					Monsi::Renderer2D::drawQuad({ i - 4,j - 4 }, { 0.9f,0.9f }, { 0.0f,0.0f,0.0f,1.0f });
				}
			}
		}
	}

	Monsi::Renderer2D::EndScene2D();
}

void AStarSearch2D::OnLayerDetach() {}

void AStarSearch2D::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}

void AStarSearch2D::OnImGuiDraw()
{
	const char* mapNames[] = {
		"First",
		"Second",
		"Third",
		"Fourth",
		"Fifth",
		"Six"
	};

	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
	ImGui::Begin("Maze selector");

	int current = static_cast<int>(m_CurrentMap);

	if (ImGui::Combo("Map", &current, mapNames, IM_ARRAYSIZE(mapNames)))
	{
		m_CurrentMap = static_cast<MapVariation>(current);

		switch (m_CurrentMap) {
		case FIRST: {
			m_Col = 11;
			m_Row = 10;
			m_Start = { 9,1 };
			m_End = { 0,8 };
			break;
		}

		case SECOND: {
			m_Col = 15;
			m_Row = 12;
			m_Start = { 1,1 };
			m_End = { 14,10 };
			break;
		}

		case THIRD: {
			m_Col = 15;
			m_Row = 15;
			m_Start = { 1,13 };
			m_End = { 9,1 };
			break;
		}

		case FOURTH: {
			m_Col = 11;
			m_Row = 20;
			m_Start = { 9,1 };
			m_End = { 1,1 };
			break;
		}
		case FIFTH: {
			m_Col = 18;
			m_Row = 18;
			m_Start = { 7,7 };
			m_End = { 11,7 };
			break;
		}

		case SIX: {
			m_Col = 5;
			m_Row = 5;
			m_Start = { 0,0 };
			m_End = { 0, 4 };
			break;
		}

		}

		InitAStar();
	}

	ImGui::End();
}
