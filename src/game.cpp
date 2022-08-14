#include <time.h>
#include "headers/game.hpp"

// Constructor de la clase Game
Game::Game()
{
    window = nullptr;
    renderer = nullptr;
    screenWidth = 800;
    screenHeight = 800;
    firstClick = false;
    gameState = GameState::PLAY;
}
Game::~Game(){}; // Destructor de la clase Game

// Inicializa la ventana y corre el gameloop
void Game::run()
{
    init("Buscaminas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    gameLoop();
}

// Inicializa la ventana
void Game::init(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        std::cout << "Failed to initialize. Error: " << SDL_GetError();
    };

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    icon = IMG_Load("res/img/inGame/MinaTerrestre.png");

    // Establece el ícono de la ventana
    SDL_SetWindowIcon(window, icon);

    // Inicio SDL mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

// Ciclo del juego
void Game::gameLoop()
{
    // cargo los audios
    //Mix_Music *backgroundMusic = Mix_LoadMUS("res/audio/dbz.mp3");
    Mix_Music *backgroundMusic = Mix_LoadMUS("res/audio/The_Shire.mp3");
    // cargo texturas
    SDL_Texture *mineTexture = loadTexture("res/img/inGame/mina-v2.png");
    SDL_Texture *boxTexture = loadTexture("res/img/inGame/Grass.png");

    Mix_VolumeMusic(music ? 20 : 0);    // Volumen del audio
    Mix_PlayMusic(backgroundMusic, -1); // Reproduzco el audio

    menu();

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);  // centro la ventana luego de la eleccion de dificultad

    // Creo la matriz de casillas
    for (int i = 0; i < f; i++)
    {
        std::vector<Entity> casilla;
        for (int j = 0; j < c; j++)
        {
            Entity casilla1(32 * j, 32 * i, boxTexture);
            casilla.push_back(casilla1);
        }
        casillas.push_back(casilla);
    }

    total = (f * c) - b;

    while (gameState != GameState::EXIT)
    {
        handleEvents();
        clear();

        // Renderizo las casillas
        for (int i = 0; i < f; i++)
        {
            for (int j = 0; j < c; j++)
            {
                render(casillas[i][j]);
            }
        }

        display();
    }
}

// Carga las texturas
SDL_Texture *Game::loadTexture(const char *filePath)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);

    if (texture == NULL)
    {
        std::cout << "Failed to load texture. Error: " << SDL_GetError();
    }

    return texture;
}

// Destruye la ventana
void Game::cleanUp()
{
    SDL_DestroyWindow(window);
}

// Apaga el audio
/*void Game::cleanMusic(){
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
}
*/

// Limpia la ventana
void Game::clear()
{
    SDL_RenderClear(renderer);
}

// Renderiza la entidad
void Game::render(Entity &entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entity.getX();
    dst.y = entity.getY();
    dst.w = entity.getCurrentFrame().w;
    dst.h = entity.getCurrentFrame().h;

    SDL_RenderCopy(renderer, entity.getTex(), &src, &dst);
}

// Muestra en la ventana lo que se renderizo
void Game::display()
{
    SDL_RenderPresent(renderer);
}

// Devuelve las coordenadas del click
Pos getClickPos()
{
    Pos clickPos;
    Uint32 buttons;

    SDL_PumpEvents(); // Reúne todas las entradas pendientes y las deja en la cola de eventos

    buttons = SDL_GetMouseState(&clickPos.x, &clickPos.y);

    return clickPos;
}

