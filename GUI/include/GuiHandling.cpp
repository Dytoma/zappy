/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** GuiHandling
*/

#include "Client.hpp"
#include "Utils.hpp"


void Client::DrawPlayerBBox()
{
    if (_selectedPlayer) {
        Vector2 gridPos = _selectedPlayer->getPosition();
        Vector3 center = {
            gridPos.x * TILESIZE + _map->_offsetX,
            0.5f,
            gridPos.y * TILESIZE + _map->_offsetZ
        };
        float playerSize = 0.5f;
        BoundingBox playerBox;
        playerBox.min = {center.x - playerSize, 0.0f, center.z - playerSize};
        playerBox.max = {center.x + playerSize, 2.0f, center.z + playerSize};
        DrawBoundingBox(playerBox, RED);
    }
}

void Client::setSelectedPlayer(const Ray &ray)
{
    for (const auto& player : _players) {
        Vector2 gridPos = player->getPosition();
        Vector3 center = {
            gridPos.x * TILESIZE + _map->_offsetX,
            0.5f,
            gridPos.y * TILESIZE + _map->_offsetZ
        };
        float playerSize = 0.5f;
        BoundingBox playerBox;
        playerBox.min = {center.x - playerSize, 0.0f, center.z - playerSize};
        playerBox.max = {center.x + playerSize, 2.0f, center.z + playerSize};
        if (MyCheckCollisionRayBox(ray, playerBox)) {
            _selectedPlayer = player;
            break;
        }
    }
    if (!_selectedPlayer) {
        for (size_t i = 0; i < _map->_grid.size(); ++i) {
            Vector3 cubeCenter = {_map->_grid[i]->GetPosition().x, 0.5f, _map->_grid[i]->GetPosition().y};
            float cubeSize = _map->_grid[i]->GetSize();
            BoundingBox box;
            box.min = {cubeCenter.x - cubeSize/2, 0.0f, cubeCenter.z - cubeSize/2};
            box.max = {cubeCenter.x + cubeSize/2, 1.0f, cubeCenter.z + cubeSize/2};
            if (MyCheckCollisionRayBox(ray, box)) {
                _map->setSelectedTile(static_cast<int>(i));
                // selectedTile = (int)i;
                break;
            }
        }
    }
}

void Client::handleGameOver(const int activeCameraIndex)
{
    if (_isGameOver) {
        _cameras[activeCameraIndex].projection = CAMERA_PERSPECTIVE;

        int width = GetScreenWidth();
        int height = GetScreenHeight();
        Rectangle quitButtonBounds = endWindow(width, height);

        if (CheckCollisionPointRec(GetMousePosition(), quitButtonBounds)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            _isRunning = false;
            stop();
            CloseWindow();
            // exit(0);
        }
    }
}

void Client::handleButtonEvents()
{
    DrawRectangleRec(_incBtn, LIGHTGRAY);
    DrawText("+", _incBtn.x + 13, _incBtn.y + 8, 30, BLACK);
    DrawRectangleRec(_decBtn, LIGHTGRAY);
    DrawText("-", _decBtn.x + 13, _decBtn.y + 8, 30, BLACK);

    DrawText(("Current tick: " + std::to_string(timeUnit)).c_str(), 130, 40, 20, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), _incBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        timeUnit += 1;
        std::string cmd = "sst " + std::to_string(timeUnit) + "\n";
        initDataToSendToServer(cmd);
        float newMoveDuration = std::max(0.05f, 1.0f / timeUnit);
        for (auto& player : _players)
            player->setMoveDuration(newMoveDuration);
    }
    if (CheckCollisionPointRec(GetMousePosition(), _decBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (timeUnit > 1) {
            timeUnit -= 1;
            std::string cmd = "sst " + std::to_string(timeUnit) + "\n";
            initDataToSendToServer(cmd);
            float newMoveDuration = std::max(0.05f, 1.0f / timeUnit);
            for (auto& player : _players)
                player->setMoveDuration(newMoveDuration);
        }
    }
}

void Client::handlePlayerInventory(const int activeCameraIndex)
{
    if (_selectedPlayer) {
        Vector2 screenPos = GetWorldToScreen(_selectedPlayer->get3DPos(), _cameras[activeCameraIndex]);

        const int inventoryWidth = 200;
        const int inventoryHeight = 300;

        screenPos.x += 90;
        screenPos.y -= 250;

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        if (screenPos.x + inventoryWidth > screenWidth) {
            screenPos.x = screenPos.x - inventoryWidth - 380;
        }
        if (screenPos.x < 0) {
            screenPos.x = 0;
        }
        if (screenPos.y < 0) {
            screenPos.y = 0;
        }
        if (screenPos.y + inventoryHeight > screenHeight) {
            screenPos.y = screenHeight - inventoryHeight;
        }
        showInventory(_selectedPlayer, screenPos, Resource::resourceTextures);
    }
}

