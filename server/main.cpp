#include "server.hpp"
#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main(int argc, char** argv) {
    std::cout << "Running tableturf server on localhost port 2611" << std::endl;

    asio::io_context ctx;
    Server server(ctx);

    ctx.run();

    return 0;
}
