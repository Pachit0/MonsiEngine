#pragma once

namespace Monsi {

	class TimeStep {
	public:
		TimeStep(float time = 0.0f) : m_Time(time) {}

		const float GetSeconds() const { return m_Time; }
		const float GetMs() const { return m_Time * 1000.0f; }

		operator float() const { return m_Time; }

	private:
		float m_Time;

	};

}