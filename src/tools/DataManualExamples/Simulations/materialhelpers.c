#include "materialhelpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MATERIAL_BASE 0

/******************************************************************************
 *
 * Purpose: Allocates a VisIt_MaterialData
 *
 * Programmer: Brad Whitlock
 * Date:       Thu Jan 17 16:54:41 PST 2008
 *
 * Input Arguments:
 *   nCells : The number of cells in the mesh.
 *
 * Output Arguments:
 *   arrlen : Pointer to an integer in which the length of the mix arrays will
 *            be stored.
 *
 * Returns:
 *   An initialized VisIt_MaterialData structure.
 *
 * Modifications:
 *   Brad Whitlock, Tue Feb 10 13:44:01 PST 2009
 *   I changed some of the arrays into VisIt_DataArray objects.
 *
 *****************************************************************************/

void
VisIt_MaterialData_init(VisIt_MaterialData *m, int nCells, int *arrlen)
{
    m->nzones = nCells;
    m->matlist.dataType = VISIT_DATATYPE_INT;
    m->matlist.owner = VISIT_OWNER_VISIT;
    m->matlist.iArray = (int*)malloc(sizeof(int)*m->nzones);

    *arrlen = m->nzones;
    m->mixlen = 0;
    m->mix_mat.dataType = VISIT_DATATYPE_INT;
    m->mix_mat.owner = VISIT_OWNER_VISIT;
    m->mix_mat.iArray = (int*)malloc(sizeof(int) * *arrlen);

    m->mix_zone.dataType = VISIT_DATATYPE_INT;
    m->mix_zone.owner = VISIT_OWNER_VISIT;
    m->mix_zone.iArray = (int*)malloc(sizeof(int) * *arrlen);

    m->mix_next.dataType = VISIT_DATATYPE_INT;
    m->mix_next.owner = VISIT_OWNER_VISIT;
    m->mix_next.iArray = (int*)malloc(sizeof(int) * *arrlen);

    m->mix_vf.dataType = VISIT_DATATYPE_FLOAT;
    m->mix_vf.owner = VISIT_OWNER_VISIT;
    m->mix_vf.fArray =   (float*)malloc(sizeof(float) * *arrlen);
}

/******************************************************************************
 *
 * Purpose: Adds a material to the object.
 *
 * Programmer: Brad Whitlock
 * Date:       Thu Jan 17 16:58:03 PST 2008
 *
 * Input Arguments:
 *   matname : The name of the material to add.
 *
 * Input / Output Arguments:
 *   m : The material object.
 *
 * Returns:
 *   An integer id that will be used to identify the material in the data.
 *
 * Modifications:
 *
 *****************************************************************************/

int
VisIt_MaterialData_addMaterial(VisIt_MaterialData *m, const char *matname)
{
    int id = m->nMaterials + MATERIAL_BASE;
    if(m->nMaterials == 0)
    {
        m->materialNumbers = (int*)malloc(sizeof(int));
        m->materialNumbers[0] = id;

        m->materialNames = (char **)malloc(sizeof(char *));
        m->materialNames[0] = strdup(matname);
    }
    else
    {
        m->materialNumbers = (int*)realloc(m->materialNumbers, (m->nMaterials+1) * sizeof(int));
        m->materialNumbers[m->nMaterials] = id;

        m->materialNames = (char **)realloc(m->materialNames, (m->nMaterials+1) * sizeof(char *));
        m->materialNames[m->nMaterials] = strdup(matname);
    }
    ++m->nMaterials;
    return id;
}

/******************************************************************************
 *
 * Purpose: Adds a clean material cell to the object.
 *
 * Programmer: Brad Whitlock
 * Date:       Thu Jan 17 16:58:03 PST 2008
 *
 * Input Arguments:
 *   cell : The cell for which we're adding material data.
 *   id   : The id of the material as returned by md_addMaterial.
 *
 * Input / Output Arguments:
 *   m : The material object.
 *
 * Modifications:
 *
 *****************************************************************************/

void
VisIt_MaterialData_addCleanCell(VisIt_MaterialData *m, int cell, int id)
{
    m->matlist.iArray[cell] = id;
}

/******************************************************************************
 *
 * Purpose: Adds a mixed material cell to the object.
 *
 * Programmer: Brad Whitlock
 * Date:       Thu Jan 17 16:58:03 PST 2008
 *
 * Input Arguments:
 *   cell  : The cell for which we're adding material data.
 *   id    : The list of material ids in the cell as returned by md_addMaterial.
 *   vf    : The list of material volume fractions. They must sum to 1.
 *   nmats : The number of items in the id,vf arrays. i.e. the number of materials
 *           in the cell.
 *
 * Input / Output Arguments:
 *   m      : The material object.
 *   arrlen : The length of the mix arrays allocated to the object.
 *
 * Modifications:
 *   Brad Whitlock, Tue Feb 10 13:45:38 PST 2009
 *   I changed some arrays to VisIt_DataArray.
 *
 *****************************************************************************/

void
VisIt_MaterialData_addMixedCell(VisIt_MaterialData *m, int cell, int *id, float *vf, int nmats, int *arrlen)
{
    int i;
    if(m->mixlen + nmats >= *arrlen)
    {
        /* Must resize the mix arrays */
        *arrlen += 1000;
        m->mix_mat.iArray =  (int*)realloc(m->mix_mat.iArray,  sizeof(int) * *arrlen);
        m->mix_zone.iArray = (int*)realloc(m->mix_zone.iArray, sizeof(int) * *arrlen);
        m->mix_next.iArray = (int*)realloc(m->mix_next.iArray, sizeof(int) * *arrlen);
        m->mix_vf.fArray =   (float*)realloc(m->mix_vf.fArray, sizeof(float) * *arrlen);
    }
    
    m->matlist.iArray[cell] = -(m->mixlen + 1);

    for(i = 0; i < nmats; ++i)
    {
        m->mix_mat.iArray[m->mixlen] = id[i];
        m->mix_zone.iArray[m->mixlen] = cell;
        m->mix_next.iArray[m->mixlen] = 
        m->mix_vf.fArray[m->mixlen] = vf[i];

        if(i < nmats - 1)
            m->mix_next.iArray[m->mixlen] = m->mixlen + 2;
        else
            m->mix_next.iArray[m->mixlen] = 0;

        ++m->mixlen;
    }
}