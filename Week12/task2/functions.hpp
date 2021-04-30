#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>

template < typename T >
const auto length(const sf::Vector2 < T > vector) noexcept
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

template < typename T >
const auto norm(const sf::Vector2 < T > vector) noexcept
{
	const auto l = length(vector);

	if (l == T(0))
	{
		return sf::Vector2 < T >(T(0), T(0));
	}
	else
	{
		return sf::Vector2 < T >(vector.x / l, vector.y / l);
	}
}


template < typename T >
const auto operator^ (const sf::Vector2 < T > lhs, const sf::Vector2 < T > rhs) noexcept {
	return lhs.x * rhs.y - rhs.x * lhs.y;
}