// Eventos
void Game::handleEvents()
{
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch (evnt.type)
    {
    case SDL_QUIT:
        cleanUp();
        gameState = GameState::EXIT;

        break;
    case SDL_MOUSEBUTTONUP:
        Pos clickPos = getClickPos();

        // Sabiendo que las imágenes son de 32 pixeles simplemente tenemos que dividir x e y por 32 para que nos de una coordenada coherente
        std::cout << "Cursor at x: " << clickPos.x / 32 << std::endl;
        std::cout << "Cursor at y: " << clickPos.y / 32 << std::endl;

        if (evnt.button.button == SDL_BUTTON_LEFT)
        { // Click izquierdo
            // std::cout << "click" << std::endl;
            if (clickPos.x / 32 >= 0 && clickPos.x / 32 < c && clickPos.y / 32 >= 0 && clickPos.y / 32 < f && casillas[clickPos.y / 32][clickPos.x / 32].flag == false)
            {
                if (!firstClick)
                {
                    onFirstClick();
                }
                else if (casillas[clickPos.y / 32][clickPos.x / 32].mine == true)
                {
                    std::cout << "Perdiste" << std::endl;
                    cleanUp();
                    gameState = GameState::EXIT;
                }
                numCasilla(clickPos.x / 32, clickPos.y / 32);
            }
        }
        else if (evnt.button.button == SDL_BUTTON_RIGHT)
        { // Click derecho
            // std::cout << "click derecho" << std::endl;
            SDL_Texture *boxTexture = loadTexture("res/img/inGame/stepedGrass.png");
            SDL_Texture *flagTexture = loadTexture("res/img/inGame/Flag.png");
            Mix_Chunk* banderaeffect = Mix_LoadWAV("res/audio/pop.mp3");

            // Si la casilla clickeada tiene la textura de una bandera cambiarla a la textura de casilla sin revelar
            if (clickPos.x / 32 >= 0 && clickPos.x / 32 < c && clickPos.y / 32 >= 0 && clickPos.y / 32 < f)
            {
                if (!firstClick)
                {
                    firstClick = true;
                    Pos firstClickPos = {firstClickPos.x = rand() % c, firstClickPos.y = rand() % f};
                    bombasAleat(firstClickPos);
                }
                if (casillas[clickPos.y / 32][clickPos.x / 32].flag == true && casillas[clickPos.y / 32][clickPos.x / 32].revealed == false)
                {
                    casillas[clickPos.y / 32][clickPos.x / 32].tex = boxTexture;
                    casillas[clickPos.y / 32][clickPos.x / 32].flag = false;
                }
                // sino, cambiar el valor a bandera
                else if (casillas[clickPos.y / 32][clickPos.x / 32].flag == false && casillas[clickPos.y / 32][clickPos.x / 32].revealed == false)
                {
                    Mix_PlayChannel(-1,banderaeffect,0);
                    casillas[clickPos.y / 32][clickPos.x / 32].tex = flagTexture;
                    casillas[clickPos.y / 32][clickPos.x / 32].flag = true;
                }
            }
        }
        break;
    }
}

// Si es el primer click se activa el posicionamiento de las minas de forma aleatoria
void Game::onFirstClick()
{
    Pos firstClickPos = getClickPos();

    firstClick = true;
    std::cout << "First Click" << std::endl;

    // std::cout << "FirstClick x: " << (firstClickPos.x/32)/32 << std::endl;
    // std::cout << "FirstClick y: " << (firstClickPos.y/32)/32 << std::endl;

    bombasAleat(firstClickPos);
}

