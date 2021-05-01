#include "system.hpp"

void System::initialize()
{

	for (auto i = 0U; i < m_particles.size(); i++)
	{
		for (auto j = 0U; j < m_particles.size(); j++)
		{
			if (i < m_particles.size() - 1)
				m_links.push_back(Link(particle(i, j), particle(i + 1, j), 1.0f));
			if (j < m_particles.size() - 1)
			{
				m_links.push_back(Link(particle(i, j), particle(i, j + 1), 1.0f));
				if (i < m_particles.size() - 1)
					m_links.push_back(Link(particle(i, j), particle(i + 1, j + 1), 1.0f));
			}
		}
	}
}

void System::push(const sf::Vector2f force) const noexcept
{

	for (auto i = 0U; i < m_particles.size(); ++i) {
		for (auto j = 0U; j < m_particles.size(); ++j) {
			m_particles[i][j]->move(force);
		}
	}
}

void System::update() const noexcept 
{
	for (auto i = 0U; i < m_particles.size(); ++i) 
	{
		for (int j = 0; j < 10; ++j) 
		{
			m_particles[i][j]->move(0.25f);

			if (m_particles[i][j]->position().y + m_particles[i][j]->radius() > m_max_point.y)
			{
				m_particles[i][j]->set_y(m_max_point.y - m_particles[i][j]->radius());
			}

			if (m_particles[i][j]->position().y - m_particles[i][j]->radius() < m_min_point.y)
			{
				m_particles[i][j]->set_y(m_min_point.y + m_particles[i][j]->radius());
			}

			if (m_particles[i][j]->position().x + m_particles[i][j]->radius() > m_max_point.x)
			{
				m_particles[i][j]->set_x(m_max_point.x - m_particles[i][j]->radius());
			}

			if (m_particles[i][j]->position().x - m_particles[i][j]->radius() < m_min_point.x)
			{
				m_particles[i][j]->set_x(m_min_point.x + m_particles[i][j]->radius());
			}
		}
	}

	for (auto i = 0U; i < m_links.size(); ++i)
	{
		m_links[i].update();
	}
}
