#include "raylib.h"
#include <math.h> // For sin function for animation

// Function to calculate a point on a quadratic Bézier curve
Vector2 CalculateQuadraticBezierPoint(Vector2 start, Vector2 control, Vector2 end, float t) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;

    Vector2 point = {
        (uu * start.x) + (2 * u * t * control.x) + (tt * end.x),
        (uu * start.y) + (2 * u * t * control.y) + (tt * end.y)
    };

    return point;
}

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Facial Expression Display");

    // Define emotions
    typedef enum {
        EMOTION_HAPPY,
        EMOTION_SAD,
        EMOTION_ANGRY
    } Emotion;

    Emotion currentEmotion = EMOTION_HAPPY; // Default emotion

    // Animation variables
    float blinkTimer = 0.0f; // Timer for blinking eyes
    float blinkDuration = 0.5f; // Duration of a blink (0.5 seconds)
    float blinkInterval = 3.0f; // Time between blinks
    bool isBlinking = false; // Whether the eyes are currently blinking

    float mouthAnimationTimer = 0.0f; // Timer for mouth animation
    float mouthAnimationSpeed = 4.0f; // Speed of mouth animation

    // Thickness of all face parts
    const float faceThickness = 5.0f;

    // Eye size
    const float eyeRadius = 10.0f; // Bigger eyes

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Function to handle emotion change
    void changeExpression() {
        if (IsKeyPressed(KEY_H)) {
            currentEmotion = EMOTION_HAPPY;
        } else if (IsKeyPressed(KEY_S)) {
            currentEmotion = EMOTION_SAD;
        } else if (IsKeyPressed(KEY_A)) {
            currentEmotion = EMOTION_ANGRY;
        }
    }

    // Main game loop
    while (!WindowShouldClose()) {
        // Handle user input
        changeExpression();

        // Update blink animation
        blinkTimer += GetFrameTime();
        if (blinkTimer >= blinkInterval) {
            isBlinking = true;
            blinkTimer = 0.0f; // Reset timer
        }
        if (isBlinking && blinkTimer >= blinkDuration) {
            isBlinking = false;
        }

        // Update mouth animation
        mouthAnimationTimer += GetFrameTime();
        float mouthOffset = sinf(mouthAnimationTimer * mouthAnimationSpeed) * 5.0f; // Smooth mouth movement

        // Begin drawing
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Face position
            int faceX = screenWidth / 2;
            int faceY = screenHeight / 2;

            // Draw eyes with blinking animation
            if (isBlinking) {
                // Draw closed eyes ("---" with the same thickness as the dot eyes)
                DrawLineEx((Vector2){faceX - 80, faceY - 50}, (Vector2){faceX - 40, faceY - 50}, faceThickness, BLACK); // Left eye
                DrawLineEx((Vector2){faceX + 40, faceY - 50}, (Vector2){faceX + 80, faceY - 50}, faceThickness, BLACK); // Right eye
            } else {
                // Draw open eyes (bigger circles)
                DrawCircle(faceX - 60, faceY - 50, eyeRadius, BLACK); // Left eye
                DrawCircle(faceX + 60, faceY - 50, eyeRadius, BLACK); // Right eye
            }

            // Draw mouth based on emotion with animation
            switch (currentEmotion) {
                case EMOTION_HAPPY: {
                    // Smile (quadratic Bézier curve)
                    Vector2 start = {faceX - 80, faceY + 50 + mouthOffset};
                    Vector2 control = {faceX, faceY + 100 + mouthOffset};
                    Vector2 end = {faceX + 80, faceY + 50 + mouthOffset};

                    for (float t = 0; t <= 1; t += 0.01) {
                        Vector2 point = CalculateQuadraticBezierPoint(start, control, end, t);
                        DrawCircleV(point, faceThickness, BLACK); // Draw small circles to form the curve
                    }
                    break;
                }
                case EMOTION_SAD: {
                    // Frown (quadratic Bézier curve)
                    Vector2 start = {faceX - 80, faceY + 100 + mouthOffset};
                    Vector2 control = {faceX, faceY + 50 + mouthOffset};
                    Vector2 end = {faceX + 80, faceY + 100 + mouthOffset};

                    for (float t = 0; t <= 1; t += 0.01) {
                        Vector2 point = CalculateQuadraticBezierPoint(start, control, end, t);
                        DrawCircleV(point, faceThickness, BLACK); // Draw small circles to form the curve
                    }
                    break;
                }
                case EMOTION_ANGRY: {
                    // Angry mouth (downward-curved frown)
                    Vector2 start = {faceX - 80, faceY + 100 + mouthOffset};
                    Vector2 control = {faceX, faceY + 50 + mouthOffset};
                    Vector2 end = {faceX + 80, faceY + 100 + mouthOffset};

                    for (float t = 0; t <= 1; t += 0.01) {
                        Vector2 point = CalculateQuadraticBezierPoint(start, control, end, t);
                        DrawCircleV(point, faceThickness, BLACK); // Draw small circles to form the curve
                    }

                    // Angry eyebrows (downward-curved)
                    Vector2 leftEyebrowStart = {faceX - 100, faceY - 70};
                    Vector2 leftEyebrowControl = {faceX - 70, faceY - 100};
                    Vector2 leftEyebrowEnd = {faceX - 40, faceY - 70};

                    Vector2 rightEyebrowStart = {faceX + 40, faceY - 70};
                    Vector2 rightEyebrowControl = {faceX + 70, faceY - 100};
                    Vector2 rightEyebrowEnd = {faceX + 100, faceY - 70};

                    for (float t = 0; t <= 1; t += 0.01) {
                        Vector2 leftPoint = CalculateQuadraticBezierPoint(leftEyebrowStart, leftEyebrowControl, leftEyebrowEnd, t);
                        Vector2 rightPoint = CalculateQuadraticBezierPoint(rightEyebrowStart, rightEyebrowControl, rightEyebrowEnd, t);
                        DrawCircleV(leftPoint, faceThickness, BLACK); // Left eyebrow
                        DrawCircleV(rightPoint, faceThickness, BLACK); // Right eyebrow
                    }
                    break;
                }
            }
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}