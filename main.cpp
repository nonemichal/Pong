#include <iostream>
#include <iomanip>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <sstream>
#include <climits>
#include "defines.h"
#include "classes/Rec.h"
#include "classes/Ball.h"
#include "classes/Player.h"
#include "classes/LTexture.h"
#include "classes/LTimer.h"


bool pause_flag = true;

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The surface contained by the window
SDL_Renderer* gRenderer = nullptr;

//Globally used font
TTF_Font* fps_font;
TTF_Font* points_font;

//Scene textures
LTexture gFPSTextTexture = LTexture();
LTexture g_point1_text_texture = LTexture();
LTexture g_point2_text_texture = LTexture();

//Set text color as black
SDL_Color textColor = {0xff, 0xff, 0xff, 0xff};

//The frames per second timer
LTimer timer;

//The music that will be played
Mix_Music* g_music = nullptr;

//The sound effects that will be used
Mix_Chunk* g_bonk = nullptr;
Mix_Chunk* g_point = nullptr;

void reset_objects();
//Starts up SDL and creates window
bool init_game();
bool load_media();
void render_fps(Uint32 frame_time);
void render_points(Player player1, Player player2);
void display_game(Player player1, Player player2, Ball &ball, Uint32 frame_time);
//Frees media and shuts down SDL
void close();

int main(int argv, char** args) {

    Player player1 = Player(Rec(REC_OFFSET, SCREEN_HEIGHT / 2));
    Player player2 = Player(Rec(SCREEN_WIDTH - REC_OFFSET - REC_WIDTH, SCREEN_HEIGHT / 2));
    Ball ball = Ball();

    std::srand (std::time (nullptr));

    //Start counting frames per second
    timer.start();
    
    Uint32 start_time = timer.getTicks();
    Uint32 current_time = start_time;

    // start up SDL and create window
    if (!init_game()) {
        printf("Failed to initialize\n");
    } else {
        load_media();
        // main loop flag
        bool quit = false;

        Mix_PlayMusic(g_music,INT_MAX);
        Mix_VolumeMusic(30);

        // event handler
        SDL_Event e;

        // while application is running
        while (!quit) {
            // handle events on queue
            // if it's 0, then it has no pending event
            // we keep polling all event in each game loop until there is no more pending one left
            while (SDL_PollEvent(&e) != 0) {
                // user requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            Uint32 frame_time = timer.getTicks() - current_time;
            current_time = timer.getTicks();

            if (current_time - start_time > 200) {
                const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
                if (currentKeyStates[SDL_SCANCODE_SPACE]) {
                    start_time = timer.getTicks();
                    pause_flag = !pause_flag;
                }
                if (!pause_flag) {
                    if (currentKeyStates[SDL_SCANCODE_UP]) {
                        --player2.rec;
                    }
                    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
                        ++player2.rec;
                    }
                    if (currentKeyStates[SDL_SCANCODE_W]) {
                        --player1.rec;
                    }
                    if (currentKeyStates[SDL_SCANCODE_S]) {
                        ++player1.rec;
                    }

                    for (int i = 0; i < VELOCITY; ++i) {
                        Collision collision = ball.detect_collision(player1.rec, player2.rec);

                        switch (static_cast<int>(collision)) {
                            case static_cast<int>(Collision::no_collision):
                                ++ball;
                                break;
                            case static_cast<int>(Collision::rec_collision):
                                ++ball;
                                Mix_PlayChannel( -1, g_bonk, 0 );
                                break;
                            case static_cast<int>(Collision::player1):
                                ++player1;
                                Mix_PlayChannel( -1, g_point, 0 );
                                break;
                            case static_cast<int>(Collision::player2):
                                ++player2;
                                Mix_PlayChannel( -1, g_point, 0 );
                                break;
                            default:
                                break;
                        }

                        if (collision == Collision::player1 || collision == Collision::player2) {
                            ball = Ball();
                            break;
                        }
                    }
                }
            }
            display_game(player1, player2, ball, frame_time);
        }
    }
    // free resource and close SDL
    close();

    return 0;

}

