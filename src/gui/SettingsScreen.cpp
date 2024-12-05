// Description: Implementation of the renderSettingsScreen function

#include "gui/SettingsScreen.h"

// Boolean variables for music and sound effects settings
bool isMusicOn = true;
bool isSoundEffectsOn = true;

// For settings chip
int defaultChipBetted = 20;

// Function to render the Settings screen
void renderSettingsScreen(GameEngine* game) {
    TTF_Font* font = TTF_OpenFont("assets/fonts/SVN-Vintage.otf", 18); // TODO: Create a constant for small, medium and big font.
    SDL_Window* window = game->getWindow();
    SDL_Renderer* renderer = game->getRenderer();
    SDL_Texture* backButtonTexture = game->getBackButtonTexture();
    SDL_Texture* nextButtonTexture = game->getNextButtonTexture();
    SDL_Texture* musicOnButtonTexture = game->getMusicOnButtonTexture();
    SDL_Texture* musicOffButtonTexture = game->getMusicOffButtonTexture();
    SDL_Texture* soundOnButtonTexture = game->getSoundOnButtonTexture();
    SDL_Texture* soundOffButtonTexture = game->getSoundOffButtonTexture();
    SDL_Texture* rasieButtonTexture = game->getRaiseButtonTexture();

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255};
    game->renderText(renderer, font, "Settings", windowWidth / 2, 50, textColor, true);

    SDL_Rect backButtonRect = {START_X, START_X, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);
    game->handleButtonHover(backButtonTexture, mouseX, mouseY, START_X, START_X, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

    // Render background music toggle buttons
    game->renderText(renderer, font, "Background music", windowWidth / 2, 150, textColor, false);
    SDL_Rect musicOnButtonRect = {windowWidth / 2 + 100, 150, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, musicOnButtonTexture, NULL, &musicOnButtonRect);
    game->handleButtonHover(musicOnButtonTexture, mouseX, mouseY, musicOnButtonRect.x, musicOnButtonRect.y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    SDL_Rect musicOffButtonRect = {windowWidth / 2 + 100 + SMALL_BUTTON_WIDTH + 20, 150, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, musicOffButtonTexture, NULL, &musicOffButtonRect);
    game->handleButtonHover(musicOffButtonTexture, mouseX, mouseY, musicOffButtonRect.x, musicOffButtonRect.y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

    // Render sound effects toggle buttons
    game->renderText(renderer, font, "Sound effects", windowWidth / 2, 180, textColor, false);
    SDL_Rect soundOnButtonRect = {windowWidth / 2 + 100, 180, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, soundOnButtonTexture, NULL, &soundOnButtonRect);
    game->handleButtonHover(soundOnButtonTexture, mouseX, mouseY, soundOnButtonRect.x, soundOnButtonRect.y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    SDL_Rect soundOffButtonRect = {windowWidth / 2 + 100 + SMALL_BUTTON_WIDTH + 20, 180, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, soundOffButtonTexture, NULL, &soundOffButtonRect);
    game->handleButtonHover(soundOffButtonTexture, mouseX, mouseY, soundOffButtonRect.x, soundOffButtonRect.y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

    // Render current game mode
    game->renderText(renderer, font, "Current Game Mode:", 400, 300, textColor, false);
    game->renderText(renderer, font, game->getCurrentGameModeString(), windowWidth / 2 + 150, 210, textColor, false);

    SDL_Rect nextButtonRect = {NEXT_BUTTON_X, NEXT_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, nextButtonTexture, NULL, &nextButtonRect);
    game->handleButtonHover(nextButtonTexture, mouseX, mouseY, NEXT_BUTTON_X, NEXT_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

    SDL_Rect rasieButtonRect = {RAISE_BUTTON_X, RAISE_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, rasieButtonTexture, NULL, &rasieButtonRect);
    game->handleButtonHover(rasieButtonTexture, mouseX, mouseY, RAISE_BUTTON_X, RAISE_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

    // Render default chip betted
    std::string defaultChipBettedText = "Default chip betted: " + std::to_string(defaultChipBetted);
    std::cout << defaultChipBettedText << std::endl;
    game->renderText(renderer, font, defaultChipBettedText.c_str(), WINDOW_WIDTH / 2, 400, textColor, true);

    // TODO:
    // Implement the functionality to toggle the music and sound effects settings
    // Implement input for min chip per game, the number of bots, game mode.

    TTF_CloseFont(font);
}