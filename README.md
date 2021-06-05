# GBA Hello

This is a simple Hello World style project for the GBA and also a place to collect all of my early learning dev notes for the GBA.

## Table of contents

* [Project structure](#project-structure)
* [Notes](#notes)
    * [Compiler quirks](#compiler-quirks)
    * [Learning](#learning)

## Project structure

Here's a quick rundown of the different files hosted in this project:

folder | description
-------|------------
`dev\dev_tools`  | This folder contains an archive of development tools I've found online. Archiving them in case their originally hosted webpages go missing. I've run across plenty of dead links so let's just be safe.
`dev\docs`  | This folder contains an archive of development docs that I wanted to archive in case they suddenly vanish. I've run across plenty of dead links so let's just be safe.
`ext`  | This folder contains source code that I'm pulling from outside of this project or which I've adapted from source code outside of this project.
`src`  | This folder contains the source code I've written for this project.

## Notes

### DevKitAdv compiler quirks

The GBA compiler used for this (devkitadv) is locked on a really old version of GCC (3.2). This has a few unfortunate caveats:

1. c99 is the highest standard supported
2. `#pragma once` isn't supported

### Learning

A reverse-chronological log of little things that I learn as I go:

* Tried to use the VDraw status register value to vsync and that didn't seem to work nearly as well as the waiting on the vcount register. I wonder why? I wonder if using interrupts is even better?
* GBA is an ARM7TDMI processor and the docs for that are still readily available online! yay!
* UGH. span types are a freaking mess in straight C (99, 11, otherwise). Lack of templates is a huge pain.
* When I googled for resources I found [loirak's tutorial](http://www.loirak.com/gameboy/gbatutor.php) so that's what a lot of my very early notes were based off of but there are some promising resources on the [GBA Jam '21 site](https://itch.io/jam/gbajam21) which look WAY more up-to-date. I should consider switching over. Probably going to finish with loirak's tutorial for completeness's sake but I'll also go check out the other resources just in case.
