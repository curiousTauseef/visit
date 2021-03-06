/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkVisItExtractGrid.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkVisItExtractGrid - select piece (e.g., volume of interest) and/or 
// subsample structured grid dataset

// .SECTION Description
// vtkVisItExtractGrid is a filter that selects a portion of an input structured
// grid dataset, or subsamples an input dataset. (The selected portion of
// interested is referred to as the Volume Of Interest, or VOI.) The output of
// this filter is a structured grid dataset. The filter treats input data of
// any topological dimension (i.e., point, line, image, or volume) and can
// generate output data of any topological dimension.
//
// To use this filter set the VOI ivar which are i-j-k min/max indices that
// specify a rectangular region in the data. (Note that these are 0-offset.)
// You can also specify a sampling rate to subsample the data.
//
// Typical applications of this filter are to extract a plane from a grid for 
// contouring, subsampling large grids to reduce data size, or extracting
// regions of a grid with interesting data.

// .SECTION See Also
// vtkGeometryFilter vtkVisItExtractGeometry vtkVisItExtractVOI 
// vtkStructuredGridGeometryFilter

#ifndef __vtkVisItExtractGrid_h
#define __vtkVisItExtractGrid_h

#include <vtkStructuredGridAlgorithm.h>
#include <visit_vtk_exports.h>

class VISIT_VTK_API vtkVisItExtractGrid : public vtkStructuredGridAlgorithm
{
public:
  static vtkVisItExtractGrid *New();
  vtkTypeMacro(vtkVisItExtractGrid,vtkStructuredGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // Description:
  // Specify i-j-k (min,max) pairs to extract. The resulting structured grid
  // dataset can be of any topological dimension (i.e., point, line, plane,
  // or 3D grid). 
  vtkSetVector6Macro(VOI,int);
  vtkGetVectorMacro(VOI,int,6);

  // Description:
  // Set the sampling rate in the i, j, and k directions. If the rate is > 1,
  // then the resulting VOI will be subsampled representation of the input.
  // For example, if the SampleRate=(2,2,2), every other point will be
  // selected, resulting in a volume 1/8th the original size.
  vtkSetVector3Macro(SampleRate, int);
  vtkGetVectorMacro(SampleRate, int, 3);

  // Description:
  // Control whether to enforce that the "boundary" of the grid is output in
  // the subsampling process. (This ivar only has effect when the SampleRate
  // in any direction is not equal to 1.) When this ivar IncludeBoundary is
  // on, the subsampling will always include the boundary of the grid even
  // though the sample rate is not an even multiple of the grid
  // dimensions. (By default IncludeBoundary is off.)
  vtkSetMacro(IncludeBoundary,bool);
  vtkGetMacro(IncludeBoundary,bool);
  vtkBooleanMacro(IncludeBoundary,bool);

protected:
  vtkVisItExtractGrid();
  ~vtkVisItExtractGrid() {};

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  virtual int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  virtual int RequestUpdateExtent(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  
  int VOI[6];
  int SampleRate[3];
  bool IncludeBoundary;
  
private:
  vtkVisItExtractGrid(const vtkVisItExtractGrid&);  // Not implemented.
  void operator=(const vtkVisItExtractGrid&);  // Not implemented.
};

#endif