// Se posicionan las minas de forma aleatoria y generando una pileta en las coordenadas del primer click
void Game::bombasAleat(const Pos &firstClickPos)
{
    SDL_Texture *mineTexture = loadTexture("res/img/inGame/MinaTerrestre.png");
    srand(time(NULL));
    std::vector<int> mineX;
    std::vector<int> mineY;

    for (int i = 0; i < b; ++i)
    {
        mineX.push_back(rand() % c);
        mineY.push_back(rand() % f);
        
        if (casillas[mineY[i]][mineX[i]].mine == true || (mineX[i] == firstClickPos.x / 32 && mineY[i] == firstClickPos.y / 32) || mineX[i] == firstClickPos.x / 32 + 1 || mineX[i] == firstClickPos.x / 32 - 1 || mineY[i] == firstClickPos.y / 32 + 1 || mineY[i] == firstClickPos.y / 32 - 1)
        {
            // Mientras la mina a crear ya existe se recalcula la posicion de la bomba
            do
            {
                mineX[i] = rand() % c;
                mineY[i] = rand() % f;
            } while (casillas[mineY[i]][mineX[i]].mine == true || (mineX[i] == firstClickPos.x / 32 && mineY[i] == firstClickPos.y / 32) || mineX[i] == firstClickPos.x / 32 + 1 || mineX[i] == firstClickPos.x / 32 - 1 || mineY[i] == firstClickPos.y / 32 + 1 || mineY[i] == firstClickPos.y / 32 - 1);

            casillas[mineY[i]][mineX[i]].mine = true;
            casillas[mineY[i]][mineX[i]].tex = mineTexture;
        }
        else 
        {
            casillas[mineY[i]][mineX[i]].mine = true;
            casillas[mineY[i]][mineX[i]].tex = mineTexture;
        }

        // Cada bomba suma 1 al contador de las casillas adyacentes
        contMasUno(mineX[i] - 1, mineY[i]);
        contMasUno(mineX[i] + 1, mineY[i]);
        contMasUno(mineX[i], mineY[i] - 1);
        contMasUno(mineX[i], mineY[i] + 1);
        contMasUno(mineX[i] - 1, mineY[i] - 1);
        contMasUno(mineX[i] - 1, mineY[i] + 1);
        contMasUno(mineX[i] + 1, mineY[i] - 1);
        contMasUno(mineX[i] + 1, mineY[i] + 1);
    }

    for (int i = 0; i < b; ++i)
    {
        std::cout << "Bomba " << i << std::endl << "X = " << mineX[i] << std::endl << "Y = " << mineY[i] << std::endl;
    }
}

// Suma al contador de la casilla pasada por las coordenadas (parámetros) mientras no sea una bomba
void Game::contMasUno(int x, int y)
{
    if (x >= 0 && x < c && y >= 0 && y < f && casillas[y][x].mine != true)
    {
        casillas[y][x].cont++;
    }
}

