// Timber.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

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

	//set a sprite object and position
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
	Sprite spriteCloud1, spriteCloud2, spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	//position clouds 'off-screen'
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	bool cloud1Active = false, cloud2Active = false, cloud3Active = false;
	float cloud1Speed = 0.0, cloud2Speed = 0.0, cloud3Speed = 0.0;//not using 0.0f

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

	while (window.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//start game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			//reset time and score
			score = 0;
			timeRemaining = 5;
		}

		if(!paused){
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

			//TODO: create function that takes in object, modifier (for rand), and modifies accordingly
			if (!cloud1Active) {
				srand((int)time(0) * 10);
				cloud1Speed = rand() % 200;

				srand((int)time(0) * 10);
				float height = rand() % 150;
				spriteCloud1.setPosition(-300, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

				if (spriteCloud1.getPosition().x > 1920) {
					cloud1Active = false;
				}
			}

			if (!cloud2Active) {
				srand((int)time(0) * 20);
				cloud2Speed = rand() % 200;

				srand((int)time(0) * 20);
				float height = rand() % 150;
				spriteCloud2.setPosition(-300, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

				if (spriteCloud2.getPosition().x > 1920) {
					cloud2Active = false;
				}
			}

			if (!cloud3Active) {
				srand((int)time(0) * 30);
				cloud3Speed = rand() % 200;

				srand((int)time(0) * 30);
				float height = rand() % 150;
				spriteCloud3.setPosition(-300, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

				if (spriteCloud3.getPosition().x > 1920) {
					cloud3Active = false;
				}
			}

			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());//TODO: just simple string concatenation? s + score
		}

		//draw scene

		//clear from last frame
		window.clear();

		//draw scene background
		window.draw(spriteBackground);
		//clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		//tree
		window.draw(spriteTree);
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

    return 0;
}