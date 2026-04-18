#include <cctype>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "../Display/HUD.h"
#include "../Scroll/Finder.h"
#include "../Scroll/Scrolls.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

using namespace std;

namespace {

string normalizeInput(const string& input) {
    if (input.empty()) {
        return "";
    }

    string lowered = input;
    for (char& ch : lowered) {
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }
    return lowered;
}

string gameBanner() {
    ostringstream out;
    out << "************************\n"
        << "*  Soul Rock v. 1.0    *\n"
        << "*  By: Trevor Natiuk   *\n"
        << "*      Kaelan Willauer *\n"
        << "*      Noah Griffith   *\n"
        << "*      Jose Hernandez  *\n"
        << "************************\n\n";
    return out.str();
}

class SoulRockWebGame {
    public:
        const string& reset() {
            mode = ENTER_NAME;
            hud.reset();
            playerName.clear();
            seed = 0;
            promptMessage.clear();
            modalMessage.clear();
            activeScroll = nullptr;
            activeFinder = nullptr;
            pendingPosition = make_pair(0, 0);
            pendingScrollFlag = OBT_WHTSCR;
            pendingStoneFlag = OBT_WHTSTN;
            screen = render();
            return screen;
        }

        const string& submit(const string& input) {
            const string normalized = normalizeInput(input);

            switch (mode) {
                case ENTER_NAME:
                    handleName(input);
                    break;
                case ENTER_SEED:
                    handleSeed(input);
                    break;
                case PLAYING:
                    handlePlayCommand(normalized);
                    break;
                case SHOWING_INVENTORY:
                case SHOWING_FINDER:
                    modalMessage.clear();
                    mode = PLAYING;
                    break;
                case ANSWERING_RIDDLE:
                    handleRiddleAnswer(input);
                    break;
                case GAME_WON:
                    break;
            }

            screen = render();
            return screen;
        }

        const string& getScreen() {
            screen = render();
            return screen;
        }

        bool isGameWon() const {
            return mode == GAME_WON;
        }

        const string& getPlayerName() const {
            return playerName;
        }

        int getSteps() const {
            if (!hud) {
                return 0;
            }
            return hud->player.getSteps();
        }

        int getSeed() const {
            return seed;
        }

        int getMode() const {
            return static_cast<int>(mode);
        }

    private:
        enum Mode {
            ENTER_NAME,
            ENTER_SEED,
            PLAYING,
            SHOWING_INVENTORY,
            SHOWING_FINDER,
            ANSWERING_RIDDLE,
            GAME_WON
        };

        Mode mode = ENTER_NAME;
        unique_ptr<HUD> hud;
        pair<int, int> playerPos = make_pair(0, 0);
        string playerName;
        int seed = 0;
        string promptMessage;
        string modalMessage;
        Scroll* activeScroll = nullptr;
        Finder* activeFinder = nullptr;
        pair<int, int> pendingPosition = make_pair(0, 0);
        Checkpoint pendingScrollFlag = OBT_WHTSCR;
        Checkpoint pendingStoneFlag = OBT_WHTSTN;
        string screen;

        void handleName(const string& input) {
            playerName = input.empty() ? "Anonymous" : input;
            mode = ENTER_SEED;
        }

        void handleSeed(const string& input) {
            istringstream in(input);
            int parsedSeed = 0;
            in >> parsedSeed;
            if (!in || !in.eof()) {
                promptMessage = "Number please!";
                return;
            }

            seed = parsedSeed;
            srand(seed);
            hud = make_unique<HUD>();
            hud->updateBoard();
            playerPos = hud->player.getPosition();
            promptMessage = "Use w/a/s/d, y, e, and x. Press Enter after each command.";
            mode = PLAYING;
        }

        void handlePlayCommand(const string& input) {
            if (!hud) {
                return;
            }

            promptMessage.clear();
            if (input.empty()) {
                return;
            }

            const char choice = input[0];
            switch (choice) {
                case 'w':
                case 'a':
                case 's':
                case 'd':
                    hud->player.move(choice);
                    hud->updateBoard();
                    playerPos = hud->player.getPosition();
                    break;
                case 'y':
                    if (!hud->isOccupied(playerPos)) {
                        promptMessage = "Nothing to pick up here.";
                        return;
                    }
                    beginPickup();
                    break;
                case 'e':
                    mode = SHOWING_INVENTORY;
                    break;
                case 'x':
                    if (isAtAltar() && hud->hasAllStones()) {
                        mode = GAME_WON;
                    } else {
                        promptMessage = "You can only finish at the altar after claiming the orange stone.";
                    }
                    break;
                default:
                    promptMessage = "Unknown command. Try w, a, s, d, y, e, or x.";
                    break;
            }

            if (hud) {
                playerPos = hud->player.getPosition();
            }
        }