// Selecciona el número que tiene la casilla según la cantidad de bombas que hay alrededor
void Game::numCasilla(int clickX, int clickY)
{
    SDL_Texture *emptyBoxTexture = loadTexture("res/img/inGame/Soil.png");
    SDL_Texture *boxOneTexture = loadTexture("res/img/inGame/Uno.png");
    SDL_Texture *boxTwoTexture = loadTexture("res/img/inGame/Dos.png");
    SDL_Texture *boxThreeTexture = loadTexture("res/img/inGame/Tres.png");

    switch (casillas[clickY][clickX].cont)
    {
    case 0:
        casillas[clickY][clickX].tex = emptyBoxTexture;
        casillas[clickY][clickX].revealed = true;

        // Si es 0 hay que revelar las casillas que estan alrededor llamando de nuevo a esta funcion revelando los puntos cardinales
        // Primero verifica que la casilla a revelar existe declarada en el tablero
        if ((clickY - 1) >= 0)
        {
            // Despues que la casilla a revelar no esté revelada
            if (casillas[clickY - 1][clickX].revealed == false)
            {
                numCasilla(clickX, clickY - 1); // Norte
            }
        }

        if ((clickY + 1) < f)
        {
            if (casillas[clickY + 1][clickX].revealed == false)
            {
                numCasilla(clickX, clickY + 1); // Sur
            }
        }

        if ((clickX - 1) > 0 && (clickY - 1) > 0)
        {
            if (casillas[clickY - 1][clickX - 1].revealed == false)
            {
                numCasilla(clickX - 1, clickY - 1); // Noroeste
            }
        }

        if ((clickX + 1) < c && (clickY - 1) > 0)
        {
            if (casillas[clickY - 1][clickX + 1].revealed == false)
            {
                numCasilla(clickX + 1, clickY - 1); // Noreste
            }
        }

        if ((clickX + 1) < c && (clickY + 1) < f)
        {
            if (casillas[clickY + 1][clickX + 1].revealed == false)
            {
                numCasilla(clickX + 1, clickY + 1); // Sureste
            }
        }

        if ((clickX - 1) > 0 && (clickY + 1) < f)
        {
            if (casillas[clickY + 1][clickX - 1].revealed == false)
            {
                numCasilla(clickX - 1, clickY + 1); // Suroeste
            }
        }

        if ((clickX - 1) >= 0)
        {
            if (casillas[clickY][clickX - 1].revealed == false)
            {
                numCasilla(clickX - 1, clickY); // Oeste
            }
        }

        if ((clickX + 1) < c)
        {
            if (casillas[clickY][clickX + 1].revealed == false)
            {
                numCasilla(clickX + 1, clickY); // Este
            }
        }

        break;
    case 1:
        casillas[clickY][clickX].tex = boxOneTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    case 2:
        casillas[clickY][clickX].tex = boxTwoTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    case 3:
        casillas[clickY][clickX].tex = boxThreeTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    /*case 4:
        casillas[clickY][clickX].tex = boxFourTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    case 5:
        casillas[clickY][clickX].tex = boxFiveTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    case 6:
        casillas[clickY][clickX].tex = boxSixTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    case 7:
        casillas[clickY][clickX].tex = boxSevenTexture;
        casillas[clickY][clickX].revealed = true;
        break;
    case 8:
        casillas[clickY][clickX].tex = boxEightTexture;
        casillas[clickY][clickX].revealed = true;
        break;*/
    default:
        casillas[clickY][clickX].tex = emptyBoxTexture;
        break;
    }
}


void Game::menu()
{
    SDL_Texture *menuTexture = loadTexture("res/img/Menu/menu.png");
    EntityMenu menu(0, 0, menuTexture);

    // Carga de texturas de los botones
    // Facil
    SDL_Texture *buttonFTexture = loadTexture("res/img/Menu/FacilN.png");
    EntityButton buttonF(screenWidth/2 - buttonWidth/2, screenHeight/2, buttonFTexture);
    // Medio
    SDL_Texture *buttonMTexture = loadTexture("res/img/Menu/MedioN.png");
    EntityButton buttonM(screenWidth/2 - buttonWidth/2, screenHeight/2 + buttonHeight + 1, buttonMTexture);
    // Dificil
    SDL_Texture *buttonDTexture = loadTexture("res/img/Menu/DificilN.png");
    EntityButton buttonD(screenWidth/2 - buttonWidth/2, screenHeight/2 + buttonHeight * 2 + 1, buttonDTexture);

    while (finMenu == 0 && gameState != GameState::EXIT)
    {
        menuHandleEvents();
        clear();
        renderMenu(menu);
        renderButton(buttonF);
        renderButton(buttonM);
        renderButton(buttonD);

        display();
    }
}

