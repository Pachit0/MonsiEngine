#pragma once

#include <Monsi.h>
#include <set>

class AStarSearch2D : public Monsi::Layer {
public:
	AStarSearch2D();
	virtual ~AStarSearch2D() = default;

	void OnLayerAttach() override;
	void OnLayerUpdate(Monsi::TimeStep timestep) override;
	void OnLayerDetach() override;
	void OnLayerEvent(Monsi::Event& event) override;
	void OnImGuiDraw() override;

	int GetRow() const { return m_Row; }
	int GetCol() const { return m_Col; }
private:
	enum BlockType {
		PATH,
		WALL
	};

	enum MapVariation {
		FIRST,
		SECOND,
		THIRD,
		FOURTH,
		FIFTH,
		SIX,
		COUNT
	};

	struct Node {
		int x, y;
		float f, g, h;
		int parentX, parentY;
	};

	Monsi::OrthographicControl m_CameraControl;

	int m_Row, m_Col;
	float m_Timer;
	float m_StepDelay;
	MapVariation m_CurrentMap;

	std::vector<std::vector<Node>> m_Nodes;
	std::vector<std::vector<bool>> m_ClosedList;
	std::set<std::pair<float, std::pair<int, int>>> m_OpenList;
	std::vector<std::vector<std::vector<BlockType>>> m_Map;
	std::vector<std::pair<int, int>> m_Path;
	std::pair<int, int> m_Start;
	std::pair<int, int> m_End;

	bool m_Finished = false;

private:
	void InitAStar();
	void StepAStar();
};