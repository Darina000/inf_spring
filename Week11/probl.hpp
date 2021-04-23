#include <time.h>
#include <iostream>
#include <list>
#include <random>

#include <SFML/Graphics.hpp>


const double Degtorad = 0.017453;

const int W = sf::VideoMode::getDesktopMode().width;
const int H = sf::VideoMode::getDesktopMode().height;

enum class Name {
    asteroid,
    player,
    bullet,
    explosion,
    none
};



int randomint(int min, int max) {
    std::random_device rd;
    std::mt19937 eng(rd());
    if (max < min)
        std::swap(max, min);
    std::uniform_int_distribution<int> ran(min, max);
    return ran(eng);
}




class Animation
{
private:
    double Frame_, speed_, time_;
    sf::Sprite sprite_;
    std::vector<sf::IntRect> frames_;
public:
    Animation(sf::Texture& t, int x, int y, int w, int h, int count, double Speed);
    
    void update();
    
    sf::Sprite* get_sprite() {
        return &sprite_;
    }
    
    const bool isEnd()
    {
        return time_ >= frames_.size();
    }

};



void Animation::update()
{
    Frame_ += speed_;
    time_ += speed_;
    auto n = frames_.size();
    if (Frame_ >= n) {
        Frame_ -= n;
    }
    int m = std::trunc(Frame_);
    if (n > 0) {
        sprite_.setTextureRect(frames_[m]);
    }
}

Animation::Animation(sf::Texture& t, int x, int y, int w, int h, int count, double Speed) : speed_(Speed), Frame_(0), time_(0)
{
    for (int i = 0; i < count; i++) {
        frames_.push_back(sf::IntRect(x + i * w, y, w, h));
    }
    sprite_.setTexture(t);
    sprite_.setOrigin(w / 2, h / 2);
    sprite_.setTextureRect(frames_[0]);
}



class Entity
{
protected:
    double x_, y_, dx_, dy_, R_ = 1, angle_ = 0;
    bool life_;
    Name name_;
    Animation anim_;
    
public:
    
    Entity(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = true, double dx = 0, double dy = 0,
           Name name = Name::none):
        x_(X), y_(Y), anim_(a), R_(rad), name_(name), angle_(angle), life_(life), dx_(dx), dy_(dx) {};
    
    
    Entity(Animation& a, int X, int Y, Name name, double angle = 0, int rad = 1, bool life = true, double dx = 0, double dy = 0):
        x_(X), y_(Y), name_(name), anim_(a), R_(rad), angle_(angle), life_(life), dx_(dx), dy_(dx) {};
    
    
    virtual void update() {};
    
    
    void death(bool life) {
        life_ = life;
    }
    
    
    const bool get_life() { return life_; }
    
    const int get_r() { return R_; }
    
    const int get_x() { return x_; }
    
    const int get_y() { return y_; }
    
    Animation* get_anim() { return &anim_; }
    
    Name name() { return name_; }
    
    friend const bool isCollide(Entity* a, Entity* b);
    
    void draw(sf::RenderWindow& app);
    

    virtual ~Entity() = default;
};



void Entity::draw(sf::RenderWindow& app)
{
    anim_.get_sprite()->setPosition(x_, y_);
    anim_.get_sprite()->setRotation(angle_ + 90);
    app.draw(*anim_.get_sprite());
}





class asteroid : public Entity
{
public:
    asteroid(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = 1) : Entity(a, X, Y, angle, rad, life) {
        
        std::default_random_engine dre;
        dre.seed(time(0));
        std::uniform_int_distribution<int> id(-4, 4);
        
        dx_ = (id(dre));
        dy_ = (id(dre));
        name_ = Name::asteroid;
    }
    void update() override;

};




void asteroid::update()
{
    x_ += std::trunc(dx_);
    y_ += std::trunc(dy_);

    if (x_ > W) x_ = 0;
    if (x_ < 0) x_ = W;
    if (y_ > H) y_ = 0;
    if (y_ < 0) y_ = H;
}




class bullet : public Entity
{
public:
    bullet(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = 1) : Entity(a, X, Y, angle, rad, life)
    {
        name_ = Name::bullet;
    }

    void  update() override;

};



void  bullet::update()
{
    dx_ = std::cos(angle_ * Degtorad) * 6;
    dy_ = std::sin(angle_ * Degtorad) * 6;
    x_ += std::trunc(dx_);
    y_ += std::trunc(dy_);

    if (x_ > W || x_<0 || y_>H || y_ < 0) life_ = 0;
}




class player : public Entity
{
private:
    
    bool thrust_ = false;
    const double maxSpeed_ = 10;
    const double num = 0.99;
    int health_points = 5;
    int points_ = 0;
    
    
public:
    const double get_angle() {
        return angle_;
    }
    
