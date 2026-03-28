# SAMP Discord Rich Presence

SA:MP Discord Rich Presence plugin

## How to add your Server Logo?

- Create an issue and upload the logo with dimensions between 512x512 to 1024x1024 in a supported format: `png`, `jpg`, `jpeg`.

## How to Build

- You can use any C++ Compiler.
- Make sure to link discord-rpc libary against your compiler. I have included the precompiled library for MinGW and MSVC in the `src/discord-rpc` folder.
   - `discord-rpc.lib` is for MSVC
   - `libdiscord-rpc.a` is for MinGW
- You can also compile the static discord-rpc library yourself, from here [discord-rpc](https://github.com/discordapp/discord-rpc).

```bash
make
make without-static # if you don't want to have standard runtime libraries to be statically linked! but require Visual C++ Redistributable.
# or
mingw32-make
mingw32-make without-static # if you don't want to have standard runtime libraries to be statically linked! but require Visual C++ Redistributable.
```

