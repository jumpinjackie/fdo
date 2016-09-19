/*
 * Copyright (C) 2004-2011  Autodesk, Inc.
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

#ifndef FdoWmsGetFeatureInfo_H
#define FdoWmsGetFeatureInfo_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "WMS/IGetFeatureInfo.h"
#include "FdoWmsCommand.h"

// Forward declaration
class FdoWmsDelegate;

class FdoWmsGetFeatureInfo : public FdoWmsCommand<FdoWmsIGetFeatureInfo>
{
	friend class FdoWmsConnection;

protected:
	FdoWmsGetFeatureInfo(FdoIConnection* connection);
	virtual ~FdoWmsGetFeatureInfo(void);
	
	// <summary>Dispose this object.</summary>
	// <returns>Returns nothing</returns> 
	virtual void Dispose ();

public:
    /// \brief
    /// Execute the FdoWmsIGetFeatureInfo command and returns a 
    /// FdoIoStream* pointer. It could be a text, image, video and any 
    /// binary data stream, ect. The result depends on the specified output
    /// format.
    /// 
    /// \return
    /// Return a FdoIoStream representing the feature information.
    /// 
    virtual FdoIoStream* Execute();

    /// \brief
    /// Set the feature class name for which feature information
    /// will be retrieved. This function is mandatory; if not specified or the feature class 
    /// name can’t be found in the schema, execution of the command will throw an
    /// exception. 
    /// 
    /// \param value 
    /// Input the feature class name.
    /// 
    /// \return
    /// Returns nothing 
    /// 
    virtual void SetFeatureClassName(FdoIdentifier* value);

    /// \brief
    /// Get the name of the feature class for which feature information 
    /// will be retrieved.
    /// 
    /// \return
    /// Input the query feature class name.
    /// 
    virtual FdoIdentifier* GetFeatureClassName();

    /// \brief
    /// Set the output feature information format. The supported values should
    /// be listed in one or more <Format> elements inside the <Request><FeatureInfo>
    /// element of the Capabilities XML.
    /// An exception would be thrown if the value isn't in the supported list when
    /// executing the command.
    /// 
    /// \para value
    /// Input the output feature information format.
    /// 
    virtual void SetOutputFormat(FdoString* value);

    /// \brief
    /// Get the format used when returning the feature information.
    /// 
    /// \return
    /// Return the output feature information format.
    /// 
    virtual FdoString* GetOutputFormat();

    /// \brief
    /// Set the bounding box that would be used to replace the one in 
    /// cached GetMap request. If it's not set or the bounding box is 
    /// empty, directly use the one in cached GetMap request.
    /// 
    /// \param value
    /// Input the interest bounding box.
    /// 
    virtual void SetBoundingBox(FdoIEnvelope* value);

    /// \brief
    /// Get the bounding box that's specified to replace the one in 
    /// cached GetMap request.
    /// 
    /// \return
    /// Return the specified bounding box for GetMap request.
    /// 
    virtual FdoIEnvelope* GetBoundingBox();

    /// \brief
    /// Set the height to replace the one used in the cached GetMap
    /// request. Its default value is zero, if the specified value is 
    /// greater than zero, the replacement would happen. Otherwise, 
    /// directly use the one in the cached GetMap request.
    /// 
    /// \param value
    /// Input the specified image height.
    /// 
    virtual void SetHeight(FdoSize value);

    /// \brief
    /// Get the height that's specified to replace the one in cached 
    /// GetMap request. Its default value is zero.
    /// 
    /// \return
    /// Return specified image height.
    /// 
    virtual FdoSize GetHeight();

    /// \brief
    /// Set the width to replace the one used in the cached GetMap
    /// request. Its default value is zero, if the specified value is 
    /// greater than zero, the replacement would happen. Otherwise, 
    /// directly use the one in the cached GetMap request.
    /// 
    /// \param value
    /// Input the specified image width.
    ///
    virtual void SetWidth(FdoSize value);

    /// \brief
    /// Get the width that's specified to replace the one in cached 
    /// GetMap request. Its default value is zero.
    /// 
    /// \return
    /// Return specified image width.
    ///
    virtual FdoSize GetWidth();

    /// \brief
    /// Set the position (X, Y) which indicates a point of interest on the map that was
    /// produced by the embedded GetMap request. The point(X, Y) is a
    /// point in the (i, j) space defined by the Map CS.
    /// This function is mandatory; if not specified, 
    /// execution of the command will throw an exception. 
    /// 
    /// \param pos 
    /// Input the point's coordinate value (X, Y)
    ///
    /// \return
    /// Return nothing
    /// 
    virtual void SetPosition(FdoIDirectPosition* pos);

    /// \brief
    /// Get the position (X, Y) which indicates a point of interest on the map that was
    /// produced by the embedded !GetMap request. The point (X, Y) is a
    /// point in the (i, j) space defined by the Map CS.
    ///
    /// \return
    /// Return the interesting position.
    /// 
    virtual FdoIDirectPosition* GetPosition();

    /// \brief
    /// Set the number of features about which to return information (default=1).
    /// 
    /// \param featureCount
    /// The number of the features to query, default to 1..
    ///
    /// \return
    /// Returns nothing
    /// 
    virtual void SetFeatureCount(FdoInt32 featureCount = 1);

    /// \brief
    /// Get the number of features about which to return information.
    /// 
    /// \return
    /// Returns candidate feature number.
    /// 
    virtual FdoInt32 GetFeatureCount ();

private:
    //
	// create a WMS delegate to get the map
	FdoWmsDelegate* _getWmsDelegate ();

	// Some layers may contain characters ":" or "." which are not allowed in FDO class name.
	// When the layer is mapped to an FDO class, it's name will be mangled: the invalid characters
	// will be filled with blank. The "mapping" between original and mangled names are kept in a
	// dictionary data structure.
	// This method is used to get the original name of the layer.
	FdoString* _getOriginalLayerName (FdoString* mangledLayerName);

    // Get the first feature info format in the capabilities document as the default one. 
    FdoString* _getDefaultFeatureInfoFormat();

private:
    //
    // The feature class mapped to WMS layers to get the feature information.
    FdoPtr<FdoIdentifier> mFeatureClass;

    // Specify the point position for GetFeatureInfo request.
    FdoPtr<FdoIDirectPosition> mPosition;

    // Store the FEATURE_COUNT for GetFeatureInfo request.
	FdoInt32 mFeatureCount;

    // Used to update the the resolution for GetMap request.
    FdoSize mHeight, mWidth;

    // Used to update the bounding box in the same CS for GetMap request.
    FdoPtr<FdoIEnvelope> mBoundingBox;

    // Specify the output feature information format.
    FdoStringP mOutputFormat;
};
#endif