void Client::handleKeyEvents(int &activeCameraIndex)
{
    if (IsKeyPressed(KEY_ONE)) activeCameraIndex = 0;
    if (IsKeyPressed(KEY_TWO)) activeCameraIndex = 1;
    if (IsKeyPressed(KEY_THREE)) activeCameraIndex = 2;
    if (IsKeyPressed(KEY_TAB)) {
        _cursorEnabled = !_cursorEnabled;
        if (_cursorEnabled)
            EnableCursor();
        else {
            DisableCursor();
            _map->setSelectedTile(-1);
        }
    }
    if (IsKeyPressed(KEY_T))
        _showChat = !_showChat;
    if (_broadcasting) {
        auto now = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - _broadcastingStartTime).count();

        if (elapsedSeconds < 7)
            displayChat();
        else
            _broadcasting = false;
    } else if (_showChat) {
        displayChat();
    }

    if (IsKeyPressed(KEY_I)) {
        showInfo = !showInfo;
    }

    if (showInfo) {
        _teamsInfo->Draw();
    }

}

void Client::showInventory(std::shared_ptr<Player> player, const Vector2& position,
    const std::vector<std::shared_ptr<Zappy::Texture>>& resourceTextures)
{
    static Texture2D &inventoryTexture = _resourceManager->getTexture(
        _assetsPath + "images/inventory.png")->_texture;

    DrawTexture(inventoryTexture, (int)position.x, (int)position.y, WHITE);

    std::string modelPath = player->get2DModel();
    Texture2D playerTexture = LoadTexture(modelPath.c_str());
    if (playerTexture.id > 0) {
        Vector2 playerSkinPosition = { position.x + 100, position.y + 30 };
        float scale = 0.35f;
        DrawTextureEx(playerTexture, playerSkinPosition, 0.0f, scale, WHITE);
    }

    std::vector<Type> types = player->getCleanInventory();

    const int columns = 10;
    const int cellSize = 41;
    const int offsetX = 47;
    const int offsetY = 337;

    for (size_t i = 0; i < types.size(); ++i) {
        Type type = types[i];
        Texture2D& texture = resourceTextures[type]->_texture;

        int col = i % columns;
        int row = i / columns;
        float scale = 0.55f;

        //assets
        int drawX = (int)position.x + offsetX + col * cellSize;
        int drawY = (int)position.y + offsetY + row * cellSize;
        DrawTextureEx(texture, { (float)drawX, (float)drawY }, 0.0f, scale, WHITE);

        //text
        std::string countText = std::to_string(player->ressourceCount(type));
        int fontSize = 17;
        int textWidth = MeasureText(countText.c_str(), fontSize);
        int textX = drawX + 36 - textWidth - 3;
        int textY = drawY + 20;
        DrawText(countText.c_str(), textX, textY, fontSize, WHITE);

    }
}

void Client::displayChat()
{
    constexpr int maxMessages = 7;
    constexpr int lineHeight = 35;
    constexpr int padding = 30;
    constexpr int fontSize = 20;

    size_t messageCount = std::min(_messages.size(), static_cast<size_t>(maxMessages));
    if (messageCount == 0)
        return;

    int chatWidth = 550;
    int chatHeight = maxMessages * lineHeight + 2 * padding;
    int chatX = 20;
    int chatY = GetScreenHeight() - chatHeight - 200;

    DrawRectangle(chatX, chatY, chatWidth, chatHeight, Fade(BLACK, 0.5f));

    size_t startIdx = _messages.size() > maxMessages ? _messages.size() - maxMessages : 0;
    for (size_t i = 0; i < _messages.size(); ++i) {
        const auto& [id, msg] = _messages[startIdx + i];
        std::string text = "[Player " + std::to_string(id) + "] " + msg;

        DrawText(text.c_str(),
                 chatX + padding,
                 chatY + padding + i * lineHeight,
                 fontSize,
                 WHITE);
    }
}

