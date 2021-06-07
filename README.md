# GBA Hello

This is a simple Hello World style project for the GBA and also a place to collect all of my early learning dev notes for the GBA. This project has me taking notes and learning from multiple sources:

1. [Loriak's gba tutorial](http://www.loirak.com/gameboy/gbatutor.php) - this was the first tutorial I looked at
2. [Tonc's gba tutorial](https://www.coranac.com/tonc/text/intro.htm) - this tutorial was recommended by the gbajam itch.io page and seems like it might be a more reliable source

## Table of contents

* [Project structure](#project-structure)
* [Notes](#notes)
    * [Compiler quirks](#compiler-quirks)
    * [Learning](#learning)

## Project structure

Here's a quick rundown of the different files hosted in this project:

folder | description
-------|------------
`dev\dev_tools\`  | This folder contains an archive of development tools I've found online. Archiving them in case their originally hosted webpages go missing. I've run across plenty of dead links so let's just be safe.
`dev\docs\`  | This folder contains an archive of development docs that I wanted to archive in case they suddenly vanish. I've run across plenty of dead links so let's just be safe.
`loriak\`  | This folder stores all files related to what I built from following the loriak tutorial.
`loriak\src\`  | This folder contains the source code I've written for the loriak tutorial project.
`loriak\ext\`  | This folder contains source code that I've pulled from outside of the loriak tutorial project or which I've adapted from source code outside of the project.
`loriak\src\`  | This folder contains the source code I've written for the loriak tutorial project.
`tonc\`  | This folder stores all files related to what I built from following the tonc tutorial.

## Notes

### DevKitAdv compiler quirks

The GBA compiler used for this (devkitadv) is locked on a really old version of GCC (3.2). This has a few unfortunate caveats:

1. c99 is the highest standard supported
2. `#pragma once` isn't supported

### Learning

A reverse-chronological log of little things that I learn as I go:

* it seems the difference between busyloops spinning on vsync and using interrupts is simplicity vs effective use of power. i.e. busyloops are simple but waste power, interrupts are a bit more complex but are power-safe. I should prefer interrupts going forward for the common case though small demos may be best to use busyloops (especially if running on an emulator only).
* oh dang, apparently the reason a lot of games played slower on PAL TVs back in the day is because they had a slower refresh rate and old games used to lock/synchronize to the refresh rate.
* oh dang, according to TonC, my use of u8s and u16s may be an awful idea.
* Oh dang, just started reading the tonc docs and it seems like starting with loriak's stuff may have been a mistake. There is a not so subtle suggestion that most tutorials are bunk and to be extra mindful of the bitmap mode tutorials
* Tried to use the VDraw status register value to vsync and that didn't seem to work nearly as well as the waiting on the vcount register. I wonder why? I wonder if using interrupts is even better?
* GBA is an ARM7TDMI processor and the docs for that are still readily available online! yay!
* UGH. span types are a freaking mess in straight C (99, 11, otherwise). Lack of templates is a huge pain.
* When I googled for resources I found [loirak's tutorial](http://www.loirak.com/gameboy/gbatutor.php) so that's what a lot of my very early notes were based off of but there are some promising resources on the [GBA Jam '21 site](https://itch.io/jam/gbajam21) which look WAY more up-to-date. I should consider switching over. Probably going to finish with loirak's tutorial for completeness's sake but I'll also go check out the other resources just in case.
