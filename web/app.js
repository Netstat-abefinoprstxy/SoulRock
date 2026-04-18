const leaderboardKey = "soulrock-browser-leaderboard";
let runtimeReady = false;
let lastRecordedWin = "";

const MODE_ENTER_NAME = 0;
const MODE_ENTER_SEED = 1;
const MODE_PLAYING = 2;
const MODE_SHOWING_INVENTORY = 3;
const MODE_SHOWING_FINDER = 4;
const MODE_ANSWERING_RIDDLE = 5;
const MODE_GAME_WON = 6;

const screenEl = document.getElementById("screen");
const formEl = document.getElementById("command-form");
const inputEl = document.getElementById("command-input");
const leaderboardEl = document.getElementById("leaderboard");
const restartButton = document.getElementById("restart-button");
const commandButtons = Array.from(document.querySelectorAll("[data-command]"));

window.Module = window.Module || {};
window.Module.onRuntimeInitialized = () => {
  runtimeReady = true;
  renderScreen(callEngine("sr_reset"));
  inputEl.focus();
};

function callEngine(fnName, value) {
  if (!runtimeReady) {
    return "Loading Soul Rock...";
  }

  if (typeof value === "string") {
    return window.Module.ccall(fnName, "string", ["string"], [value]);
  }

  return window.Module.ccall(fnName, "string", [], []);
}

function engineInt(fnName) {
  return window.Module.ccall(fnName, "number", [], []);
}

function getMode() {
  if (!runtimeReady) {
    return MODE_ENTER_NAME;
  }

  return engineInt("sr_get_mode");
}

function readLeaderboard() {
  try {
    const parsed = JSON.parse(localStorage.getItem(leaderboardKey) || "[]");
    return Array.isArray(parsed) ? parsed : [];
  } catch (error) {
    return [];
  }
}

function writeLeaderboard(entries) {
  localStorage.setItem(leaderboardKey, JSON.stringify(entries));
}

function recordWinIfNeeded() {
  if (!runtimeReady || !engineInt("sr_is_game_won")) {
    return;
  }

  const entry = {
    name: window.Module.ccall("sr_get_player_name", "string", [], []) || "Anonymous",
    seed: engineInt("sr_get_seed"),
    steps: engineInt("sr_get_steps"),
  };

  const signature = JSON.stringify(entry);
  if (signature === lastRecordedWin) {
    return;
  }

  lastRecordedWin = signature;
  const entries = readLeaderboard();
  entries.push(entry);
  entries.sort((left, right) => left.steps - right.steps);
  writeLeaderboard(entries.slice(0, 10));
  renderLeaderboard();
}

function renderLeaderboard() {
  const entries = readLeaderboard();
  leaderboardEl.innerHTML = "";

  if (entries.length === 0) {
    const empty = document.createElement("li");
    empty.textContent = "No browser scores yet.";
    leaderboardEl.appendChild(empty);
    return;
  }

  entries.forEach((entry) => {
    const item = document.createElement("li");
    item.textContent = `${entry.name} - ${entry.steps} steps - seed ${entry.seed}`;
    leaderboardEl.appendChild(item);
  });
}

function renderScreen(text) {
  screenEl.textContent = text;
  screenEl.scrollTop = screenEl.scrollHeight;
  recordWinIfNeeded();
  syncInputMode();
}

function submitCommand(value) {
  renderScreen(callEngine("sr_submit", value));
  inputEl.value = "";
  syncInputMode();
}

function syncInputMode() {
  const mode = getMode();
  const needsTypedInput = (
    mode === MODE_ENTER_NAME ||
    mode === MODE_ENTER_SEED ||
    mode === MODE_ANSWERING_RIDDLE
  );

  inputEl.readOnly = !needsTypedInput;

  if (mode === MODE_PLAYING) {
    inputEl.placeholder = "Movement is instant now. Type only if a prompt asks for text.";
    inputEl.blur();
    return;
  }

  if (mode === MODE_SHOWING_INVENTORY || mode === MODE_SHOWING_FINDER) {
    inputEl.placeholder = "Press Enter to continue.";
    inputEl.focus();
    return;
  }

  if (mode === MODE_GAME_WON) {
    inputEl.placeholder = "Game complete. Restart to play again.";
    inputEl.blur();
    return;
  }

  if (mode === MODE_ENTER_NAME) {
    inputEl.placeholder = "Enter username";
  } else if (mode === MODE_ENTER_SEED) {
    inputEl.placeholder = "Enter seed";
  } else if (mode === MODE_ANSWERING_RIDDLE) {
    inputEl.placeholder = "Type your answer and press Enter";
  }

  inputEl.focus();
}

formEl.addEventListener("submit", (event) => {
  event.preventDefault();
  submitCommand(inputEl.value.trim());
});

commandButtons.forEach((button) => {
  button.addEventListener("click", () => submitCommand(button.dataset.command));
});

restartButton.addEventListener("click", () => {
  lastRecordedWin = "";
  renderScreen(callEngine("sr_reset"));
  inputEl.value = "";
  inputEl.focus();
});

window.addEventListener("keydown", (event) => {
  if (event.metaKey || event.ctrlKey || event.altKey) {
    return;
  }

  const mode = getMode();
  const key = event.key;
  const command = key.toLowerCase();

  if (mode === MODE_PLAYING && ["w", "a", "s", "d", "y", "e", "x"].includes(command)) {
    event.preventDefault();
    submitCommand(command);
    return;
  }

  if ((mode === MODE_SHOWING_INVENTORY || mode === MODE_SHOWING_FINDER) && key === "Enter") {
    event.preventDefault();
    submitCommand("");
    return;
  }

  if (document.activeElement !== inputEl) {
    return;
  }
});

renderLeaderboard();
