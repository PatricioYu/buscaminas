#include <time.h>
#include "headers/game.hpp"


// Constructor de la clase Game
Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1024;
    screenHeight = 600;
    f = 10;              // cantidad de filas
    c = 40;              // cantidad de columnas
    firstClick = false;
    gameState = GameState::PLAY;
};
Game::~Game() {};    // Destructor de la clase Game   

// Inicializa la ventana y corre el gameloop
void Game::run() {
    init("Buscaminas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    gameLoop();
}

// Inicializa la ventana
void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cout << "Failed to initialize. Error: " << SDL_GetError();
    };

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
    icon = IMG_Load("res/img/mina.png");

    // Establece el icono de la ventana
    SDL_SetWindowIcon(window, icon);

    // Inicio SDL mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

// Ciclo del juego
void Game::gameLoop() {
    // cargo los audios
    Mix_Music* backgroundMusic = Mix_LoadMUS("res/audio/dbz.mp3");
    // cargo texturas
    SDL_Texture* mineTexture = loadTexture("res/img/mina-v2.png");
    SDL_Texture* boxTexture = loadTexture("res/img/casilla.png");

    Mix_VolumeMusic(20);    // Volumen del audio
    Mix_PlayMusic(backgroundMusic, -1);    // Reproduzco el audio     

    menu();

    // Creo la matriz de casillas
    for(int i = 0; i < f; i++) {
        std::vector<Entity> casilla;
        for(int j = 0; j < c; j++) {
            Entity casilla1(32 * j, 32 * i, boxTexture); 
            casilla.push_back(casilla1);
        }
        casillas.push_back(casilla);
    }
    
    while (gameState != GameState::EXIT) {
        handleEvents();
        clear();
        
        // Renderizo las casillas
        for(int i = 0; i < f; i++){
            for(int j = 0; j < c; j++){
                render(casillas[i][j]);
            }
        }

        display();
    }
}

// Carga las texturas
SDL_Texture* Game::loadTexture(const char* filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);

    if(texture == NULL) {
        std::cout << "Failed to load texture. Error: " << SDL_GetError();
    }

    return texture;
}

// Destruye la ventana
void Game::cleanUp() {
    SDL_DestroyWindow(window);
}

// Apaga el audio
/*void Game::cleanMusic(){
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
}
*/

// Limpia la ventana
void Game::clear() {
    SDL_RenderClear(renderer); 
}

