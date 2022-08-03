#include <vector>
#include <time.h>
#include <SDL2/SDL_mixer.h>
#include "headers/game.hpp"
#include "headers/menu.hpp"

// Constructor de la clase Game
Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1024;
    screenHeight = 600;
    f = 10;    // cantidad de filas
    c = 40;    // cantidad de columnas
    firstClick = false;
    gameState = GameState::PLAY;
};
Game::~Game() {};    // Destructor de la clase Game   

void Game::run() {
    init("Buscaminas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed to initialize. Error: " << SDL_GetError();
    };

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
     
    icon = IMG_Load("res/img/mina.png");
    SDL_SetWindowIcon(window, icon);

    //Inicio Audio
    SDL_Init(SDL_INIT_AUDIO);

    //Inicio SDL mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void Game::gameLoop() {

    //Se carga los archivos de audio
    Mix_Music* backgroundMusic = Mix_LoadMUS("res/audio/dbz.mp3");

    //Volumen de la musica
    Mix_VolumeMusic(20);

    //Se reproduce la musica de fondo
    Mix_PlayMusic(backgroundMusic, -1);

    SDL_Texture* minaTexture = loadTexture("res/img/mina-v2.png");
    SDL_Texture* casillaTexture = loadTexture("res/img/casilla.png");

    menu();

    //Creo la matriz de casillas
    for(int i=0; i<f; i++){
        std::vector<Entity> casilla;
        for(int j=0; j<c; j++){
            Entity casilla1(32 * j, 32 * i, casillaTexture); 
            casilla.push_back(casilla1);
        }
        casillas.push_back(casilla);
    }
    
    while (gameState != GameState::EXIT) {
        handleEvents();
        clear();
        
        for(int i = 0; i<f; i++){
            for(int j=0; j<c; j++){
                render(casillas[i][j]);
            }
        }
        display();
    }
}

SDL_Texture* Game::loadTexture(const char* filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);

    if(texture == NULL)
        std::cout << "Failed to load texture. Error: " << SDL_GetError();

    return texture;
}

void Game::cleanUp() {
    SDL_DestroyWindow(window);
    
}

/*void Game::cleanMusic(){
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
}
*/

void Game::clear() {
    SDL_RenderClear(renderer);
    
}

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

void Game::display() {
    SDL_RenderPresent(renderer);
}

Pos getClickPos() {
    Pos clickPos;
    Uint32 buttons;

    SDL_PumpEvents();  // make sure we have the latest mouse state

    buttons = SDL_GetMouseState(&clickPos.x, &clickPos.y);

    return clickPos;
}

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
                std::cout << "click" << std::endl;
                if(!firstClick){
                    onFirstClick();
                }
                numCasilla(clickPos);
            }
            if(evnt.button.button == SDL_BUTTON_RIGHT) {     // Click derecho 
                std::cout << "clock" << std::endl;
                bool bandera = false;
                SDL_Texture* casillaTexture = loadTexture("res/img/casilla.png");
                SDL_Texture* banderaTexture = loadTexture("res/img/minesweeper_banderilla.png");

                if(casillas[clickPos.y/32][clickPos.x/32].bandera == true){
                    casillas[clickPos.y/32][clickPos.x/32].tex = casillaTexture;
                    casillas[clickPos.y/32][clickPos.x/32].bandera = false;
                }
                else if(casillas[clickPos.y/32][clickPos.x/32].bandera == false){
                    casillas[clickPos.y/32][clickPos.x/32].tex = banderaTexture;
                    casillas[clickPos.y/32][clickPos.x/32].bandera = true;    
                }
            }
            break;
    }
}

void Game::onFirstClick(){
    Pos firstClickPos = getClickPos();

    firstClick = true;
    std::cout << "First Click" << std::endl;

    // std::cout << "FirstClickCursor at x: " << firstClickPos.x/32 << std::endl;  
    // std::cout << "FirstClickCursor at y: " << firstClickPos.y/32 << std::endl;

    bombasAleat(firstClickPos);
}

