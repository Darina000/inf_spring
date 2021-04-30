#include "particle.hpp"

// =============================================================================

void Particle::move(const sf::Vector2f delta) noexcept
{
	m_position += delta;
}

// =============================================================================

void Particle::move(const float dt) noexcept
{
	sf::Vector2f current_position = m_position;
	m_position += ((m_position - m_previous_position) * dt +
		m_acceleration * dt * dt);
	m_previous_position = current_position;
}

// =============================================================================
