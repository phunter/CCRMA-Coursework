Stanford CS 248
Final Project

Team Members: P. Hunter McCurry
SUNet ID: hmccurry

Project Title: Liquid Score


Advanced Features Implemented:
------------------------------
- Fullscreen anti-aliasing by rendering first to extra-large texture, then interpolating sub-pixel values on a second pass.

- Audio input tracking as a means of game control. Audio is handled by MaxMSP (http://cycling74.com/products/max/) in a custom-written patch that does pitch-tracking and amplitude analysis of a live instrument. The sound is recorded, parsed and stored in a format that is easy to sample and play back later using granular synthesis. This serves as both a location-based audio sampler, and musical and harmonic accompaniment to the player's movements.

- Pseudo-physics node clustering and avoidance, using exponential slewing to allow a graph of connected to converge to some "ideal" configuration in three dimensions.

- Smooth camera motions to track the focus of the players movement throughout the graph.

- Custom 3D model generation including cylinders, toruses, musical sharps and flats. All geometry was produced entirely on my own.

- Toon shaders that include bold, black outlines. This is done by checking the dot product of the surface normal and the eye position.