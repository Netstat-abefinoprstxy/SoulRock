CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I.
TARGET := soulrock
SOURCES := SoulRock.cpp Display/*.cpp Player/*.cpp Scroll/*.cpp Stone/*.cpp
WEB_DIST := dist
WEB_SOURCES := web/SoulRockWeb.cpp Display/*.cpp Player/*.cpp Scroll/*.cpp Stone/*.cpp
WEB_EXPORTS := -sEXPORTED_FUNCTIONS='["_malloc","_free","_sr_reset","_sr_submit","_sr_get_screen","_sr_is_game_won","_sr_get_player_name","_sr_get_steps","_sr_get_seed","_sr_get_mode"]'
WEB_RUNTIME := -sEXPORTED_RUNTIME_METHODS='["ccall"]'

.PHONY: all run clean web web-assets

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	./$(TARGET)

web: web-assets
	em++ -std=c++17 -O2 -I. $(WEB_SOURCES) $(WEB_EXPORTS) $(WEB_RUNTIME) -sWASM=1 -o $(WEB_DIST)/soulrock.js

web-assets:
	mkdir -p $(WEB_DIST)
	cp web/index.html web/app.js web/styles.css web/.nojekyll $(WEB_DIST)/

clean:
	rm -f $(TARGET)
	rm -rf $(WEB_DIST)
