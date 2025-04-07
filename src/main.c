#include <"raylib.h">
#include <"menu.h">

#define SNAKE_LENGTH 256 // Snake Max Length 
#define SQUARE_SIZE 40 // Size of each segment
#define GRID_WIDTH (800 / SQUARE_SIZE) // Assuming 800x600 screen
#define GRID_HEIGHT (600 / SQUARE_SIZE)


typedef struct Snake // Snake Structure 
{
    Vector2 position; 
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Fruit // Fruit Structure
{
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Fruit;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Snake Game in C with Raylib");

    // Initialize audio device
    InitAudioDevice();
    Music music = LoadMusicStream("resources/music.wav"); // Background music
    PlayMusicStream(music);                               // Start playing music

    // Load music and sound effects
    Sound eatSound = LoadSound("resources/eat.mp3");             // Sound for eating the fruit
    Sound gameOverSound = LoadSound("resources/game_over.wav");  // Game Over sound
    Sound startSound = LoadSound("resources/start.wav");         // Sound for game start
    Sound specialSong = LoadSound("resources/special_song.wav"); // Special sound for score 10+

    Snake snake[SNAKE_LENGTH] = {0}; // Snake struct with exact position, size, speed and color
    snake[0].position = (Vector2){(GRID_WIDTH / 2-2) * SQUARE_SIZE, (GRID_HEIGHT / 2-2) * SQUARE_SIZE};
    snake[0].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake[0].speed = (Vector2){SQUARE_SIZE, 0};
    snake[0].color = GREEN;

    int snakeLength = 1;
    int score = 0;
    int bestScore = 0; // Variable to store the best score

    bool gameOver = false; // Game is ongoing
    bool gameStarted = false; // Game hasn't started
    bool isInMenu = true; // Game is in menu page

    Fruit fruit = {0}; // fruit size, color, position and activate to appear in the game
    fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    fruit.color = RED;
    fruit.position = (Vector2){GetRandomValue(0, GRID_WIDTH - 1) * SQUARE_SIZE, GetRandomValue(0, GRID_HEIGHT - 1) * SQUARE_SIZE};
    fruit.active = true;

    int framesCounter = 0; // Use for track elapsed frames (timing) 
        SetTargetFPS(60); // Run at 60 (frames per sec)

    PlaySound(startSound);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music); // Keep playing music

        // Game restart logic
        if (gameOver)
        {
            if (IsKeyPressed(KEY_R)) // Press "R" to restart
            { 
                gameOver = false;
                snakeLength = 1;
                score = 0;
                //Snake position after press R key
                snake[0].position = (Vector2){ 
                    (GRID_WIDTH / 2) * SQUARE_SIZE, 
                    (GRID_HEIGHT / 2) * SQUARE_SIZE};  
                snake[0].speed = (Vector2){SQUARE_SIZE, 0}; // will start moving right 
                //Random fruit position
                fruit.position = (Vector2){
                    GetRandomValue(0, GRID_WIDTH - 1) * SQUARE_SIZE, 
                    GetRandomValue(0, GRID_HEIGHT - 1) * SQUARE_SIZE};
                fruit.active = true; 
                PlaySound(startSound);
            }
        }

        if (!gameOver) // Handaling key by IsKeyPressed function
        {
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0)) // Press right key and do not reverse x(horizontally) to right immediately
                snake[0].speed = (Vector2){SQUARE_SIZE, 0}; // Set the snake speed to move right for one square(20 pixels)
            if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0)) // Press left and ....
                snake[0].speed = (Vector2){-SQUARE_SIZE, 0}; // left ...
            if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0)) // Press Up key and do not reverse y(vertically) to down 
                snake[0].speed = (Vector2){0, -SQUARE_SIZE}; // up ...
            if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0)) // Press Down key and ...
                snake[0].speed = (Vector2){0, SQUARE_SIZE}; // down ...

            // If the game hasn't started yet and any arrow key is pressed, start the game
            if (!gameStarted && (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)))
            {
                gameStarted = true;
            }
        }

        framesCounter++;
        if (framesCounter >= 15) // Every 15 Frames for each grid square
        {
            framesCounter = 0; // Restart the Frame

            // Only update snake if game is started and not over
            if (gameStarted && !gameOver)
            {
                // If snake overlap(eat) fruit
                if (CheckCollisionRecs((Rectangle){snake[0].position.x, snake[0].position.y, snake[0].size.x, snake[0].size.y},
                                       (Rectangle){fruit.position.x, fruit.position.y, fruit.size.x, fruit.size.y}))
                {
                    fruit.active = false; // No fruit visible while eaten by snake
                    score++; // +1 score
                    snakeLength++; // +1 length
                    snake[snakeLength - 1].position = snake[snakeLength - 2].position; // Put the new snakeLength +1 to the position of the last segment before eaten a fruit
                    fruit.position = (Vector2){GetRandomValue(0, screenWidth / SQUARE_SIZE - 1) * SQUARE_SIZE, GetRandomValue(0, screenHeight / SQUARE_SIZE - 1) * SQUARE_SIZE}; // generate a new fruit in random position after eaten
                    fruit.active = true; // Fruit visible
                    PlaySound(eatSound); // Play the chop chop eating song
                    if (score % 10 == 0) // If the score is 10+ (e.g., 10, 20, 30...)
                    {
                        PlaySound(specialSong); // Play special song
                    }
                    snake[0].color = GREEN; // Snake head is always green
                }

                // Make sure that every segments of snake except head(snake[0]) will follow the segment before it 
                for (int i = snakeLength - 1; i > 0; i--)
                {
                    snake[i].position = snake[i - 1].position;
                }

                // Move the head based on the current direction
                snake[0].position.x += snake[0].speed.x;
                snake[0].position.y += snake[0].speed.y;

                // Check for collision with itself (snake body)
                for (int i = 1; i < snakeLength; i++)
                {
                    // Check if the head collides with any body part
                    if (snake[0].position.x == snake[i].position.x && snake[0].position.y == snake[i].position.y)
                    {
                        // Update best score if necessary
                        if (score > bestScore)
                        {
                            bestScore = score;
                        }

                        gameOver = true; // GAME OVER !!!!!
                        PlaySound(gameOverSound);
                        break; // Exit the loop once the snake hits its body
                    }
                }

                // Check for collision with walls (edges of the game screen)
                if ((snake[0].position.x >= screenWidth) || (snake[0].position.x < 0) ||
                    (snake[0].position.y >= screenHeight) || (snake[0].position.y < 0))
                {
                    // Update best score if necessary
                    if (score > bestScore)
                    {
                        bestScore = score;
                    }

                    gameOver = true;
                    PlaySound(gameOverSound);
                }
            }
        }

        BeginDrawing(); // Start drawing the frame
        ClearBackground(RAYWHITE); // Clear the background in white

        if (isInMenu)
        {
            DrawMainMenu(); // Draw the main menu

            // Check if the start button is clicked
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){200, 200, 200, 50}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                isInMenu = false; // Transition to the game loop
            }
        }
        else
        {

            // Draw grid with alternating colors
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                for (int x = 0; x < GRID_WIDTH; x++)
                {
                    Color cellColor = ((x + y) % 2 == 0) ? DARKGRAY : BLACK;
                    DrawRectangle(x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, cellColor);
                }
            }

            // Draw the fruit
            DrawCircleV((Vector2){fruit.position.x + SQUARE_SIZE / 2, fruit.position.y + SQUARE_SIZE / 2}, SQUARE_SIZE / 2, RED);

            // Draw the snake
            for (int i = 0; i < snakeLength; i++)
            {
                
            if (i == 0)
                DrawCircleV((Vector2){snake[i].position.x + SQUARE_SIZE / 2, snake[i].position.y + SQUARE_SIZE / 2}, SQUARE_SIZE / 2, GREEN);
            else
            {
                Color bodyColor = (Color){0, 255 - i * 5, 0, 255};
                DrawCircleV((Vector2){snake[i].position.x + SQUARE_SIZE / 2, snake[i].position.y + SQUARE_SIZE / 2}, SQUARE_SIZE / 2, bodyColor);
            }
            }

            // Draw Eyes (only when game is not over)
            if (!gameOver)
            {
                float eyeSize = 8;
                Vector2 eyeOffset1 = {0, 0};
                Vector2 eyeOffset2 = {0, 0};
                Vector2 mouthStart = {0, 0};
                Vector2 mouthEnd = {0, 0};

                // Determine eye and mouth positions based on movement direction
                if (snake[0].speed.x > 0)
                { // Moving Right
                    eyeOffset1 = (Vector2){18, 8};
                    eyeOffset2 = (Vector2){18, 22};
                    mouthStart = (Vector2){28, 15};
                    mouthEnd = (Vector2){28, 20};
                }
                else if (snake[0].speed.x < 0)
                { // Moving Left
                    eyeOffset1 = (Vector2){8, 8};
                    eyeOffset2 = (Vector2){8, 22};
                    mouthStart = (Vector2){2, 15};
                    mouthEnd = (Vector2){2, 20};
                }
                else if (snake[0].speed.y > 0)
                { // Moving Down
                    eyeOffset1 = (Vector2){8, 18};
                    eyeOffset2 = (Vector2){22, 18};
                    mouthStart = (Vector2){14, 28};
                    mouthEnd = (Vector2){18, 28};
                }
                else if (snake[0].speed.y < 0)
                { // Moving Up
                    eyeOffset1 = (Vector2){8, 8};
                    eyeOffset2 = (Vector2){22, 8};
                    mouthStart = (Vector2){14, 2};
                    mouthEnd = (Vector2){18, 2};
                }

                // Draw eyes
                DrawCircle(snake[0].position.x + eyeOffset1.x, snake[0].position.y + eyeOffset1.y, eyeSize, WHITE);
                DrawCircle(snake[0].position.x + eyeOffset2.x, snake[0].position.y + eyeOffset2.y, eyeSize, WHITE);
                DrawCircle(snake[0].position.x + eyeOffset1.x + 1, snake[0].position.y + eyeOffset1.y, eyeSize / 2, BLACK);
                DrawCircle(snake[0].position.x + eyeOffset2.x + 1, snake[0].position.y + eyeOffset2.y, eyeSize / 2, BLACK);

                // Draw mouth
                DrawLineEx((Vector2){snake[0].position.x + mouthStart.x, snake[0].position.y + mouthStart.y},
                           (Vector2){snake[0].position.x + mouthEnd.x, snake[0].position.y + mouthEnd.y},
                           3, BLACK);
            }

            // Draw rounded border around the game area
            DrawRectangleLinesEx((Rectangle){0, 0, screenWidth, screenHeight}, 3, BLACK);

            // Draw the score
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
            DrawText(TextFormat("Best Score: %d", bestScore), 10, 40, 20, GOLD); // Display Best Score

            // Game Over Screen
            if (gameOver)
            {
                DrawText("GAME OVER", screenWidth / 2 - 120, screenHeight / 2 - 20, 40, RED);
                DrawText("Press R to Restart", screenWidth / 2 - 100, screenHeight / 2 + 20, 20, WHITE);
            }
            else if (!gameStarted)
            {
                // Show Welcome Message when the game starts
                DrawText("Welcome to Snake Game!", screenWidth / 2 -250 , screenHeight / 2 - 20, 40, YELLOW);
                DrawText("Use arrow keys to play", screenWidth / 2 - 130, screenHeight / 2 + 20, 20, WHITE);
            }
        }

        EndDrawing();
    }

    // Stop music and close the audio device
    StopMusicStream(music);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
