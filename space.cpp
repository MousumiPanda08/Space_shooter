#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace sf;
using namespace std;

enum GameState
{
    MENU,
    PLAYING,
    GAMEOVER
};

struct Bullet
{
    Sprite sprite;

    Bullet(Texture& tex, float x, float y)
    {
        sprite.setTexture(tex);
        sprite.setScale(0.25f, 0.25f);
        sprite.setPosition(x, y);
    }
};

struct Enemy
{
    Sprite sprite;

    Enemy(Texture& tex, float x, float y)
    {
        sprite.setTexture(tex);
        sprite.setScale(0.5f, 0.5f);
        sprite.setPosition(x, y);
    }
};

struct Explosion
{
    Sprite sprite;
    Clock timer;

    Explosion(Texture& tex, float x, float y)
    {
        sprite.setTexture(tex);
        sprite.setScale(0.5f, 0.5f);
        sprite.setPosition(x, y);
    }
};

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    RenderWindow window(VideoMode(800, 600), "Space Shooter");
    window.setFramerateLimit(60);

    //=========================
    // LOAD TEXTURES
    //=========================
    Texture playerTex;
    Texture enemyTex;
    Texture bulletTex;
    Texture explosionTex;
    Texture bgTex;

    if (!playerTex.loadFromFile("player.png")) return -1;
    if (!enemyTex.loadFromFile("enemy.png")) return -1;
    if (!bulletTex.loadFromFile("bullet.png")) return -1;
    if (!explosionTex.loadFromFile("explosion.png")) return -1;
    if (!bgTex.loadFromFile("background.jpg")) return -1;

    Font font;
    if (!font.loadFromFile("KOMIKAP_.ttf")) return -1;

    //=========================
    // BACKGROUND
    //=========================
    Sprite background(bgTex);

    background.setScale(
        800.f / bgTex.getSize().x,
        600.f / bgTex.getSize().y
    );

    //=========================
    // PLAYER
    //=========================
    Sprite player(playerTex);

    player.setScale(0.25f, 0.25f);

    player.setPosition(
        400.f - player.getGlobalBounds().width / 2,
        500.f
    );

    //=========================
    // TEXT
    //=========================
    Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(50);
    titleText.setString("SPACE SHOOTER");
    titleText.setPosition(180, 150);

    Text startText;
    startText.setFont(font);
    startText.setCharacterSize(30);
    startText.setString("Press ENTER To Start");
    startText.setPosition(220, 260);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setPosition(10, 10);

    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(28);
    livesText.setPosition(650, 10);

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setPosition(160, 220);

    //=========================
    // GAME DATA
    //=========================
    GameState state = MENU;

    vector<Bullet> bullets;
    vector<Enemy> enemies;
    vector<Explosion> explosions;

    Clock shootClock;
    Clock enemyClock;

    int score = 0;
    int lives = 3;

    //=========================
    // GAME LOOP
    //=========================
    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //=========================
        // MENU
        //=========================
        if (state == MENU)
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                score = 0;
                lives = 3;

                bullets.clear();
                enemies.clear();
                explosions.clear();

                player.setPosition(
                    400.f - player.getGlobalBounds().width / 2,
                    500.f
                );

                state = PLAYING;
            }
        }

        //=========================
        // PLAYING
        //=========================
        else if (state == PLAYING)
        {
            float speed = 6.f;

            if (Keyboard::isKeyPressed(Keyboard::A) ||
                Keyboard::isKeyPressed(Keyboard::Left))
            {
                if (player.getPosition().x > 0)
                    player.move(-speed, 0);
            }

            if (Keyboard::isKeyPressed(Keyboard::D) ||
                Keyboard::isKeyPressed(Keyboard::Right))
            {
                if (player.getPosition().x <
                    800 - player.getGlobalBounds().width)
                {
                    player.move(speed, 0);
                }
            }

            // SHOOT
            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (shootClock.getElapsedTime().asMilliseconds() > 250)
                {
                    bullets.push_back(
                        Bullet(
                            bulletTex,
                            player.getPosition().x +
                            player.getGlobalBounds().width / 2 - 10,
                            player.getPosition().y
                        )
                    );

                    shootClock.restart();
                }
            }

            // SPAWN ENEMY
            if (enemyClock.getElapsedTime().asSeconds() > 1.0f)
            {
                float x = static_cast<float>(rand() % 700);

                enemies.push_back(
                    Enemy(enemyTex, x, -80)
                );

                enemyClock.restart();
            }

            // MOVE BULLETS
            for (int i = 0; i < bullets.size(); i++)
            {
                bullets[i].sprite.move(0, -10);

                if (bullets[i].sprite.getPosition().y < -100)
                {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }
            }

            // MOVE ENEMIES
            for (int i = 0; i < enemies.size(); i++)
            {
                enemies[i].sprite.move(0, 3);

                if (enemies[i].sprite.getPosition().y > 650)
                {
                    enemies.erase(enemies.begin() + i);

                    lives--;

                    i--;
                }
            }

            // COLLISION
            for (int i = 0; i < bullets.size(); i++)
            {
                bool bulletRemoved = false;

                for (int j = 0; j < enemies.size(); j++)
                {
                    if (bullets[i].sprite.getGlobalBounds()
                        .intersects(
                            enemies[j].sprite.getGlobalBounds()))
                    {
                        explosions.push_back(
                            Explosion(
                                explosionTex,
                                enemies[j].sprite.getPosition().x,
                                enemies[j].sprite.getPosition().y
                            )
                        );

                        bullets.erase(bullets.begin() + i);
                        enemies.erase(enemies.begin() + j);

                        score += 10;

                        bulletRemoved = true;
                        break;
                    }
                }

                if (bulletRemoved)
                {
                    i--;
                }
            }

            // PLAYER COLLISION
            for (int i = 0; i < enemies.size(); i++)
            {
                if (player.getGlobalBounds().intersects(
                    enemies[i].sprite.getGlobalBounds()))
                {
                    state = GAMEOVER;
                }
            }

            // REMOVE EXPLOSIONS
            for (int i = 0; i < explosions.size(); i++)
            {
                if (explosions[i].timer
                    .getElapsedTime()
                    .asSeconds() > 0.3f)
                {
                    explosions.erase(explosions.begin() + i);
                    i--;
                }
            }

            if (lives <= 0)
            {
                state = GAMEOVER;
            }

            scoreText.setString(
                "Score: " + to_string(score));

            livesText.setString(
                "Lives: " + to_string(lives));
        }

        //=========================
        // GAME OVER
        //=========================
        else if (state == GAMEOVER)
        {
            gameOverText.setString(
                "GAME OVER\n\nScore: " +
                to_string(score) +
                "\n\nPress R To Restart"
            );

            if (Keyboard::isKeyPressed(Keyboard::R))
            {
                state = MENU;
            }
        }

        //=========================
        // DRAW
        //=========================
        window.clear();

        window.draw(background);

        if (state == MENU)
        {
            window.draw(titleText);
            window.draw(startText);
        }
        else if (state == PLAYING)
        {
            window.draw(player);

            for (auto& bullet : bullets)
                window.draw(bullet.sprite);

            for (auto& enemy : enemies)
                window.draw(enemy.sprite);

            for (auto& explosion : explosions)
                window.draw(explosion.sprite);

            window.draw(scoreText);
            window.draw(livesText);
        }
        else if (state == GAMEOVER)
        {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
