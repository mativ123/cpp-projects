#pragma once

class Ball
{
    float pi { 3.14159265359 };

    public:
        SDL_Rect rect;
        int ballRot { 20 };
        bool movingLeft { false };
        int speed { 250 };

        void logic(SDL_Renderer *rendere, float deltaTime, int windowW, int windowH)
        {
            if(!movingLeft && rect.x > windowW - rect.w)
            {
                movingLeft = true;
                ballRot = 360 - ballRot;
            }
            else if(movingLeft && rect.x < 0)
            {
                movingLeft = false;
                ballRot = 360 - ballRot;
            }

            if(rect.y > windowH - rect.h)
                ballRot = 360 - ballRot;
            else if(rect.y < 0)
                ballRot = 360 - ballRot;

            if(!movingLeft)
            {
                rect.x += std::round(speed * deltaTime * cos(ballRot*pi/180));
                rect.y += std::round(speed * deltaTime * sin(ballRot*pi/180));
            }
            else
            {
                rect.x -= std::round(speed * deltaTime * cos(ballRot*pi/180));
                rect.y -= std::round(speed * deltaTime * sin(ballRot*pi/180));
            }

            SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
            SDL_RenderFillRect(rendere, &rect);
            SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
        }
};

class Player
{
    bool movingUp { false };
    bool movingDown { false } ;
    public:
        SDL_Rect rect;
        int speed { 300 } ;

        void logic(SDL_Renderer *rendere, float deltaTime, int windowW, int windowH)
        {
            if(movingDown && rect.y < windowH - rect.h)
                rect.y += speed * deltaTime;
            else if(movingUp && rect.y > 0)
                rect.y -= speed * deltaTime;

            SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
            SDL_RenderFillRect(rendere, &rect);
            SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
        }

        void arrowDown()
        {
            movingDown = true;
        }

        void arrowUp()
        {
            movingUp = true;
        }

        void letGo()
        {
            movingUp = movingDown = false;
        }
};