        void beginPickup() {
            Map* map = hud->getMapPtr();
            Player* player = hud->getPlayerPtr();
            const Checkpoint flag = map->getItem(playerPos, false);
            pendingPosition = playerPos;

            switch (flag) {
                case OBT_WHTSCR:
                    startRiddle(map->getScroll(WHITE), OBT_WHTSCR, OBT_WHTSTN);
                    return;
                case OBT_BLKSCR:
                    startRiddle(map->getScroll(BLACK), OBT_BLKSCR, OBT_BLKSTN);
                    return;
                case OBT_ORGSCR:
                    if (!player->hasRequiredStones()) {
                        promptMessage = "The orange stone stays dormant until you have the other six stones.";
                        return;
                    }
                    startRiddle(map->getScroll(ORANGE), OBT_ORGSCR, OBT_ORGSTN);
                    return;
                case OBT_BLUSCR:
                    startRiddle(map->getScroll(BLUE), OBT_BLUSCR, OBT_BLUSTN);
                    return;
                case OBT_REDSCR:
                    if (!player->hasRedFinder()) {
                        promptMessage = "You need the red finder scroll before the red stone will listen.";
                        return;
                    }
                    startRiddle(map->getScroll(RED), OBT_REDSCR, OBT_REDSTN);
                    return;
                case OBT_GRNSCR:
                    startRiddle(map->getScroll(GREEN), OBT_GRNSCR, OBT_GRNSTN);
                    return;
                case OBT_PPLSCR:
                    if (map->willPurpleStoneKickBack()) {
                        player->kickBackPlayer();
                        hud->updateBoard();
                        playerPos = player->getPosition();
                        promptMessage = "The purple stone kicked you back.";
                        return;
                    }
                    startRiddle(map->getScroll(PURPLE), OBT_PPLSCR, OBT_PPLSTN);
                    return;
                case OBT_WHTFDR:
                    revealFinder(map->getFinder(WHITE), OBT_WHTFDR);
                    return;
                case OBT_ORGFDR:
                    revealFinder(map->getFinder(ORANGE), OBT_ORGFDR);
                    return;
                case OBT_REDFDR:
                    revealFinder(map->getFinder(RED), OBT_REDFDR);
                    return;
                case OBT_GRNFDR:
                    revealFinder(map->getFinder(GREEN), OBT_GRNFDR);
                    return;
                case OBT_BLKFDR:
                    revealFinder(map->getFinder(BLACK), OBT_BLKFDR);
                    return;
                case OBT_BLUFDR:
                    revealFinder(map->getFinder(BLUE), OBT_BLUFDR);
                    return;
                case OBT_PPLFDR:
                    revealFinder(map->getFinder(PURPLE), OBT_PPLFDR);
                    return;
                default:
                    promptMessage = "That item did not respond the way I expected.";
                    return;
            }
        }

        void startRiddle(Scroll* scroll, Checkpoint scrollFlag, Checkpoint stoneFlag) {
            activeScroll = scroll;
            pendingScrollFlag = scrollFlag;
            pendingStoneFlag = stoneFlag;
            modalMessage.clear();
            mode = ANSWERING_RIDDLE;
        }

        void revealFinder(Finder* finder, Checkpoint flag) {
            activeFinder = finder;
            hud->player.setCheckpoint(flag);
            hud->getMapPtr()->removeItem(playerPos);
            hud->updateBoard();
            mode = SHOWING_FINDER;
        }

        void handleRiddleAnswer(const string& input) {
            if (!activeScroll) {
                mode = PLAYING;
                return;
            }

            if (!activeScroll->checkAnswer(input)) {
                modalMessage = "Incorrect. Try again.";
                return;
            }

            hud->player.setCheckpoint(pendingScrollFlag);
            hud->player.setCheckpoint(pendingStoneFlag);
            hud->getMapPtr()->removeItem(pendingPosition);
            hud->updateBoard();
            playerPos = hud->player.getPosition();
            activeScroll = nullptr;
            modalMessage = "Correct!";
            promptMessage = "You claimed the stone.";
            mode = PLAYING;
        }

