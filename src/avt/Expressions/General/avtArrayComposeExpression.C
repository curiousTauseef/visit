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
//                      avtArrayComposeExpression.C                          //
// ************************************************************************* //

#include <avtArrayComposeExpression.h>

#include <math.h>

#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkUnsignedIntArray.h>

#include <ExprToken.h>
#include <avtExprNode.h>

#include <avtCallback.h>
#include <avtMaterial.h>
#include <avtSpecies.h>

#include <DebugStream.h>
#include <ExpressionException.h>
#include <ImproperUseException.h>


// ****************************************************************************
//  Method: avtArrayComposeExpression constructor
//
//  Purpose:
//      Defines the constructor.  Note: this should not be inlined in the
//      header because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   July 21, 2005
//
// ****************************************************************************

avtArrayComposeExpression::avtArrayComposeExpression()
{
    nargs = 0;
}


// ****************************************************************************
//  Method: avtArrayComposeExpression destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   July 21, 2005
//
// ****************************************************************************

avtArrayComposeExpression::~avtArrayComposeExpression()
{
}


// ****************************************************************************
//  Method: avtArrayComposeExpression::DeriveVariable
//
//  Purpose:
//      Creates an array.
//
//  Arguments:
//      inDS      The input dataset.
//
//  Returns:      The derived variable.  The calling class must free this
//                memory.
//
//  Programmer:   Hank Childs
//  Creation:     July 21, 2005
//
// ****************************************************************************

vtkDataArray *
avtArrayComposeExpression::DeriveVariable(vtkDataSet *in_ds, int currentDomainsIndex)
{
    if (varnames.size() == 0)
        EXCEPTION0(ImproperUseException);

    int nvars = (int)varnames.size();
    vtkDataArray **vars = new vtkDataArray*[nvars];
    avtCentering  *centering = new avtCentering[nvars];

    for (int i = 0 ; i < nvars ; i++)
    {
        vars[i] = in_ds->GetPointData()->GetArray(varnames[i]);
        centering[i] = AVT_NODECENT;
        if (vars[i] == NULL)
        {
            vars[i] = in_ds->GetCellData()->GetArray(varnames[i]);
            centering[i] = AVT_ZONECENT;
        }
    }

    int outType = VTK_FLOAT;
    for (int i = 0 ; i < nvars ; i++)
    {
        if (vars[i] == NULL)
            EXCEPTION2(ExpressionException, outputVariableName,
                  "Cannot create array because: cannot locate all variables");
        if (vars[i]->GetNumberOfComponents() != 1)
            EXCEPTION2(ExpressionException, outputVariableName,
                  "Cannot create array because: all inputs must be scalars");
        if (centering[i] != centering[0])
            EXCEPTION2(ExpressionException, outputVariableName,
                  "Cannot create array because: the centering of the "
                  "variables does not agree.");
        if (vars[i]->GetDataType() == VTK_DOUBLE)
            outType = VTK_DOUBLE;
    }

    vtkDataArray *rv = vtkDataArray::CreateDataArray(outType);
    rv->SetNumberOfComponents(nvars);
    vtkIdType nvals = vars[0]->GetNumberOfTuples();
    rv->SetNumberOfTuples(nvals);
    for (vtkIdType i = 0 ; i < nvals ; i++)
        for (int j = 0 ; j < nvars ; j++)
            rv->SetComponent(i, j, vars[j]->GetTuple1(i));

    delete [] vars;
    delete [] centering;
    return rv;
}


// ****************************************************************************
//  Method: avtArrayComposeExpression::ProcessArguments
//
//  Purpose:
//      Tells the first argument to go generate itself.  Parses the second
//      argument into a list of material names.
//
//  Arguments:
//      inDS      The input dataset.
//
//  Returns:      The derived variable.  The calling class must free this
//                memory.
//
//  Programmer:   Hank Childs
//  Creation:     July 21, 2005
//
// ****************************************************************************

void
avtArrayComposeExpression::ProcessArguments(ArgsExpr *args,
                                        ExprPipelineState *state)
{
    // Check the number of arguments
    std::vector<ArgExpr*> *arguments = args->GetArgs();
    nargs = (int)arguments->size();

    // Let the base class do this processing.  We only had to over-ride this
    // function to determine the number of arguments.
    avtMultipleInputExpressionFilter::ProcessArguments(args, state);
}


// ****************************************************************************
//  Method: avtArrayComposeExpression::UpdateDataObjectInfo
//
//  Purpose:
//      Tell the output what the component names are.
//
//  Programmer:   Hank Childs
//  Creation:     August 5, 2005
//
//  Modifications:
//    Jeremy Meredith, Thu Feb  7 18:01:29 EST 2008
//    This wasn't setting the dimension of the output variable, which
//    was necessary, so I added it.
//
// ****************************************************************************

void
avtArrayComposeExpression::UpdateDataObjectInfo(void)
{
    avtMultipleInputExpressionFilter::UpdateDataObjectInfo();

    // If we don't know the name of the variable, we can't set it up in the
    // output.
    if (outputVariableName == NULL)
        return;

    std::vector<std::string> subnames(varnames.size());
    for (size_t i = 0 ; i < varnames.size() ; i++)
        subnames[i] = varnames[i];

    avtDataAttributes &outAtts = GetOutput()->GetInfo().GetAttributes();
    outAtts.SetVariableDimension((int)varnames.size(), outputVariableName);
    outAtts.SetVariableSubnames(subnames, outputVariableName);
}

