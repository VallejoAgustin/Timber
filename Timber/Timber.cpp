// Timber.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
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

	while (window.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//update scene

		//draw scene

		//clear from last frame
		window.clear();

		//draw scene background
		window.draw(spriteBackground);

		//display drawn for current frame
		window.display();
	}

    return 0;
}