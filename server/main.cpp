#include "server.hpp"
#include <algorithm>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <vector>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main(int argc, char** argv) {
    int threads = std::thread::hardware_concurrency();

    if (argc == 2) {
        int t = std::stoi(argv[1]);
        threads = std::clamp(t, 1, threads);
    }

    std::cout << "Running tableturf server on localhost port 2611 (" << threads << " thread(s))" << std::endl;

    asio::io_context ctx;
    Server server(ctx);

    std::vector<std::thread> thread_pool;

    for (int i = 0; i < threads; i++) {
        thread_pool.push_back(std::thread([&ctx]() {
            ctx.run();
        }));
    }

    for (auto thread = thread_pool.begin(); thread != thread_pool.end(); thread++) {
        thread->join();
    }

    return 0;
}
