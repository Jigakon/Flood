#ifndef TIMESTEP__H
#define TIMESTEP__H

namespace Flood
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time) {}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time; // in seconds
	};
}

#endif // !TIMESTEP