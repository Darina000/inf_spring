#pragma once
#include <cmath>
#include <memory>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "functions.hpp"
#include "link.hpp"
#include "particle.hpp"

class System {
public:

	using particle_t = std::shared_ptr < Particle >;

public:

	explicit System(
		const sf::Vector2f min_point, const sf::Vector2f max_point,
		std::array < std::array<particle_t, 10>, 10 >& particles) noexcept :
		m_min_point(min_point), m_max_point(max_point),
		m_particles(particles)
	{
		initialize();
	}

	~System() noexcept = default;

private:

	void initialize();

public:

	particle_t particle(int i, int j) const 
	{
		return m_particles[i][j];
	}

	const auto& particles() const noexcept
	{
		return m_particles;
	}

public:

	void push(const sf::Vector2f force) const noexcept;

	void update() const noexcept;

private:

	sf::Vector2f m_min_point;
	sf::Vector2f m_max_point;
	std::array < std::array<particle_t, 10>, 10 > m_particles;
	std::vector < Link > m_links;
};