void Game::menuHandleEvents()
{
    SDL_Event evnt;
    SDL_PollEvent(&evnt);
    int selectDif;
    Mix_Chunk* clickeffect = Mix_LoadWAV("res/audio/click.mp3");

    switch (evnt.type)
    {
    case SDL_QUIT:
        cleanUp();
        gameState = GameState::EXIT;

        break;
    case SDL_MOUSEBUTTONUP:
        Pos clickPos = getClickPos();

        std::cout << "Cursor at x: " << clickPos.x << std::endl;
        std::cout << "Cursor at y: " << clickPos.y << std::endl;

        if (evnt.button.button == SDL_BUTTON_LEFT)
        { // Click izquierdo
            std::cout << "click" << std::endl;

            /* dificultad(eligeDif); */
        }
        if ((clickPos.y >= screenHeight/2 - buttonHeight) && (clickPos.y <= screenHeight/2 + buttonHeight) && (clickPos.x >= screenWidth/2 - buttonWidth/2) && (clickPos.x <= screenWidth/2 + buttonWidth/2))
        {
            Mix_PlayChannel(-1,clickeffect,0);
            selectDif = 1;
            dificultad(selectDif);
        }
        // Boton "Medio"
        if ((clickPos.y >= screenHeight/2 + buttonHeight + 1) && (clickPos.y <= screenHeight/2 + buttonHeight*2) && (clickPos.x >= screenWidth/2 - buttonWidth/2) && (clickPos.x <= screenWidth/2 + buttonWidth/2))
        {
            Mix_PlayChannel(-1,clickeffect,0);
            selectDif = 2;
            dificultad(selectDif);
        }
        // Boton "Dificil"
        if ((clickPos.y >= screenHeight/2 + buttonHeight*2 + 1) && (clickPos.y <= screenHeight/2 + buttonHeight*3) && (clickPos.x >= screenWidth/2 - buttonWidth/2) && (clickPos.x <= screenWidth/2 + buttonWidth/2))
        {
            Mix_PlayChannel(-1,clickeffect,0);
            selectDif = 3;
            dificultad(selectDif);
        }
        // Boton Presonalizado
        /*  
        if (clickPos.y >= screenHeight/2 + buttonHeight*5 + 1 && clickPos.y <= screenHeight/2 + buttonHeight*7 && clickPos.x >= screenWidth/2 - buttonWidth && clickPos.x <= screenWidth/2 + buttonWidth)
        {
            std::cout << "PERSONALIZADO" << std::endl;
            eligeDif = 4;
            dificultad(eligeDif);
        } 
        */
        if (clickPos.x >= 0 && clickPos.x <= 30 && clickPos.y >= 0 && clickPos.y <= 30 && music==true)
        {
            Mix_VolumeMusic(0);
            music = false;
            std::cout << "Music off " << std::endl;
        }
        break;
    }
}

void Game::dificultad(int selectDif)
{
    switch (selectDif)
    {
    case 1:
        // std::cout << "Case = " <<selectDif << std::endl;
        b = 10;
        f = 8;
        c = 8;
        finMenu = 1;
        break;
    case 2:
        // std::cout << "Case = " <<selectDif << std::endl;
        b = 40;
        f = 16;
        c = 16;
        finMenu = 1;
        break;
    case 3:
        // std::cout << "Case = " << selectDif << std::endl;
        b = 99;
        f = 16;
        c = 30;
        finMenu = 1;
        break;
    /*
    case 4:
        std::cout << "Case = " <<selectDif << std::endl;
        finMenu = 1;
        break;
    */
    }
    SDL_SetWindowSize(window, c*32, f*32);
}

void Game::renderMenu(EntityMenu &entityMenu)
{
    SDL_Rect src;
    src.x = entityMenu.getCurrentFrame().x;
    src.y = entityMenu.getCurrentFrame().y;
    src.w = entityMenu.getCurrentFrame().w;
    src.h = entityMenu.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entityMenu.getX();
    dst.y = entityMenu.getY();
    dst.w = entityMenu.getCurrentFrame().w;
    dst.h = entityMenu.getCurrentFrame().h;

    SDL_RenderCopy(renderer, entityMenu.getTex(), &src, &dst);
}
void Game::renderButton(EntityButton &entityButton)
{
    SDL_Rect src;
    src.x = entityButton.getCurrentFrame().x;
    src.y = entityButton.getCurrentFrame().y;
    src.w = entityButton.getCurrentFrame().w;
    src.h = entityButton.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entityButton.getX();
    dst.y = entityButton.getY();
    dst.w = entityButton.getCurrentFrame().w;
    dst.h = entityButton.getCurrentFrame().h;

    SDL_RenderCopy(renderer, entityButton.getTex(), &src, &dst);
}