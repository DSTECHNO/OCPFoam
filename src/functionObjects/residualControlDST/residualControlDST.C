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


\*---------------------------------------------------------------------------*/

#include "residualControlDST.H"
#include "Time.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "Residuals.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(residualControlDST, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        residualControlDST,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::functionObjects::residualControlDST::condition() const
{
    return converged_;
}

void Foam::functionObjects::residualControlDST::checkResiduals()
{
    if (this->time_.time().value() > iterationStart_)
    {
        converged_ = true;
        forAll(residuals_(), residuali)
        {
            scalar sumXi = 0;
            scalar sumYi = 0;
            scalar sumXiSqr = 0;
            scalar sumYiXi = 0;
            int N = 0.2*residuals_()[residuali].size();
            scalar maxY = Foam::log(max(residuals_()[residuali]));
            scalar minY = Foam::log(min(residuals_()[residuali]));
            scalar deltaY = maxY - minY;
            for(int i = 0; i<N; i++)
            {
                int xi = residuals_()[residuali].size()-1-i;
                scalar yi = Foam::log(residuals_()[residuali][xi]);
                sumXi += xi;// TODO: use algebraic expressions if possible
                sumXiSqr += pow(xi,2);
                sumYi += yi;
                sumYiXi += yi*xi;
            }
            scalar magScaledSlope = (residuals_()[residuali].size()-1)/deltaY*mag((sumYiXi - sumXi*sumYi/N)/(sumXiSqr-pow(sumXi,2)/N));
            converged_ = converged_ && (magScaledSlope < magConvergedSlope_);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::residualControlDST::residualControlDST
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    stopAt(name, runTime, dict),
    obr_
    (
        runTime.lookupObject<objectRegistry>
        (
            dict.lookupOrDefault("region", polyMesh::defaultRegion)
        )
    ),
    mesh_(refCast<const fvMesh>(obr_)),
    iterationStart_(2000),
    magConvergedSlope_(0),
    converged_(false)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::residualControlDST::~residualControlDST()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::residualControlDST::read(const dictionary& dict)
{
    stopAt::read(dict);
    dict.lookup("iterationStart") >> iterationStart_;
    dict.lookup("magConvergedSlope") >> magConvergedSlope_;
    
    residualFields_ = autoPtr<List<word>>
    (
        new List<word>
        (
            dict.lookup<List<word>>("residualFields")
        )
    );
    label size = 0;
    forAll(residualFields_(), fieldi)
    {
        if(obr_.foundObject<volScalarField>(residualFields_()[fieldi]))
            size += 1;

        if(obr_.foundObject<volVectorField>(residualFields_()[fieldi]))
        {
            typename pTraits<vector>::labelType validComponents
            (
                mesh_.validComponents<vector>()
            );

            for (direction cmpt=0; cmpt<pTraits<vector>::nComponents; cmpt++)
            {
                if (component(validComponents, cmpt) != -1)
                {
                    size++;
                }
            }
        }

    }
    residuals_ = autoPtr<List<DynamicList<scalar>>>
    (
        new List<DynamicList<scalar>>(size)
    );
    

    return true;
}

bool Foam::functionObjects::residualControlDST::execute()
{
    checkResiduals();
    label residualLoc = 0;
    forAll(residualFields_(), fieldi)
    {
        // volScalarFields
        if(obr_.foundObject<volScalarField>(residualFields_()[fieldi]))
        {
            scalar residual = 1;
            if (Residuals<scalar>::found(mesh_, residualFields_()[fieldi]))
            {
                const DynamicList<SolverPerformance<scalar>>& sp
                (
                    Residuals<scalar>::field(mesh_, residualFields_()[fieldi])
                );
                residual = sp.first().initialResidual();
            }
            residuals_()[residualLoc].append(residual);
            residualLoc++;
        }
        
        
        // volVectorFields
        if(obr_.foundObject<volVectorField>(residualFields_()[fieldi]))
        {
            vector residual = vector(1,1,1);
            if (Residuals<vector>::found(mesh_, residualFields_()[fieldi]))
            {
                const DynamicList<SolverPerformance<vector>>& sp
                (
                    Residuals<vector>::field(mesh_, residualFields_()[fieldi])
                );

                residual = sp.first().initialResidual();

            }
            
            typename pTraits<vector>::labelType validComponents
            (
                mesh_.validComponents<vector>()
            );

            for (direction cmpt=0; cmpt<pTraits<vector>::nComponents; cmpt++)
            {
                if (component(validComponents, cmpt) != -1)
                {
                    residuals_()[residualLoc].append(component(residual, cmpt));
                    residualLoc++;
                }
            }
        }
    }
    stopAt::execute();
    return true;
}


// ************************************************************************* //
