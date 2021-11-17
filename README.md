# Simulation-of-particle-flows
A particle based simulation developed for my master thesis Bio-Engineering at KULeuven, with promotor Prof Dr. H. Ramon and E. Tijskens

To simulate the inelastic interactions that take place in grain flows, the discrete element method is used. The state of each particle is monitored at all times. It is the interactions between the particles on a microscopic scale that determine the macroscopic behavior of the granulate.
## Goals
1. Modular structure: By developing a flexible toolbox, built from extensible building blocks, it should be possible to quickly and elegantly develop programs for all conceivable problems to which the discrete element method can be applied.
2. Maximum efficiency: Given the high computational demands of the simulation of the flow of granular materials, it is of paramount importance to develop an efficient code; both in terms of computing time and memory usage.
3. Maximum realism: The following degree of realism was put forward: 3D simulation with 6 degrees of freedom per particle, implementation of walls from which machine parts can be assembled. Some walls must be able to perform a rotational and/or translational movement. All material properties must be user-configurable.
## Methodology
In order to achieve the above objectives, C++ was chosen as the programming language, as it allows the most freedom for optimization in terms of speed and memory usage. In addition, C++ supports the object-oriented approach that is essential for the creation of an extensible toolbox.
A grid search algorithm was developed for contact detection. The force model consists of the Kelvin-Voight model for the normal force and for the tangential force a damped linear spring with friction was implemented.

![image](https://user-images.githubusercontent.com/22667944/142173196-27f8dba7-a55f-494b-a2a2-99939b4321b1.png)

1. Validation: In a first phase, the numerical solutions of the equations of motion were validated. Subsequently, various test setups were simulated, such as the relationship between the angle of inclination and the coefficient of friction at an inclined plane, the maximum fall speed due to the air resistance and the impulse transfer in perfectly elastic collisions.
2. Visualization: Via the utility toolkit GLUT, a visualization program was developed with OpenGL to evaluate the simulated results in a three-dimensional space. The simulation results can be played back as movies, whereby the camera position can be freely determined at any time.
3. Case study: As a case study, a simulation of a centrifugal fertilizer spreader with a conical disc and trailing blades was performed. The material parameters were obtained from the literature.
## Results
A high-performance simulation program was developed that meets all the aforementioned objectives. In addition, a visualization and a demo program were also written. The figure above shows some snapshots from the simulation of a falling column consisting of 144 particles. The validations showed that the simulations yield accurate results. Moreover, thanks to the implementation of the grid search algorithm, a linear relationship between the simulation time and the number of particles was obtained, opening the way for research into systems with many thousands of particles.

![spreaderFront](https://user-images.githubusercontent.com/22667944/142173787-70307be5-d29e-47c1-8a93-0aa82bb3d932.png)
![spreaderTop](https://user-images.githubusercontent.com/22667944/142173803-69499dd9-ffd2-40fd-894c-f1e155353bb4.png)
