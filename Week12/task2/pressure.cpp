#include "pressure.hpp"

// =============================================================================

/*const float Pressure::compute_volume() const noexcept // площадь шарика 
{
	float volume = 0.0f;

	for (auto i = 0U; i < m_particles.size(); ++i)
	{
		volume += (
			m_particles[(i + 1) % m_particles.size()]->position() ^ m_particles[i]->position()) * 0.5f;
	}

	return volume;
}

// =============================================================================

void Pressure::update() const noexcept
{
	const auto pressure_difference = initial_pressure * m_initial_volume / compute_volume() - atmosphere_pressure;

	for (auto i = 0U; i < m_particles.size(); ++i)
	{
		auto delta = m_particles[(i + 1) % m_particles.size()]->position() - m_particles[i]->position();
		auto force = sf::Vector2f(delta.y, -delta.x) * pressure_difference;

		m_particles[(i + 1) % m_particles.size()]->move(force);
		m_particles[i]->move(force);
	}
}

*/
// =============================================================================