#pragma once

#include <SFML/Graphics.hpp>

class View
{
public:

	explicit View(sf::Vector2f center, float width, float height) noexcept :
		m_center(center), m_width(width), m_height(height)
	{}

	~View() noexcept = default;

public:

	void move(sf::Vector2f delta)
	{
		m_center += delta;
	}

	void scale(float factor)
	{
		m_width  *= factor; 
		m_height *= factor;
	}

	const auto center() const noexcept 
	{
		return m_center; 
	}

	const auto width () const noexcept 
	{ 
		return m_width;  
	}

	const auto height() const noexcept 
	{ 
		return m_height; 
	}

private:

	sf::Vector2f m_center;

	float m_width;
	float m_height;
};