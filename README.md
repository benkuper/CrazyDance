# CrazyDance
Realtime Modular Choreography Tool for Drones

**This software is under heavy development and is subject to frequent changes !**

## The Download Zone

Releases are on the release page.

You can find the bleeding edge version, compiled against latest commits here :

- Linux (x64):  http://benjamin.kuperberg.fr/download/bento/CrazyDance-linux-x64-bleedingedge.zip

- MacOS :  http://benjamin.kuperberg.fr/download/bento/CrazyDance-osx-bleedingedge.zip
 
- Windows (x64):  http://benjamin.kuperberg.fr/download/bento/CrazyDance-win-x64-bleedingedge.zip

Additionally, you can download a set of example scripts that you can then use as Script Blocks in the software : http://benjamin.kuperberg.fr/download/bento/BenTo-Example-Scripts.zip

## Building the software

CrazyDance is built on top of the JUCE frameworks. You can find more infos about it there : http://www.juce.com
Some code needs to be on the develop branch of juce, so you can't use the official release of JUCE until the next one.
To build the software, you need to :
  - Clone the develop-local branch of my JUCE fork here :
  
  `git clone --branch=develop-local http://github.com/benkuper/JUCE`
  - Download the dependencies (see below)
  - Launch the Projucer and open CrazyDance.jucer
  - Link the dependencies on the modules section
  - Save the jucer
  - Open the solution for you platform in the Builds folder
  - Build the solution from your IDE or make in linux without Code::Blocks

### Dependencies

  Chataigne relies on the OrganicUI JUCE module and juce_timeline that you can find here :  
  - https://github.com/benkuper/juce_organicui
  - https://github.com/benkuper/juce_timeline
  
  You will need to clone those repositories and link the Projucer juce_organicui and juce_timeline modules to its parent folder

## The Badge Zone

- Linux (x64):  [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/benkuper/CrazyDance/branches/master/2)](https://travis-ci.org/benkuper/CrazyDance)

- MacOS : [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/CrazyDance/CrazyDance/branches/master/1)](https://travis-ci.org/benkuper/CrazyDance)

- Windows (x64) : [![Build status](hhttps://ci.appveyor.com/api/projects/status/eckfb76a18twxna5?svg=true)](https://ci.appveyor.com/project/benkuper/crazydance)

[![forthebadge](http://forthebadge.com/images/badges/gluten-free.svg)](http://forthebadge.com)

## Screenshots

![Bento](http://benjamin.kuperberg.fr/download/bento.png)
