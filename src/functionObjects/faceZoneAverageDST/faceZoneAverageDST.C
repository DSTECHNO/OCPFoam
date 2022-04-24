/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2016-2019 OpenFOAM Foundation
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


\*---------------------------------------------------------------------------*/

#include "faceZoneAverageDST.H"
#include "dictionary.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(faceZoneAverageDST, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        faceZoneAverageDST,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::faceZoneAverageDST::faceZoneAverageDST
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    logFiles(obr_, name),
    writeLocalObjects(obr_, log)
{
    faceZones_ = autoPtr<List<word>>(new List<word>(dict.lookup<List<word>>("faceZones")));
    field_ = dict.lookup<word>("field");
    
    read(dict);
    resetName(typeName);
    
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::faceZoneAverageDST::~faceZoneAverageDST()
{}


// * * * * * * * * * * * * * * * read Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::faceZoneAverageDST::read(const dictionary& dict)
{
    fvMeshFunctionObject::read(dict);
    writeLocalObjects::read(dict);
    return true;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::faceZoneAverageDST::execute()
{
    return true;
}

bool Foam::functionObjects::faceZoneAverageDST::write()
{
    writeLocalObjects::write();

    logFiles::write();
    if (Pstream::master())
        writeTime(file());
    writeAverage<scalar>(field_);
    writeAverage<vector>(field_);
    if (Pstream::master())
        file() << endl;
    return true;
}


void Foam::functionObjects::faceZoneAverageDST::writeFileHeader(const label i)
{
    writeHeader(file(), "faceZoneAverage");
    writeCommented(file(), "Time");
 
    forAll(faceZones_(), i)
        writeTabbed(file(), faceZones_()[i]);
    
    file() << endl;
}
// ************************************************************************* //
