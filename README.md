# Plater

![Plater](plater.jpg)

Plater is a 3D printer plates placer and optimizer. It takes a few STL files
and some parameters such as the plate dimension, part orientation and spacing,
and it tries to generates 3D printable plates.

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
