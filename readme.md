# SAMP Discord Rich Presence

SA:MP Discord Rich Presence plugin. This is a support plugin of originally made by [Hual](https://github.com/Hual).

- Adds support to fetch the server logo directly from the http/https url with having compatibility with old discord portal server logos.
- Adds a log file for better debugging.

## How to add your Server Logo?

- Create an issue and upload the logo with dimensions between 512x512 to 1024x1024 in a supported format: `png`, `jpg`, `jpeg`.

**Note: I'm not going to accept the PR which adds their server logo url into `assets/custom-logos.txt` due to security and privacy concerns.**

## Preview:
![Screenshot](https://i.imgur.com/PqvWFbp.png)
![Screenshot](https://i.imgur.com/CXh1hDT.png)
![Screenshot](https://i.imgur.com/khG9OZe.png)

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

## Credits

- [Hual](https://github.com/Hual) for creating the original plugin.

