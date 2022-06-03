# ah3

alienhook (version 3) for Counter-Strike: Global Offensive

Written in ANSI C (C89).

## Features

* Configs in JSON
* Simple GUI
* Legitbot
* Backtrack
* Glow
* Chams
* Visuals
  * Disable fog
  * Disable post-processing
  * Disable shadows
  * Force crosshair (for sniper rifles)
  * Simple grenade prediction
  * Opposite hand knife
  * Override FOV
  * Remove 3D skybox
  * Reveal money
  * Reveal radar
  * Reveal ranks
  * Viewmodel changer
* Misc
  * Anti AFK kick
  * Bunnyhop
  * Clantag changer
  * Fast duck
  * Slidewalk

## Installing dependencies

### Arch (Artix, Manjaro, etc.)

```
sudo pacman -S git make gcc cjson sdl2 glew
```

### Debian (Ubuntu, etc.)

```
sudo apt install git make gcc libcjson-dev libsdl2-dev libglew-dev
```

### Fedora

```
sudo dnf install glibc-devel cjson-devel sdl2-devel glew-devel
```

## Downloading

```
git clone --depth 1 https://git.debooger.xyz/debooger/ah3.git
```

## Building

```
cd ah3
make
```

## Loading

> If the cheat is already loaded it will get reloaded automatically.

```
sudo ./ah-load.sh
```

## Unloading

```
sudo ./ah-unload.sh
```

## Updating

```
git pull
make
```
