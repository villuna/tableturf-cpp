#pragma once
#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <variant>
#include <vector>
#include "client.hpp"
#include "config.hpp"

class Game;
class GameState;

namespace trans {
    struct None {};
    struct Exit {};
    struct Pop {};
    struct Push {
        std::unique_ptr<GameState> state;
    };
    struct Swap {
        std::unique_ptr<GameState> state;
    };
}

// A variant that describes the different ways to transition between game states
// The game state is implemented as a stack of states. Every frame, the game updates the state
// and decides whether to change to a different state. These are the possible transitions.
typedef std::variant<
    // None - stay on this state in the next frame
    trans::None,
    // Exit - Immediately close the game
    trans::Exit,
    // Pop - Remove this state from the stack and go to the previous one
    trans::Pop,
    // Push - Go to a new state, saving this one on the stack
    trans::Push,
    // Swap - Remove this state from the top of the stack and replace it with a new one
    trans::Swap
> StateTransition;

class GameState {
protected:
    Game& parent; 
public:
    GameState(Game& parent) : parent(parent) {}
    virtual ~GameState() {}
    virtual StateTransition update() { return trans::None{}; }
    virtual void draw() { }
};

class Game {
// These declarations are in a weird order because of initialisation order requirements.
public:
    std::deque<ServerMessage> messages;
private:
    asio::io_context ctx;
    std::vector<std::unique_ptr<GameState>> state;
public:
    Config config;
    Client client;

    Game(int argc, char* argv[]);
    // No copying and no moving. We only need one, and we need to make sure that the GameState
    // can hold a reference to this without it being unsafe.
    Game(const Game&) = delete;
    Game& operator=(const Game& other) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&& other) = delete;

    void run();
};
