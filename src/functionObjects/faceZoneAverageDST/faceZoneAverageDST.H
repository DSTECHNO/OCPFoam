/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2012-2019 OpenFOAM Foundation
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
    Foam::functionObjects::faceZoneAverageDST

Description

SourceFiles
    faceZoneAverageDST.C
    faceZoneAverageDSTTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_faceZoneAverageDST_H
#define functionObjects_faceZoneAverageDST_H

#include "fvCFD.H"
#include "functionObject.H"
#include "HashPtrTable.H"
#include "OFstream.H"
#include "fvMeshFunctionObject.H"
#include "logFiles.H"
#include "writeLocalObjects.H"
#include "Field.H"
#include "volFieldsFwd.H"
#include "cellSet.H"
#include "fluidThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class faceZoneAverageDST Declaration
\*---------------------------------------------------------------------------*/

class faceZoneAverageDST
:
    public fvMeshFunctionObject,
    public logFiles,
    public writeLocalObjects
{
        autoPtr<List<word>> faceZones_;
        word field_;
        
        
protected:

    // Protected member data

    // Protected member functions
    virtual void writeFileHeader(const label i);
    
    template<class Type>
    void writeAverage(const word fieldName);
public:

    //- Runtime type information
    TypeName("faceZoneAverageDST");


    // Constructors

        //- Construct from Time and dictionary
        faceZoneAverageDST
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

    //- Destructor
       virtual ~faceZoneAverageDST();


    // Member Functions

        virtual bool read(const dictionary&);
        
        virtual bool execute();
        
        virtual bool write();

    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const faceZoneAverageDST&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
#ifdef NoRepository
    #include "faceZoneAverageDSTTemplates.C"
#endif
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