        bool isAtAltar() const {
            return playerPos.first == 0 && playerPos.second == 0;
        }

        string renderInventory() const {
            ostringstream out;
            out << "Inventory:\n";
            if (!hud || hud->player.invSize() == 0) {
                out << "- Empty\n";
            } else {
                for (int i = 0; i < hud->player.invSize(); ++i) {
                    out << "- " << hud->player.displayInventory(i) << "\n";
                }
            }
            out << "\nPress Enter to continue.\n>> ";
            return out.str();
        }

        string renderFinder() const {
            ostringstream out;
            if (!modalMessage.empty()) {
                out << modalMessage << "\n\n";
            }
            if (activeFinder) {
                out << activeFinder->displayHint() << "\n\n";
            }
            out << "Press Enter to continue.\n>> ";
            return out.str();
        }

        string renderRiddle() const {
            ostringstream out;
            if (!modalMessage.empty()) {
                out << modalMessage << "\n\n";
            }
            out << "Q: ";
            if (activeScroll) {
                out << activeScroll->showRiddle();
            }
            out << "\n>> ";
            return out.str();
        }

        string renderGameWon() const {
            ostringstream out;
            out << gameBanner()
                << "You escaped the labyrinth.\n\n"
                << "Player: " << playerName << "\n"
                << "Seed: " << seed << "\n"
                << "Steps: " << getSteps() << "\n\n"
                << "Your score is ready for the browser leaderboard below.\n";
            return out.str();
        }

        string render() const {
            ostringstream out;
            switch (mode) {
                case ENTER_NAME:
                    out << gameBanner();
                    if (!promptMessage.empty()) {
                        out << promptMessage << "\n\n";
                    }
                    out << "Enter username:\n>> ";
                    break;
                case ENTER_SEED:
                    out << gameBanner();
                    if (!promptMessage.empty()) {
                        out << promptMessage << "\n\n";
                    }
                    out << "Enter seed:\n>> ";
                    break;
                case PLAYING:
                    out << hud->render(playerPos);
                    if (isAtAltar() && hud->hasAllStones()) {
                        out << "You are at the altar! Enter 'x' to return stones.\n";
                    } else if (hud->isOccupied(playerPos)) {
                        out << "There is an item here. Enter 'y' to pick up.\n";
                    }
                    if (!promptMessage.empty()) {
                        out << promptMessage << "\n";
                    }
                    out << ">> ";
                    break;
                case SHOWING_INVENTORY:
                    out << renderInventory();
                    break;
                case SHOWING_FINDER:
                    out << renderFinder();
                    break;
                case ANSWERING_RIDDLE:
                    out << renderRiddle();
                    break;
                case GAME_WON:
                    out << renderGameWon();
                    break;
            }
            return out.str();
        }
};

SoulRockWebGame& game() {
    static SoulRockWebGame instance;
    return instance;
}

string outputBuffer;

const char* storeScreen(const string& value) {
    outputBuffer = value;
    return outputBuffer.c_str();
}

} // namespace

extern "C" {

EMSCRIPTEN_KEEPALIVE const char* sr_reset() {
    return storeScreen(game().reset());
}

EMSCRIPTEN_KEEPALIVE const char* sr_submit(const char* input) {
    return storeScreen(game().submit(input == nullptr ? "" : input));
}

EMSCRIPTEN_KEEPALIVE const char* sr_get_screen() {
    return storeScreen(game().getScreen());
}

EMSCRIPTEN_KEEPALIVE int sr_is_game_won() {
    return game().isGameWon() ? 1 : 0;
}

EMSCRIPTEN_KEEPALIVE const char* sr_get_player_name() {
    return storeScreen(game().getPlayerName());
}

EMSCRIPTEN_KEEPALIVE int sr_get_steps() {
    return game().getSteps();
}

EMSCRIPTEN_KEEPALIVE int sr_get_seed() {
    return game().getSeed();
}

EMSCRIPTEN_KEEPALIVE int sr_get_mode() {
    return game().getMode();
}

}
