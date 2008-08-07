// ************************************************************************* //
//                            avtMFIXFileFormat.h                            //
// ************************************************************************* //

#ifndef AVT_MFIX_FILE_FORMAT_H
#define AVT_MFIX_FILE_FORMAT_H

#include <avtMTMDFileFormat.h>

#include <vector>
#include <string>
#include <vtkFloatArray.h>
#include "vtkHexahedron.h"
#include "vtkWedge.h"
#include "vtkQuad.h"


// ****************************************************************************
//  Class: avtMFIXFileFormat
//
//  Purpose:
//      Reads in MFIX files as a plugin to VisIt.
//
//  Programmer: bdotson -- generated by xml2avt
//  Creation:   Fri May 26 08:59:22 PDT 2006
//
//  Modifications:
//    Jeremy Meredith, Thu Aug  7 15:46:38 EDT 2008
//    Use a C++ string for Version so comparisons are easier.
//
// ****************************************************************************

class avtMFIXFileFormat : public avtMTMDFileFormat
{
  public:
                       avtMFIXFileFormat(const char *);
    virtual           ~avtMFIXFileFormat() {;};

    virtual int            GetNTimesteps(void);

    virtual void           GetTimes(std::vector<double> &times);

    virtual const char    *GetType(void)   { return "MFIX"; };
    virtual void           FreeUpResources(void); 

    virtual vtkDataSet    *GetMesh(int, int, const char *);
    virtual vtkDataArray  *GetVar(int, int, const char *);
    virtual vtkDataArray  *GetVectorVar(int, int, const char *);

    char RestartFileName[256];


  protected:
  vtkPoints *Points;            // Points array for building grid
  vtkUnstructuredGrid *FluidMesh;    // Unstructured Grid
  vtkUnstructuredGrid *InletMesh;    // Unstructured Grid
  vtkUnstructuredGrid *OutletMesh;    // Unstructured Grid
  vtkUnstructuredGrid *ObstructionMesh;    // Unstructured Grid
  vtkHexahedron *AHexahedron;   // Hexahedron type cell
  vtkWedge *AWedge;             // Wedge type cell
  vtkQuad  *AQuad;


    // Restart File Variables
    int MaxTimeStep;
    char DataBuffer[513];
    int IMinimum1;
    int JMinimum1;
    int KMinimum1;
    int IMaximum;
    int JMaximum;
    int KMaximum;
    int IMaximum1;
    int JMaximum1;
    int KMaximum1;
    int IMaximum2;
    int JMaximum2;
    int KMaximum2;
    int IJMaximum2;
    int IJKMaximum2;
    int MMAX;
    double XLength;
    double YLength;
    double ZLength;
    double DeltaTime;
    int    DimensionIc;
    int    DimensionBc;
    int    DimensionC;
    int    DimensionIs;
    double Ce;
    double Cf;
    double Phi;
    double PhiW;
    double XMinimum;
    int SPXRecordsPerTimestep; 
    std::vector<int> NMax;
    std::vector<double> C;
    std::vector<int> Flag;
    std::vector<double> Dx;
    std::vector<double> Dy;
    std::vector<double> Dz;
    float VersionNumber;
    char CoordinateSystem[17];
    char Units[17];
    int DIM_tmp;
    std::vector<int> TempI;
    std::vector<double> TempD;
    std::string Version;
    int NumberOfSPXFilesUsed;
    int  NumberOfScalars;
    int NumberOfReactionRates;
    bool BkEpsilon;
    std::vector<int> SpxFileExists;
    std::vector<std::string> VariableNames;
    std::vector<int> VariableComponents;
    std::vector<int> VariableIndexToSPX;
    std::vector<int> VariableTimesteps;
    std::vector< std::vector<int> > VariableTimestepTable;
    std::vector<int> SPXToNVarTable;
    std::vector<int> VariableToSkipTable;
    std::vector< std::vector<int> > SPXTimestepIndexTable;
    int numberOfFloatsInBlock;

    virtual void PopulateDatabaseMetaData(avtDatabaseMetaData *, int);

    // Helper Functions
    void ReadRestartFile(const char *filename);
    void GetInt(istream& in, int &val);
    void GetDouble(istream& in, double& val);
    void SkipBytes(istream& in, int n);
    void GetBlockOfDoubles(istream& in, std::vector<double> &d , int n);
    void GetBlockOfInts(istream& in, std::vector<int> &v, int n);
    void RestartVersionNumber(char* buffer);
    void SwapInt(int &value);
    void SwapFloat(float &value);
    void SwapDouble(double &value);
    void CreateVariableNames(const char *filename);
    void GetTimeSteps(const char *filename);
    int  GetMaxTimeStep();
    void MakeTimeStepTable();
    void GetNumberOfVariablesInSPXFiles();
    void MakeSPXTimeStepIndexTable();
    void MakeMeshes();
    void GetVariableAtTimestep(int vari , int tstep, vtkFloatArray *v, const char *filename);
    void GetBlockOfFloats(istream& in, vtkFloatArray *v, int n);

};


#endif