    void c_angle(double angle) { angle_ += angle; }
    void c_thrust(bool thrust) { thrust_ = thrust; }
    bool thrust() { return thrust_; }
    void anim(Animation& a) { anim_ = a; }
    void inc_points() { points_++; }
    int get_points() { return points_; }
    int get_hp() { return health_points; }
    void set_dx() { dx_ = 0; }
    void set_dy() { dy_ = 0; }
    void dec_hp() { health_points--; }
    player(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = 1) : Entity(a, X, Y, angle, rad, life)
    {
        name_ = Name::player;
    }
    
    void settings(Animation& a, int X, int Y, double Angle = 0, int radius = 1, double dx = 0, double dy = 0)
    {
        anim_ = a;
        x_ = X; y_ = Y;
        angle_ = Angle;
        R_ = radius;
        dx_ = dx;
        dy_ = dy;
    }

    void update() override;

};



void player::update()
{
    if (thrust_)
    {
        dx_ += std::cos(angle_ * Degtorad) * 0.2;
        dy_ += std::sin(angle_ * Degtorad) * 0.2;
    }
    else
    {
        dx_ *= num;
        dy_ *= num;
    }

    double speed = std::sqrt(dx_ * dx_ + dy_ * dy_);
    if (speed > maxSpeed_)
    {
        dx_ *= maxSpeed_ / speed;
        dy_ *= maxSpeed_ / speed;
    }

    x_ += std::trunc(dx_);
    y_ += std::trunc(dy_);

    if (x_ > W) x_ = 0; if (x_ < 0) x_ = W;
    if (y_ > H) y_ = 0; if (y_ < 0) y_ = H;
}


const bool isCollide(Entity* a, Entity* b)
{
    return (b->x_ - a->x_) * (b->x_ - a->x_) +
        (b->y_ - a->y_) * (b->y_ - a->y_) <
        (a->R_ + b->R_) * (a->R_ + b->R_);
}


void init(std::string name, sf::Texture T) {
    T.loadFromFile(name);
}


class Textures {
private:
    sf::Texture texture_;
public:
    explicit Textures(std::string name) {
        texture_.loadFromFile(name);
    }
    sf::Texture* textures() {
        return &texture_;
    }

};





class Health_Points {
public:
    Animation anim;
    float x, y, angle;
    Health_Points(Animation& a, int X, int Y, float Angle = 0) :x(X), y(Y), angle(Angle), anim(a) {}
    void draw(sf::RenderWindow& app_)
    {
        anim.get_sprite()->setPosition(x, y);
        anim.get_sprite()->setRotation(angle + 90);
        app_.draw(*anim.get_sprite());
    }
};





class Game {
private:
    sf::RenderWindow app_;
    Textures t1, t2, t3, t4, t5, t6, t7, t8, t9;
    sf::Sprite background;
    sf::Sprite game_over;
    Animation sExplosion;
    Animation sRock;
    Animation sRock_small;
    Animation sBullet;
    Animation sPlayer;
    Animation sPlayer_go;
    Animation sExplosion_ship;
    Animation health_points;
    player* player_;
    std::list<Entity*> entities_;
    sf::Text text_points;
    Health_Points* hp;
    sf::Font font;

public:
    Game() : app_(sf::VideoMode(W, H), "Asteroids!"),

    
    
        t1("/Users/darazemlanskaa/Downloads/asteroids/images/spaceship.png"),
        t2("/Users/darazemlanskaa/Downloads/asteroids/images/background.jpg"),
        t3("/Users/darazemlanskaa/Downloads/asteroids/images/explosions/type_C.png"),
        t4("/Users/darazemlanskaa/Downloads/asteroids/images/rock.png"),
        t5("/Users/darazemlanskaa/Downloads/asteroids/images/fire_blue.png"),
        t6("/Users/darazemlanskaa/Downloads/asteroids/images/rock_small.png"),
        t7("/Users/darazemlanskaa/Downloads/asteroids/images/explosions/type_B.png"),
    
