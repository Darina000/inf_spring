#include "link.hpp"

void Link::update() const noexcept
{
	sf::Vector2f delta = m_particle_2->position() - m_particle_1->position();

	m_particle_1->move(norm(delta) * (length(delta) - m_distance) * 0.5f * m_stiffness);
	m_particle_2->move(norm(delta) * (length(delta) - m_distance) * 0.5f * m_stiffness * -1.0f);
}
