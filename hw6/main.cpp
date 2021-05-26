#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <iostream>

/*
 * CHANGE DOCUMENTATION:
 * Fixed bullet firing so that bullets are automatically fired on a timer instead of requiring button-mashing
 * Added a break system (push down)
 * Added 'seekers' which move towards the players at high speeds. Try to out-maneuver them!
 */

using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

class Animation {
public:
	float Frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation() {}

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed) {
		Frame = 0;
		speed = Speed;

		for (int i = 0; i < count; i++) {
			frames.push_back(IntRect(x + i * w, y, w, h));
		}

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}


	void update() {
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
	}

	bool isEnd() {
		return Frame + speed >= frames.size();
	}

};


class Entity {
public:
	float x, y, dx, dy, R, angle;
	bool life;
	std::string name;
	Animation anim;

	Entity() {
		life = 1;
	}

	void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1) {
		anim = a;
		x = X;
		y = Y;
		angle = Angle;
		R = radius;
	}

	virtual void update() {};

	void draw(RenderWindow &app) {
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
		//app.draw(circle);
	}

	virtual ~Entity() {};
};


class asteroid : public Entity {
public:
	static int count;
	int stage = 3;
	asteroid() {
		dx = rand() % 8 - 4;
		dy = rand() % 8 - 4;
		name = "asteroid";
		count++;
	}
	~asteroid() {
		count--;
	}

	void update() {
		x += dx;
		y += dy;

		if (x > W) x = 0;
		if (x < 0) x = W;
		if (y > H) y = 0;
		if (y < 0) y = H;
	}

};

int asteroid::count {0};

class saucer : public Entity {
public:
	int direction; // If Direction is 0, it goes right. 1, it goes left

	saucer() {
		name = "saucer";
		direction = float(rand() % 2);
		dx = 1 - 2*direction;
		dy = 0;
		x = 0 + W*direction;
		y = floor((rand() % H)*0.8+H*0.1);
		//std::cout  << x << " : " << dx << std::endl;
	}

	void update() {
		x += dx;
		angle+=5;
		if (x > W || x < 0) life = 0;
	}

};

class seeker : public Entity {
public:
	Entity* focus;

	seeker() {
		name = "seeker";
		focus = nullptr;
	}

	void update() {
		if (focus == nullptr) return;
		angle = atan2(focus->y-y,focus->x-x) / DEGTORAD;
		dx += cos(angle * DEGTORAD) * 0.15;
		dy += sin(angle * DEGTORAD) * 0.15;

		dx*=0.995;
		dy*=0.995;

		x+=dx;
		y+=dy;

		if (x > W) x = 0;
		if (x < 0) x = W;
		if (y > H) y = 0;
		if (y < 0) y = H;
	}

	void setFocus(Entity* e) {
		focus = e;
	}
};


class bullet : public Entity {
public:
	bullet() {
		name = "bullet";
	}

	void update() {
		dx = cos(angle * DEGTORAD) * 6;
		dy = sin(angle * DEGTORAD) * 6;
		// angle+=rand()%6-3;
		x += dx;
		y += dy;

		if (x > W || x < 0 || y > H || y < 0) life = 0;
	}

};


class player : public Entity {
public:
	bool thrust;
	bool slow;
	int shotTimer;

	player() {
		name = "player";
		shotTimer = 0;
	}

	void update() {
		if (thrust) {
			dx += cos(angle * DEGTORAD) * 0.2;
			dy += sin(angle * DEGTORAD) * 0.2;
		} else if (slow) {
			dx *= 0.92;
			dy *= 0.92;
		} else {
			dx *= 0.99;
			dy *= 0.99;
		}

		int maxSpeed = 15;
		float speed = sqrt(dx * dx + dy * dy);
		if (speed > maxSpeed) {
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x > W) x = 0;
		if (x < 0) x = W;
		if (y > H) y = 0;
		if (y < 0) y = H;

		shotTimer--;
	}

	bullet* fire(Animation projectile) {
		if (shotTimer < 1) {
			shotTimer = 20;
			bullet* b = new bullet();
			b->settings(projectile, x, y, angle, 10);
			return b;
		}
		return nullptr;
	}

};


bool isCollide(Entity *a, Entity *b) {
	return (b->x - a->x) * (b->x - a->x) +
		   (b->y - a->y) * (b->y - a->y) <
		   (a->R + b->R) * (a->R + b->R);
}


