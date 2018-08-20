// Timber.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main(){
	//create video object
	VideoMode vm(1920, 1080);

	//create and open window
	RenderWindow window(vm, "Timber!!", Style::Fullscreen);

	//create texture object and load image from file
	Texture textureBackground;
	if (!textureBackground.loadFromFile("graphics/background.png")) {
		std::cout << "ERROR: image could not be loaded!";
		return 1;
		//TODO: try/catch exception
	}
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	//load tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//load bee (will move)
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0;//not using 0.0f

	//TODO: create cloud class
	//load cloud
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	int numberOfClouds = 5;//TODO: make a global constant
	Sprite *spriteCloud = new Sprite[numberOfClouds];
	bool *cloudActive = new bool[numberOfClouds];
	float *cloudSpeed = new float [numberOfClouds];

	for (int i = 0; i < numberOfClouds; i++) {
		spriteCloud[i].setTexture(textureCloud);
		spriteCloud[i].setPosition(0, 250 * i);
		cloudActive[i] = false;
		cloudSpeed[i] = 0.0;
	}

	//to control framerate
	Clock clock;

	//timebar code
	RectangleShape timeBar;
	float timeBarStartWidth = 400, timeBarHeight = 40;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920/2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f, timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//game starts at 'paused' state
	bool paused = true;

	//HUD code
	int score = 0;
	sf::Text messageText, scoreText;
	//choose font
	Font font;
	if (!font.loadFromFile("fonts/komikap.ttf")) {
		std::cout << "ERROR: font could not be loaded!";
		return 3;
		//TODO: try/catch exception
	}
	messageText.setFont(font);
	scoreText.setFont(font);
	//assign message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	//set size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	//choose color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	//position text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//prepare branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, 2000);

		branches[i].setOrigin(220, 20);
	}

	//player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side playerSide = side::LEFT;

	//gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	//sound fx
	SoundBuffer chopBuffer, deathBuffer, ootBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	deathBuffer.loadFromFile("sound/death.wav");
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound chop, death, outOfTime;
	chop.setBuffer(chopBuffer);
	death.setBuffer(deathBuffer);
	outOfTime.setBuffer(ootBuffer);

	//TODO: error check all loadfromfiles

	while (window.isOpen()) {
		//used to poll if key is being pressed
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;

				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//start/restart game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			//reset time and score
			score = 0;
			timeRemaining = 5;

			//makes branches dissapear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}
			//hide gravestone
			spriteRIP.setPosition(675, 2000);
			//set playerPos
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;

				//add to amt of timer emaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				//send log to left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;
				score++;

				//add to amt of timer emaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				//send log to left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
		}

		if (!paused) {
			//update scene

				//measures the time
			Time dt = clock.restart();//deltaTime

			//subtract time remaining
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining < 0) {
				paused = true;

				messageText.setString("Game over! Out of time, hit enter to restart!");
				//position text
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();
			}

			//setup bee
			if (!beeActive) {
				srand((int)time(0) * 10);

				//speed
				beeSpeed = rand() % 400 + 700;

				//y position
				srand((int)time(0) * 10);
				float height = rand() % 500 + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else {
				//move bee
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				//check if reached edge
				if (spriteBee.getPosition().x < -100) {
					beeActive = false;
				}
			}

			//move clouds
			for (int i = 0; i < numberOfClouds; i++) {
				if (!cloudActive[i]) {
					cloudSpeed[i] = rand() % ((i + 2) * 50) + 30;//to achieve p-random (different) speed

					float height = rand() % 150;//p-random diff height
					spriteCloud[i].setPosition(-300, height);
					cloudActive[i] = true;
				}
				else {
					spriteCloud[i].setPosition(spriteCloud[i].getPosition().x + (cloudSpeed[i] * dt.asSeconds()), spriteCloud[i].getPosition().y);

					if (spriteCloud[i].getPosition().x > 1920) {
						cloudActive[i] = false;
					}
				}
			}
			//TODO: create function that takes in object, modifier (for rand), and modifies accordingly



			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());//TODO: just simple string concatenation? s + score

			//update branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else {//off screen
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				bool logReachedEdge = (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000);
				if (logReachedEdge) {
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			bool playerDead = branchPositions[5] == playerSide;
			if (playerDead) {
				paused = true;
				acceptInput = false;


				int xPosition;
				if (playerSide == side::LEFT) {
					xPosition = AXE_POSITION_LEFT - 100;
				}
				else {
					xPosition = AXE_POSITION_RIGHT + 100;
				}
				spriteRIP.setPosition(xPosition, 760);

				spritePlayer.setPosition(2000, 660);
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
				spriteLog.setPosition(2000, 660);

				messageText.setString("SQUISHED!!");
				
				//position text
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}
		}

		//draw scene

		//clear from last frame
		window.clear();

		//draw scene background
		window.draw(spriteBackground);
		//clouds
		for (int i = 0; i < numberOfClouds; i++) {
			window.draw(spriteCloud[i]);
		}

		//tree
		window.draw(spriteTree);

		//draw player and related sprites
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);

		//branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		//bee
		window.draw(spriteBee);
		//score and message
		window.draw(scoreText);
		if (paused) {
			window.draw(messageText);
		}
		window.draw(timeBar);

		//display drawn for current frame
		window.display();
	}

	//properly freeing memory
	delete[]spriteCloud;
	delete[]cloudActive;
	delete[]cloudSpeed;
	spriteCloud = NULL; cloudActive = NULL; cloudSpeed = NULL;

    return 0;
}

void updateBranches(int seed) {
	for (int i = NUM_BRANCHES - 1; i > 0; i--) {
		branchPositions[i] = branchPositions[i - 1];
	}
	srand((int)time(0) + seed);
	//spawn new branch at top
	int r = rand() % 5;

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}

}