bool init_game() {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    //Create window
    int window_flags = SDL_WINDOW_FULLSCREEN;
    gWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if (gWindow == nullptr) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    //Create renderer for window
    //as we useSDL_Texture, now we need renderer to render stuff
    int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    gRenderer = SDL_CreateRenderer(gWindow, -1, renderer_flags);
    if (gRenderer == nullptr) {
        printf("SDL could not create renderer! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }

    return true;
}

bool load_media()
{
    fps_font = TTF_OpenFont( "/home/michal/CLionProjects/pong/fonts/AovelSansRounded-rdDL.ttf", 14 );
    points_font = TTF_OpenFont( "/home/michal/CLionProjects/pong/fonts/AovelSansRounded-rdDL.ttf", 30 );

    if(fps_font == nullptr || points_font == nullptr)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    gFPSTextTexture = LTexture(fps_font);
    g_point1_text_texture = LTexture(points_font);
    g_point2_text_texture = LTexture(points_font);


    //Load music
    g_music = Mix_LoadMUS( "/home/michal/CLionProjects/pong/audio/pokemon.wav" );
    if( g_music == nullptr )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    //Load sound effects
    g_bonk = Mix_LoadWAV( "/home/michal/CLionProjects/pong/audio/bonk.wav" );
    if( g_bonk == nullptr )
    {
        printf( "Failed to load bonk sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    g_point = Mix_LoadWAV( "/home/michal/CLionProjects/pong/audio/point.wav" );
    if( g_point == nullptr )
    {
        printf( "Failed to load point sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }


    return true;
}

void render_fps(Uint32 frame_time) {
    //Calculate and correct fps
    static int fps, full_time, counter;
    full_time += frame_time;

    if (counter == 20) {
        fps = 1000 / (full_time / 20);
        full_time = 0;
        counter = 0;

        //In memory text stream
        std::stringstream timeText;

        //Set text to be rendered
        timeText.str("");
        timeText << "FPS: " << fps;

        //Render text
        if( !gFPSTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
        {
            printf("Unable to render FPS texture!\n");
        }
    }
    //Render textures
    gFPSTextTexture.render((SCREEN_WIDTH - gFPSTextTexture.getWidth()), (SCREEN_HEIGHT - gFPSTextTexture.getHeight()));

    ++counter;
}

void render_points(Player player1, Player player2) {
    int points1 = player1.get_points();
    int points2 = player2.get_points();

    //In memory text stream
    std::stringstream points1_text, points2_text;

    points1_text.str("");
    points1_text << points1;
    points2_text.str("");
    points2_text << points2;

    if( !g_point1_text_texture.loadFromRenderedText(points1_text.str().c_str(), textColor))
    {
        printf("Unable to render points 1 texture!\n");
    }

    if( !g_point2_text_texture.loadFromRenderedText(points2_text.str().c_str(), textColor))
    {
        printf("Unable to render points 2 texture!\n");
    }

    g_point1_text_texture.render((SCREEN_WIDTH / 3 - g_point1_text_texture.getWidth() / 2), g_point1_text_texture.getHeight());
    g_point2_text_texture.render((SCREEN_WIDTH / 3 * 2 - g_point2_text_texture.getWidth() / 2), g_point1_text_texture.getHeight());
}

void display_game(Player player1, Player player2, Ball &ball, Uint32 frame_time) {
    // clear screen
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(gRenderer);

    render_fps(frame_time);
    render_points(player1, player2);

    // render blue horizontal line
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

    // render the filled quad
    SDL_Rect rect1 = {player1.rec.get_x(), player1.rec.get_y(), player1.rec.get_w(), player1.rec.get_h()};
    SDL_SetRenderDrawColor(gRenderer, player1.rec.get_color(RED), player1.rec.get_color(GREEN), player1.rec.get_color(BLUE), 0xFF);
    SDL_RenderFillRect(gRenderer, &rect1);

    SDL_Rect rect2 = {player2.rec.get_x(), player2.rec.get_y(), player2.rec.get_w(), player2.rec.get_h()};
    SDL_RenderFillRect(gRenderer, &rect2);

    SDL_Rect rect3 = {ball.get_x(), ball.get_y(), ball.get_w(), ball.get_h()};
    SDL_SetRenderDrawColor(gRenderer, ball.get_color(RED), ball.get_color(GREEN), ball.get_color(BLUE), 0xFF);
    SDL_RenderFillRect(gRenderer, &rect3);

    // update screen
    SDL_RenderPresent(gRenderer);
}

void close() {
    //Free loaded images
    gFPSTextTexture.free();

    //Free global font
    TTF_CloseFont( fps_font );
    fps_font = nullptr;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
