# Plater

![Plater](plater.jpg)

Plater is a 3D printer plates placer and optimizer. It takes a few STL files
and some parameters such as the plate dimension, part orientation and spacing,
and it tries to generates 3D model to print your parts with at least as possible
plates.

# Using

## With the GUI

To make a plate, first load your STL files. Click "Add Part(s)" button and select
one or more `.stl` file.

This will open a wizard, you'll be able to select the orientation and the quantity
of the part.

Then, enter your plate dimension (in mm). You can also adjust the spacing of the
parts and the precision (internally, plater use a discrete representation of parts,
lower is better).

Hit "Run", this will generate you the STL file(s) corresponding to your plates.

## With the command line

Plater can take several arguments, you can get the help with `-h`:

* `-v`, increase the verbosity, this will output more things on `stderr` during
  the placing of the parts
* `-W width`, width of the plate, in mm (default `150`)
* `-H height`, height of the plate, in mm (default `150`)
* `-j precision`, precision, in mm (default `0.5`)
* `-s spacing`, parts spacing, in mm (default `2`)
* `-d delta`, sets the spacing of the brute forcing (see below), default `2`mm
* `-r rotation`, sets the angle of the brute forcing, default `90`°
* `-p`, will output .ppm files instead of STLs
* `-o pattern`, sets the pattern of output files, default is `plate_%03d`, this
  means that the first plate will be named plate_001.stl, the second plate_002.stl
  and so on.

# How does it works?

The problem of placing parts is quite hard and very likely NP-complete. Plater is
an heuristic based on greedy algorithm that use brute force.

Each part is first pixelized into a bitmap (each pixel is white for free space and 
black for used space), with a given precision. Then, this bitmap is dilatated with
a given spacing.

Then, the placer tries to put each part one by one on the plate, brute forcing
position to maximize a score based on a simple gravity-like property, which tend to
pack the parts. If it can't place the part, it try adding a new plate and so on.

It is runned multiple times with multiple parameters, and the best solution, i.e the
one with the less plates is kept.

Note that the result can be bad in some limit cases, and it will not replace your
expert brain! However, it is useful to do the placing automatically and quickly.

# Building

## Plater

To build plater, go in the `plater/` directory and then use the cmake:

```
mkdir build
cd build
cmake ..
make
```

This will create for you the libplater and the plater command tool

## GUI

First, install Qt 4.8.0. Then, be sure you built plater in `plater/build/`.

### Using QtCreator

You can simply run QtCreator on `gui.pro` and build it.

### Using command line

Go in the `gui/` directory, and do:

```
mkdir build
qmake ..
make
```

This will create the `plater-gui` binary file.
