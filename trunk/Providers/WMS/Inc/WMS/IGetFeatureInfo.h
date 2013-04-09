// 
//
// Copyright (C) 2004-2011  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

//
// By Cheney Sun.
//

#ifndef IGETFEATUREINFO_H
#define IGETFEATUREINFO_H

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Commands/ICommand.h>
#include <Fdo/Connections/IConnection.h>

/// \brief
/// The FdoWmsIGetFeatureInfo interface defines the 
/// FdoWmsIGetFeatureInfo command, which retrieves more information
/// about features in the maps. The Execute operation returns a 
/// FdoIoStream* type pointer pointing to the feature information
/// data stream.
class FdoWmsIGetFeatureInfo : public FdoICommand
{
    friend class FdoIConnection;
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
    FDO_API virtual FdoIoStream* Execute() = 0;

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
    FDO_API virtual void SetFeatureClassName(FdoIdentifier* value) = 0;

    /// \brief
    /// Get the name of the feature class for which feature information 
    /// will be retrieved.
    /// 
    /// \return
    /// Return the query feature class name.
    /// 
    FDO_API virtual FdoIdentifier* GetFeatureClassName() = 0;

    /// \brief
    /// Set the output feature information format. The supported values are MIME type
    /// strings and should be listed in one or more <Format> elements inside the 
    /// <Request><FeatureInfo> element of the Capabilities XML.
    /// An exception would be thrown if the value isn't in the supported list when
    /// executing the command. The default value is the first one specified in the 
    /// Capabilities document.
    /// 
    /// \para value
    /// Input the output feature information format.
    /// 
    FDO_API virtual void SetOutputFormat(FdoString* value) = 0;

    /// \brief
    /// Get the specified format for the returning feature information.
    /// 
    /// \return
    /// Return the output feature information format.
    /// 
    FDO_API virtual FdoString* GetOutputFormat() = 0;

    /// \brief
    /// Set the bounding box that would be used to replace the one in 
    /// cached GetMap request. If it's not set or the bounding box is 
    /// empty, directly use the one in cached GetMap request.
    /// 
    /// \param value
    /// Input the interest bounding box.
    /// 
    FDO_API virtual void SetBoundingBox(FdoIEnvelope* value) = 0;

    /// \brief
    /// Get the bounding box that's specified to replace the one in 
    /// cached GetMap request.
    /// 
    /// \return
    /// Return the specified bounding box for GetMap request.
    /// 
    FDO_API virtual FdoIEnvelope* GetBoundingBox() = 0;

    /// \brief
    /// Set the height to replace the one used in the cached GetMap
    /// request. Its default value is zero, if the specified value is 
    /// greater than zero, the replacement would happen. Otherwise, 
    /// directly use the one in the cached GetMap request.
    /// 
    /// \param value
    /// Input the specified image height.
    /// 
    FDO_API virtual void SetHeight(FdoSize value) = 0;

    /// \brief
    /// Get the height that's specified to replace the one in cached 
    /// GetMap request. Its default value is zero.
    /// 
    /// \return
    /// Return specified image height.
    /// 
    FDO_API virtual FdoSize GetHeight() = 0;

    /// \brief
    /// Set the width to replace the one used in the cached GetMap
    /// request. Its default value is zero, if the specified value is 
    /// greater than zero, the replacement would happen. Otherwise, 
    /// directly use the one in the cached GetMap request.
    /// 
    /// \param value
    /// Input the specified image width.
    ///
    FDO_API virtual void SetWidth(FdoSize value) = 0;

    /// \brief
    /// Get the width that's specified to replace the one in cached 
    /// GetMap request. Its default value is zero.
    /// 
    /// \return
    /// Return specified image width.
    ///
    FDO_API virtual FdoSize GetWidth() = 0;

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
    FDO_API virtual void SetPosition(FdoIDirectPosition* pos) = 0;

    /// \brief
    /// Get the position (X, Y) which indicates a point of interest on the map that was
    /// produced by the embedded !GetMap request. The point (X, Y) is a
    /// point in the (i, j) space defined by the Map CS.
    ///
    /// \return
    /// Return the point coordinate value (X, Y).
    /// 
    FDO_API virtual FdoIDirectPosition* GetPosition() = 0;

    /// \brief
    /// Set the number of features about which to return information (default=1).
    /// 
    /// \param featureCount
    /// The number of the features to query, default to 1..
    ///
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void SetFeatureCount(FdoInt32 featureCount = 1) = 0;

    /// \brief
    /// Get the number of features about which to return information.
    /// 
    /// \return
    /// Returns candidate feature number.
    /// 
    FDO_API virtual FdoInt32 GetFeatureCount () = 0;
};
#endif
