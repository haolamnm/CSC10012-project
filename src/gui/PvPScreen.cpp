/*
FILE: src/gui/PvPScreen.cpp

DESCRIPTION: Implementation file for the Player vs Player (PvP)
screen. This file is part of the GUI subsystem and is used to
render the PvP screen. This file contains the function definition
for rendering the PvP screen.

NOTE: PvP availables in all game mode.

AUTHOR: Lam Chi Hao & Le Nguyen Anh Tri.
*/

#include "gui/PvPScreen.h"

bool isDealtPvP = false;
bool isSavedPvP = false;
bool isDrawButtonClicked = false;
bool drawButtonFlag = false;
bool isFoldButtonClicked = false;
bool foldButtonFlag = false;
bool isCallButtonClicked = false;
bool callButtonFlag = false;
bool isRaiseButtonClicked = false;
bool raiseButtonFlag = false;
int currentCardIndex = -1;

unsigned int highestBet = 0;

// Function to render the PvP screen
void renderPvPScreen(GameEngine* game) {
    TTF_Font* bigFontVintage = game->getBigFontVintage();
    TTF_Font* mediumFontVintage = game->getMediumFontVintage();
    TTF_Font* smallFontVintage = game->getSmallFontVintage();
    SDL_Renderer* renderer = game->getRenderer();
    SDL_Texture* backButtonTexture = game->getBackButtonTexture();
    SDL_Texture* nextButtonTexture = game->getNextButtonTexture();
    SDL_Texture* foldButtonTexture = game->getFoldButtonTexture();
    SDL_Texture* callButtonTexture = game->getCallButtonTexture();
    SDL_Texture* drawButtonTexture = game->getdrawButtonTexture();
    SDL_Texture* raiseButtonTexture = game->getRaiseButtonTexture();
    SDL_Color textColor = {255, 255, 255, 255};

    // Get the mouse coordinates
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Render the PvP screen background
    SDL_SetRenderDrawColor(renderer, 9, 70, 27, 255);
    SDL_RenderClear(renderer);

    // Render the back button
    SDL_Rect backButtonRect = {START_X, START_X, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
    SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);
    game->handleButtonHover(backButtonTexture, mouseX, mouseY, START_X, START_X, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

    // Check if there are at least 2 players in the lobby
    const std::vector<std::string>& usernames = lobby.getUsernames();
    if (usernames.size() < 2) {
        // Render a message indicating that more players are needed
        SDL_Color redColor = {255, 0, 0, 255}; // Red color
        game->renderText(renderer, mediumFontVintage, "Need at least 2 players to play PvP", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, redColor, true);
    } else if (game->currentPlayer <= static_cast<int>(usernames.size())) {
        // Render the current game mode at the bottom of the screen
        std::string gameModeText = "Game Mode: " + std::string(game->getCurrentGameModeString());
        game->renderText(renderer, smallFontVintage, gameModeText.c_str(), WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50, textColor, true);
        if (game->currentGameMode == GameEngine::BASIC_POKER) {
            static Gameplay gameplay;
            int numberOfCards = 5;

            if (!isDealtPvP) {
                isDealtPvP = true;
                gameplay.entryFee = defaultChipsBetted;
                gameplay.init(usernames, 0);
                gameplay.resetDeck(); // Reset the deck for a new game
                gameplay.dealCards(numberOfCards);
            }
            const char*** cardSets = new const char**[gameplay.numberOfPlayers];
            for (size_t i = 0; i < gameplay.numberOfPlayers; i++) {
                cardSets[i] = new const char*[5];
            }
            // Array of card file paths
            for (int i = 0; i < gameplay.numberOfPlayers; i++) {
                for (int j = 0; j < numberOfCards; j++) {
                    Card& currentCard = gameplay.players[i].hand.cards[j];
                    cardSets[i][j] = CARD_FILES[currentCard.rank * 4 + currentCard.suit].c_str();
                }
            }
            
            // Render the 5 cards
            if (game->currentPlayer < gameplay.numberOfPlayers) {
                // Render the "username" text
                game->renderText(renderer, bigFontVintage, gameplay.players[gameplay.players[game->currentPlayer].id].username.c_str(), WINDOW_WIDTH / 2, 50, textColor, true);
                game->renderCards(cardSets[gameplay.players[game->currentPlayer].id], true, 0, true);
                SDL_Rect nextButtonRect = {NEXT_BUTTON_X, NEXT_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                bool allCardsFaceUp = true;
                for (int i = 0; i < 5; i++) {
                    if (!game->getCardRevealed()[i]) {
                        allCardsFaceUp = false;
                        break;
                    }
                }
                std::string entryFeeText = "Entry fee: " + std::to_string(gameplay.entryFee);
                game->renderText(renderer, smallFontVintage, entryFeeText.c_str(), 780, 50, textColor, false, true);
                std::string betText = "Bet: " + std::to_string(gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted);
                game->renderText(renderer, smallFontVintage, betText.c_str(), 780, 100, textColor, false, true);
                std::string totalBetText = "Total bet: " + std::to_string(gameplay.totalChipsBetted);
                game->renderText(renderer, smallFontVintage, totalBetText.c_str(), 780, 125, textColor, false, true);
                if (allCardsFaceUp) {
                    std::string chipText = "Chips: " + std::to_string(gameplay.players[gameplay.players[game->currentPlayer].id].chips -
                                                                    gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted);
                    game->renderText(renderer, smallFontVintage, chipText.c_str(), 780, 75, textColor, false, true);
                    gameplay.players[gameplay.players[game->currentPlayer].id].hand.evaluateHand();
                    game->renderText(renderer, mediumFontVintage, gameplay.players[gameplay.players[game->currentPlayer].id].hand.handName.c_str(), WINDOW_WIDTH / 2, 450, textColor, true);
                }
                SDL_RenderCopy(renderer, nextButtonTexture, NULL, &nextButtonRect);
                game->handleButtonHover(nextButtonTexture, mouseX, mouseY, NEXT_BUTTON_X, NEXT_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
            } else if (game->currentPlayer == static_cast<int>(gameplay.numberOfPlayers)) {
                // Save player data after dealing cards and determining the winner
                if (!isSavedPvP) {
                    isSavedPvP = true;
                    gameplay.whoWins();
                    for (Player& player : gameplay.players) {
                        if (player.isBot == false) gameplay.savePlayerData(player);
                    }
                    gameplay.saveAllPlayerData();
                }
                if (gameplay.winner != -1) {
                    std::string winner = gameplay.players[gameplay.winner].username;
                    game->renderText(renderer, bigFontVintage, "Winner:", WINDOW_WIDTH / 2, 50, textColor, true);
                    game->renderText(renderer, bigFontVintage, winner.c_str(), WINDOW_WIDTH / 2, 125, textColor, true);
                    game->renderCards(cardSets[gameplay.winner], false, 0, false);
                    // Winner hand stregth
                    game->renderText(renderer, mediumFontVintage, gameplay.players[gameplay.winner].hand.handName.c_str(), WINDOW_WIDTH / 2, 450, textColor, true);
                } else {
                    game->renderText(renderer, bigFontVintage, "It's a tie!", WINDOW_WIDTH / 2, 50, textColor, true);
                }
            }
            for (size_t i = 0; i < gameplay.numberOfPlayers; i++) {
                delete[] cardSets[i];
            }
            delete[] cardSets;
        } else if (game->currentGameMode == GameEngine::DRAW_POKER) {
            static Gameplay gameplay;
            int numberOfCards = 5;
            if (!isDealtPvP) {
                isDealtPvP = true;
                gameplay.entryFee = defaultChipsBetted;
                gameplay.init(usernames, 0);
                gameplay.resetDeck(); // Reset the deck for a new game
                gameplay.dealCards(numberOfCards); 
            }

            const char*** cardSets = new const char**[gameplay.numberOfPlayers];
            for (size_t i = 0; i < gameplay.numberOfPlayers; i++) {
                cardSets[i] = new const char*[5];
            }
            
            // Array of card file paths
            for (int i = 0; i < gameplay.numberOfPlayers; i++) {
                for (int j = 0; j < numberOfCards; j++) {
                    Card& currentCard = gameplay.players[i].hand.cards[j];
                    cardSets[i][j] = CARD_FILES[currentCard.rank * 4 + currentCard.suit].c_str();
                }
            }
            
            if (game->currentPlayer < gameplay.numberOfPlayers) {
                foldButtonFlag = gameplay.players[gameplay.players[game->currentPlayer].id].isFolded;

                // Render the entry fee, bet, and total bet
                std::string entryFeeText = "Entry fee: " + std::to_string(gameplay.entryFee);
                game->renderText(renderer, smallFontVintage, entryFeeText.c_str(), 780, 50, textColor, false, true);
                std::string betText = "Bet: " + std::to_string(gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted);
                game->renderText(renderer, smallFontVintage, betText.c_str(), 780, 100, textColor, false, true);
                std::string totalBetText = "Total bet: " + std::to_string(gameplay.totalChipsBetted);
                game->renderText(renderer, smallFontVintage, totalBetText.c_str(), 780, 125, textColor, false, true);


                if ((game->currentDrawPokerRound == GameEngine::FIRST_BETTING_ROUND || game->currentDrawPokerRound == GameEngine::SECOND_BETTING_ROUND) && foldButtonFlag == false) {
                    if (isRaiseButtonClicked == true) {
                        isRaiseButtonClicked = false;
                        raiseButtonFlag = true;
                        gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted += 10;
                        if (gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted > gameplay.highestBet) {
                            gameplay.highestBet = gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted;
                            highestBet = gameplay.highestBet;
                        }
                        gameplay.totalChipsBetted += 10;
                    }
                    if (foldButtonFlag == false && isFoldButtonClicked == true) {
                        isFoldButtonClicked = false;
                        foldButtonFlag = true;
                        gameplay.players[gameplay.players[game->currentPlayer].id].isFolded = true;
                    }
                }

                if ((game->currentDrawPokerRound == GameEngine::FIRST_BETTING_FOLD_CALL || game->currentDrawPokerRound == GameEngine::SECOND_BETTING_FOLD_CALL) && foldButtonFlag == false) {
                    std::string highestBetText = "Highest bet: " + std::to_string(gameplay.highestBet);
                    game->renderText(renderer, smallFontVintage, highestBetText.c_str(), 780, 150, textColor, false, true);
                    if (foldButtonFlag == false && isFoldButtonClicked == true) {
                        isFoldButtonClicked = false;
                        foldButtonFlag = true;
                        gameplay.players[gameplay.players[game->currentPlayer].id].isFolded = true;
                    }
                    if (gameplay.highestBet > 0 && callButtonFlag == false && isCallButtonClicked == true) {
                        isCallButtonClicked = false;
                        callButtonFlag = true;
                        if (gameplay.players[gameplay.players[game->currentPlayer].id].chips >= gameplay.highestBet &&
                            gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted < gameplay.highestBet) {
                            int cost = gameplay.highestBet - gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted;
                            gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted += cost;
                            gameplay.totalChipsBetted += cost;
                        }
                    }
                }

                if (game->currentDrawPokerRound == GameEngine::DRAW_ROUND) {
                    if (drawButtonFlag == false && isDrawButtonClicked == true) {
                        isDrawButtonClicked = false;
                        drawButtonFlag = true;
                        gameplay.drawPlayerCards(gameplay.players[gameplay.players[game->currentPlayer].id]);
                    }
                    if (currentCardIndex != -1) {
                        int selectedCards = gameplay.countSelectedCards(gameplay.players[game->currentPlayer].id);
                        if (selectedCards < 3 || gameplay.players[game->currentPlayer].hand.removedCards[currentCardIndex]) {
                            gameplay.players[gameplay.players[game->currentPlayer].id].hand.removedCards[currentCardIndex] = 
                            !gameplay.players[gameplay.players[game->currentPlayer].id].hand.removedCards[currentCardIndex];
                        }
                        currentCardIndex = -1;
                    }
                    // Outline for the selected card
                    SDL_Rect cardRect = {0, 0, 0, 0};
                    for (int i = 0; i < 5; i++) {
                        if (gameplay.players[gameplay.players[game->currentPlayer].id].hand.removedCards[i]) {
                            cardRect = game->getCardRects()[i];
                            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan color
                            SDL_Rect outlineRect = {cardRect.x - 6, cardRect.y - 6, cardRect.w + 12, cardRect.h + 12}; // Make the outline thicker
                            SDL_RenderDrawRect(renderer, &outlineRect);
                            outlineRect = {cardRect.x - 5, cardRect.y - 5, cardRect.w + 10, cardRect.h + 10}; // Draw another outline for thickness
                            SDL_RenderDrawRect(renderer, &outlineRect);
                        }
                    }
                }
                // Render round text                
                game->renderText(renderer, mediumFontVintage, game->getCurrentRoundString(), WINDOW_WIDTH / 2, 50, textColor, true);

                // Render the "username" text
                game->renderText(renderer, mediumFontVintage, gameplay.players[gameplay.players[game->currentPlayer].id].username.c_str(), WINDOW_WIDTH / 2, 100, textColor, true);
                if (foldButtonFlag) {
                    game->renderText(renderer, mediumFontVintage, "Folded", WINDOW_WIDTH / 2, 150, textColor, true);
                    const char* foldedCardSet[] = {CARD_BACK, CARD_BACK, CARD_BACK, CARD_BACK, CARD_BACK};
                    game->renderCards(foldedCardSet, true, 0, true);
                } else game->renderCards(cardSets[gameplay.players[game->currentPlayer].id], true, 0, true);
                SDL_Rect nextButtonRect = {NEXT_BUTTON_X, NEXT_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                bool allCardsFaceUp = true;
                for (int i = 0; i < 5; i++) {
                    if (!game->getCardRevealed()[i]) {
                        allCardsFaceUp = false;
                        break;
                    }
                }
                if (allCardsFaceUp) {
                    std::string chipText = "Chips: " + std::to_string(gameplay.players[gameplay.players[game->currentPlayer].id].chips -
                                                                    gameplay.players[gameplay.players[game->currentPlayer].id].chipsBetted);
                    game->renderText(renderer, smallFontVintage, chipText.c_str(), 780, 75, textColor, false, true);
                    if (foldButtonFlag == false) {
                        gameplay.players[gameplay.players[game->currentPlayer].id].hand.evaluateHand();
                        game->renderText(renderer, mediumFontVintage, gameplay.players[gameplay.players[game->currentPlayer].id].hand.handName.c_str(), WINDOW_WIDTH / 2, 450, textColor, true);
                    }
                }
                SDL_RenderCopy(renderer, nextButtonTexture, NULL, &nextButtonRect);
                game->handleButtonHover(nextButtonTexture, mouseX, mouseY, NEXT_BUTTON_X, NEXT_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

                // Render fold button
                SDL_Rect foldButtonRect = {FOLD_BUTTON_X, FOLD_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                SDL_RenderCopy(renderer, foldButtonTexture, NULL, &foldButtonRect);
                game->handleButtonHover(foldButtonTexture, mouseX, mouseY, FOLD_BUTTON_X, FOLD_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

                // Render call button
                SDL_Rect callButtonRect = {CALL_BUTTON_X, CALL_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                SDL_RenderCopy(renderer, callButtonTexture, NULL, &callButtonRect);
                game->handleButtonHover(callButtonTexture, mouseX, mouseY, CALL_BUTTON_X, CALL_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

                // Render raise button
                SDL_Rect raiseButtonRect = {RAISE_BUTTON_X, RAISE_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                SDL_RenderCopy(renderer, raiseButtonTexture, NULL, &raiseButtonRect);
                game->handleButtonHover(raiseButtonTexture, mouseX, mouseY, RAISE_BUTTON_X, RAISE_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

                // Render reset button
                SDL_Rect drawButtonRect = {DRAW_BUTTON_X, DRAW_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                SDL_RenderCopy(renderer, drawButtonTexture, NULL, &drawButtonRect);
                game->handleButtonHover(drawButtonTexture, mouseX, mouseY, DRAW_BUTTON_X, DRAW_BUTTON_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);

                if (game->currentDrawPokerRound == GameEngine::SHOWDOWN_ROUND) {
                    // Reveal all players' cards if not folded
                    if (foldButtonFlag == false) {
                        game->renderCards(cardSets[gameplay.players[game->currentPlayer].id], false, 0, false);
                        game->renderText(renderer, mediumFontVintage, gameplay.players[gameplay.players[game->currentPlayer].id].hand.handName.c_str(), WINDOW_WIDTH / 2, 450, textColor, true);
                    }
                }
            } else if (game->currentPlayer == static_cast<int>(gameplay.numberOfPlayers)) {
                if (game->currentDrawPokerRound == GameEngine::SHOWDOWN_ROUND) {
                    if (!isSavedPvP) {
                        isSavedPvP = true;
                        gameplay.whoWins();
                        for (Player& player : gameplay.players) {
                            if (player.isBot == false) gameplay.savePlayerData(player);
                        }
                        gameplay.saveAllPlayerData();
                    }
                    if (gameplay.winner != -1) {
                        std::string winner = gameplay.players[gameplay.winner].username;
                        game->renderText(renderer, bigFontVintage, "Winner:", WINDOW_WIDTH / 2, 50, textColor, true);
                        game->renderText(renderer, bigFontVintage, winner.c_str(), WINDOW_WIDTH / 2, 125, textColor, true);
                        game->renderCards(cardSets[gameplay.winner], false, 0, false);
                        // Winner hand stregth
                        game->renderText(renderer, mediumFontVintage, gameplay.players[gameplay.winner].hand.handName.c_str(), WINDOW_WIDTH / 2, 450, textColor, true);
                    } else {
                        game->renderText(renderer, bigFontVintage, "It's a tie!", WINDOW_WIDTH / 2, 50, textColor, true);
                    }
                } else {
                    game->currentDrawPokerRound = (GameEngine::drawPokerRound) ((int) game->currentDrawPokerRound + 1);
                    game->currentPlayer = 0;
                }
            }
            for (size_t i = 0; i < gameplay.numberOfPlayers; i++) {
                delete[] cardSets[i];
            }
            delete[] cardSets;
        }
    }
}