int main() {
	srand(time(0));

	RenderWindow app(VideoMode(W, H), "Asteroids!");
	app.setFramerateLimit(60);

	Texture t1, t2, t3, t4, t5, t6, t7, t8;
	t1.loadFromFile("images/spaceship.png");
	t2.loadFromFile("images/background.jpg");
	t3.loadFromFile("images/explosions/type_C.png");
	t4.loadFromFile("images/rock.png");
	t5.loadFromFile("images/fire_blue.png");
	t6.loadFromFile("images/rock_small.png");
	t7.loadFromFile("images/explosions/type_B.png");
	t8.loadFromFile("images/ships_saucer.png");

	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite background(t2);

	Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
	Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
	Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
	Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
	Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
	Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
	Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
	Animation sSaucer(t8, 385, 268, 94, 94, 1, 0);
	Animation sSeeker(t8,48,232,16,32,1,0);

	SoundBuffer buzzBuffer, warningBuffer;
	if (!buzzBuffer.loadFromFile("sounds/saucer_fly.wav")) return -1;
	if (!warningBuffer.loadFromFile("sounds/saucer_warning.wav")) return -1;
	Sound buzz, warning;
	buzz.setBuffer(buzzBuffer);
	buzz.setLoop(true);
	buzz.setVolume(30);
	warning.setBuffer(warningBuffer);
	warning.setLoop(false);
	warning.setVolume(80);


	std::list<Entity *> entities;

	//int sauceTimer = 500;
	int sauceTimer = 10;

	player *p = new player();
	p->settings(sPlayer, 200, 200, 0, 20);
	entities.push_back(p);

	/////main loop/////
	while (app.isOpen()) {
		Event event;
		while (app.pollEvent(event)) {
			if (event.type == Event::Closed) {
				app.close();
			}

			/*if (event.type == Event::KeyPressed) { // DEPRICATED CODE. KEPT FOR BACKUP PURPOSES.
				if (event.key.code == Keyboard::Space) {
					bullet *b = new bullet();
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					entities.push_back(b);
				}
			}*/
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;
		if (Keyboard::isKeyPressed(Keyboard::Left)) p->angle -= 3;
		if (Keyboard::isKeyPressed(Keyboard::Up)) { p->thrust = true; }
		else { p->thrust = false; }
		if (Keyboard::isKeyPressed(Keyboard::Down)) { p->slow = true; }
		else { p->slow = false; }
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			bullet* a = p->fire(sBullet);
			if (a != nullptr) entities.push_back(a);
		}

		// Spawn Asteroid Wave
		if (asteroid::count < 1) for (int i = 0; i < 15; i++) {
			asteroid *a = new asteroid();
			a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
			entities.push_back(a);
		}

		for (auto a:entities) {
			for (auto b:entities) {
				if ((a->name == "asteroid" || a->name == "saucer" || a->name == "seeker") && b->name == "bullet") {
					if (isCollide(a, b)) {
						a->life = false;
						b->life = false;

						Entity *e = new Entity();
						e->settings(sExplosion, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);


						for (int i = 0; i < 2; i++) {
							if (a->R == 15) continue;
							Entity *e = new asteroid();
							e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
							entities.push_back(e);
						}

					}
				}

				if (a->name == "player" && (b->name == "asteroid" || b->name == "saucer" || b->name == "seeker")) {
					if (isCollide(a, b)) {
						b->life = false;

						Entity *e = new Entity();
						e->settings(sExplosion_ship, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						p->settings(sPlayer, W / 2, H / 2, 0, 20);
						p->dx = 0;
						p->dy = 0;
					}
				}
			}
		}

		// Changes player graphic if moving
		if (p->thrust) { p->anim = sPlayer_go; }
		else { p->anim = sPlayer; }

		// Gets rid of old explosions
		for (auto e:entities) {
			if (e->name == "explosion") {
				if (e->anim.isEnd()) e->life = 0;
			}
		}

		// Seeker Spawning
		if (rand() % 200 == 0) {
			seeker *s = new seeker();
			//s->settings(sSeeker, 100, 100, 0, 25);
			s->settings(sSeeker, rand() % W, rand()%2 * H, 0, 40);
			s->setFocus(p);
			entities.push_back(s);
		}

		// Checking if there's an active saucer
		bool saucerAlive = false;
		for (Entity* e : entities) if (e->name == "saucer") saucerAlive = true;

		// Saucer spawning & sound management
		if (!saucerAlive && sauceTimer < -499) {
			sauceTimer = 700;
			warning.stop();
			buzz.stop();
		} else if (!saucerAlive && sauceTimer < 1) {
			sauceTimer = -500;
			saucer* s = new saucer();
			s->settings(sSaucer,s->x,s->y,0,50);
			entities.push_back(s);
			warning.play();
		} else if (saucerAlive && sauceTimer < -575 && buzz.getStatus() != buzz.Playing)
			buzz.play();

		if (sauceTimer > -600) sauceTimer--;
		//if (s!= nullptr) std::cout << sauceTimer << " : " << s->x << " : " << s->y << std::endl;
		//std::cout << sauceTimer << std::endl;


		for (auto i = entities.begin(); i != entities.end();) {
			Entity *e = *i;

			e->update();
			e->anim.update();

			if (e->life == false) {
				i = entities.erase(i);
				delete e;
			} else { i++; }
		}



		//////draw//////
		app.draw(background);

		for (auto i:entities) {
			i->draw(app);
		}

		app.display();
	}

	return 0;
}
