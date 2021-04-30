#include <cmath>
#include <array>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "system.hpp"

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(800U, 600U), "PHYSICS");

	sf::Vector2f min_point(10.0f, 10.0f);
	sf::Vector2f max_point(750.0f, 550.0f);

	sf::Vector2f center = (min_point + max_point) * 0.5f;

	using particle_t = System::particle_t;

	const auto R = length(max_point - min_point) * 0.02f;
	const auto count = 10U;
	const auto r = 3.0f;

	std::array < std::array<particle_t, 10>, 10 > particles;

	for (int i = 0; i < 10; ++i) 
	{
		for (int j = 0; j < 10; ++j)
		{
			auto position = sf::Vector2f(i, j) * R + center;

			particles[i][j] = std::make_shared < Particle >(position, position, sf::Vector2f(0.0f, 10.0f), r);
		}
	}

	System system(min_point, max_point, particles);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		system.update();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			system.push(sf::Vector2f(0.0f, -2.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			system.push(sf::Vector2f(0.0f, 2.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			system.push(sf::Vector2f(-2.0f, 0.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			system.push(sf::Vector2f(2.0f, 0.0f));
		}

		window.clear();

		for (auto i = 0U; i < system.particles().size(); ++i)
		{
			for (int j = 0; j < 10; ++j) 
			{
				sf::CircleShape circle(2.0f * r);

				circle.setPosition(system.particle(i, j)->position() + sf::Vector2f(r, r));

				circle.setFillColor(sf::Color::Red);

				window.draw(circle);
			}
		}

		window.display();
	}

	return EXIT_SUCCESS;
}
