

#include <ctime> // ctime
#include <iostream>
#include <list>
#include <random>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

enum class Name {
    asteroid,
    player,
    bullet,
    explosion,
    none
};

namespace magic_constants {

const int W = 1200; //1200
const int H = 800; //800

const int const_update {64};
const double Degtorad {0.017453};
const double num { 0.99};
const int bullet_update {6};
const double player_update {0.2};
const int isCollide_R {25};
const int Game_asteroid {15};
const int keyboard_angle_r {3};
const int keyboard_angle_l {-3};
const int color_part {7};
const int color {255};
}



class Animation
{
public:
    explicit Animation(sf::Texture& t, int x, int y, int w, int h, int count, double Speed);
    
    void update();
    
    sf::Sprite* get_sprite() {
        return &sprite_;
    }
    
    bool isEnd()
    {
        return time_ >= frames_.size();
    }
    
private:
    double Anim_Frame_;
    double speed_;
    double time_;
    sf::Sprite sprite_;
    std::vector<sf::IntRect> frames_;

};



void Animation::update()
{
    Anim_Frame_ += speed_;
    time_ += speed_;
    auto n = frames_.size();
    if (Anim_Frame_ >= n) {
        Anim_Frame_ -= n;
    }
    
    if (n > 0) {
        sprite_.setTextureRect(frames_[static_cast<int>(Anim_Frame_)]);
    }
}

Animation::Animation(sf::Texture& t, int x, int y, int w, int h, int count, double Speed) : speed_(Speed), Anim_Frame_(0), time_(0)
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
public:
    
    explicit Entity(Animation& a, int X, int Y, Name name, double angle = 0, int rad = 1, bool life = true, double dx = 0, double dy = 0):
        x_(X), y_(Y), name_(name), anim_(a), R_(rad), angle_(angle), life_(life), dx_(dx), dy_(dx) {};
    
    
    virtual void update() = 0;
    
    
    void death(bool life) {
        life_ = life;
    }
    
    Animation* get_anim() { return &anim_; }
    
    friend const bool isCollide(Entity* a, Entity* b);
    
    void draw(sf::RenderWindow& app);
    

    virtual ~Entity() = default;
    
protected:
    Animation anim_;
    
public:
    double x_, y_, dx_, dy_, R_ = 1, angle_ = 0;
    bool life_;
    Name name_;
};



void Entity::draw(sf::RenderWindow& app)
{
    anim_.get_sprite()->setPosition(x_, y_);
    anim_.get_sprite()->setRotation(angle_ + 90);
    app.draw(*anim_.get_sprite());
}


namespace rou {
std::default_random_engine dre;
std::uniform_int_distribution<int> id(-4, 4);
}



class Entity_asteroid : public Entity
{
public:
    explicit Entity_asteroid(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = 1) : Entity(a, X, Y, Name::asteroid, angle, rad, life, (rou::id(rou::dre)), (rou::id(rou::dre))) {
     rou::dre.seed(time(0));
    }
    void update() override;

};




void Entity_asteroid::update()
{
    x_ += static_cast<int>(dx_);
    y_ += static_cast<int>(dy_);


    if (x_ > magic_constants::W) x_ = 0;
    if (x_ < 0) x_ = magic_constants::W;
    if (y_ > magic_constants::H) y_ = 0;
    if (y_ < 0) y_ = magic_constants::H;
}


class bullet : public Entity
{
public:
    explicit bullet(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = 1) : Entity(a, X, Y, Name::bullet, angle, rad, life){}

    void  update() override;

};



void  bullet::update()
{
    
    dx_ = std::cos(angle_ * magic_constants::Degtorad) * magic_constants::bullet_update;//6
    dy_ = std::sin(angle_ * magic_constants::Degtorad) * magic_constants::bullet_update;//6
    
    x_ += std::trunc(dx_);
    y_ += std::trunc(dy_);
    

    if (x_ > magic_constants::W || x_<0 || y_>magic_constants::H || y_ < 0) life_ = 0;
}




class player : public Entity
{
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
    
    player(Animation& a, int X, int Y, double angle = 0, int rad = 1, bool life = 1, double dx = 0, double dy = 0) : Entity(a, X, Y, Name::player, angle, rad, life, dx, dy)
    {}


    void update() override;
    
private:
    bool thrust_ = false;
    const double maxSpeed_ = 10;
    int health_points = 5;
    int points_ = 0;

};



void player::update()
{
    if (thrust_)
    {
        dx_ += std::cos(angle_ * magic_constants::Degtorad) * magic_constants::player_update;
        dy_ += std::sin(angle_ * magic_constants::Degtorad) * magic_constants::player_update;
    }
    else
    {
        dx_ *= magic_constants::num;
        dy_ *= magic_constants::num;
    }

    double speed = std::sqrt(dx_ * dx_ + dy_ * dy_);
    if (speed > maxSpeed_)
    {
        dx_ *= maxSpeed_ / speed;
        dy_ *= maxSpeed_ / speed;
    }
    
    x_ += std::trunc(dx_);
    y_ += std::trunc(dy_);

    if (x_ > magic_constants::W) x_ = 0; if (x_ < 0) x_ = magic_constants::W;
    if (y_ > magic_constants::H) y_ = 0; if (y_ < 0) y_ = magic_constants::H;
}


