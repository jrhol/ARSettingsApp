// ARSettingsApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>

#include <GL/freeglut.h>
#include "Headers/GUI.h"

int main(int argc, char** argv)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	//Init Glut 
	glutInit(&argc, argv);

	//Create the Settings Window 
	createSettingsWindow();

	return 0;
}

