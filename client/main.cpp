#include <exception>
#include <iostream>
#include <print>
#include "channel.hpp"
#include "client.hpp"
#include "protocol.hpp"
#include "raylib.h"
#include <boost/asio.hpp>
#include <thread>

namespace asio = boost::asio;

int main(int argc, char** argv) {
    asio::io_context ctx;
    auto chan = channel<ServerMessage>();
    Reciever<ServerMessage> rx = chan.first;
    Client c(chan.second, ctx);

    // Use this thread to run async read and write operations to the server so that the main thread
    // can handle running the game
    //std::thread async_thread = std::thread([&ctx]() {
    //    try {
    //        ctx.run(); 
    //    } catch (const std::exception& e) {
    //        std::cerr << "Error running async context: " << e.what() << std::endl;
    //    }
    //});

    InitWindow(800, 400, "hello raylib cpp");

    double target_fps = 60.0;
    int frame_micros = static_cast<int>((1.0 / target_fps) * 1000000.0);
    asio::steady_timer frame_timer(ctx, std::chrono::microseconds(frame_micros));

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_C)) {
            bool online = c.connect();

            if (!online) {
                std::cout << "Connecting now..." << std::endl;
            } else {
                std::cout << "Was already online, hold ya horses" << std::endl;
            }
        }

        if (IsKeyPressed(KEY_S)) {
            // This is probably a bad idea and i'll like set up a messag equeue in a bit
            c.send_message(HelloServer { .info = PublicPlayerInfo { .name = "villuna" }});
        }

        std::optional<ServerMessage> msg = rx.recv();

        if (msg.has_value()) {
            std::cout << "Recieved!: " << server_message_to_json(*msg) << std::endl;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        DrawText("Press C to connect to the server!", 200, 200, 20, LIGHTGRAY);

        EndDrawing();

        ctx.poll();
        ctx.restart();
        frame_timer.wait();
        frame_timer.expires_at(frame_timer.expiry() + std::chrono::microseconds(frame_micros));
    }

    ctx.stop();

    return 0;
}
