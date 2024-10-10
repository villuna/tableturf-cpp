# Tableturf (C++ edition)

A recreation of tableturf from splatoon 3. This is a project I'm starting to learn both C++ and how to make a multiplayer game client/server.

## Building

Note this program is still completely unfinished so the results might be underwhelming. But this is how you build it anyway!

First you have to clone this repository, then clone the submodules so that you van build raylib:

```bash
git clone https://github.com/villuna/tableturf-cpp.git tableturf
cd tableturf
git submodule init
git submodule update
```

To build, we will need meson and cmake installed. You also need to install the Boost C++ library collection. Once that's done, just run:

```bash
mkdir build
meson setup build
meson compile -C build

# optionally you can run the unit tests like so
meson test -C build -v
```

If all goes well, the client program will be at `build/client/tableturf` and the server at `build/server/tableturf-server`
