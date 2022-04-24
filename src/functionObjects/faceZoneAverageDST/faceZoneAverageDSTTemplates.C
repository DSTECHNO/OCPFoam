/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2015-2019 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class Type>
void Foam::functionObjects::faceZoneAverageDST::writeAverage(const word fieldName)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
    typedef GeometricField<Type, Foam::fvsPatchField, Foam::surfaceMesh> surfaceFieldType;
    if (obr_.foundObject<fieldType>(fieldName))
    {
        const fieldType& f = lookupObject<fieldType>(fieldName);
        
        surfaceFieldType ff = fvc::interpolate(f);
        
        forAll(faceZones_(), i)
        {
            label inletZoneId = mesh_.faceZones().findZoneID(faceZones_()[i]);
            labelList faces = mesh_.faceZones()[inletZoneId];

            Field<Type> faceVals(faces.size(),Zero);
            scalarField magSf(faces.size(),0);
            forAll(faceVals, faceI)
            {
                if (mesh_.isInternalFace(faces[faceI]))
                {
                    magSf[faceI] = mesh_.magSf()[faces[faceI]];
                    faceVals[faceI] = ff[faces[faceI]];
                }
                else
                {
                    label facePatchId = mesh_.boundaryMesh().whichPatch(faces[faceI]);
                    const polyPatch& pp = mesh_.boundaryMesh()[facePatchId];
                    label faceId = pp.whichFace(faces[faceI]);
                    
                    magSf[faceI] = mesh_.magSf().boundaryField()[facePatchId][faceId];
                    faceVals[faceI] = ff.boundaryField()[facePatchId][faceId];
                }
            }
            scalar totalArea = gSum(magSf);
            Type avgf = gSum(faceVals*magSf)/totalArea;
            
            if (Pstream::master())
                file() << tab << avgf;
        }
    }
}


// ************************************************************************* //
