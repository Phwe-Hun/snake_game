#include "raylib.h"
#include "menu.h" // Include the header file for declarations

// Global flag to track whether to show instructions
bool showInstructions = false;

void DrawMainMenu()
{
    // Clear the screen with a solid color (e.g., light gray)
    ClearBackground(GREEN);

    // If not showing instructions, draw the main menu
    if (!showInstructions)
    {
        // Draw the game title
        DrawText("Snake Game", 250, 100, 50, YELLOW);

        // Define button dimensions and positions
        int buttonWidth = 300;
        int buttonHeight = 50;
        int startButtonX = 250;
        int startButtonY = 200;
        int instructionsButtonX = 250;
        int instructionsButtonY = 270;
        int exitButtonX = 250;
        int exitButtonY = 340;

        // Measure text for each button
        int startTextWidth = MeasureText("Start Game", 30);
        int instructionsTextWidth = MeasureText("How to Play", 30);
        int exitTextWidth = MeasureText("Exit", 30);

        // Calculate text positions to center them
        int startTextX = startButtonX + (buttonWidth - startTextWidth) / 2;
        int startTextY = startButtonY + (buttonHeight - 30) / 2;
        int instructionsTextX = instructionsButtonX + (buttonWidth - instructionsTextWidth) / 2;
        int instructionsTextY = instructionsButtonY + (buttonHeight - 30) / 2;
        int exitTextX = exitButtonX + (buttonWidth - exitTextWidth) / 2;
        int exitTextY = exitButtonY + (buttonHeight - 30) / 2;

        // Draw the start button
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){startButtonX, startButtonY, buttonWidth, buttonHeight}))
        {
            DrawRectangle(startButtonX, startButtonY, buttonWidth, buttonHeight, DARKGRAY);
            DrawText("Start Game", startTextX, startTextY, 30, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Indicate to start the game (could transition here to game logic)
            }
        }
        else
        {
            DrawRectangle(startButtonX, startButtonY, buttonWidth, buttonHeight, LIGHTGRAY);
            DrawText("Start Game", startTextX, startTextY, 30, DARKGRAY);
        }

        // Draw instructions button
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){instructionsButtonX, instructionsButtonY, buttonWidth, buttonHeight}))
        {
            DrawRectangle(instructionsButtonX, instructionsButtonY, buttonWidth, buttonHeight, DARKGRAY);
            DrawText("How to Play", instructionsTextX, instructionsTextY, 30, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                showInstructions = true; // Switch to instructions screen
            }
        }
        else
        {
            DrawRectangle(instructionsButtonX, instructionsButtonY, buttonWidth, buttonHeight, LIGHTGRAY);
            DrawText("How to Play", instructionsTextX, instructionsTextY, 30, DARKGRAY);
        }

        // Draw exit button
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){exitButtonX, exitButtonY, buttonWidth, buttonHeight}))
        {
            DrawRectangle(exitButtonX, exitButtonY, buttonWidth, buttonHeight, DARKGRAY);
            DrawText("Exit", exitTextX, exitTextY, 30, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                CloseWindow(); // Exit the game
            }
        }
        else
        {
            DrawRectangle(exitButtonX, exitButtonY, buttonWidth, buttonHeight, LIGHTGRAY);
            DrawText("Exit", exitTextX, exitTextY, 30, DARKGRAY);
        }
    }
    else
    {
        // If on the "How to Play" page, show instructions and hide game
        DrawInstructions();
    }
}

void DrawInstructions()
{
    // Clear background for instructions page
    ClearBackground(GREEN);

    // Draw instructions text
    DrawText("How to Play", 300, 50, 40, YELLOW);
    DrawText("Use arrow keys to move the snake.", 130, 150, 30, BLACK);
    DrawText("Eat food to grow longer.", 130, 200, 30, BLACK);
    DrawText("Avoid running into walls or yourself!", 130, 250, 30, BLACK);
    DrawText("Press ESC to return to the main menu.", 130, 300, 30, BLACK);

    // Draw back button
    int backButtonX = 280;
    int backButtonY = 380;
    int backButtonWidth = 250;
    int backButtonHeight = 50;
    int backTextWidth = MeasureText("Back to Menu", 30);
    int backTextX = backButtonX + (backButtonWidth - backTextWidth) / 2;
    int backTextY = backButtonY + (backButtonHeight - 30) / 2;

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){backButtonX, backButtonY, backButtonWidth, backButtonHeight}))
    {
        DrawRectangle(backButtonX, backButtonY, backButtonWidth, backButtonHeight, DARKGRAY);
        DrawText("Back to Menu", backTextX, backTextY, 30, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            showInstructions = false; // Return to main menu
        }
    }
    else
    {
        DrawRectangle(backButtonX, backButtonY, backButtonWidth, backButtonHeight, LIGHTGRAY);
        DrawText("Back to Menu", backTextX, backTextY, 30, DARKGRAY);
    }
}
