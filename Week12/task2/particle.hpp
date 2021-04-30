#pragma once

#include <SFML/Graphics.hpp>

class Particle // частица 
{
public:

	Particle() noexcept = default;

	explicit Particle(
		const sf::Vector2f position, const sf::Vector2f previous_position,
		const sf::Vector2f acceleration, const float radius) noexcept :
		m_position(position), m_previous_position(previous_position),
		m_acceleration(acceleration), m_radius(radius)
	{}

	~Particle() noexcept = default;

public:

	const auto position() const noexcept 
	{ 
		return m_position; 
	}

	const auto radius() const noexcept
	{
		return m_radius;
	}

public:

	void set_x(const float x) noexcept // 
	{
		m_position.x = x;
	}

	void set_y(const float y) noexcept
	{
		m_position.y = y;
	}

public:

	void move(const sf::Vector2f delta) noexcept; // сдиг положения камеры на дельта 

	void move(const float dt) noexcept;// сдвиг за время дт 

private:

	sf::Vector2f m_position; // текущая позиция 
	sf::Vector2f m_previous_position; // предыдущая позиция 
	sf::Vector2f m_acceleration; // ускорение 

	float m_radius; // радиус частицы 
};
