/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include <Sm/Ph/SpatialContextGeom.h>
#include <Sm/Error.h>


FdoSmPhSpatialContextP FdoSmPhSpatialContextGeom::GetSpatialContext()
{
    Finalize();

	return mSpatialContext;
}

void FdoSmPhSpatialContextGeom::Finalize()
{
    if ( GetState() == FdoSmObjectState_Initial) {
        SetState( FdoSmObjectState_Finalizing );

        FdoStringP  scInfoTable = GetManager()->GetRealDbObjectName( FdoSmPhMgr::ScInfoNoMetaTable );
        bool tableIsScInfo = (GetGeomTableName() == scInfoTable);
	    int indexSC = -1; 
        
        if ( mIsDerived && !tableIsScInfo ) 
        {
            // Try to derive spatial context from base SCGeom.
            // Set this SCGeom to not derived and set it back to true if we successfully
            // derive the spatial context.
            mIsDerived = false;
            
            // Get the column for this SCGeom
            FdoSmPhDbObjectP dbObject = mpOwner->FindDbObject( GetGeomTableName() );

            if ( dbObject ) 
            {
                FdoSmPhColumnP geomColumn = dbObject->GetColumns()->FindItem( GetGeomColumnName() );
                
                if ( geomColumn ) 
                {
                    // Get the root column for the column.
                    FdoSmPhColumnGeomP rootColumn = geomColumn->GetRootColumn().p->SmartCast<FdoSmPhColumnGeom>();
        
                    if ( rootColumn )
                    {
                        // Get the Spatial Context for the root column.
                        FdoSmPhSpatialContextP rootSc = rootColumn->GetSpatialContext();
                        
                        if ( rootSc ) 
                        {
                            // If SRID already determined for this SCGeom, derive the spatial context
                            // only if it has the same SRID. It is possible for a view column to have a
                            // different sampled SRID than its corresponding base table column.
                            if ( (mSpatialContext->GetSrid() <= 0) || (rootSc->GetSrid() == mSpatialContext->GetSrid()) ) 
                            {
                                mSpatialContext = rootSc;
                                mIsDerived = true;
                            }
                        }
                    }
                }
            }
        }

        // Check if there is already a Spatial Contexts with same attributes as current
        // spatial contexts. If there is, use the existing SC instead. This coalesces
        // equivalent SC's.
        indexSC = mSpatialContexts->FindExistingSC( mSpatialContext );

        if ( indexSC >= 0 ) 
        {
            FdoSmPhSpatialContextP existingSC = mSpatialContexts->GetItem( indexSC );
            // Don't coalesce spatial contexts from the ScInfo table; each
            // column represents a different spatial context even if the
            // spatial context attributes are the same.
            if ( tableIsScInfo && (FdoStringP(mSpatialContext->GetName()) != existingSC->GetName()) ) 
                indexSC = -1;
            else
                mSpatialContext = existingSC;
        }

        if ( indexSC < 0 ) 
        {
            // Spatial Context not coalesced, need to add it.
	        FdoStringP	scName;
            if ( tableIsScInfo ) 
            {
                // For column in ScInfo table, the spatial context name is the
                // column name.
                scName = GetGeomColumnName();
            }
            else
            {
                scName = mSpatialContexts->AutoGenName();
            }

            FdoSmPhSpatialContextP newSC = new FdoSmPhSpatialContext(
                GetManager(),
                mSpatialContext->GetSrid(),
                scName,
                mSpatialContext->GetDescription(),
                mSpatialContext->GetCoordinateSystem(),
                mSpatialContext->GetCoordinateSystemWkt(),
                mSpatialContext->GetExtentType(),
                FdoPtr<FdoByteArray>(mSpatialContext->GetExtent()),
                mSpatialContext->GetXYTolerance(),
                mSpatialContext->GetZTolerance()
            );

            if (NULL == newSC.p)
		        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            mSpatialContext = newSC;
            mSpatialContexts->Add( mSpatialContext );
        }

        SetState( FdoSmObjectState_Final );
    }
}
