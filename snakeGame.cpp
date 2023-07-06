#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int N = 30, M = 20;
int size = 16;
int w = size * N;
int h = size * M;

int dir = 0;
int num = 4;

struct Snake {
    int x, y;
} s[100];

struct Fruit {
    int x, y;
} f;

bool gameOver = false;

void Tick() {
    for (int i = num; i > 0; i--) {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }
    if (dir == 0) s[0].y += 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y -= 1;

    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++;
        f.x = rand() % N;
        f.y = rand() % M;
    }

    if (s[0].x >= N || s[0].x < 0 || s[0].y >= M || s[0].y < 0) {
        gameOver = true;
        return;
    }

    for (int i = 1; i < num; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            gameOver = true;
            return;
        }
    }
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(w, h), "SNAKE GAME");
    Texture t1, t2, t3;

    t1.loadFromFile("images/white.png");
    t2.loadFromFile("images/green.png");
    t3.loadFromFile("images/red.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);

    Clock clock;
    float timer = 0, delay = 0.1;
    f.x = 10;
    f.y = 10;

    Font font;
    font.loadFromFile("images/THEBOLDFONT.ttf");

    Text gameOverText("Game Over", font, 60);
    gameOverText.setFillColor(sf::Color::Red);

    FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(w / 2, h / 2);

    Text scoreText("Score: 0", font, 30);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(10, h - 40);

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (!gameOver) {
            if (Keyboard::isKeyPressed(Keyboard::Left))
                dir = 1;
            if (Keyboard::isKeyPressed(Keyboard::Right))
                dir = 2;
            if (Keyboard::isKeyPressed(Keyboard::Down))
                dir = 0;
            if (Keyboard::isKeyPressed(Keyboard::Up))
                dir = 3;

            if (timer > delay) {
                timer = 0;
                Tick();
            }
        }

        window.clear();

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                sprite1.setPosition(i * size, j * size);
                window.draw(sprite1);
            }

        if (gameOver) {
            window.draw(gameOverText);
            window.draw(scoreText);
        }
        else {
            for (int i = 0; i < num; i++) {
                sprite2.setPosition(s[i].x * size, s[i].y * size);
                window.draw(sprite2);
            }

            sprite3.setPosition(f.x * size, f.y * size);
            window.draw(sprite3);

            scoreText.setString("Score: " + std::to_string(num - 4));
        }

        window.display();
    }

    return 0;
}
