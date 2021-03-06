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
//                            ViewerPlotFactory.C                            //
// ************************************************************************* //

#include <ViewerPlotFactory.h>

#include <AttributeSubject.h>
#include <PlotPluginInfo.h>
#include <PlotPluginManager.h>
#include <ViewerPlot.h>


// ****************************************************************************
//  Method: ViewerPlotFactory constructor
//
//  Programmer: Eric Brugger
//  Creation:   August 23, 2000
//
//  Modifications:
//    Hank Childs, Wed Sep 27 11:40:52 PDT 2000
//    Initialize the Functions.
//
//    Eric Brugger, Wed Dec 20 10:29:59 PST 2000
//    I removed the argument maxTypes.  I added code to register the various
//    plot types with the factory.
//
//    Hank Childs, Wed Jan 10 11:55:30 PST 2001
//    Added volume plots.
//
//    Kathleen Bonnell, Thu Feb 22 14:11:43 PST 2001 
//    Added contour plots.
//
//    Eric Brugger, Wed Feb 28 12:03:16 PST 2001
//    Renamed the class ViewerPseudocolorPlot to ViewerPseudoColorPlot.
//
//    Kathleen Bonnell, Tue Mar  6 16:36:03 PST 2001 
//    Add surface plots. 
//
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
//    Jeremy Meredith, Thu Jul 26 09:53:57 PDT 2001
//    Renamed plugin info to include the word "plot".
//
//    Jeremy Meredith, Fri Sep 28 13:47:32 PDT 2001
//    Removed the general plugin info since the viewer info is derived
//    from it now.
//
//    Brad Whitlock, Tue Jun 24 14:48:45 PDT 2008
//    Inherit ViewerBase.
//
// ****************************************************************************

ViewerPlotFactory::ViewerPlotFactory() : ViewerBase()
{

    PlotPluginManager *plotPluginMgr = GetPlotPluginManager();

    nTypes = plotPluginMgr->GetNEnabledPlugins();
    viewerPluginInfo  = new ViewerPlotPluginInfo*[nTypes];

    for (int i = 0; i < nTypes; ++i)
    {
        viewerPluginInfo[i]  = plotPluginMgr->GetViewerPluginInfo(
                                               plotPluginMgr->GetEnabledID(i));
    }
}

// ****************************************************************************
//  Method: ViewerPlotFactory destructor
//
//  Programmer: Eric Brugger
//  Creation:   August 23, 2000
//
//  Modifications:
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
//    Jeremy Meredith, Fri Sep 28 13:47:32 PDT 2001
//    Removed the general plugin info since the viewer info is derived
//    from it now.
//
// ****************************************************************************

ViewerPlotFactory::~ViewerPlotFactory()
{
    delete [] viewerPluginInfo;
}

// ****************************************************************************
//  Method: ViewerPlotFactory::GetNPlotTypes
//
//  Purpose:
//    Return the number of plot types.
//
//  Returns:    The number of plot types.
//
//  Programmer: Eric Brugger
//  Creation:   August 24, 2000
//
//  Modifications:
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
// ****************************************************************************

int
ViewerPlotFactory::GetNPlotTypes() const
{
    return nTypes;
}

// ****************************************************************************
//  Method: ViewerPlotFactory::CreatePlot
//
//  Purpose:
//    Create a plot of the specified type.
//
//  Arguments:
//    type    The type of plot to create.
//
//  Programmer: Eric Brugger
//  Creation:   September 21, 2000
//
//  Modifications:
//    Brad Whitlock, Fri Dec 8 17:19:19 PST 2000
//    I added database-related arguments.
//
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
//    Hank Childs, Fri Jun 15 11:41:58 PDT 2001
//    Added sil restriction argument.
//
//    Jeremy Meredith, Fri Sep 28 13:47:32 PDT 2001
//    Removed the general plugin info since the viewer info is derived
//    from it now.
//
//    Brad Whitlock, Fri Apr 4 10:28:30 PDT 2003
//    I added the nStates argument.
//
//    Brad Whitlock, Fri Mar 26 13:53:59 PST 2004
//    I replaced time0, time1 with plotState.
//
//    Jeremy Meredith, Tue Mar 30 10:39:20 PST 2004
//    Added an engine key to map a plot to the engine used to create it.
//
//    Brad Whitlock, Fri Apr 2 16:06:22 PST 2004
//    I added a couple more arguments.
//
// ****************************************************************************

