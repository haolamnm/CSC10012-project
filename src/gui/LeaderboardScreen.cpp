/*
FILE: src/gui/LeaderboardScreen.cpp

DESCRIPTION: Implementation file for the leaderboard screen. This 
file is part of the GUI subsystem and is used to render the 
leaderboard screen. This file contains the function definition
for rendering the leaderboard screen. We also handle the mouse
scroll event in this file.

NOTE: The leaderboard screen is where the player can view the
leaderboard of the game. The leaderboard shows the top players
based on their winrate.

AUTHOR: Lam Chi Hao & Le Nguyen Anh Tri.
*/

#include "gui/LeaderboardScreen.h"

int scrollOffset = 0;
int maxScrollOffset = 0;
constexpr const int SCROLL_SPEED = 15;

void renderLeaderboardScreen(GameEngine* game) {
    TTF_Font* bigFontVintage = game->getBigFontVintage();
    TTF_Font* smallFontVintage = game->getSmallFontVintage();
    SDL_Renderer* renderer = game->getRenderer();
    SDL_Texture* backButtonTexture = game->getBackButtonTexture();

    // Get the mouse coordinations
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Render the Leaderboard screen background
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    // Render the Leaderboard screen title
    SDL_Color textColor = {255, 255, 255, 255};
    game->renderText(renderer, bigFontVintage, "Leaderboard", WINDOW_WIDTH / 2, 50, textColor, true);

    // Get the leaderboard data    
    std::vector<std::vector<std::string>> leaderboard = lobby.handleLeaderboard();
    int nRow = leaderboard.size();
    int nCol = leaderboard[0].size();

    // Define column headers
    const char* headers[] = {"Rank", "Username", "Chips", "Games", "Winrate", "Favorite Strategy"};
    int headerCount = sizeof(headers) / sizeof(headers[0]);

    // Define column widths
    int columnWidths[] = {75, 150, 100, 100, 100, 200};

    // Render column headers
    int startX = 50;
    int startY = 150;
    for (int j = 0; j < headerCount; j++) {
        game->renderText(renderer, smallFontVintage, headers[j], startX, startY, textColor, false);
        startX += columnWidths[j];
    }

    maxScrollOffset = std::max(0, (nRow * 30) - (490 - 180)); // Maximum scroll offset

    startY += 40 - scrollOffset; // Move down for the first row of data
    for (int i = 0; i < nRow; i++) {
        startX = 50; // Reset to the start of the row
        for (int j = 0; j < nCol; j++) {
            if (startY >= 180 && startY <= 490) {
                game->renderText(renderer, smallFontVintage, leaderboard[i][j].c_str(), startX, startY, textColor, false);
            }
            startX += columnWidths[j];
        }
        startY += 30; // Move down for the next row
    }

    if ((nRow * 30) > (490 - 180)) {
        game->renderText(renderer, smallFontVintage, "Scroll to view more", WINDOW_WIDTH / 2, 550, textColor, true);
    }

    // Render the back button
    SDL_Rect backButtonRect = {START_X, START_X, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);
    game->handleButtonHover(backButtonTexture, mouseX, mouseY, START_X, START_X, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
}

void handleMouseWheelEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) {
            scrollOffset -= SCROLL_SPEED;
            if (scrollOffset < 0) {
                scrollOffset = 0;
            }
        } else if (event.wheel.y < 0) {
            scrollOffset += SCROLL_SPEED;
            if (scrollOffset > maxScrollOffset) {
                scrollOffset = maxScrollOffset;
            }
        }
    }
}