Rectangle Client::endWindow(int width, int height)
{
    (void)height;
    // _map->_grid.clear();

    static Texture2D &zappyLogo = _resourceManager->getTexture(_assetsPath + "images/zappy.png")->_texture;
    static Texture2D &windowWin = _resourceManager->getTexture(_assetsPath + "images/window.png")->_texture;
    static Texture2D &buttonQuit = _resourceManager->getTexture(_assetsPath + "images/button.png")->_texture;

    int logoX = (width - zappyLogo.width) / 2;
    int logoY = 70;
    DrawTexture(zappyLogo, logoX, logoY, WHITE);


    float scale = 2.5f;
    int windowScaledWidth = (int)(windowWin.width * scale);
    int windowScaledHeight = (int)(windowWin.height * scale);

    Vector2 position = {
        (float)(width - windowScaledWidth) / 2.0f,
        300.0f
    };

    int rectX = (int)position.x + 20;
    int rectY = (int)position.y;
    int rectWidth = windowScaledWidth - 50;
    int rectHeight = windowScaledHeight - 300;
    DrawRectangle(rectX, rectY, rectWidth, rectHeight, Color{0, 0, 0, 128});
    DrawTextureEx(windowWin, position, 0.0f, scale, WHITE);

    Vector2 buttonPosition =
    {
        (float)(width - (int)(windowWin.width * 1.25f)) / 2.0f,
        570.0f
    };

    DrawTextureEx(buttonQuit, buttonPosition, 0.0f, 0.25f, WHITE);

    const char* buttonText = "QUIT";
    int buttonFont = 40;
    int buttonTextWidth = MeasureText(buttonText, buttonFont);
    int buttonTextX = rectX + (rectWidth - buttonTextWidth) / 2;
    int buttonTextY = rectY + 405;

    DrawText(buttonText, buttonTextX, buttonTextY, buttonFont, WHITE);


    const char* gameOverText = "GAME OVER";
    int fontSize = 40;
    int textWidth = MeasureText(gameOverText, fontSize);
    int textX = rectX + (rectWidth - textWidth) / 2;
    int textY = rectY + 90;

    DrawText(gameOverText, textX, textY, fontSize, WHITE);

    std::string teamText = "Winning team: ";
    const char* teamTextCStr = teamText.c_str();
    int teamTextSize = 30;
    int teamTextWidth = MeasureText(teamTextCStr, teamTextSize);
    int teamTextX = rectX + (rectWidth - teamTextWidth) / 2;
    int teamTextY = textY + fontSize + 40;

    DrawText(teamTextCStr, teamTextX, teamTextY, teamTextSize, WHITE);

    std::string winText = _winningTeam;
    const char* winTextCStr = winText.c_str();
    int winTextSize = 30;
    int winTextWidth = MeasureText(winTextCStr, winTextSize);
    int winTextX = rectX + (rectWidth - winTextWidth) / 2;
    int winTextY = textY + fontSize + 80;

    DrawText(winTextCStr, winTextX, winTextY, winTextSize, RED);

    Rectangle buttonBounds = {
        buttonPosition.x,
        buttonPosition.y + 110,
        buttonQuit.width * 0.25f,
        buttonQuit.height * 0.07f
    };

    return buttonBounds;
}

void Client::DrawHUD(Tile* tile, const std::vector<std::shared_ptr<Zappy::Texture>>& resourceTextures)
{
    constexpr int numResources = 7;
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    float hudWidthRatio = 0.45f;
    int hudWidth = screenW * hudWidthRatio;
    int innerPaddingX = hudWidth * 0.012f;
    int boxGap = hudWidth * 0.005f;
    int boxesWidth = hudWidth - 2 * innerPaddingX - (numResources - 1) * boxGap;
    int boxSize = boxesWidth / numResources;
    if (boxSize < 64) boxSize = 64;
    if (boxSize > 210) boxSize = 210;

    hudWidth = 2 * innerPaddingX + numResources * boxSize + (numResources - 1) * boxGap;
    int innerPaddingY = boxSize * 0.05f;
    int hudHeight = boxSize + 2 * innerPaddingY;
    int hudX = (screenW - hudWidth) / 2;
    int hudY = screenH - hudHeight - int(screenW * 0.037f);

    DrawRectangle(hudX, hudY, hudWidth, hudHeight, Fade(DARKGRAY, 0.7f));
    DrawRectangle(hudX + 4, hudY + 4, hudWidth - 8, hudHeight - 8, Fade(DARKGRAY, 0.5f));
    DrawRectangleLines(hudX, hudY, hudWidth, hudHeight, BLACK);

    int boxStartX = hudX + innerPaddingX;
    int boxStartY = hudY + innerPaddingY;

    int imgSize = boxSize * 0.50f;
    int nameFont = boxSize * 0.16f;
    int numFont = boxSize * 0.18f;
    int spacing = (boxSize - imgSize - nameFont - numFont) / 4;

    for (int i = 0; i < numResources; ++i) {
        Type type = static_cast<Type>(i);
        int x = boxStartX + i * (boxSize + boxGap);
        int y = boxStartY;

        DrawRectangle(x, y, boxSize, boxSize, Fade(BLACK, 0.4f));
        DrawRectangleLines(x, y, boxSize, boxSize, GRAY);

        int imgY = y + spacing;
        int nameY = imgY + imgSize + spacing;
        int numY = nameY + nameFont + spacing;

        float imgScale = (float)imgSize / resourceTextures[i]->_texture.width;
        int imgX = x + (boxSize - imgSize) / 2;
        DrawTextureEx(resourceTextures[i]->_texture, {(float)imgX, (float)imgY}, 0, imgScale, WHITE);

        std::string name = tile->getResourceName(type);
        int nameW = MeasureText(name.c_str(), nameFont);
        DrawText(name.c_str(), x + (boxSize - nameW) / 2, nameY, nameFont, WHITE);

        std::string num = std::to_string(tile->getResourceCount(type));
        int numW = MeasureText(num.c_str(), numFont);
        DrawText(num.c_str(), x + (boxSize - numW) / 2, numY, numFont, LIGHTGRAY);
    }
}
