// Timber.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int main(){
	//create video object
	VideoMode vm(1920, 1080);

	//create and open window
	RenderWindow window(vm, "Timber!!", Style::Fullscreen);

    return 0;
}