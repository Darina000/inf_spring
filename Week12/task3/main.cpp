#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <thread>
#include<vector>
class Particle
{
public:
    Particle(const sf::Vector2f position, const float radius, const sf::Vector2f speed) :
        m_position(position), m_radius(radius), m_speed(speed)
    {}
public:
    void draw(sf::RenderWindow &m_window) {
        sf::CircleShape circle(2.0f * m_radius);
        circle.setPosition(m_position);
        circle.setFillColor(sf::Color::Red);
        m_window.draw(circle);
    }
    void move(const unsigned int W, const unsigned int H, std::mt19937 gen) {
        std::uniform_real_distribution<float> disx(- m_speed.x, m_speed.x);
        std::uniform_real_distribution<float> disy(- m_speed.y, m_speed.y);
        m_position.x += disx(gen);
        m_position.y += disy(gen);
        if (m_position.x > W) m_position.x = 0.f; if (m_position.x < 0) m_position.x = static_cast<float>(W);
        if (m_position.y > H) m_position.y = 0.f; if (m_position.y < 0) m_position.y = static_cast<float>(H);
    }
    sf::Vector2f getPosition() {
        return m_position;
    }
    sf::Vector2f getSpeed() {
        return m_speed;
    }
private:
    sf::Vector2f m_position;
    float m_radius;
    sf::Vector2f m_speed;

};

class Visualizer
{
public:
    Visualizer(int count, float width, float height): m_count(count), m_width(width), m_height(height)
    {
    }
    bool isInside(Particle p, float xPos, float yPos, float step) {
        return ((p.getPosition().x < xPos) && (p.getPosition().x > (xPos - step)) && (p.getPosition().y < yPos) && (p.getPosition().y > (yPos - step)));
    }
    void paintSquare(float step, float width, float height, sf::RenderWindow& window, std::vector < Particle > particles) {
        for (auto i = step; i <= width; i += step) {
            for (auto j = step; j <= height; j += step) {
                float counter = 0;
                for (auto p : particles) {
                    if (isInside(p, i, j, step)) {
                        ++counter;
                    }
                }
                sf::RectangleShape rectangle;
                rectangle.setPosition(sf::Vector2f(i - step, j - step));
                rectangle.setSize(sf::Vector2f(step, step));
                rectangle.setFillColor(sf::Color(0, static_cast<int>(255 * counter / m_count * 3), 0));
                window.draw(rectangle);
            }
        }
    }
    int run() {
        const auto step = m_width / 20.0f;
        sf::RenderWindow window{ sf::VideoMode(static_cast<const unsigned int>(m_width),static_cast<const unsigned int> (m_height)), "PHYSICS" };
        window.setFramerateLimit(40);
        std::vector < Particle > particles;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0, 600);
        sf::Vector2f speed(10.f, 10.f);
        for (int i = 0; i < m_count; i++)
        {
            sf::Vector2f position(dis(gen), dis(gen));
            particles.push_back(Particle(position, 5.0, speed));
        }
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
            window.clear();
            paintSquare(step, m_width, m_height, window, particles);
            for (auto i = 0U; i < static_cast<unsigned int>(m_count); ++i) {
                std::random_device rd;
                std::mt19937 genm(rd());
                particles[i].move(600U, 600U, genm);
                particles[i].draw(window);
            }
            window.display();
        }
        return EXIT_SUCCESS;
    }
private:
    int m_count;
    const float m_width ;
    const float m_height ;
};
int main() {
    Visualizer myV(20, 600.f, 600.f);
    myV.run();
}
