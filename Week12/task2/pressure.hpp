#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "particle.hpp"

/*class Pressure
{
public:

	using particle_t = std::shared_ptr < Particle >;

public:

	Pressure() noexcept = default;

	explicit Pressure(const std::vector < particle_t > & particles) noexcept :
		m_particles(particles), m_initial_volume(compute_volume())
	{}

	~Pressure() noexcept = default;

private:

	const float compute_volume() const noexcept;

public:

	void update() const noexcept;

private:

	const float initial_pressure    = 0.16f; // внутреннее
	const float atmosphere_pressure = 0.04f;

private:

	std::vector < particle_t > m_particles;

	float m_initial_volume;
};
*/