// ***************************************************************************
//
// Copyright (c) 2000 - 2010, Lawrence Livermore National Security, LLC
// Produced at the Lawrence Livermore National Laboratory
// LLNL-CODE-442911
// All rights reserved.
//
// This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
// full copyright notice is contained in the file COPYRIGHT located at the root
// of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
//
// Redistribution  and  use  in  source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
//  - Redistributions of  source code must  retain the above  copyright notice,
//    this list of conditions and the disclaimer below.
//  - Redistributions in binary form must reproduce the above copyright notice,
//    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
//    documentation and/or other materials provided with the distribution.
//  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
//    be used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
// ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
// LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
// DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
// SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
// CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
// OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ***************************************************************************

package llnl.visit.operators;

import llnl.visit.AttributeSubject;
import llnl.visit.CommunicationBuffer;
import llnl.visit.Plugin;

// ****************************************************************************
// Class: TubeAttributes
//
// Purpose:
//    This class contains attributes for the tube operator.
//
// Notes:      Autogenerated by xml2java.
//
// Programmer: xml2java
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

public class TubeAttributes extends AttributeSubject implements Plugin
{
    private static int numAdditionalAttributes = 5;

    public TubeAttributes()
    {
        super(numAdditionalAttributes);

        scaleByVarFlag = false;
        width = 0.5f;
        scaleVariable = new String("");
        fineness = 3;
        capping = false;
    }

    public TubeAttributes(int nMoreFields)
    {
        super(numAdditionalAttributes + nMoreFields);

        scaleByVarFlag = false;
        width = 0.5f;
        scaleVariable = new String("");
        fineness = 3;
        capping = false;
    }

    public TubeAttributes(TubeAttributes obj)
    {
        super(numAdditionalAttributes);

        scaleByVarFlag = obj.scaleByVarFlag;
        width = obj.width;
        scaleVariable = new String(obj.scaleVariable);
        fineness = obj.fineness;
        capping = obj.capping;

        SelectAll();
    }

    public int Offset()
    {
        return super.Offset() + super.GetNumAdditionalAttributes();
    }

    public int GetNumAdditionalAttributes()
    {
        return numAdditionalAttributes;
    }

    public boolean equals(TubeAttributes obj)
    {
        // Create the return value
        return ((scaleByVarFlag == obj.scaleByVarFlag) &&
                (width == obj.width) &&
                (scaleVariable.equals(obj.scaleVariable)) &&
                (fineness == obj.fineness) &&
                (capping == obj.capping));
    }

    public String GetName() { return "Tube"; }
    public String GetVersion() { return "1.0"; }

    // Property setting methods
    public void SetScaleByVarFlag(boolean scaleByVarFlag_)
    {
        scaleByVarFlag = scaleByVarFlag_;
        Select(0);
    }

    public void SetWidth(float width_)
    {
        width = width_;
        Select(1);
    }

    public void SetScaleVariable(String scaleVariable_)
    {
        scaleVariable = scaleVariable_;
        Select(2);
    }

    public void SetFineness(int fineness_)
    {
        fineness = fineness_;
        Select(3);
    }

    public void SetCapping(boolean capping_)
    {
        capping = capping_;
        Select(4);
    }

    // Property getting methods
    public boolean GetScaleByVarFlag() { return scaleByVarFlag; }
    public float   GetWidth() { return width; }
    public String  GetScaleVariable() { return scaleVariable; }
    public int     GetFineness() { return fineness; }
    public boolean GetCapping() { return capping; }

    // Write and read methods.
    public void WriteAtts(CommunicationBuffer buf)
    {
        if(WriteSelect(0, buf))
            buf.WriteBool(scaleByVarFlag);
        if(WriteSelect(1, buf))
            buf.WriteFloat(width);
        if(WriteSelect(2, buf))
            buf.WriteString(scaleVariable);
        if(WriteSelect(3, buf))
            buf.WriteInt(fineness);
        if(WriteSelect(4, buf))
            buf.WriteBool(capping);
    }

    public void ReadAtts(int index, CommunicationBuffer buf)
    {
        switch(index)
        {
        case 0:
            SetScaleByVarFlag(buf.ReadBool());
            break;
        case 1:
            SetWidth(buf.ReadFloat());
            break;
        case 2:
            SetScaleVariable(buf.ReadString());
            break;
        case 3:
            SetFineness(buf.ReadInt());
            break;
        case 4:
            SetCapping(buf.ReadBool());
            break;
        }
    }

    public String toString(String indent)
    {
        String str = new String();
        str = str + boolToString("scaleByVarFlag", scaleByVarFlag, indent) + "\n";
        str = str + floatToString("width", width, indent) + "\n";
        str = str + stringToString("scaleVariable", scaleVariable, indent) + "\n";
        str = str + intToString("fineness", fineness, indent) + "\n";
        str = str + boolToString("capping", capping, indent) + "\n";
        return str;
    }


    // Attributes
    private boolean scaleByVarFlag;
    private float   width;
    private String  scaleVariable;
    private int     fineness;
    private boolean capping;
}