const bool isCollide(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    return (b->x_ - a->x_) * (b->x_ - a->x_) +
        (b->y_ - a->y_) * (b->y_ - a->y_) <
        (a->R_ + b->R_) * (a->R_ + b->R_);
}


void init(std::string name, sf::Texture T) {
    T.loadFromFile(name);
}


class Textures {
public:
    explicit Textures(const std::string& name) {
        texture_.loadFromFile(name);
    }
    sf::Texture* textures() {
        return &texture_;
    }
private:
    sf::Texture texture_;
};





class Health_Points {
public:
    Health_Points(Animation& a, int X, int Y, float Angle = 0) :x(X), y(Y), angle(Angle), anim(a) {}
    
    void draw(sf::RenderWindow& app_)
    {
        anim.get_sprite()->setPosition(x, y);
        anim.get_sprite()->setRotation(angle + 90);
        app_.draw(*anim.get_sprite());
    }
public:
    Animation anim;
    float x, y, angle;
};





class Game {
public:
    explicit Game() : app_(sf::VideoMode(magic_constants::W, magic_constants::H), "Asteroids!"),

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

        for (int i = 0; i < magic_constants::Game_asteroid; i++)//magic_constants::Game_asteroid
        {
                std::random_device rd;
                std::mt19937 eng(rd());
                std::uniform_int_distribution<int> ran_w(0, magic_constants::W);
                std::uniform_int_distribution<int> ran_h(0, magic_constants::H);
                std::uniform_int_distribution<int> ran(0, 360);
            
            std::shared_ptr<Entity_asteroid> a(new Entity_asteroid(sRock, ran_w(eng), ran_h(eng), ran(eng), 25));
            entities_.push_back(a);
        }
            
        std::shared_ptr<player> player_(new player(sPlayer, magic_constants::W / 2, magic_constants::H / 2, 0, 20));
            
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
                        std::shared_ptr<bullet> b(new bullet(sBullet, player_-> x_, player_->y_, player_->angle_, 10));
                        entities_.push_back(b);
                    }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player_->c_angle(magic_constants::keyboard_angle_r);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  player_->c_angle(magic_constants::keyboard_angle_l);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player_->c_thrust(true);
            else player_->c_thrust(false);


            for (auto a : entities_)
                for (auto b : entities_)
                {
                    if (a->name_ == Name::asteroid && b->name_ == Name::bullet)
                        if (isCollide(a, b))
                        {
                            a->death(false);
                            b->death(false);
                            player_->inc_points();
                            text_points.setString(std::to_string(player_->get_points()));
                            
                            std::shared_ptr<bullet> e(new bullet(sExplosion, a->x_, a->y_));
                            entities_.push_back(e);

                            if (a->R_ == magic_constants::isCollide_R) {
                                for (int i = 0; i < 2; i++)
                                {
                                    std::random_device rd;
                                    std::mt19937 eng(rd());
                                    std::uniform_int_distribution<int> ran(0, 360);
                                   
                                    std::shared_ptr<Entity> e(new Entity_asteroid(sRock_small, a->x_, a->y_, ran(eng), 15));
                                    entities_.push_front(e);
                                }
                            }

                        }

                    if (a->name_ == Name::player && b->name_ == Name::asteroid)
                        if (isCollide(a, b))
                        {
                            b->death(false);
                            
                            
                            std::shared_ptr<player> k(new player(sExplosion_ship, a->x_, a->y_));                    
                          
                            entities_.push_front(k);

                            if (player_->get_hp() > 1)
                            {
                                player_ = new player(sPlayer, magic_constants::W / 2, magic_constants::H / 2, 0, 20);
                                player_->set_dx();
                                player_->set_dy();
                                player_->dec_hp();

                            }
                            else
                            {
                                player_->dec_hp();
                                for (int i = 0; i < magic_constants::const_update; i++) {
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
                for (int i = 0; i < magic_constants::color; i = i + magic_constants::color_part) {//magic_constants::color_part

                    game_over.setColor(sf::Color(magic_constants::color, magic_constants::color, magic_constants::color, i));
                    app_.draw(game_over);
                    app_.display();
                }
                system("pause");
                return;
            }

            if (player_->thrust())  player_->anim(sPlayer_go);
            else   player_->anim(sPlayer);


            for (auto e : entities_)
                if (e->name_ == Name::explosion)
                    if (e->get_anim()->isEnd()) {
                        e->death(false);
                    }
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<int> ran(0, 150);

            if (ran(eng) == 0)
            {
                std::random_device rd;
                std::mt19937 eng(rd());
                std::uniform_int_distribution<int> ran_w(0, magic_constants::W);
                std::uniform_int_distribution<int> ran_h(0, magic_constants::H);
                std::uniform_int_distribution<int> ran(0, 360);
                std::shared_ptr<Entity_asteroid> a(new Entity_asteroid(sRock, ran_w(eng), ran_h(eng), ran(eng), 25));
                entities_.push_back(a);
            }

            for (auto i = entities_.begin(); i != entities_.end();)
            {
                std::shared_ptr<Entity> e = *i;
                //Entity* e = *i;
                
                e->update();
                e->get_anim()->update();

                if (e->life_ == false) {
                    i = entities_.erase(i);

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
    std::list<std::shared_ptr<Entity>> entities_;
    
    sf::Text text_points;
    Health_Points* hp;
    sf::Font font;
};

