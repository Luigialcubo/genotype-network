# infAH3N2-genotype-criticality
 Codes and data related to *Immunity-induced criticality of the genotype network of influenza A (H3N2) hemagglutinin* by Williams, Ogbunugafor, Althouse & Hébert-Dufresne, PNAS Nexus (2022)

### Codes

The integration of the ODEs is done in C++ using the Gnu Scientific Library with the Boost library for random number generation and data structures.

The code tevol_source.cpp takes in all parameters, path to edgelist and distance matrix, and writes a file containing the temporal evolution of the system before spitting out a final epidemic size in the standard output.

The code shortestpath.cpp uses the Boost graph library to produce a distance matrix given an edgelist, if needed.

### Data

The data folder contains network and distance files for the largest two components of the Influenza A (H3N2) genotype network, stored as simple data files. It also contains the full network data in gephi format.
