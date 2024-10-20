#include "server.hpp"
#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main(int argc, char** argv) {
    std::cout << "Hello, server!" << std::endl;

    asio::io_context ctx;
    Server server(ctx);

    ctx.run();

    return 0;
}
