#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

const int tileWidth = 64;
const int tileHeight = 32;

//////////////////////////////////////////////////////////////

struct Vector2 {
    float x,y;
};

//////////////////////////////////////////////////////////////

enum TowerType
{
    FIRE,
    ICE,
    LIGHTNING,
    WIND,
    LIGHT
};

//////////////////////////////////////////////////////////////

int getTowerCost(TowerType type)
{
    switch(type)
    {
        case FIRE: return 100;
        case ICE: return 120;
        case LIGHTNING: return 90;
        case WIND: return 150;
        case LIGHT: return 70;
    }
    return 100;
}

//////////////////////////////////////////////////////////////

enum TileType
{
    EMPTY,
    PATH,
    OCCUPIED
};

struct Tile
{
    TileType type = EMPTY;
    bool buildable = true;
};

Tile mapGrid[MAP_WIDTH][MAP_HEIGHT];

//////////////////////////////////////////////////////////////

class Tower
{
private:

    int gridX,gridY;
    Vector2 worldPos;
    float rotation;

public:

    TowerType type;
    int cost;

    Tower(int gx,int gy,TowerType t)
    {
        type = t;
        cost = getTowerCost(t);

        setGridPosition(gx,gy);

        rotation = 0;
    }

    void setGridPosition(int gx,int gy)
    {
        gridX = gx;
        gridY = gy;

        worldPos.x = (gx - gy) * tileWidth / 2.0f;
        worldPos.y = (gx + gy) * tileHeight / 2.0f;
    }

    bool isClicked(Vector2 mousePos)
    {
        float dx = mousePos.x - worldPos.x;
        float dy = mousePos.y - worldPos.y;

        float dist = sqrt(dx*dx + dy*dy);

        return dist < 40;
    }

    int getGridX(){ return gridX; }
    int getGridY(){ return gridY; }

    int getSellPrice()
    {
        return cost / 2;
    }

    void render()
    {
        std::cout<<"Tower ("<<gridX<<","<<gridY<<") Type "<<type<<"\n";
    }
};

//////////////////////////////////////////////////////////////

std::vector<Tower*> towers;

int playerMoney = 300;

bool buyingTower = false;

TowerType selectedTower;

Uint32 lastClickTime = 0;

//////////////////////////////////////////////////////////////

Vector2 screenToGrid(float screenX,float screenY)
{
    float gx = (screenY/tileHeight + screenX/tileWidth);
    float gy = (screenY/tileHeight - screenX/tileWidth);

    return { floor(gx), floor(gy) };
}

//////////////////////////////////////////////////////////////

bool canPlace(int x,int y)
{
    if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)
        return false;

    if(mapGrid[x][y].type != EMPTY)
        return false;

    if(!mapGrid[x][y].buildable)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////

void buyTower(TowerType type)
{
    int cost = getTowerCost(type);

    if(playerMoney < cost)
    {
        std::cout<<"Not enough money\n";
        return;
    }

    selectedTower = type;

    buyingTower = true;

    std::cout<<"Selected tower cost "<<cost<<"\n";
}

//////////////////////////////////////////////////////////////

void placeTower(int x,int y)
{
    if(!canPlace(x,y))
    {
        std::cout<<"Cannot place here\n";
        return;
    }

    int cost = getTowerCost(selectedTower);

    if(playerMoney < cost)
    {
        std::cout<<"Not enough money\n";
        return;
    }

    Tower* tower = new Tower(x,y,selectedTower);

    towers.push_back(tower);

    mapGrid[x][y].type = OCCUPIED;

    playerMoney -= cost;

    buyingTower = false;

    std::cout<<"Tower placed. Money "<<playerMoney<<"\n";
}

//////////////////////////////////////////////////////////////

void sellTower(int index)
{
    Tower* tower = towers[index];

    int sellPrice = tower->getSellPrice();

    int x = tower->getGridX();
    int y = tower->getGridY();

    mapGrid[x][y].type = EMPTY;

    playerMoney += sellPrice;

    delete tower;

    towers.erase(towers.begin()+index);

    std::cout<<"Tower sold +"<<sellPrice<<" Money "<<playerMoney<<"\n";
}

//////////////////////////////////////////////////////////////

int main(int argc,char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Tower Defense",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    SDL_Event event;

    bool running = true;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;

            ////////////////////////////////////////////////////

            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_1)
                    buyTower(FIRE);

                if(event.key.keysym.sym == SDLK_2)
                    buyTower(ICE);

                if(event.key.keysym.sym == SDLK_3)
                    buyTower(LIGHTNING);

                if(event.key.keysym.sym == SDLK_4)
                    buyTower(WIND);

                if(event.key.keysym.sym == SDLK_5)
                    buyTower(LIGHT);
            }

            ////////////////////////////////////////////////////

            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                float mouseX = event.button.x;
                float mouseY = event.button.y;

                Vector2 mousePos = {mouseX,mouseY};

                Uint32 currentTime = SDL_GetTicks();

                for(int i=0;i<towers.size();i++)
                {
                    if(towers[i]->isClicked(mousePos))
                    {
                        if(currentTime - lastClickTime < 300)
                        {
                            sellTower(i);
                            break;
                        }

                        lastClickTime = currentTime;
                    }
                }

                if(buyingTower)
                {
                    Vector2 grid = screenToGrid(mouseX,mouseY);

                    placeTower((int)grid.x,(int)grid.y);
                }
            }
        }

        for(auto tower : towers)
            tower->render();
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}