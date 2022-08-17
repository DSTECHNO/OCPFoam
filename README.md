# OCPFoam
An open source CFD software for the flow and thermal analysis of OCP (Open Compute Project) servers. Internal flow field is simulated using a hexahedra mesh with 5.98 million cells.

<p align="center">
   <img src="https://user-images.githubusercontent.com/89465885/164973779-05e188ba-f645-4e2c-a579-e36468c17234.jpg" alt="[OCP]"/>
</p>

**Figure 1.** Three-dimensional, front and back views of the Leopard V3.1 model of OCP server.  

![Airflow_through_an_OCP_server(1)](https://user-images.githubusercontent.com/89465885/165092919-fd1022da-e005-4bfd-8344-fd42d40261e5.gif) |  ![Airflow_through_an_OCP_server(2)](https://user-images.githubusercontent.com/89465885/165092937-282e75bd-7ef2-4196-a9b2-be82b117069e.gif) | 
--- | ---


[![Airflow through an OCP server](https://yt-embed.herokuapp.com/embed?v=w-Xrcj8y-bA)](https://www.youtube.com/watch?v=w-Xrcj8y-bA "Airflow through an OCP server")

This repository provides an object function to check whether the solution reachs steady-state considering three conditions. The first and second conditions are relative tolerance between two successive iterations and maximum iteration number, respectively. The last condition, which is proposed here, checks the slope of the trendline in the log-scaled space. 

<p align="center">
   <img src="https://user-images.githubusercontent.com/89465885/164973808-08815c7c-c071-48c6-bfb6-f27ca39dc4e1.jpg" alt="[OCP2]"/>
</p>

**Figure 2.** Residual control procedure residuals, normalized residuals and the last 20% of normalized residuals with linear approximations. (Sp = -0.038, SUx = -0.015, SUy = -0.03, SUz = -0.033)

The second object function calculates  average pressure over face zones created at the inlet and outlet of a server. 

<p align="center">
   <img src="https://user-images.githubusercontent.com/30440239/165032510-35682979-b377-4219-b1f6-ebb65aa6e8a1.png" alt="[Domain]"/>
</p>

**Figure 3.** Computational domain and boundaries of the computational domain.

# Usage
**Step 1.** Compile the library in OpenFOAM-8.

**$** ./Allwmake

**Step 2.** The desired tutorial is entered into the folder and the case is run.

**$** cd tutorials/pitzDailyFaceZoneAverage

**$** ./Allrun

or

**$** cd tutorials/pitzDailyResidualControl

**$** ./Allrun

**Addition.** To delete the library:

**$** cd ../OCPFoam

**$** ./Allwclean

# Paper
Dogan A, Yilmaz S, Kuzay M, Yilmaz C, Demirel E: **CFD modeling of pressure drop through an OCP server for data center applications.** *Energies* 2022.

# Underlying Data
Zenodo: OpenFOAM cases of the paper **" CFD modeling of pressure drop through an OCP server for data center applications " [Data Set]**, https://www.zenodo.org/deposit/6342699 

# Developers

Aras Dogan

Sibel Yilmaz

Mustafa Kuzay

Prof. Dr. Ender Demirel

# Acknowledgements
This study is part of a project that has received funding from the European Union’s Horizon 2020 research and innovation programme under grant agreement **No 956059**

# Licence

Copyright 2022 Design and Simulation Technologies Inc.

Distributed using the GNU General Public License (GPL), version 3; see the LICENSE file for details.
