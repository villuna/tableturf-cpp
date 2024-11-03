# Tableturf (C++ edition)

A recreation of tableturf from splatoon 3. This is a project I'm starting to learn both C++ and how to make a multiplayer game client/server.

## Building

This program is still unfinished, so the results might be underwhelming. But this is how you build it anyway!

To download, you'll have to clone this repository (or just download the source code). You will also need to install the C++ [Boost](https://www.boost.org/) libraries. This is easy on linux and a bit annoying on windows (sorry).

To build, you will also need meson and cmake installed. You also need to install the Boost C++ library collection. Once that's done, just run:

```bash
mkdir build
meson setup build
meson compile -C build

# optionally you can run the unit tests like so
meson test -C build -v
```

If all goes well, the client program will be at `build/client/tableturf` and the server at `build/server/tableturf-server [NUM_THREADS]`
