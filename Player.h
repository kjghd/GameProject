#pragma once
#include "Character.h"

#include "Input.h"
#include "Camera.h"

class Player : public Character
{
public:
    Input* m_pInput;
    Camera m_camera;

    Player();

    virtual void Update(float deltaTime) override;
};