void Game::bombasAleat(Pos) {
    int b = 30;
    SDL_Texture* minaTexture = loadTexture("res/img/minesweeper_mina_blanca.png");
    srand(time(NULL));
    std::vector<int> bombPosX;
    std::vector<int> bombPosY;
    
    for(int i=0; i < b; ++i) {
        bombPosX.push_back(rand() % c);
        bombPosY.push_back(rand() % f);       
        if(casillas[bombPosY[i]][bombPosX[i]].bomb == false){
            casillas[bombPosY[i]][bombPosX[i]].bomb = true;
            casillas[bombPosY[i]][bombPosX[i]].tex = minaTexture;
        }
        else if(casillas[bombPosY[i]][bombPosX[i]].bomb == true){     
            bombPosX[i] = rand() % c;
            bombPosY[i] = rand() % f;
            //si es true que recalcule la posicion de la bomba
            while(casillas[bombPosY[i]][bombPosX[i]].bomb == true){
                bombPosX[i] = rand() % c;
                bombPosY[i] = rand() % f;
            }
            casillas[bombPosY[i]][bombPosX[i]].bomb = true;
            casillas[bombPosY[i]][bombPosX[i]].tex = minaTexture;
        }
        
        numero_casilla(bombPosX[i] - 1, bombPosY[i]);
        numero_casilla(bombPosX[i] + 1, bombPosY[i]);
        numero_casilla(bombPosX[i], bombPosY[i] - 1);
        numero_casilla(bombPosX[i], bombPosY[i] + 1);
        numero_casilla(bombPosX[i] - 1, bombPosY[i] - 1);
        numero_casilla(bombPosX[i] - 1, bombPosY[i] + 1);
        numero_casilla(bombPosX[i] + 1, bombPosY[i] - 1);
        numero_casilla(bombPosX[i] + 1, bombPosY[i] + 1);
    }

    for(int i=0; i < b; ++i) {
        std::cout << "Bomba " << i << std::endl << "X = " << bombPosX[i] << std::endl << "Y = " << bombPosY[i] << std::endl;
    }
}
void Game::numero_casilla(int x, int y){
    if(x >= 0 && x < c && y >= 0 && y < f && casillas[y][x].bomb != true) {
        casillas[y][x].cont++;
    }
}
// si las bombas son mas de 12 crashea (30) y si en numcasillas() llamo a dos numCasillas() tambien 
void Game::numCasilla(Pos clickPos){
    SDL_Texture* casillavaciaTexture = loadTexture("res/img/minesweeper_casilla.png");
    SDL_Texture* casilla1Texture = loadTexture("res/img/minesweeper1.png");
    SDL_Texture* casilla2Texture = loadTexture("res/img/minesweeper2.png");
    SDL_Texture* casilla3Texture = loadTexture("res/img/minesweeper3.png");

    switch(casillas[clickPos.y/32][clickPos.x/32].cont) {
        case 0:
            casillas[clickPos.y/32][clickPos.x/32].tex = casillavaciaTexture;
            if(clickPos.x/32 > 0 && clickPos.x/32 < c && clickPos.y/32 > 0 && clickPos.y/32 < f){
                Pos llamada;
                //Pos envio;
                llamada.x = clickPos.x;
                llamada.y = clickPos.y;
                numCasilla(llamada = {llamada.x + 32, llamada.y});
                //std::cout<<"ejecutado1"<<std::endl;
                //numCasilla(llamada = {llamada.x - 32, llamada.y});
                //std::cout<<"ejecutado2"<<std::endl;
                
                //std::cout<<"ejecutado2"<<std::endl;
                //numCasilla(llamada = {llamada.x, llamada.y + 32});
                //numCasilla(llamada = {llamada.x, llamada.y - 32});
                //numCasilla(llamada = {llamada.x + 32, llamada.y + 32});
                //numCasilla(llamada = {llamada.x + 32, llamada.y - 32});
                //numCasilla(llamada = {llamada.x - 32, llamada.y + 32});
                //numCasilla(llamada = {llamada.x - 32, llamada.y - 32});
            }
            break;
        case 1:
            casillas[clickPos.y/32][clickPos.x/32].tex = casilla1Texture;
            break;
        case 2:
            casillas[clickPos.y/32][clickPos.x/32].tex = casilla2Texture;
            break;
        case 3:
            casillas[clickPos.y/32][clickPos.x/32].tex = casilla3Texture;
            break;
        /*case 4:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum4;
            break;
        case 5:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum5;
            break;
        case 6:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum6;
            break;
        case 7:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum7;
            break;
        case 8:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum8;
            break;*/
        default:
            casillas[clickPos.y/32][clickPos.x/32].tex = casillavaciaTexture;
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