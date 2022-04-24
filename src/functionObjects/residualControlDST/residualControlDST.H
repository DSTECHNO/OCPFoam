/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2020 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Author:
    Design and Simulation Technologies Inc. (DSTECH)
    http://dstechno.net/
         
      _____   _____ _______ ______ _____ _    _ 
     |  __ \ / ____|__   __|  ____/ ____| |  | |
     | |  | | (___    | |  | |__ | |    | |__| |
     | |  | |\___ \   | |  |  __|| |    |  __  |
     | |__| |____) |  | |  | |___| |____| |  | |
     |_____/|_____/   |_|  |______\_____|_|  |_|
                                      
Class
    Foam::functionObjects::residualControlDST

Description
    Stops the run when the residuals of the specified variables falls
    below a specified value.

    Examples of function object specification:
    \verbatim
    steadyStateResidualControl
    {
        type        residualControlDST;
        libs        ("libdataCenterFunctionObjectsDST.so");

        iterationStart 100;
        magConvergedSlope 0.05;
        residualFields (p U);
        action   writeNow;
    }
    \endverbatim

SourceFiles
    residualControlDST.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_residualControlDST_H
#define functionObjects_residualControlDST_H

#include "fvMesh.H"
#include "stopAt.H"
#include "DynamicList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
class objectRegistry;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                      Class residualControlDST Declaration
\*---------------------------------------------------------------------------*/

class residualControlDST
:
    public stopAt
{
    // Private Data
        const objectRegistry& obr_;
        //- Reference to the fvMesh
        const fvMesh& mesh_;
        
        //-
        label iterationStart_;

        scalar magConvergedSlope_;
        bool converged_;
        
        autoPtr<List<word>> residualFields_;
        autoPtr<List<DynamicList<scalar>>> residuals_;


    // Private Member Functions

        //- Return true when the stop condition is achieved
        virtual bool condition() const;
        
        void checkResiduals();


public:

    //- Runtime type information
    TypeName("residualControlDST");


    // Constructors

        //- Construct from Time and dictionary
        residualControlDST
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );

        //- Disallow default bitwise copy construction
        residualControlDST(const residualControlDST&) = delete;


    //- Destructor
    virtual ~residualControlDST();


    // Member Functions

        //- Read the dictionary settings
        virtual bool read(const dictionary&);

        //- Execute, check existence of stopAt file and take action
        virtual bool execute();

    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const residualControlDST&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
