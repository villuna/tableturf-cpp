#include "server.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <stdexcept>
#include <thread>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void usage() {
    std::cerr << "usage: tableturf-server [NUM_THREADS]" << std::endl;
    exit(1);
}

int main(int argc, char** argv) {
    int threads = 1;

    if (argc == 2) {
        try {
            threads = std::stoi(argv[1]);
        } catch (std::invalid_argument& e) {
            usage();
        }

        if (threads <= 0) {
            std::cerr << "number of threads must be at least 1" << std::endl;
            return 1;
        } if ((unsigned int)threads > std::thread::hardware_concurrency()) {
            // If the user tries to use more threads than the hardware supports, it will do nothing
            // so limit the number of threads to save on overhead
            threads = std::thread::hardware_concurrency();
        }
    }

    std::cout << "Running tableturf server on localhost port 2611" << std::endl;

    asio::io_context ctx;
    Server server(ctx);

    std::vector<std::thread> thread_handles;

    for (int i = 0; i < threads - 1; i++) {
        thread_handles.push_back(std::thread {[&] {ctx.run();}});
    }

    ctx.run();

    for (auto i = thread_handles.begin(); i != thread_handles.end(); i++) {
        i->join();
    }

    return 0;
}
