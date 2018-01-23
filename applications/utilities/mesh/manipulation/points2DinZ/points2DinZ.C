/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

Application
    transformPoints

Description
    Transforms the mesh points in the polyMesh directory according to the
    translate, rotate and scale options.

Usage
    Options are:

    -translate vector
        Translates the points by the given vector,

    -rotate (vector vector)
        Rotates the points from the first vector to the second,

     or -yawPitchRoll (yawdegrees pitchdegrees rolldegrees)
     or -rollPitchYaw (rolldegrees pitchdegrees yawdegrees)

    -scale vector
        Scales the points by the given vector.

    The any or all of the three options may be specified and are processed
    in the above order.

    With -rotateFields (in combination with -rotate/yawPitchRoll/rollPitchYaw)
    it will also read & transform vector & tensor fields.

    Note:
    yaw (rotation about z)
    pitch (rotation about y)
    roll (rotation about x)

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "fvMesh.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "ReadFields.H"
#include "pointFields.H"
#include "transformField.H"
#include "transformGeometricField.H"
#include "IStringStream.H"
#include "mathematicalConstants.H"

using namespace Foam;
using namespace Foam::constant::mathematical;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


int main(int argc, char *argv[])
{
    #include "addRegionOption.H"
    #include "setRootCase.H"
    #include "createTime.H"

    word regionName = polyMesh::defaultRegion;
    fileName meshDir;

    if (args.optionReadIfPresent("region", regionName))
    {
        meshDir = regionName/polyMesh::meshSubDir;
    }
    else
    {
        meshDir = polyMesh::meshSubDir;
    }

    pointIOField points
    (
        IOobject
        (
            "points",
            runTime.findInstance(meshDir, "points"),
            meshDir,
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
        )
    );

	scalarField z = points.component(vector::Z);
	scalar minz = min(z);
	scalar maxz = max(z);
	scalar h = (maxz - minz)/2;
	forAll(z, i)
	{
		if(z[i] < h)
		{
			z[i] = 0;
		}
		else
		{
			z[i] = 1;
		}
	}

	points.replace(vector::Z, z);

    Info<< "Writing points into directory " << points.path() << nl << endl;
    points.write();

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