        t8("/Users/darazemlanskaa/Downloads/asteroids/images/health_points.png"),
        t9("/Users/darazemlanskaa/Downloads/asteroids/images/game_over.png"),
    
    
        sExplosion(*t3.textures(), 0, 0, 256, 256, 48, 0.5),
        sRock(*t4.textures(), 0, 0, 64, 64, 16, 0.2),
        sRock_small(*t6.textures(), 0, 0, 64, 64, 16, 0.2),
        sBullet(*t5.textures(), 0, 0, 32, 64, 16, 0.8),
        sPlayer(*t1.textures(), 40, 0, 40, 40, 1, 0),
        sPlayer_go(*t1.textures(), 40, 40, 40, 40, 1, 0),
        sExplosion_ship(*t7.textures(), 0, 0, 192, 192, 64, 0.5),
        background(*t2.textures()),
        health_points(*t8.textures(), 0, 0, 225, 225, 1, 0),
        game_over(*t9.textures()) {
        app_.setFramerateLimit(60);
            
            

        font.loadFromFile("/Users/darazemlanskaa/Downloads/asteroids/images/font.h");
        text_points.setFont(font);
        text_points.setFillColor(sf::Color(255, 0, 0, 200));
        text_points.setOutlineColor(sf::Color::Black);
        text_points.setOutlineThickness(2);
        text_points.setCharacterSize(40);
        text_points.setStyle(sf::Text::Regular);
        text_points.setString(std::to_string(0));
        text_points.setPosition(20, 0);
        app_.setFramerateLimit(60);

        for (int i = 0; i < 15; i++)
        {
            asteroid* a = new asteroid(sRock, randomint(0, W), randomint(0, H), randomint(0, 360), 25);
            entities_.push_back(a);
        }
        player_ = new player(sPlayer, W / 2, H / 2, 0, 20);
        entities_.push_back(player_);

    }
    void run() {
        while (app_.isOpen())
        {
            sf::Event event;
            while (app_.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    app_.close();

                if (event.type == sf::Event::KeyPressed)
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        bullet* b = new bullet(sBullet, player_->get_x(), player_->get_y(), player_->get_angle(), 10);
                        entities_.push_back(b);
                    }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player_->c_angle(3);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  player_->c_angle(-3);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player_->c_thrust(true);
            else player_->c_thrust(false);


            for (auto a : entities_)
                for (auto b : entities_)
                {
                    if (a->name() == Name::asteroid && b->name() == Name::bullet)
                        if (isCollide(a, b))
                        {
                            a->death(false);
                            b->death(false);
                            player_->inc_points();
                            text_points.setString(std::to_string(player_->get_points()));
                            Entity* e = new Entity(sExplosion, a->get_x(), a->get_y(), Name::explosion);
                            entities_.push_back(e);

                            if (a->get_r() == 25) {
                                for (int i = 0; i < 2; i++)
                                {
                                    Entity* e = new asteroid(sRock_small, a->get_x(), a->get_y(), randomint(0, 360), 15);
                                    entities_.push_front(e);
                                }
                            }

                        }

                    if (a->name() == Name::player && b->name() == Name::asteroid)
                        if (isCollide(a, b))
                        {
                            b->death(false);

                            Entity* k = new Entity(sExplosion_ship, a->get_x(), a->get_y(), Name::explosion);
                            entities_.push_front(k);

                            if (player_->get_hp() > 1)
                            {
                                player_->settings(sPlayer, W / 2, H / 2, 0, 20);
                                player_->set_dx(); player_->set_dy();
                                player_->dec_hp();

                            }
                            else
                            {
                                player_->dec_hp();
                                for (int i = 0; i < 64; i++) {
                                    app_.draw(background);
                                    k->get_anim()->update();
                                    k->draw(app_);
                                    app_.display();
                                }
                                app_.draw(background);
                                app_.display();
                            }

                        }
                }

            if (player_->get_hp() == 0) {

                app_.clear();
                app_.draw(background);
                for (int i = 0; i < 255; i = i + 7) {

                    game_over.setColor(sf::Color(255, 255, 255, i));
                    app_.draw(game_over);
                    app_.display();
                }
                system("pause");
                return;
            }

            if (player_->thrust())  player_->anim(sPlayer_go);
            else   player_->anim(sPlayer);


            for (auto e : entities_)
                if (e->name() == Name::explosion)
                    if (e->get_anim()->isEnd()) {
                        e->death(false);
                    }

            if (randomint(0, 150) == 0)
            {
                asteroid* a = new asteroid(sRock, randomint(0, W), randomint(0, H), randomint(0, 360), 25);
                entities_.push_back(a);
            }

            for (auto i = entities_.begin(); i != entities_.end();)
            {
                Entity* e = *i;

                e->update();
                e->get_anim()->update();

                if (e->get_life() == false) {
                    i = entities_.erase(i);

                    delete e;
                }
                else i++;
            }
            app_.draw(background);
            for (auto i : entities_) i->draw(app_);

            for (auto i = 0; i < player_->get_hp(); i++) {
                hp = new Health_Points(health_points, 1050 + i * 75, 120, -90);
                hp->draw(app_);

            }
            app_.draw(text_points);
            app_.display();

        }

    }
};
