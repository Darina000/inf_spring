#pragma once

#include <complex>
#include <iostream>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

class Calculator
{
public:

    using field_t = std::vector < std::vector < std::size_t > > ;

public:

    explicit Calculator(std::size_t width, std::size_t height, std::size_t max_iterations) :
		m_width(width), m_height(height), m_max_iterations(max_iterations)
	{
		initialize();
	}

	~Calculator() noexcept = default;

private:

	void initialize();

public:

    const auto & field() const noexcept 
	{ 
		return m_field; 
	}

	const auto width () const noexcept 
	{ 
		return m_width;  
	}

	const auto height() const noexcept 
	{ 
		return m_height; 
	}

	const auto max_iterations() const noexcept 
	{ 
		return m_max_iterations; 
	}

public:

	void calculate(sf::Vector2f center, float width, float height);

private:

	std::size_t m_width;
	std::size_t m_height;

    std::size_t m_max_iterations;

	field_t m_field;
};