ViewerPlot *
ViewerPlotFactory::CreatePlot(const int type, const EngineKey &ek,
                              const std::string &hostName,
                              const std::string &databaseName,
                              const std::string &varName,
                              avtSILRestriction_p silr,
                              const int plotState,
                              const int nStates,
                              const int cacheIndex,
                              const int nCacheIndices) const
{
    //
    // Check that the type is within range and that the type has been
    // registered.
    //
    if (type < 0 || type >= nTypes)
    {
        return 0;
    }

    //
    // Create the plot.
    //
    return new ViewerPlot(type,
        viewerPluginInfo[type], ek, hostName, databaseName, varName, silr,
        plotState, nStates, cacheIndex, nCacheIndices);
}

// ****************************************************************************
//  Method: ViewerPlotFactory::GetDefaultAtts
//
//  Purpose:
//    Return a pointer to the default attribute subject for the specified
//    plot type.
//
//  Arguments:
//    type    The type of plot to return the attribute subject for.
//
//  Programmer: Eric Brugger
//  Creation:   December 20, 2000
//
//  Modifications:
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
// ****************************************************************************

AttributeSubject *
ViewerPlotFactory::GetDefaultAtts(const int type) const
{
    //
    // Check that the type is within range.
    //
    if (type < 0 || type >= nTypes)
    {
        return 0;
    }

    //
    // Return the appropriate attribute subject.
    //
    return viewerPluginInfo[type]->GetDefaultAtts();
}

// ****************************************************************************
//  Method: ViewerPlotFactory::GetClientAtts
//
//  Purpose:
//    Return a pointer to the client attribute subject for the specified
//    plot type.
//
//  Arguments:
//    type    The type of plot to return the attribute subject for.
//
//  Programmer: Eric Brugger
//  Creation:   December 20, 2000
//
//  Modifications:
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
// ****************************************************************************

AttributeSubject *
ViewerPlotFactory::GetClientAtts(const int type) const
{
    //
    // Check that the type is within range.
    //
    if (type < 0 || type >= nTypes)
    {
        return 0;
    }

    //
    // Return the appropriate attribute subject.
    //
    return viewerPluginInfo[type]->GetClientAtts();
}

// ****************************************************************************
//  Method: ViewerPlotFactory::SetClientAttsFromDefault
//
//  Purpose:
//    Set the client attributes based on the default plot attributes
//    for the specified plot type.
//
//  Arguments:
//    type    The type of plot to set the client attributes for.
//
//  Programmer: Eric Brugger
//  Creation:   August 28, 2000
//
//  Modifications:
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
//    Eric Brugger, Thu Apr 26 14:03:49 PDT 2001
//    I added code to notify the client that the plot's client attributes
//    have changed.
//
//    Jeremy Meredith, Fri Sep 28 13:47:32 PDT 2001
//    Removed the general plugin info since the viewer info is derived
//    from it now.
//
// ****************************************************************************

void
ViewerPlotFactory::SetClientAttsFromDefault(const int type)
{
    //
    // Check that the type is within range and that the type has been
    // registered.
    //
    if (type < 0 || type >= nTypes)
    {
        return;
    }

    //
    // Copy the attributes.
    //
    AttributeSubject *clientAtts  = viewerPluginInfo[type]->GetClientAtts();
    AttributeSubject *defaultAtts = viewerPluginInfo[type]->GetDefaultAtts();

    viewerPluginInfo[type]->CopyAttributes(clientAtts, defaultAtts);
    clientAtts->Notify();
}

// ****************************************************************************
//  Method: ViewerPlotFactory::SetDefaultAttsFromClient
//
//  Purpose:
//    Set the default attributes based on the client plot attributes
//    for the specified plot type.
//
//  Arguments:
//    type    The type of plot to set the default attributes for.
//
//  Programmer: Eric Brugger
//  Creation:   August 29, 2000
//
//  Modifications:
//    Eric Brugger, Thu Mar  8 15:11:24 PST 2001
//    I modified the routine to use the plot plugin manager.
//
//    Jeremy Meredith, Fri Sep 28 13:47:32 PDT 2001
//    Removed the general plugin info since the viewer info is derived
//    from it now.
//
// ****************************************************************************

void
ViewerPlotFactory::SetDefaultAttsFromClient(const int type)
{
    //
    // Check that the type is within range and that the type has been
    // registered.
    //
    if (type < 0 || type >= nTypes)
    {
        return;
    }

    //
    // Copy the attributes.
    //
    viewerPluginInfo[type]->CopyAttributes(
        viewerPluginInfo[type]->GetDefaultAtts(),
        viewerPluginInfo[type]->GetClientAtts());
}
