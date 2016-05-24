# biscotti

## Introduction
`biscotti` is a discrete ordinates solver written using `C++11` standards. Problems are solved in 1-D slab geometry, with groupwise cross sections, even-order Gauss-Legendre quadratures, and isotropic scattering.

## Dependencies
The only dependency for building and running `biscotti` is `gcc` version 4.2.1. Other versions may work. 

## Building
1. Clone the repository

    `git clone git@github.com:agtumulak/biscotti.git`

2. Change directory to the `biscotti` directory

    `cd biscotti`

3. Run make

    `make`

At this point, you should have the `biscotti` binary under the `bin/` directory. To execute it, do `./bin/biscotti`. It you see the calculation running, you have built `biscotti`.
