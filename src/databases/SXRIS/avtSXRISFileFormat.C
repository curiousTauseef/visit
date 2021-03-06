/*****************************************************************************
*
* Copyright (c) 2000 - 2019, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                            avtSXRISFileFormat.C                           //
// ************************************************************************* //

#include <avtSXRISFileFormat.h>
#include <string>

#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkRectilinearGrid.h>

#include <avtDatabaseMetaData.h>

#include <DBOptionsAttributes.h>
#include <Expression.h>

#include <InvalidVariableException.h>
#include <InvalidFilesException.h>
#include <InvalidDBTypeException.h>
#include <DebugStream.h>
#include <Expression.h>

using     std::string;
using     std::vector;


// ****************************************************************************
//  Method: avtSXRISFileFormat constructor
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

avtSXRISFileFormat::avtSXRISFileFormat(const char *fname)
    : avtMTSDFileFormat(&fname, 1)
{
    fileId = -1;
    filename = strdup(fname);
    opened = false;
    dimensions[0] = 0;
    dimensions[1] = 0;
    dimensions[2] = 0;
}

//****************************************************************************
// Method:  avtSXRISFileFormat::~avtSXRISFileFormat
//
// Purpose:
//   destructor
//
// Programmer:  Dave Pugmire
// Creation:    October  3, 2012
//
// Modifications:
//
//****************************************************************************

avtSXRISFileFormat::~avtSXRISFileFormat()
{
    if (filename)
        free(filename);
    filename = NULL;
    
    if (fileId > 0)
        H5Fclose(fileId);
    
    fileId = -1;
    opened = false;
    dimensions[0] = 0;
    dimensions[1] = 0;
    dimensions[2] = 0;
}

// ****************************************************************************
//  Method: avtEMSTDFileFormat::GetNTimesteps
//
//  Purpose:
//      Tells the rest of the code how many timesteps there are in this file.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

int
avtSXRISFileFormat::GetNTimesteps(void)
{
    LoadFile();
    return dimensions[2];
}

// ****************************************************************************
//  Method: avtEMSTDFileFormat::GetTimes
//
//  Purpose:
//      Tells the rest of the code the timesteps in this file.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

void
avtSXRISFileFormat::GetTimes(vector<double> &t)
{
    LoadFile();
    
    t = times;
}

// ****************************************************************************
//  Method: avtEMSTDFileFormat::GetCycles
//
//  Purpose:
//      Tells the rest of the code the cycles in this file.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

void
avtSXRISFileFormat::GetCycles(vector<int> &t)
{
    LoadFile();
    
    t.resize(times.size());
    for (size_t i = 0; i < times.size(); i++)
        t[i] = i;
}


// ****************************************************************************
//  Method: avtSXRISFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

void
avtSXRISFileFormat::FreeUpResources(void)
{
}


// ****************************************************************************
//  Method: avtSXRISFileFormat::PopulateDatabaseMetaData
//
//  Purpose:
//      This database meta-data object is like a table of contents for the
//      file.  By populating it, you are telling the rest of VisIt what
//      information it can request from you.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

void
avtSXRISFileFormat::PopulateDatabaseMetaData(avtDatabaseMetaData *md, int timeState)
{
    LoadFile();
    
    avtMeshMetaData *mesh = new avtMeshMetaData;
    mesh->name = "mesh";
    mesh->meshType = AVT_RECTILINEAR_MESH;
    mesh->spatialDimension = 2;
    mesh->topologicalDimension = 2;
    mesh->xUnits = "m";
    mesh->xLabel = "R";
    mesh->yUnits = "m";
    mesh->yLabel = "Z";
    md->Add(mesh);
    
    AddScalarVarToMetaData(md, "frame", "mesh", AVT_NODECENT);

    Expression expr1;
    expr1.SetName("prevFrame");
    expr1.SetDefinition("conn_cmfe(<[-1]id:frame>, mesh)");
    expr1.SetType(Expression::ScalarMeshVar);
    md->AddExpression(&expr1);

    Expression expr2;
    expr2.SetName("deltaFrame");
    expr2.SetDefinition("frame-prevFrame");
    expr2.SetType(Expression::ScalarMeshVar);
    md->AddExpression(&expr2);
}

//****************************************************************************
// Method:  avtSXRISFileFormat::LoadFile
//
// Purpose: 
//   Load file
//
// Programmer:  Dave Pugmire
// Creation:    October  3, 2012
//
// Modifications:
//
//****************************************************************************

void
avtSXRISFileFormat::LoadFile()
{
    if (opened)
        return;
    
    // Initialize HDF5.
    H5open();
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);
    fileId = H5Fopen(filename, H5F_ACC_RDONLY, 0);

    if (fileId < 0)
        EXCEPTION1(InvalidFilesException, filename);

    hid_t dataId = H5Dopen(fileId, "/frames", H5P_DEFAULT);
    if (dataId < 0)
    {
        H5Fclose(fileId);
        EXCEPTION1(InvalidDBTypeException,
            "Cannot be a SXRIS file because it doesn't have frames dataset.");
    }

    hid_t spaceId = H5Dget_space(dataId);

    hsize_t dims[3], maxDims[3];
    int ndims = H5Sget_simple_extent_dims(spaceId, dims, maxDims); (void) ndims;

    dimensions[0] = dims[0];
    dimensions[1] = dims[1];
    dimensions[2] = dims[2];

    int numAttrs = H5Aget_num_attrs(dataId);
    char buf[1024];
    vector<int> timevals;
    for (int i = 0; i < numAttrs; i++)
    {
        hid_t attrId = H5Aopen_idx(dataId, i);
        H5Aget_name(attrId, 1024, buf);
        timevals.resize(dimensions[2]);
        if (strcmp(buf, "X-axis Values") == 0)
        {
            H5Aread(attrId, H5T_NATIVE_INT, &timevals[0]);
            for (size_t j = 0; j < timevals.size(); j++)
                times.push_back((double)timevals[j]);
        }
        else if (strcmp(buf, "X-axis Label") == 0)
        {
        }
        else if (strcmp(buf, "Y-axis Tick Values") == 0)
        {
            YSteps.resize(dimensions[1]);
            H5Aread(attrId, H5T_NATIVE_DOUBLE, &YSteps[0]);
        }
        else if (strcmp(buf, "Y-axis Label") == 0)
        {
        }
        else if (strcmp(buf, "Z-axis Tick Values") == 0)
        {
            ZSteps.resize(dimensions[0]);
            H5Aread(attrId, H5T_NATIVE_DOUBLE, &ZSteps[0]);
        }
        else if (strcmp(buf, "Z-axis Label") == 0)
        {
        }
        H5Aclose(attrId);
    }
    
    H5Sclose(spaceId);
    H5Dclose(dataId);

    opened = true;
}


// ****************************************************************************
//  Method: avtSXRISFileFormat::GetMesh
//
//  Purpose:
//      Gets the mesh associated with this file.  The mesh is returned as a
//      derived type of vtkDataSet (ie vtkRectilinearGrid, vtkStructuredGrid,
//      vtkUnstructuredGrid, etc).
//
//  Arguments:
//      timestate   The index of the timestate.  If GetNTimesteps returned
//                  'N' time steps, this is guaranteed to be between 0 and N-1.
//      meshname    The name of the mesh of interest.  This can be ignored if
//                  there is only one mesh.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

vtkDataSet *
avtSXRISFileFormat::GetMesh(int timestate, const char *meshname)
{
    LoadFile();
    vtkRectilinearGrid *rv = vtkRectilinearGrid::New();

    vtkFloatArray *x = vtkFloatArray::New();
    vtkFloatArray *y = vtkFloatArray::New();
    vtkFloatArray *z = vtkFloatArray::New();

    int nX = dimensions[1];
    int nY = dimensions[0];
    int nZ = 1;

    x->SetNumberOfTuples(nX);
    for (int i = 0; i < nX; i++)
        x->SetTuple1(i, YSteps[i]);
    
    y->SetNumberOfTuples(nY);
    for (int i = 0; i < nY; i++)
        y->SetTuple1(i, ZSteps[i]);
    
    z->SetNumberOfTuples(nZ);
    for (int i = 0; i < nZ; i++)
        z->SetTuple1(i, i);

    int dims[3] = {nX, nY, nZ};
    rv->SetDimensions(dims);
    rv->SetXCoordinates(x);
    rv->SetYCoordinates(y);
    rv->SetZCoordinates(z);
    x->Delete();
    y->Delete();
    z->Delete();
    return rv;
}


// ****************************************************************************
//  Method: avtSXRISFileFormat::GetVar
//
//  Purpose:
//      Gets a scalar variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      timestate  The index of the timestate.  If GetNTimesteps returned
//                 'N' time steps, this is guaranteed to be between 0 and N-1.
//      varname    The name of the variable requested.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

vtkDataArray *
avtSXRISFileFormat::GetVar(int timestate, const char *varname)
{
    LoadFile();

    vtkDoubleArray *arr = vtkDoubleArray::New();
    arr->SetNumberOfComponents(1);
    arr->SetNumberOfTuples(dimensions[0]*dimensions[1]);

    hid_t dataId = H5Dopen(fileId, "/frames", H5P_DEFAULT);
    hid_t spaceId = H5Dget_space(dataId);

    hsize_t memDim[2] = {static_cast<hsize_t>(dimensions[0]),
                         static_cast<hsize_t>(dimensions[1])};
    hid_t memspaceId = H5Screate_simple(2, memDim, NULL);


    hsize_t offset[3] = {0,0,static_cast<hsize_t>(timestate)};
    hsize_t count[3] = {static_cast<hsize_t>(dimensions[0]),
                        static_cast<hsize_t>(dimensions[1]),
                        1};
    H5Sselect_hyperslab(spaceId, H5S_SELECT_SET, offset, NULL, count, NULL);
    
    herr_t status = H5Dread(dataId, H5T_NATIVE_DOUBLE, memspaceId, spaceId, H5P_DEFAULT, arr->GetVoidPointer(0)); (void) status;
    
    return arr;
}


// ****************************************************************************
//  Method: avtSXRISFileFormat::GetVectorVar
//
//  Purpose:
//      Gets a vector variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      timestate  The index of the timestate.  If GetNTimesteps returned
//                 'N' time steps, this is guaranteed to be between 0 and N-1.
//      varname    The name of the variable requested.
//
//  Programmer: pugmire -- generated by xml2avt
//  Creation:   Wed Oct 3 10:13:30 PDT 2012
//
// ****************************************************************************

vtkDataArray *
avtSXRISFileFormat::GetVectorVar(int timestate, const char *varname)
{
    LoadFile();
    EXCEPTION1(InvalidVariableException, varname);
}
