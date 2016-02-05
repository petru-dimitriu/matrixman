#pragma once
#include "mmerror.h"
#include <iostream>
#include <cstring>
using namespace MatrixMan;

MMError::MMError()
{
    this->message = NULL;
}

MMError::MMError(char* message)
{
    this->message = new char[strlen(message)+1];
    strcpy(this->message,message);
    display();
}


MMError::~MMError()
{
    if (message)
        delete[] message;
}

void MMError::display()
{
    std::cout << "ERROR: " << message << "\n";
}