// Renderiza la entidad
void Game::render(Entity& entity) {
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
void Game::display() {
    SDL_RenderPresent(renderer);
}

// Devuelve las coordenadas del click
Pos getClickPos() {
    Pos clickPos;
    Uint32 buttons;

    SDL_PumpEvents();  // reune todas las entradas pendientes y las deja en la cola de eventos

    buttons = SDL_GetMouseState(&clickPos.x, &clickPos.y);

    return clickPos;
}

// eventos
void Game::handleEvents() {
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch(evnt.type) {
        case SDL_QUIT:
            cleanUp();
            gameState = GameState::EXIT;

            break;
        case SDL_MOUSEBUTTONUP:
            Pos clickPos = getClickPos();

            // Sabiendo que las imagenes son de 32 pixeles simplemente tenemos que dividir x e y por 32 para que nos de una coordenada coherente
            std::cout << "Cursor at x: " << clickPos.x/32 << std::endl;  
            std::cout << "Cursor at y: " << clickPos.y/32 << std::endl;

            if(evnt.button.button == SDL_BUTTON_LEFT) {      // Click izquierdo
                // std::cout << "click" << std::endl;
                if(!firstClick){
                    onFirstClick();
                }
                numCasilla(clickPos.x / 32, clickPos.y / 32);
            }
            else if(evnt.button.button == SDL_BUTTON_RIGHT) {     // Click derecho 
                // std::cout << "click derecho" << std::endl;
                SDL_Texture* boxTexture = loadTexture("res/img/casilla.png");
                SDL_Texture* flagTexture = loadTexture("res/img/minesweeper_banderilla.png");

                // Si la casilla clickeada tiene la textura de una bandera cambiarla a la textura de casilla sin revelar
                if(casillas[clickPos.y/32][clickPos.x/32].flag == true) {
                    casillas[clickPos.y/32][clickPos.x/32].tex = boxTexture;
                    casillas[clickPos.y/32][clickPos.x/32].flag = false;
                }
                // sino, cambiar el valor a bandera
                else if(casillas[clickPos.y/32][clickPos.x/32].flag == false) {
                    casillas[clickPos.y/32][clickPos.x/32].tex = flagTexture;
                    casillas[clickPos.y/32][clickPos.x/32].flag = true;    
                }
            }
            break;
    }
}

// Si es el primer click se activa el posicionamiento de las minas de forma aleatoria
void Game::onFirstClick() {
    Pos firstClickPos = getClickPos();

    firstClick = true;
    std::cout << "First Click" << std::endl;

    // std::cout << "FirstClick x: " << firstClickPos.x/32 << std::endl;  
    // std::cout << "FirstClick y: " << firstClickPos.y/32 << std::endl;

    bombasAleat(firstClickPos);
}

// Se posicionan las minas de forma aleatoria y generando una pileta en las coordenadas del primer click
void Game::bombasAleat(Pos) {
    int b = 30;
    SDL_Texture* mineTexture = loadTexture("res/img/minesweeper_mina_blanca.png");
    srand(time(NULL));
    std::vector<int> mineX;
    std::vector<int> mineY;
    
    for(int i = 0; i < b; ++i) {
        mineX.push_back(rand() % c);
        mineY.push_back(rand() % f);

        if(casillas[mineY[i]][mineX[i]].mine == false) {
            casillas[mineY[i]][mineX[i]].mine = true;
            casillas[mineY[i]][mineX[i]].tex = mineTexture;
        }
        else if(casillas[mineY[i]][mineX[i]].mine == true) {     
            mineX[i] = rand() % c;
            mineY[i] = rand() % f;
            //si es true que recalcule la posicion de la bomba
            while(casillas[mineY[i]][mineX[i]].mine == true) {
                mineX[i] = rand() % c;
                mineY[i] = rand() % f;
            }
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

    for(int i=0; i < b; ++i) {
        std::cout << "Bomba " << i << std::endl << "X = " << mineX[i] << std::endl << "Y = " << mineY[i] << std::endl;
    }
}

// Suma al contador de la casilla pasada por las coordenadas (parametros) mientras no sea una bomba
void Game::contMasUno(int x, int y) {
    if(x >= 0 && x < c && y >= 0 && y < f && casillas[y][x].mine != true) {
        casillas[y][x].cont++;
    }
}

void Game::numCasilla(int clickx, int clicky){
    SDL_Texture* casillavaciaTexture = loadTexture("res/img/minesweeper_casilla.png");
    SDL_Texture* casilla1Texture = loadTexture("res/img/minesweeper1.png");
    SDL_Texture* casilla2Texture = loadTexture("res/img/minesweeper2.png");
    SDL_Texture* casilla3Texture = loadTexture("res/img/minesweeper3.png");

    switch(casillas[clicky][clickx].cont) {
        case 0:
            casillas[clicky][clickx].tex = casillavaciaTexture;
            casillas[clicky][clickx].reveal = true;
            if(clickx > 0 && clickx < c && clicky > 0 && clicky < f && casillas[clicky][clickx].mine == false){
                if(casillas[clicky - 1][clickx].reveal == false && (clicky - 1) > 0){
                    numCasilla(clickx, clicky - 1);
                }
                
                if(casillas[clicky + 1][clickx].reveal == false && (clicky + 1) < f){
                    numCasilla(clickx, clicky + 1);
                }
                
                if(casillas[clicky - 1][clickx - 1].reveal == false && (clickx - 1) > 0 && (clicky - 1) > 0 ){
                    numCasilla(clickx - 1, clicky - 1);
                }

                if(casillas[clicky - 1][clickx + 1].reveal == false && (clickx + 1) < c && (clicky - 1) > 0){
                    numCasilla(clickx + 1, clicky - 1);
                }
                
                if(casillas[clicky + 1][clickx + 1].reveal == false && (clickx + 1) < c && (clicky + 1) < f){
                    numCasilla(clickx + 1, clicky + 1);
                }
                
                if(casillas[clicky + 1][clickx - 1].reveal == false && (clickx - 1) > 0 && (clicky + 1) < f){
                    numCasilla(clickx - 1, clicky + 1);
                }
                
                if(casillas[clicky][clickx - 1].reveal == false && (clickx - 1) > 0){
                    numCasilla(clickx - 1, clicky);
                }

                if(casillas[clicky][clickx + 1].reveal == false && (clickx + 1) < c){
                    numCasilla(clickx + 1, clicky);
                }
            }
            break;
        case 1:
            casillas[clicky][clickx].tex = casilla1Texture;
            casillas[clicky][clickx].reveal = true;
            break;
        case 2:
            casillas[clicky][clickx].tex = casilla2Texture;
            casillas[clicky][clickx].reveal = true;
            break;
        case 3:
            casillas[clicky][clickx].tex = casilla3Texture;
            casillas[clicky][clickx].reveal = true;
            break;
        /*case 4:
            casillas[clicky][clickx].tex = texturaNum4;
            casillas[clicky][clickx].reveal = true;
            break;
        case 5:
            casillas[clicky][clickx].tex = texturaNum5;
            casillas[clicky][clickx].reveal = true;
            break;
        case 6:
            casillas[clicky][clickx].tex = texturaNum6;
            casillas[clicky][clickx].reveal = true;
            break;
        case 7:
            casillas[clicky][clickx].tex = texturaNum7;
            casillas[clicky][clickx].reveal = true;
            break;
        case 8:
            casillas[clicky][clickx].tex = texturaNum8;
            casillas[clicky][clickx].reveal = true;
            break;*/
        default:
            casillas[clicky][clickx].tex = casillavaciaTexture;
            break;
    }
    
}


void Game::menu() {
    SDL_Texture* menuTexture = loadTexture("res/img/menu.png");
    EntityMenu menu(0, 0, menuTexture);

    while(finMenu == 0 && gameState != GameState::EXIT){
        menuHandleEvents();
        clear();
        renderMenu(menu);
        display();
    }
}

void Game::menuHandleEvents() {
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch(evnt.type) {
        case SDL_QUIT:
            cleanUp();
            gameState = GameState::EXIT;

            break;
        case SDL_MOUSEBUTTONUP:
            Pos clickPos = getClickPos();

            std::cout << "Cursor at x: " << clickPos.x << std::endl;  
            std::cout << "Cursor at y: " << clickPos.y << std::endl;

            if(evnt.button.button == SDL_BUTTON_LEFT) {      // Click izquierdo
                std::cout << "click" << std::endl;
                dificultad(clickPos);
            }
            break;
    }
}

void Game::dificultad(Pos clickPos) {
    if(clickPos.x >= 520 && clickPos.y >= 220 && clickPos.x <= 700 && clickPos.y <= 260){
        std::cout << "Facil" << std::endl;
        finMenu = 1;
    }
    if(clickPos.x >= 520 && clickPos.y >= 265 && clickPos.x <= 700 && clickPos.y <= 297){
        std::cout << "Normal" << std::endl;
        finMenu = 1;
    }
    if(clickPos.x >= 520 && clickPos.y >= 300 && clickPos.x <= 700 && clickPos.y <= 333){
        std::cout << "Dificil" << std::endl;
        finMenu = 1;
    }
    if(clickPos.x >= 520 && clickPos.y >= 340 && clickPos.x <= 700 && clickPos.y <= 375){
        std::cout << "Personalizado" << std::endl;
        finMenu = 1;
    }
}  

void Game::renderMenu(EntityMenu& entityMenu) {
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