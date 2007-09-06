;//
;//Copyright (C) 2004-2006  Autodesk, Inc.
;//
;//This library is free software; you can redistribute it and/or
;//modify it under the terms of version 2.1 of the GNU Lesser
;//General Public License as published by the Free Software Foundation.
;//
;//This library is distributed in the hope that it will be useful,
;//but WITHOUT ANY WARRANTY; without even the implied warranty of
;//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;//Lesser General Public License for more details.
;//
;//You should have received a copy of the GNU Lesser General Public
;//License along with this library; if not, write to the Free Software
;//Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
;//

MessageId=1
SymbolicName=FDO_1_INDEXOUTOFBOUNDS
Language=English
Item index out of bounds.
.
MessageId=2
SymbolicName=FDO_1_BADPARAMETER
Language=English
Bad Parameter.
.
MessageId=3
SymbolicName=FDO_1_UNKNOWN_GEOMETRY_TYPE
Language=English
'%1$ls': Unknown geometry type '%2$d' discovered.
.
MessageId=4
SymbolicName=FDO_1_INVALID_NUM_OF_COORDINATE_POSITIONS
Language=English
'%1$ls': Invalid number of coordinate positions.
.
MessageId=5
SymbolicName=FDO_1_UNKNOWN_GEOMETRY_COMPONENT_TYPE
Language=English
'%1$ls': Unknown geometry component type '%2$d' discovered.
.
MessageId=6
SymbolicName=FDO_1_OPEN_RING
Language=English
'%1$ls': Identified an open ring when expecting a closed one.
.
MessageId=7
SymbolicName=FDO_1_RESOLUTION_TOO_FINE_FOR_EXTENT
Language=English
'%1$ls': The resolution is too fine for the extents.
.
MessageId=8
SymbolicName=FDO_1_RESOLUTION_TOO_LARGE_FOR_EXTENT
Language=English
'%1$ls': The resolution is too large (> 0.1) for the extents.
.
MessageId=9
SymbolicName=FDO_1_COLLINEAR_POINTS
Language=English
'%1$ls': The points are collinear.
.
MessageId=10
SymbolicName=FDO_1_MEMORY_DEALLOCATION_ERROR
Language=English
'%1$ls': Failed to free memory for object '%2$ls'.
.
MessageId=11
SymbolicName=FDO_1_INVALID_INPUT_ON_CLASS_CREATION
Language=English
Cannot create instance of class '%1$ls' due to invalid value for the input parameter(s) '%2$ls'.
.
MessageId=12
SymbolicName=FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION
Language=English
'%1$ls': Cannot execute function due to invalid value for the input parameter(s) '%2$ls'.
.

MessageId=1001
SymbolicName=SCHEMA_1_BADATTRIBUTENAME
Language=English
Bad schema attribute name.
.
MessageId=1002
SymbolicName=SCHEMA_2_BASEPROPERTIESALREADYSET
Language=English
Base properties already set.
.
MessageId=1003
SymbolicName=SCHEMA_3_CLASSNOTADDED
Language=English
Class has not already been added to the FeatureSchema.
.
MessageId=1004
SymbolicName=SCHEMA_4_IDENTITYPROPERTYNOTPROPERTY
Language=English
Identity Property must be member of Property collection.
.
MessageId=1005
SymbolicName=SCHEMA_5_INVALIDELEMENTNAME
Language=English
Invalid element name.
.
MessageId=1006
SymbolicName=SCHEMA_6_NAMENOTINATTRIBUTEDICTIONARY
Language=English
Name not in schema attribute dictionary.
.
MessageId=1007
SymbolicName=SCHEMA_7_RELATIONENDNOTADDED
Language=English
RelationEnd has not been added to a FeatureSchema.
.
MessageId=1008
SymbolicName=SCHEMA_8_ATTRIBUTEALREADYINDICTIONARY
Language=English
Schema attribute already in dictionary.
.
MessageId=1009
SymbolicName=SCHEMA_9_ATTRIBUTENOTFOUND
Language=English
Schema attribute not found.
.
MessageId=1010
SymbolicName=SCHEMA_10_OBJECTHASPARENT
Language=English
Object already has parent.
.
MessageId=1011
SymbolicName=SCHEMA_11_OWNPARENT
Language=English
Element cannot be its own parent/grandparent.
.
MessageId=1012
SymbolicName=SCHEMA_12_OWNBASECLASS
Language=English
ClassDefinition cannot be its own base class.
.
MessageId=1013
SymbolicName=SCHEMA_13_IDENTITYPROPERTYONSUBCLASS
Language=English
A Class with a Base Class cannot have an Identity Property.
.
MessageId=1014
SymbolicName=SCHEMA_14_SUBCLASSWITHIDENTITYPROPERTIES
Language=English
A Class with Identity Properties cannot have a Base Class.
.
MessageId=1015
SymbolicName=SCHEMA_15_NEGATIVELENGTHNOTALLOWED
Language=English
A Data Property cannot have a negative length.
.
MessageId=1016
SymbolicName=SCHEMA_16_NEGATIVEPRECISIONNOTALLOWED
Language=English
A Data Property cannot have a negative precision.
.
MessageId=1017
SymbolicName=SCHEMA_17_ASSOCIATIONMISSMATCHERROR
Language=English
Association Property '%1$ls' has type or number missmatch between the identity properties and the reverse identity properties. 
.
MessageId=1018
SymbolicName=SCHEMA_18_ASSOCIATIONIDENTITYERROR
Language=English
Identity properties not part of the associated class.
.
MessageId=1019
SymbolicName=SCHEMA_19_UNSUPPORTEDXSDELEMENT
Language=English
Error reading schema '#1$ls' from XML, element '#2$ls' in #3$ls not supported by FDO.
.
MessageId=1020
SymbolicName=SCHEMA_20_UNSUPPORTEDXSDATTRIBUTE
Language=English
Error reading schema '#1$ls' from XML, attribute '#2$ls' in #3$ls not supported by FDO.
.
MessageId=1021
SymbolicName=SCHEMA_21_UNSUPPORTEDXSDATTRIBUTEVALUE
Language=English
Error reading schema '#1$ls' from XML, #2$ls/@#3$ls value '#4$ls' not supported by FDO.
.
MessageId=1022
SymbolicName=SCHEMA_22_MISSINGOBJPROPCLASS
Language=English
Error reading schema from XML, class %1$ls for object property %2$ls not found in current feature schema collection.
.
MessageId=1023
SymbolicName=SCHEMA_23_UNSUPPORTEDXSDGBLELEM
Language=English
Error reading schema '#1$ls' from XML, global element #2$ls type must be '#3$ls' or '#4$ls'.
.
MessageId=1024
SymbolicName=SCHEMA_24_CLASSTYPECONFLICT
Language=English
Cannot change class type of class '%1$ls'. 
.
MessageId=1025
SymbolicName=SCHEMA_25_PROPERTYTYPECONFLICT
Language=English
Cannot change property type of property '%1$ls'. 
.
MessageId=1026
SymbolicName=SCHEMA_26_BASECLASSREF
Language=English
Error resolving Feature Schema references; base class '%1$ls:%2$ls' for class '%3$ls' not found. 
.
MessageId=1027
SymbolicName=SCHEMA_27_OBJPROPCLASSREF
Language=English
Error reading Feature Schemas from XML; class '%1$ls:%2$ls' for object property '%3$ls' not found. 
.
MessageId=1028
SymbolicName=SCHEMA_28_BASEIDPROPS
Language=English
Error reading Feature Schemas from XML; identity properties for class '%1$ls' differ from those of base class '%2$ls'.
.
MessageId=1029
SymbolicName=SCHEMA_29_IDPROPREF
Language=English
Error reading Feature Schemas from XML; identity property '%1$ls' is not a property in class '%2$ls'.
.
MessageId=1030
SymbolicName=SCHEMA_30_BASEIDPROPS
Language=English
Error reading Feature Schemas from XML; object property '%1$ls' has no class; cannot resolve its identity property.
.
MessageId=1031
SymbolicName=SCHEMA_31_OBJPROPIDPROPREF
Language=English
Error reading Feature Schemas from XML; identity property '%1$ls' for object property '%2$ls' not found.
.
MessageId=1032
SymbolicName=SCHEMA_32_GEOMPROPREF
Language=English
Error resolving Feature Schemas references; geometry property '%1$ls' for feature class '%2$ls' not found.
.
MessageId=1033
SymbolicName=SCHEMA_33_BADDATAPROPSTRING
Language=English
Invalid property data type string %1$ls.
.
MessageId=1034
SymbolicName=SCHEMA_34_BADDATAPROPTYPE
Language=English
Invalid property data type %1$d.
.
MessageId=1035
SymbolicName=SCHEMA_35_NOCLASSTYPE
Language=English
Cannot determine class type for class %1$ls.
.
MessageId=1036
SymbolicName=SCHEMA_36_ABSTRACTCONFLICT
Language=English
Error reading schema '#1$ls' from XML, abstract setting for  '#2$ls' differs from that of its global element '#3$ls.
.
MessageId=1037
SymbolicName=SCHEMA_37_REQUIREDNODEFEATURE
Language=English
Start and end nodes of a network link feature can only be associated to a network node feature
.
MessageId=1038
SymbolicName=SCHEMA_38_REQUIREDLAYERCLASS
Language=English
The layer property can only be associated to a network layer class
.
MessageId=1039
SymbolicName=SCHEMA_39_REQUIREDNETWORKFCLASS
Language=English
The network property can only be associated to a network class
.
MessageId=1040
SymbolicName=SCHEMA_40_REQUIREDNUMERICTYPE
Language=English
The cost property type must be numeric.
.
MessageId=1041
SymbolicName=SCHEMA_41_PARENTCHILDNOTSAMETYPE
Language=English
The network feature parent is not the same type as the child.
.
MessageId=1042
SymbolicName=SCHEMA_42_PARENTCHILDNETWORKNOTSAME
Language=English
The network feature parent network class is not the same as the child network class.
.
MessageId=1043
SymbolicName=SCHEMA_43_COSTPROPERTYNOTFOUND
Language=English
The network feature cost property is not a feature property.
.
MessageId=1044
SymbolicName=SCHEMA_44_LINKSTARTORENDNODENETWORKERROR
Language=English
The start and end node network class must be the same as the link network class.
.
MessageId=1045
SymbolicName=SCHEMA_45_ASSOCPROPCLASSREF
Language=English
Error reading Feature Schemas from XML; class '%1$ls:%2$ls' for association property '%3$ls' not found.
.
MessageId=1046
SymbolicName=SCHEMA_46_ASSOCPROPS
Language=English
Error reading Feature Schemas from XML; association property '%1$ls' has no class; cannot resolve its identity property.
.
MessageId=1047
SymbolicName=SCHEMA_47_ASSOCPROPIDPROPREF
Language=English
Error reading Feature Schemas from XML; identity property '%1$ls' for association property '%2$ls' not found.
.
MessageId=1048
SymbolicName=SCHEMA_48_ASSOCREVERSEPROPCLASSREF
Language=English
Error reading Feature Schemas from XML; reverse association property '%1$ls' has no class; cannot resolve its identity property.
.
MessageId=1049
SymbolicName=SCHEMA_49_ASSOCREVERSEPROPIDPROPREF
Language=English
Error reading Feature Schemas from XML; identity property '%1$ls' for reverse association property '%2$ls' not found.
.
MessageId=1050
SymbolicName=SCHEMA_50_NETWORKLAYERPROPREF
Language=English
Error reading Feature Schemas from XML; class '%1$ls:%2$ls' for network layer property '%3$ls' not found.
.
MessageId=1051
SymbolicName=SCHEMA_51_CLASSNOSCHEMA
Language=English
Element '%1$ls' references class (%2$ls) which does not belong to a feature schema.
.
MessageId=1052
SymbolicName=SCHEMA_52_BADELEMENTNAME
Language=English
Invalid Feature schema element name '%1$ls'; must not contain '%2$c'.
.
MessageId=1053
SymbolicName=SCHEMA_53_BADASSOCREVERSEPROP
Language=English
Cannot use auto-generated or system properties as reverse identity properties for association property '%1$ls'
.
MessageId=1054
SymbolicName=SCHEMA_54_AUTOGENREADONLY
Language=English
Cannot set autogenerated property '%1$ls' to be not read-only.
.
MessageId=1055
SymbolicName=SCHEMA_55_ASSOCIATEDCLASSREQUIRED
Language=English
Association property '%1$ls' is missing the associated class
.
MessageId=1056
SymbolicName=SCHEMA_56_UNSUPPORTEDXSDGBLELEM
Language=English
Error reading schema '#1$ls' from XML, global element #2$ls type must be '#3$ls' when error level is high.
.
MessageId=1057
SymbolicName=SCHEMA_57_BADCOMPLEXTYPENAME
Language=English
Error reading schema '#1$ls' from XML, complexType #2$ls name must end with 'Type' when error level is high.
.
MessageId=1058
SymbolicName=SCHEMA_58_UNSUPPORTEDXSDGBLELEM
Language=English
Error reading schema '#1$ls' from XML, global element #2$ls type is from a different schema: '#3$ls'.
.
MessageId=1059
SymbolicName=SCHEMA_59_SCHEMAFROMPREFIX
Language=English
Schema names can only be generated from xmlns prefixes when ErrorLevel is VeryLow.
.
MessageId=1060
SymbolicName=SCHEMA_60_MULTIELEMMAPPING
Language=English
Multiple Schema Mappings found for element '%1$ls:%2$ls'.
.
MessageId=1061
SymbolicName=SCHEMA_61_MULTIELEMMAPPING
Language=English
Multiple Schema Mappings found for element '%1$ls:%2$ls.%3$ls'.
.
MessageId=1062
SymbolicName=SCHEMA_62_NOCLASSMAPPING
Language=English
Cannot add element '%1$ls', containing class mapping '%2$ls:%3$ls' not found.
.
MessageId=1063
SymbolicName=SCHEMA_63_MULTICLASSMAPPING
Language=English
Multiple Schema Mappings found for class '%1$ls:%2$ls'.
.
MessageId=1064
SymbolicName=SCHEMA_64_MULTINAMESPACES
Language=English
Schema '%1$ls' has multiple targetNamespace values: '%2$ls', '%3$ls'.
.
MessageId=1065
SymbolicName=SCHEMA_65_UNICONSPROPREF
Language=English
Error reading Feature Schemas from XML; unique constraint property '%1$ls' for feature class '%2$ls' not found.
.
MessageId=1066
SymbolicName=SCHEMA_66_SCHEMARENAME
Language=English
Unsupported Schema modification; cannot rename feature schema '%1$ls' to '%2$ls'.
.
MessageId=1067
SymbolicName=SCHEMA_67_CLASSEXISTS
Language=English
Cannot add class '%1$ls'; it already exists
.
MessageId=1068
SymbolicName=SCHEMA_68_BADCLASSTYPE
Language=English
Cannot create class '%1$ls', unknown class type: %2$d
.
MessageId=1069
SymbolicName=SCHEMA_69_CLASSNOTEXISTS
Language=English
Cannot modify class '%1$ls'; it does not exist
.
MessageId=1070
SymbolicName=SCHEMA_70_MODDESCRIPTION
Language=English
Unsupported Schema modification; cannot modify description for element '%1$ls'.
.
MessageId=1071
SymbolicName=SCHEMA_71_MODCLASSCONCRETE
Language=English
Unsupported Schema modification; Cannot make abstract class '%1$ls' concrete
.
MessageId=1072
SymbolicName=SCHEMA_72_MODCLASSABSTRACT
Language=English
Unsupported Schema modification; Cannot make class '%1$ls' abstract
.
MessageId=1073
SymbolicName=SCHEMA_73_MODBASECLASS
Language=English
Unsupported Schema modification; Cannot change base class for '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1074
SymbolicName=SCHEMA_74_MODCLASSIDENTITY
Language=English
Unsupported Schema modification; Cannot change identity properties for class '%1$ls' from (%2$ls) to (%3$ls)
.
MessageId=1075
SymbolicName=SCHEMA_75_PROPEXISTS
Language=English
Cannot add property '%1$ls'; it already exists
.
MessageId=1076
SymbolicName=SCHEMA_76_BADPROPTYPE
Language=English
Cannot create property '%1$ls', unknown property type: %2$d
.
MessageId=1077
SymbolicName=SCHEMA_77_PROPNOTEXISTS
Language=English
Cannot modify property '%1$ls'; it does not exist
.
MessageId=1078
SymbolicName=SCHEMA_78_MODFEATGEOM
Language=English
Unsupported Schema modification; Cannot modify GeometryProperty for feature class '%1$ls'
.
MessageId=1079
SymbolicName=SCHEMA_79_MODNETLAYER
Language=English
Unsupported Schema modification; Cannot modify layer class for network class '%1$ls'
.
MessageId=1080
SymbolicName=SCHEMA_80_MODNETCOST
Language=English
Unsupported Schema modification; Cannot modify cost property for network feature class '%1$ls'
.
MessageId=1081
SymbolicName=SCHEMA_81_MODFEATNET
Language=English
Unsupported Schema modification; Cannot modify network class for network feature class '%1$ls'
.
MessageId=1082
SymbolicName=SCHEMA_82_MODNETFEATREF
Language=English
Unsupported Schema modification; Cannot modify referenced feature class for network feature class '%1$ls'
.
MessageId=1083
SymbolicName=SCHEMA_83_MODPARENTNETFEAT
Language=English
Unsupported Schema modification; Cannot modify parent network feature class for network feature class '%1$ls'
.
MessageId=1084
SymbolicName=SCHEMA_84_MODSTARTNODE
Language=English
Unsupported Schema modification; Cannot modify start node for network link feature class '%1$ls'
.
MessageId=1085
SymbolicName=SCHEMA_85_MODENDNODE
Language=English
Unsupported Schema modification; Cannot modify end node for network link feature class '%1$ls'
.
MessageId=1086
SymbolicName=SCHEMA_86_MODNODELAYER
Language=English
Unsupported Schema modification; Cannot modify layer for network node feature class '%1$ls'
.
MessageId=1087
SymbolicName=SCHEMA_87_MODDATATYPE
Language=English
Unsupported Schema modification; Cannot change data type for property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1088
SymbolicName=SCHEMA_88_MODDEFAULTVALUE
Language=English
Unsupported Schema modification; Cannot change default value for data property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1089
SymbolicName=SCHEMA_89_MODDATALENGTH
Language=English
Unsupported Schema modification; Cannot change length for data property '%1$ls' from '%2$d' to '%3$d'
.
MessageId=1090
SymbolicName=SCHEMA_90_MODPROPNULLABLE
Language=English
Unsupported Schema modification; Cannot change property '%1$ls' to be nullable
.
MessageId=1091
SymbolicName=SCHEMA_91_MODPROPNNULLABLE
Language=English
Unsupported Schema modification; Cannot change property '%1$ls' to be not nullable
.
MessageId=1092
SymbolicName=SCHEMA_92_MODDATAPRECISION
Language=English
Unsupported Schema modification; Cannot change precision for data property '%1$ls' from '%2$d' to '%3$d'
.
MessageId=1093
SymbolicName=SCHEMA_93_MODDATASCALE
Language=English
Unsupported Schema modification; Cannot change scale for data property '%1$ls' from '%2$d' to '%3$d'
.
MessageId=1094
SymbolicName=SCHEMA_94_MODDATAAUTOGEN
Language=English
Unsupported Schema modification; Cannot change autogeneration setting for data property '%1$ls'
.
MessageId=1095
SymbolicName=SCHEMA_95_MODPROPRDONLY
Language=English
Unsupported Schema modification; Cannot change property '%1$ls' to be read-only
.
MessageId=1096
SymbolicName=SCHEMA_96_MODPROPWRITABLE
Language=English
Unsupported Schema modification; Cannot change property '%1$ls' to be writable
.
MessageId=1097
SymbolicName=SCHEMA_97_BADCONSTRAINTTYPE
Language=English
Cannot modify constraint for data property '%1$ls'; unknown constraint type: %2$d
.
MessageId=1098
SymbolicName=SCHEMA_98_MODDATACONSTRAINT
Language=English
Unsupported Schema modification; Cannot change constraint for data property '%1$ls'
.
MessageId=1099
SymbolicName=SCHEMA_99_MODGEOMTYPES
Language=English
Unsupported Schema modification; Cannot change geometry types for geometric property '%1$ls'
.
MessageId=1100
SymbolicName=SCHEMA_100_MODGEOMELEVATION
Language=English
Unsupported Schema modification; Cannot add/remove elevation dimension to/from geometric property '%1$ls'
.
MessageId=1101
SymbolicName=SCHEMA_101_MODGEOMMEASURE
Language=English
Unsupported Schema modification; Cannot add/remove measure dimension to/from geometric property '%1$ls'
.
MessageId=1102
SymbolicName=SCHEMA_102_MODPROPSC
Language=English
Unsupported Schema modification; Cannot change spatial context for property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1103
SymbolicName=SCHEMA_103_MODASSOCCLASS
Language=English
Unsupported Schema modification; Cannot change associated class for association property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1104
SymbolicName=SCHEMA_104_MODASSOCREVNAME
Language=English
Unsupported Schema modification; Cannot change reverse name for association property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1105
SymbolicName=SCHEMA_105_MODASSOCDELRULE
Language=English
Unsupported Schema modification; Cannot change delete rule for association property '%1$ls'
.
MessageId=1106
SymbolicName=SCHEMA_106_MODASSOCCASCLOCK
Language=English
Unsupported Schema modification; Cannot change cascade lock setting for association property '%1$ls'
.
MessageId=1107
SymbolicName=SCHEMA_107_MODASSOCMULT
Language=English
Unsupported Schema modification; Cannot change multiplicity for association property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1108
SymbolicName=SCHEMA_108_MODASSOCREVMULT
Language=English
Unsupported Schema modification; Cannot change reverse multiplicity for association property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1109
SymbolicName=SCHEMA_109_MODASSOCID
Language=English
Unsupported Schema modification; Cannot change identity properties for association property '%1$ls' from (%2$ls) to (%3$ls)
.
MessageId=1110
SymbolicName=SCHEMA_110_MODREVASSOCID
Language=English
Unsupported Schema modification; Cannot change reverse identity properties for association property '%1$ls' from (%2$ls) to (%3$ls)
.
MessageId=1111
SymbolicName=SCHEMA_111_MODOBJCLASS
Language=English
Unsupported Schema modification; Cannot change class for object property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1112
SymbolicName=SCHEMA_112_MODOBJID
Language=English
Unsupported Schema modification; Cannot change identity property for object property '%1$ls' from '%2$ls' to '%3$ls'
.
MessageId=1113
SymbolicName=SCHEMA_113_MODOBJTYPE
Language=English
Unsupported Schema modification; Cannot change object type for object property '%1$ls'
.
MessageId=1114
SymbolicName=SCHEMA_114_MODOBJORDERTYPE
Language=English
Unsupported Schema modification; Cannot change order type for object property '%1$ls'
.
MessageId=1115
SymbolicName=SCHEMA_115_MODRASTERMODEL
Language=English
Unsupported Schema modification; Cannot change default data model for raster property '%1$ls'
.
MessageId=1116
SymbolicName=SCHEMA_116_MODRASTERX
Language=English
Unsupported Schema modification; Cannot change default image X size for raster property '%1$ls' from '%2$d' to '%3$d'
.
MessageId=1117
SymbolicName=SCHEMA_117_MODRASTERY
Language=English
Unsupported Schema modification; Cannot change default image Y size for raster property '%1$ls' from '%2$d' to '%3$d'
.
MessageId=1118
SymbolicName=SCHEMA_118_SCHEMAEXISTS
Language=English
Cannot add feature schema '%1$ls'; it already exists
.
MessageId=1119
SymbolicName=SCHEMA_119_SCHEMANOTEXISTS
Language=English
Cannot modify feature schema '%1$ls'; it does not exist
.
MessageId=1120
SymbolicName=SCHEMA_120_ADDSCHEMA
Language=English
Cannot add feature schema '%1$ls'; multiple schemas not supported
.
MessageId=1121
SymbolicName=SCHEMA_121_DELSCHEMA
Language=English
Unsupport Schema modification; Cannot delete feature schema '%1$ls'
.
MessageId=1122
SymbolicName=SCHEMA_122_ADDCLASS
Language=English
Unsupport Schema modification; Cannot add class definition '%1$ls'
.
MessageId=1123
SymbolicName=SCHEMA_123_DELCLASS
Language=English
Unsupport Schema modification; Cannot delete class definition '%1$ls'
.
MessageId=1124
SymbolicName=SCHEMA_124_ADDPROP
Language=English
Unsupport Schema modification; Cannot add property '%1$ls'
.
MessageId=1125
SymbolicName=SCHEMA_125_DELPROP
Language=English
Unsupport Schema modification; Cannot delete property '%1$ls'
.
MessageId=1126
SymbolicName=SCHEMA_126_DELBASECLASS
Language=English
Cannot delete class '%1$ls'; it is the base class for '%2$ls'
.
MessageId=1127
SymbolicName=SCHEMA_127_DELOBJCLASS
Language=English
Cannot delete class '%1$ls'; it is the class for object property '%2$ls'
.
MessageId=1128
SymbolicName=SCHEMA_128_DELCLASSID
Language=English
Cannot delete property '%1$ls'; it is an identity property for class '%2$ls'
.
MessageId=1129
SymbolicName=SCHEMA_129_DELOBJID
Language=English
Cannot delete property '%1$ls'; it is an identity property for object property '%2$ls'
.
MessageId=1130
SymbolicName=SCHEMA_130_DELASSOCCLASS
Language=English
Cannot delete class '%1$ls'; it is the associated class for association property '%2$ls'
.
MessageId=1131
SymbolicName=SCHEMA_131_DELASSOCID
Language=English
Cannot delete property '%1$ls'; it is an identity property for association property '%2$ls'
.
MessageId=1132
SymbolicName=SCHEMA_132_DELASSOCREVID
Language=English
Cannot delete property '%1$ls'; it is a reverse identity property for association property '%2$ls'
.
MessageId=1133
SymbolicName=SCHEMA_133_DELLAYERCLASS
Language=English
Cannot delete class '%1$ls'; it is the network layer class for network class '%2$ls'
.
MessageId=1134
SymbolicName=SCHEMA_134_DELNETCOST
Language=English
Cannot delete property '%1$ls'; it is the cost property for network feature class '%2$ls'
.
MessageId=1135
SymbolicName=SCHEMA_135_DELFEATNET
Language=English
Cannot delete property '%1$ls'; it is the network class property for network feature class '%2$ls'
.
MessageId=1136
SymbolicName=SCHEMA_136_DELREFFEAT
Language=English
Cannot delete property '%1$ls'; it is the referenced feature property for network feature class '%2$ls'
.
MessageId=1137
SymbolicName=SCHEMA_137_DELNETPARENT
Language=English
Cannot delete property '%1$ls'; it is the parent network feature property for network feature class '%2$ls'
.
MessageId=1138
SymbolicName=SCHEMA_138_DELSTARTNODE
Language=English
Cannot delete property '%1$ls'; it is the start node property for network link feature class '%2$ls'
.
MessageId=1139
SymbolicName=SCHEMA_139_DELENDNODE
Language=English
Cannot delete property '%1$ls'; it is the end node property for network link feature class '%2$ls'
.
MessageId=1140
SymbolicName=SCHEMA_140_DELNODELAYER
Language=English
Cannot delete property '%1$ls'; it is the layer property for network node feature class '%2$ls'
.
MessageId=1141
SymbolicName=SCHEMA_141_BASETYPECONFLICT
Language=English
Cannot set base class for '%1$ls' to '%2$ls'; they have different class types
.
MessageId=1142
SymbolicName=SCHEMA_142_DELFEATGEOM
Language=English
Cannot delete property '%1$ls'; it is the geometry property for feature class '%2$ls'
.
MessageId=1143
SymbolicName=SCHEMA_143_DELCLASSOBJECTS
Language=English
Cannot delete class definition '%1$ls'; it has objects
.
MessageId=1144
SymbolicName=SCHEMA_144_ADDPROPOBJECTS
Language=English
Cannot add not-nullable property '%1$ls'; its class has objects
.
MessageId=1145
SymbolicName=SCHEMA_145_DELPROPOBJECTS
Language=English
Cannot delete a property '%1$ls'; its class has objects
.
MessageId=1146
SymbolicName=SCHEMA_146_DELSCHEMAOBJECTS
Language=English
Cannot delete schema definition '%1$ls'; it has objects
.
MessageId=1148
SymbolicName=SCHEMA_148_MODCONSTRAINTTYPE
Language=English
Cannot modify constraint type for property '%1$ls'; property has values
.
MessageId=1147
SymbolicName=SCHEMA_147_MODCONSTRAINT
Language=English
Cannot modify constraint for property '%1$ls'; property has values and new constraint is more restrictive
.

MessageId=2001
SymbolicName=EXPRESSION_1_BLOBVALUENULL
Language=English
BLOBValue is null.
.
MessageId=2002
SymbolicName=EXPRESSION_2_BOOLEANVALUENULL
Language=English
BooleanValue is null.
.
MessageId=2003
SymbolicName=EXPRESSION_3_BYTEVALUENULL
Language=English
ByteValue is null.
.
MessageId=2004
SymbolicName=EXPRESSION_4_CLOBVALUENULL
Language=English
CLOBValue is null.
.
MessageId=2005
SymbolicName=EXPRESSION_5_DATETIMEVALUENULL
Language=English
DateTimeValue is null.
.
MessageId=2006
SymbolicName=EXPRESSION_6_DECIMALVALUENULL
Language=English
DecimalValue is null.
.
MessageId=2007
SymbolicName=EXPRESSION_7_DOUBLEVALUENULL
Language=English
DoubleValue is null.
.
MessageId=2008
SymbolicName=EXPRESSION_8_INCOMPLETEBINARYEXPRESSION
Language=English
Incomplete binary expression.
.
MessageId=2009
SymbolicName=EXPRESSION_9_INCOMPLETEFUNCTION
Language=English
Incomplete function.
.
MessageId=2010
SymbolicName=EXPRESSION_10_INCOMPLETEPARAMETER
Language=English
Incomplete parameter.
.
MessageId=2011
SymbolicName=EXPRESSION_11_INCOMPLETEUNARYEXPRESSION
Language=English
Incomplete unary expression.
.
MessageId=2012
SymbolicName=EXPRESSION_12_INT16VALUENULL
Language=English
Int16Value is null.
.
MessageId=2013
SymbolicName=EXPRESSION_13_INT32VALUENULL
Language=English
Int32Value is null.
.
MessageId=2014
SymbolicName=EXPRESSION_14_INT64VALUENULL
Language=English
Int64Value is null.
.
MessageId=2015
SymbolicName=EXPRESSION_15_INVALIDDATAVALUE
Language=English
Invalid data type.
.
MessageId=2016
SymbolicName=EXPRESSION_16_NULLIDENTIFIER
Language=English
Null identifier.
.
MessageId=2017
SymbolicName=EXPRESSION_17_STRINGNOTEXPRESSION
Language=English
String does not represent a valid expression.
.
MessageId=2018
SymbolicName=EXPRESSION_18_SINGLEVALUENULL
Language=English
SingleValue is null.
.
MessageId=2019
SymbolicName=EXPRESSION_19_STRINGVALUENULL
Language=English
StringValue is null.
.
MessageId=2020
SymbolicName=EXPRESSION_20_STRINGNOTVALUEEXPRESSION
Language=English
String does not represent a valid value expression.
.


MessageId=3001
SymbolicName=FILTER_1_INCOMPLETEBINARYOPERATOR
Language=English
Incomplete binary logical operator.
.
MessageId=3002
SymbolicName=FILTER_2_INCOMPLETECOMPARISONCONDITION
Language=English
Incomplete comparison condition.
.
MessageId=3003
SymbolicName=FILTER_3_INCOMPLETEDISTANCECONDITION
Language=English
Incomplete distance condition.
.
MessageId=3004
SymbolicName=FILTER_4_INCOMPLETEINCONDITION
Language=English
Incomplete in condition.
.
MessageId=3005
SymbolicName=FILTER_5_INCOMPLETENULLCONDITION
Language=English
Incomplete null condition.
.
MessageId=3006
SymbolicName=FILTER_6_INCOMPLETESPATIALCONDITION
Language=English
Incomplete spatial condition.
.
MessageId=3007
SymbolicName=FILTER_7_INCOMPLETEUNARYOPERATOR
Language=English
Incomplete unary logical operator.
.
MessageId=3008
SymbolicName=FILTER_8_INVALIDFILTERSTRING
Language=English
Invalid filter string.
.
MessageId=3009
SymbolicName=FILTER_9_STRINGNOTFILTER
Language=English
String does not represent a valid filter.
.


MessageId=4001
SymbolicName=COMMANDS_1_INVALIDIDENTIFIERNAME
Language=English
Invalid identifier name '%1$ls'.
.
MessageId=4002
SymbolicName=COMMANDS_2_INVALIDPARAMETERVALUENAME
Language=English
Invalid parameter value name '%1$ls'.
.
MessageId=4003
SymbolicName=COMMANDS_3_INVALIDPROPERTYVALUENAME
Language=English
Invalid property value name '%1$ls'.
.
MessageId=4004
SymbolicName=COMMANDS_4_INVALIDPROVIDERNAME
Language=English
Schema Override set for schema '%1$ls' has invalid provider name '%2$ls'; must be of the form: Company.Provider.Version.
.
MessageId=4005
SymbolicName=COMMANDS_5_OVERRIDEPROVIDERMISMATCH
Language=English
Registered provider '%1$ls' is registered with library for provider '%2$ls'.
.
MessageId=4006
SymbolicName=COMMANDS_6_MISSINGPROVIDERNAME
Language=English
Schema Override set for schema '%1$ls' has no provider name.
.
MessageId=4007
SymbolicName=COMMANDS_7_OBJECTHASPARENT
Language=English
Schema Override '%1$ls' already has parent.
.
MessageId=4008
SymbolicName=COMMANDS_8_BADMAPPINGSUBELEMENT
Language=English
Element '%1$ls' not allowed inside %2$ls Schema Mapping element %3$ls[@name='%4$ls'].
.
MessageId=4009
SymbolicName=COMMANDS_9_MULTIMAPPINGSUBELEMENT
Language=English
Multiple instances of Element '%1$ls' not allowed inside %2$ls Schema Mapping element %3$ls[@name='%4$ls'].
.
MessageId=4010
SymbolicName=COMMANDS_10_CHOICESUBELEMENT
Language=English
Only one of '%1$ls' allowed inside %2$ls Schema Mapping element %3$ls[@name='%4$ls'].
.
MessageId=4011
SymbolicName=COMMANDS_11_OWNPARENT
Language=English
Element '%1$ls' cannot be its own parent/grandparent.
.
MessageId=4012
SymbolicName=COMMANDS_12_DUPLICATESUBELEMENT
Language=English
Only one of '%1$ls[@name=%2$ls]' allowed inside %3$ls Schema Mapping element %4$ls[@name='%5$ls'].
.

MessageId=5001
SymbolicName=PARSE_1_INVALIDDIGIT
Language=English
Invalid digit in number.
.
MessageId=5002
SymbolicName=PARSE_2_INVALIDDATETIME
Language=English
Invalid DateTime value.
.
MessageId=5003
SymbolicName=PARSE_3_DATETIMEOUTOFRANGE
Language=English
DateTime value out of range.
.
MessageId=5004
SymbolicName=PARSE_4_STRINGINCORRECTLYFORMATTED
Language=English
String incorrectly formatted.
.
MessageId=5005
SymbolicName=PARSE_5_STRINGTOOLONG
Language=English
String too long.
.
MessageId=5006
SymbolicName=PARSE_6_MISSINGQUOTE_d
Language=English
Unmatched quote following quotation mark at column %1$d.
.
MessageId=5007
SymbolicName=PARSE_7_INVALIDHEXDIGIT
Language=English
Invalid hex digit.
.
MessageId=5008
SymbolicName=PARSE_8_INVALIDBITDIGIT
Language=English
Invalid bit digit.
.
MessageId=5009
SymbolicName=PARSE_9_WRONGTOKEN
Language=English
Wrong token.
.

MessageId=6001
SymbolicName=SC_1_XMLNONAME
Language=English
Current spatial context has no name; cannot write to XML.
.
MessageId=6002
SymbolicName=SC_2_NOSTATETRANSITION
Language=English
Error in SCReadHandler state transition table, no transition after element '%1$ls'.
.
MessageId=6003
SymbolicName=SC_3_XMLNOEXTENTS
Language=English
Current spatial context '%1$ls' has no extents; cannot write to XML.
.
MessageId=6004
SymbolicName=SC_4_XMLNOREAD
Language=English
%1$ls failed: no spatial context has been read from XML.
.
MessageId=6005
SymbolicName=SC_5_MISSINGATTRIBUTE
Language=English
Error reading spatial context %1$ls from XML, required attribute %2$ls/@%3$ls is missing.
.
MessageId=6006
SymbolicName=SC_6_BADATTRIBUTE
Language=English
Error reading spatial context %1$ls from XML, invalid attribute %2$ls/@%3$ls value '%4$ls'.
.
MessageId=6007
SymbolicName=SC_7_NAMEIDDIFF
Language=English
Error reading spatial context %1$ls from XML, gml:id '%2$ls' differs from srsName.
.
MessageId=6008
SymbolicName=SC_8_BADELEMENT
Language=English
Error reading spatial context %1$ls from XML, invalid %2$ls value '%3$ls'.
.
MessageId=6009
SymbolicName=SC_9_BADSUBELEMENT
Language=English
Error reading spatial context %1$ls from XML, unexpected first element %2$ls inside '%3$ls'.
.
MessageId=6010
SymbolicName=SC_10_BADNEXTELEMENT
Language=English
Error reading spatial context %1$ls from XML, unexpected element %2$ls after '%3$ls'.
.
MessageId=6011
SymbolicName=SC_11_MISSINGNEXTELEMENT
Language=English
Error reading spatial context %1$ls from XML, expected element %2$ls after '%3$ls'.
.
MessageId=6012
SymbolicName=SC_12_EMPTYELEMENT
Language=English
Error reading spatial context %1$ls from XML, element %2$ls has no sub-elements.
.
MessageId=60013
SymbolicName=SC_13_NAMEIDDIFF
Language=English
Error reading spatial context %1$ls from XML, gml:baseCRS/fdo:WKTCRS/gml:id '%2$ls' differs from gml:baseCRS/fdo:WKTCRS/srsName.
.
MessageId=6014
SymbolicName=SC_14_EMPTYBASECRS
Language=English
Error reading spatial context %1$ls from XML, gml:baseCRS subelement must have an xlink:href attribute or fdo:WKTCRS subelement.
.
MessageId=6015
SymbolicName=SC_15_NEEDCALLREADNEXT
Language=English
Error reading from feature reader, need to call ReadNext before getting property.
.
MessageId=6016
SymbolicName=SC_16_INDEXOUTOFBOUND
Language=English
Error reading from feature reader, reach end in the feature collection.
.

MessageId=7001
SymbolicName=CLNT_1_NULLINPUTPOINTER
Language=English
NULL Input Pointer
.
MessageId=7002
SymbolicName=CLNT_2_EMPTYINPUTPARAMETER
Language=English
Empty Input Parameter
.
MessageId=7003
SymbolicName=CLNT_3_NULLPOINTER
Language=English
NULL Pointer
.
MessageId=7004
SymbolicName=CLNT_4_PROVIDERNOTREGISTERED
Language=English
FDO Provider Not Registered
.
MessageId=7005
SymbolicName=CLNT_5_OUTOFMEMORY
Language=English
Out Of Memory
.
MessageId=7006
SymbolicName=CLNT_6_INVALIDINPUTPARAMETER
Language=English
Invalid Input Parameter
.
MessageId=7007
SymbolicName=CLNT_7_FAILURE
Language=English
Action Failed
.
MessageId=7008
SymbolicName=CLNT_8_UNABLE_TO_LOAD_LIBRARY
Language=English
Unable to load the FDO Provider: %1$s
.
MessageId=7009
SymbolicName=CLNT_9_INVALID_PROVIDER_ENTRY_POINT
Language=English
FDO Provider entry point function 'CreateConnection' was not found.
.
MessageId=7010
SymbolicName=CLNT_10_NULL_CONNECTION_FROM_PROVIDER
Language=English
FDO Provider entry point function 'CreateConnection' returned NULL.
.
MessageId=7011
SymbolicName=CLNT_11_PARSER_ERROR
Language=English
Parser Error
.
MessageId=7012
SymbolicName=CLNT_12_FILE_NOT_EXISTS
Language=English
File '%1$ls' does not exist
.
MessageId=7013
SymbolicName=CLNT_13_UNKNOWN_MESSAGE
Language=English
Unknown message id: %1$d
.
MessageId=7014
SymbolicName=CLNT_14_XML_ERROR
Language=English
Error in registry file '%1$ls', line %2$d: %3$s
.
MessageId=7015
SymbolicName=CLNT_15_XML_WRITE_ERROR
Language=English
Error writing to registry file '%1$ls', %2$s
.

MessageId=8001
SymbolicName=FUNCTION_CEIL
Language=English
Determines the smallest integer greater than or equal to the expression value
.
MessageId=8002
SymbolicName=FUNCTION_FLOOR
Language=English
Returns the largest integer equal to or less than the expression value
.
MessageId=8003
SymbolicName=FUNCTION_AVG
Language=English
Determines the average value of an expression
.
MessageId=8004
SymbolicName=FUNCTION_COUNT
Language=English
Determines the number of objects in the query
.
MessageId=8005
SymbolicName=FUNCTION_MAX
Language=English
Determines the maximum value of an expression
.
MessageId=8006
SymbolicName=FUNCTION_MIN
Language=English
Returns the minimum value of an expression
.
MessageId=8007
SymbolicName=FUNCTION_SUM
Language=English
Returns the sum of values of an expression
.
MessageId=8008
SymbolicName=FUNCTION_LOWER
Language=English
Converts all uppercase letters in a string expression into lowercase letters
.
MessageId=8009
SymbolicName=FUNCTION_UPPER
Language=English
Converts all lowercase letters in a string expression into uppercase letters
.
MessageId=8010
SymbolicName=FUNCTION_CONCAT
Language=English
Returns the string concatenation of 2 expressions
.
MessageId=8011
SymbolicName=FUNCTION_DBL_ARG
Language=English
Argument that represents a double
.
MessageId=8012
SymbolicName=FUNCTION_INT_ARG
Language=English
Argument that represents an integer
.
MessageId=8013
SymbolicName=FUNCTION_STR_ARG
Language=English
Argument that represents a string
.
MessageId=8014
SymbolicName=FUNCTION_SPATIALEXTENTS
Language=English
Returns the spatial extents of values of a geometric expression
.
MessageId=8015
SymbolicName=FUNCTION_GEOM_ARG
Language=English
Argument that represents a geometry
.
MessageId=8016
SymbolicName=FUNCTION_BOOL_ARG
Language=English
Argument that represents a boolean
.
MessageId=8017
SymbolicName=FUNCTION_BYTE_ARG
Language=English
Argument that represents a byte
.
MessageId=8018
SymbolicName=FUNCTION_DT_ARG
Language=English
Argument that represents a date-time value
.
MessageId=8019
SymbolicName=FUNCTION_DEC_ARG
Language=English
Argument that represents a decimal
.
MessageId=8020
SymbolicName=FUNCTION_INT16_ARG
Language=English
Argument that represents a 16-bit integer
.
MessageId=8021
SymbolicName=FUNCTION_INT32_ARG
Language=English
Argument that represents a 32-bit integer
.
MessageId=8022
SymbolicName=FUNCTION_INT64_ARG
Language=English
Argument that represents a 64-bit integer
.
MessageId=8023
SymbolicName=FUNCTION_SINGLE_ARG
Language=English
Argument that represents a single value
.
MessageId=8024
SymbolicName=FUNCTION_BLOB_ARG
Language=English
Argument that represents a BLOB value
.
MessageId=8025
SymbolicName=FUNCTION_CLOB_ARG
Language=English
Argument that represents a CLOB value
.
MessageId=8026
SymbolicName=FUNCTION_ASSOCIATION_ARG
Language=English
Argument that represents an association
.
MessageId=8027
SymbolicName=FUNCTION_OBJECT_ARG
Language=English
Argument that represents an object
.
MessageId=8028
SymbolicName=FUNCTION_RASTER_ARG
Language=English
Argument that represents a raster
.

MessageId=9001
SymbolicName=FDO_1_BADALLOC
Language=English
Memory allocation failed.
.
MessageId=9002
SymbolicName=FDO_2_BADPARAMETER
Language=English
Bad parameter to method.
.
MessageId=9003
SymbolicName=FDO_3_NOTIMPLEMENTED
Language=English
Method is not implemented.
.
MessageId=9004
SymbolicName=FDO_4_UNREADY
Language=English
Data is not in a state to respond to the request.
.
MessageId=9005
SymbolicName=FDO_5_INDEXOUTOFBOUNDS
Language=English
Item index out of bounds.
.
MessageId=9006
SymbolicName=FDO_6_OBJECTNOTFOUND
Language=English
Invalid delete; object not present.
.
MessageId=9007
SymbolicName=FDO_7_INVALIDFGF
Language=English
String is not a valid FGF string.
.
MessageId=9008
SymbolicName=FDO_8_INCORRECTSTRINGFORMAT
Language=English
Parse string is incorrectly formatted.
.
MessageId=9009
SymbolicName=FDO_9_INVALIDDIGIT
Language=English
Invalid digit in number.
.
MessageId=9010
SymbolicName=FDO_10_UNSUPPORTEDGEOMETRYTYPE
Language=English
Unsupported geometry type.
.
MessageId=9011
SymbolicName=FDO_11_INVALIDPOINT
Language=English
Invalid point.
.
MessageId=9012
SymbolicName=FDO_12_SHAREDARRAY
Language=English
Cannot resize a shared array.
.
MessageId=9013
SymbolicName=FDO_13_INVALIDRESIZE
Language=English
Invalid array resize.
.
MessageId=9014
SymbolicName=FDO_14_NULLSTRING
Language=English
Null String.
.
MessageId=9015
SymbolicName=FDO_15_UNSUPPORTEDGEOMETRYDATA
Language=English
Invalid Geometry.
.
MessageId=9016
SymbolicName=FDO_17_UNSUPPORTEDDIMENSIONALITY
Language=English
Invalid dimensionality.
.
MessageId=9017
SymbolicName=FDO_18_DATAPROVIDERFAILURE
Language=English
OCI failure.
.
MessageId=9018
SymbolicName=FDO_18_FILEOPENFAILURE
Language=English
Failed to open file '%1$ls' with access modes: '%2$ls'.
.
MessageId=9019
SymbolicName=FDO_19_STREAMREADERROR
Language=English
Cannot read %1$d bytes from unreadable stream.
.
MessageId=9020
SymbolicName=FDO_20_STREAMWRITEERROR
Language=English
Cannot write %1$d bytes to unwritable stream.
.
MessageId=9021
SymbolicName=FDO_21_STREAMWRITEERROR
Language=English
Cannot write %1$d bytes to writable stream, only %2$d bytes were written.
.
MessageId=9022
SymbolicName=FDO_22_STREAMSEEKERROR
Language=English
Cannot seek on non-seekable stream.
.
MessageId=9023
SymbolicName=FDO_23_STREAMSEEKFAILURE
Language=English
Seek on stream failed.
.
MessageId=9024
SymbolicName=FDO_24_STREAMBADFP
Language=English
Cannot create FdoIoFileStream, bad file pointer.
.
MessageId=9025
SymbolicName=FDO_25_STREAMSIZEFAILURE
Language=English
Setting length of stream to %1$d bytes failed.
.
MessageId=9026
SymbolicName=FDO_26_BUFFEROVERWRITEERROR
Language=English
Cannot write %1$lld bytes to buffer with %2$lld bytes remaining.
.
MessageId=9027
SymbolicName=FDO_27_BUFFERLENGTHERROR
Language=English
Cannot change length of fixed buffer stream.
.
MessageId=9028
SymbolicName=FDO_28_UTF8FAILURE
Language=English
Unicode to UTF8 conversion failed for string '%1$ls'.
.
MessageId=9029
SymbolicName=FDO_29_UNICODEFAILURE
Language=English
UTF8 to Unicode conversion failed for string '%1$s'.
.
MessageId=9030
SymbolicName=FDO_30_BADPARAM
Language=English
Bad %1$ls value '%2$ls' passed to %3$ls.
.
MessageId=9031
SymbolicName=FDO_31_ENDELEMENTERROR
Language=English
FdoXmlWriter::EndElement: there is no current element to end.
.
MessageId=9032
SymbolicName=FDO_32_XMLATTRIBUTEERROR
Language=English
Cannot add attribute %1$ls; tag for element %2$ls is no longer open.
.
MessageId=9033
SymbolicName=FDO_33_XMLPARSEERROR
Language=English
XML Document parse error at line %1$d column %2$d: %3$ls
.
MessageId=9034
SymbolicName=FDO_34_MEMORYSTREAMOVERFLOW
Language=English
%1$ls: Maximum length for a %2$ls exceeded, please increase buffer size (currently %3$d)
.
MessageId=9035
SymbolicName=FDO_35_FILENOCONTEXT
Language=English
Stream is not contextual (has no absolute position); %1$ls::%2$ls cannot be called
.
MessageId=9036
SymbolicName=FDO_36_STREAMINDEXALIGN
Language=English
%1$ls::%2$ls cannot be called when file size exceeds %3$d bytes (current size: %4$d bytes)
.
MessageId=9037
SymbolicName=FDO_37_FILEFLUSH
Language=English
%1$ls: failed to flush file
.
MessageId=9038
SymbolicName=FDO_38_ITEMNOTFOUND
Language=English
Item '%1$ls' not found in collection
.
MessageId=9039
SymbolicName=FDO_39_STACKPOP
Language=English
Stack is empty, cannot pop.
.
MessageId=9040
SymbolicName=FDO_40_BADXMLELEMENT
Language=English
Element '%1$ls' is not a valid XML 1.0 element name.
.
MessageId=9041
SymbolicName=FDO_41_BADXMLATTRIBUTE
Language=English
Attribute '%1$ls' is not a valid XML 1.0 attribute name.
.
MessageId=9042
SymbolicName=FDO_42_GENERICCHAR
Language=English
%1$ls
.
MessageId=9043
SymbolicName=FDO_43_XSLERROR
Language=English
XSL Error at line %1$d column %2$d: %3$ls.
.
MessageId=9044
SymbolicName=FDO_44_BADDEFAULTROOT
Language=English
Cannot change default root setting for FdoXmlWriter; root element has already been written.
.
MessageId=9045
SymbolicName=FDO_45_ITEMINCOLLECTION
Language=English
Item %1$ls is already in this named collection.
.
MessageId=9046
SymbolicName=FDO_46_REMAININGSTREAMSIZE
Language=English
%1$ls: Cannot read remainder of stream, there are more than %$2d items remaining.
.
MessageId=9047
SymbolicName=FDO_47_READARRAYSIZE
Language=English
%1$ls: FdoArray is too small to read %2$d items from stream.
.
MessageId=9048
SymbolicName=FDO_48_NOCONTENTELEMENT
Language=English
%1$ls called when there is no open element; content is '%2$ls'.
.
MessageId=9049
SymbolicName=FDO_49_MULTIROOTELEMENT
Language=English
%1$ls: cannot write second root element '%2$ls' to XML document.
.
MessageId=9050
SymbolicName=FDO_50_XMLTRANSCODEFAILED
Language=English
%1$ls: %2$ls failed.
.
MessageId=9051
SymbolicName=FDO_51_SETFILESIZE
Language=English
Cannot change file size to %1$d Windows; maximum is %2$d
.
MessageId=9052
SymbolicName=FDO_52_BADSUBELEMENT
Language=English
Error reading from XML, unexpected element %1$ls inside '%2$ls'.
.
MessageId=9053
SymbolicName=FDO_53_NESTEDXMLPARSEERROR
Language=English
Nested call to FdoXmlReader::Parse() attempted.
.
MessageId=9054
SymbolicName=FDO_54_XMLINPUTEOF
Language=English
Cannot parse XML document whose stream is at end-of-file.
.
MessageId=9055
SymbolicName=FDO_55_XSLINPUTEOF
Language=English
Cannot XSL Transform XML document whose stream is at end-of-file.
.
MessageId=9056
SymbolicName=FDO_56_XSLSTYLESHEETEOF
Language=English
Cannot read XSL stylesheet whose stream is at end-of-file.
.
MessageId=9057
SymbolicName=FDO_57_UNEXPECTEDERROR
Language=English
Unexpected error encountered.
.
MessageId=9058
SymbolicName=FDO_58_READERHASNODATA
Language=English
End of reader data or ReadNext not called.
.
MessageId=9059
SymbolicName=FDO_59_CANNOTFETCHNULLVALUE
Language=English
Cannot fetch NULL value.
.
MessageId=9060
SymbolicName=FDO_60_NULL_POINTER
Language=English
Unexpected NULL pointer.
.
MessageId=9061
SymbolicName=FDO_61_NULL_ARGUMENT
Language=English
A required argument was set to NULL.
.
MessageId=9062
SymbolicName=FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH
Language=English
Fetching a property value did not match the property type.
.
MessageId=9063
SymbolicName=FDO_63_CONNECTION_INVALID
Language=English
Connection is invalid.
.
MessageId=9064
SymbolicName=FDO_64_CONNECTION_REQUIRED_PROPERTY_NULL
Language=English
The required property '%1$ls' cannot be set to NULL.
.
MessageId=9065
SymbolicName=FDO_65_CONNECTION_ENUM_PROPERTY_WRONG_VALUE
Language=English
The given value '%1$ls' for enumerable property '%2$ls' is not in the enumeration value collection.
.
MessageId=9066
SymbolicName=FDO_66_CONNECTION_PROPERTY_NOT_FOUND
Language=English
The connection property '%1$ls' was not found.
.
MessageId=9067
SymbolicName=FDO_67_CONNECTION_TIMEOUT_NOT_SUPPORTED
Language=English
Connection timeout is not supported.
.
MessageId=9068
SymbolicName=FDO_68_COMMAND_PARAMETERS_NOT_SUPPORTED
Language=English
Command parameters are not supported.
.
MessageId=9069
SymbolicName=FDO_69_PROPERTY_NOT_AVAILABLE
Language=English
The requested property is not available.
.
MessageId=9070
SymbolicName=FDO_70_PROPERTY_TYPE_NOT_SUPPORTED
Language=English
The property type '%1$ls' is not supported by this provider.
.
MessageId=9071
SymbolicName=FDO_71_DATA_TYPE_NOT_SUPPORTED
Language=English
The data type '%1$ls' is not supported by this provider.
.
MessageId=9072
SymbolicName=FDO_72_LOB_DATA_TYPE_NOT_SUPPORTED
Language=English
The BLOB and CLOB data types are not supported by this provider.
.
MessageId=9073
SymbolicName=FDO_73_PROPERTY_INDEXOUTOFBOUNDS
Language=English
Property index '%1$d' is out of bounds.
.
MessageId=9074
SymbolicName=FDO_74_PROPERTY_NAME_NOT_FOUND
Language=English
The property '%1$ls' was not found.
.
MessageId=9075
SymbolicName=FDO_75_INVALID_NUM_ARGUMENTS
Language=English
Invalid number of arguments to function '%1$ls'; expected %2$d argument(s), got %3$d instead.
.
MessageId=9076
SymbolicName=FDO_76_CLASS_NAME_NOT_FOUND
Language=English
The class '%1$ls' was not found.
.
MessageId=9077
SymbolicName=FDO_77_ORDERBY_INVALID_TYPE
Language=English
The data type '%1$ls' cannot be used with Order By.
.
MessageId=9078
SymbolicName=FDO_78_AGGREGATE_IN_AGGREGATE_FUNCTION
Language=English
Aggregate functions cannot contain aggregate functions as their arguments.
.
MessageId=9079
SymbolicName=FDO_79_AGGREGATE_INVALID_MIX
Language=English
Invalid combination of aggregate functions and non-aggregate expressions.
.
MessageId=9080
SymbolicName=FDO_80_INVALID_EXPRESSION
Language=English
Invalid expression; expression types incompatible.
.
MessageId=9081
SymbolicName=FDO_81_IDENTIFIER_NOT_FOUND
Language=English
The identifier '%1$ls' was not recognized.
.
MessageId=9082
SymbolicName=FDO_82_UNSUPPORTED_LOGICAL_OPERATION
Language=English
Unsupported FDO logical operation.
.
MessageId=9083
SymbolicName=FDO_83_UNSUPPORTED_COMPARISON_OPERATION
Language=English
Unsupported FDO comparison operation.
.
MessageId=9084
SymbolicName=FDO_84_SPATIAL_CONDITION_NOT_LITERAL_GEOMETRY
Language=English
Only literal geometric values are allowed in spatial conditions.
.
MessageId=9085
SymbolicName=FDO_85_DISTANCE_SPATIAL_CONDITION_NOT_SUPPORTED
Language=English
The Distance spatial condition is not supported by this provider.
.
MessageId=9086
SymbolicName=FDO_86_UNSUPPORTED_BINARY_OPERATION
Language=English
Unsupported FDO binary operation.
.
MessageId=9087
SymbolicName=FDO_87_UNSUPPORTED_UNARY_OPERATION
Language=English
Unsupported FDO unary operation.
.
MessageId=9088
SymbolicName=FDO_88_INVALID_FUNCTION_ARG_TYPE
Language=English
Argument number %1$d to function '%2$ls' was of type '%3$ls', expected type '%4$ls'.
.
MessageId=9089
SymbolicName=FDO_89_UNSUPPORTED_FUNCTION
Language=English
The function '%1$ls' is not supported by this provider.
.
MessageId=9090
SymbolicName=FDO_90_GEOMETRY_VALUES_IN_FILTER_UNSUPPORTED
Language=English
Geometry values in filters are not supported by this provider.
.
MessageId=9091
SymbolicName=FDO_91_FILE_NOT_FOUND
Language=English
The file '%1$ls' was not found.
.
MessageId=9092
SymbolicName=FDO_92_PATH_NOT_FOUND
Language=English
The path to file '%1$ls' was not found.
.
MessageId=9093
SymbolicName=FDO_93_TOO_MANY_OPEN_FILES
Language=English
Already too many open files when opening file '%1$ls'.
.
MessageId=9094
SymbolicName=FDO_94_ACCESS_DENIED
Language=English
Access to file '%1$ls' was denied.
.
MessageId=9095
SymbolicName=FDO_95_FILE_IO_ERROR
Language=English
File '%1$ls' I/O error in context '%2$ls': %3$ls
.
MessageId=9096
SymbolicName=FDO_96_READ_ONLY
Language=English
Location for file '%1$ls' is read-only.
.
MessageId=9097
SymbolicName=FDO_97_CANNOT_SET_READONLY_PROPERTY
Language=English
Property '%1$ls' cannot be set because it is read-only.
.
MessageId=9098
SymbolicName=FDO_98_MISSING_DEFAULT_VALUE
Language=English
Read-only property '%1$ls' requires a default value.
.
MessageId=9099
SymbolicName=FDO_99_CANNOT_DEFAULT_READONLY_PROPERTY
Language=English
Read-only identity property '%1$ls' cannot have a default value.
.
MessageId=9100
SymbolicName=FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED
Language=English
Command timeout is not supported.
.
MessageId=9101
SymbolicName=FDO_101_TRANSACTIONS_NOT_SUPPORTED
Language=English
Transactions are not supported.
.
MessageId=9102
SymbolicName=FDO_102_COMMAND_NOT_SUPPORTED
Language=English
The command '%1$ls' is not supported.
.
MessageId=9103
SymbolicName=FDO_103_CONNECTION_ALREADY_OPEN
Language=English
The connection is already open.
.
MessageId=9104
SymbolicName=FDO_104_READER_PROPERTY_NOT_SELECTED
Language=English
The property '%1$ls' is not in the reader's selected property set.
.
MessageId=9105
SymbolicName=FDO_105_UNSUPPORTED_SPATIAL_OPERATION
Language=English
Unsupported FDO spatial operation.
.
MessageId=9106
SymbolicName=FDO_106_LOB_NOT_SUPPORTED_IN_FILTER
Language=English
LOB objects not supported in the FDO Filter.
.
MessageId=9107
SymbolicName=FDO_107_PARAEMETRS_NOT_SUPPORTED_IN_FILTER
Language=English
Parameters not supported in the FDO Filter.
.
MessageId=9108
SymbolicName=FDO_108_INVALID_FDO_IN_CONDITION
Language=English
Invalid FDO In condition.
.
MessageId=9109
SymbolicName=FDO_109_INVALID_FDO_COMPARISON_CONDITION
Language=English
Invalid FDO In condition.
.
MessageId=9110
SymbolicName=FDO_110_PROPERTY_NOT_NULLABLE
Language=English
The property '%1$ls' cannot be set to NULL.
.
MessageId=9111
SymbolicName=FDO_111_PROPERTY_RANGE_CONSTRAINT_VIOLATED
Language=English
The property '%1$ls' range constraint (%2$ls) is violated by value %3$ls.
.
MessageId=9112
SymbolicName=FDO_112_PROPERTY_LIST_CONSTRAINT_VIOLATED
Language=English
The property '%1$ls' list constraint (%2$ls) is violated by value %3$ls.
.
MessageId=9113
SymbolicName=FDO_113_PROPERTY_UNKNOWN_CONSTRAINT_VIOLATED
Language=English
The property '%1$ls' unknown constraint is violated by value %2$ls.
.
MessageId=9114
SymbolicName=FDO_114_XSL_LOG_ERROR
Language=English
ERROR
.
MessageId=9115
SymbolicName=FDO_115_XSL_LOG_WARNING
Language=English
WARNING
.
MessageId=9116
SymbolicName=FDO_116_XSL_LOG_MESSAGE
Language=English
MESSAGE
.
MessageId=9117
SymbolicName=FDO_117_XSL_LOG_ORIGINATOR_XMLPARSER
Language=English
XML Parser
.
MessageId=9118
SymbolicName=FDO_118_XSL_LOG_ORIGINATOR_XSLPROCESSOR
Language=English
XSL Processor
.
MessageId=9119
SymbolicName=FDO_119_XSL_LOG_ORIGINATOR_XPATH
Language=English
XPath
.
MessageId=9120
SymbolicName=FDO_120_XSL_LOG_ORIGINATOR_UNKNOWN
Language=English
Unknown originator
.
MessageId=9121
SymbolicName=FDO_121_XSL_ATSOURCENODE
Language=English
At source node '%1$ls'
.
MessageId=9122
SymbolicName=FDO_122_XSL_ATURI
Language=English
at URI '%1$ls' (line %2$d, column %3$d)
.
MessageId=9123
SymbolicName=FDO_123_NOATTRIBUTEELEMENT
Language=English
%1$ls called when there is no open element; cannot write attribute '%2$ls'.
.
MessageId=9124
SymbolicName=FDO_124_BYTESAFTERCLOSE
Language=English
%1$ls called after document closed; cannot write bytes.
.
MessageId=9125
SymbolicName=FDO_125_SCHEMA_ALREADY_EXISTS
Language=English
The schema '%1$ls' already exists.
.
MessageId=9126
SymbolicName=FDO_126_CLASS_ALREADY_EXISTS
Language=English
The class '%1$ls' already exists.
.
MessageId=9127
SymbolicName=FDO_127_PROPERTY_ALREADY_EXISTS
Language=English
The property '%1$ls' already exists.
.
MessageId=9128
SymbolicName=FDO_128_GEOMETRY_MAPPING_ERROR
Language=English
The provided input is invalid for the current mapping process.
.
MessageId=9129
SymbolicName=FDO_129_PROPERTY_TYPE_NOT_SUPPORTED_BY_OPERATION
Language=English
The property type '%1$ls' is not supported by this operation.
.
MessageId=9130
SymbolicName=FDO_130_DATA_TYPE_NOT_SUPPORTED_BY_OPERATION
Language=English
The data type '%1$ls' is not supported by this operation.
.
MessageId=9131
SymbolicName=FDO_131_UNSUPPORTED_GEOMETRY_TYPE
Language=English
The '%1$ls' geometry type (or combination of types) is not supported.
.
MessageId=9132
SymbolicName=FDO_132_CONNECTION_BROKEN
Language=English
Network connection is broken, or the server stopped responding.
.
MessageId=9133
SymbolicName=FDO_133_UNEXPECTEDERROR_PERFORMING
Language=English
Unexpected error encountered while accessing the server.
.
MessageId=9134
SymbolicName=FDO_134_SPATIAL_CONTEXT_ERROR_DEFAULT_EXISTS
Language=English
'%1$ls': Default spatial context exists; Cannot create spatial context info.
.
MessageId=9135
SymbolicName=FDO_135_SPATIAL_CONTEXT_ERROR_ID_MISSMATCH
Language=English
'%1$ls': The spatial context ids don't match.
.
MessageId=9136
SymbolicName=FDO_136_SPATIAL_CONTEXT_ERROR_UNKNOWN_EXTENT_TYPE
Language=English
'%1$ls': Unknown extent type.
.
MessageId=9137
SymbolicName=FDO_137_NAMED_COLLECTION_INDEX_NAME_ERROR
Language=English
'%1$ls': Cannot find index in Named Collection for unspecified name (NULL).
.
MessageId=9138
SymbolicName=FDO_138_MBC_ERROR_ROOT_CELL
Language=English
'%1$ls': Invalid MBC format: missing root cell.
.
MessageId=9139
SymbolicName=FDO_139_MBC_ERROR_ILLEGAL_CHAR
Language=English
'%1$ls': Invalid MBC format: illegal character.
.
MessageId=9140
SymbolicName=FDO_140_MBC_ERROR_SKIP
Language=English
'%1$ls': MBC error: too many skip characters specified.
.
MessageId=9141
SymbolicName=FDO_141_MBC_ERROR_MAX_DEPTH
Language=English
'%1$ls': MBC error: depth exceeds maximum depth.
.
MessageId=9142
SymbolicName=FDO_142_MBC_ERROR_EXTENTS
Language=English
'%1$ls': MBC error: extents outside spatial index extents.
.
MessageId=9143
SymbolicName=FDO_143_URL_ERROR
Language=English
The requested URL returned error: '%1$ls'.
.
MessageId=9144
SymbolicName=FDO_144_URL_SERVICE_UNAVAILABLE
Language=English
The HTTP service is unavailable.
.
MessageId=9145
SymbolicName=FDO_145_URL_BAD_REQUEST
Language=English
The HTTP request could not be understood by the server due to malformed syntax.
.
MessageId=9146
SymbolicName=FDO_146_URL_NOT_AUTHORIZED
Language=English
The HTTP request is missing user authentication or the user authorization has been refused by the server. 
.
MessageId=9147
SymbolicName=FDO_147_URL_ACCESS_DENIED
Language=English
The HTTP request was denied (forbidden) by the server. 
.
MessageId=9148
SymbolicName=FDO_148_URI_NOT_FOUND
Language=English
The HTTP request URI was not found by the server.
.
MessageId=9149
SymbolicName=FDO_149_URL_REQUEST_NOT_ALLOWED
Language=English
The method specified in the HTTP request is not allowed for the resource identified by the URI.
.
MessageId=9150
SymbolicName=FDO_150_URL_REQUEST_NOT_ACCEPTABLE
Language=English
The HTTP request generated an unaceptable response by the server.
.
MessageId=9151
SymbolicName=FDO_151_URL_PROXY_AUTHENTICATION_REQUIRED
Language=English
The client is not authorized to access the HTTP Proxy Server.
.
MessageId=9152
SymbolicName=FDO_152_URL_REQUEST_TIMEOUT
Language=English
HTTP server timeout. The client did not produce a request within the time that the server allowed.
.
MessageId=9153
SymbolicName=FDO_153_URL_NOT_AVAILABLE
Language=English
The requested HTTP resource is no longer available at the server.
.
MessageId=9154
SymbolicName=FDO_154_URL_REQUEST_TOO_LONG
Language=English
The HTTP server is refusing to service the request because the Request-URI is longer than the server is willing to interpret.
.
MessageId=9155
SymbolicName=FDO_155_URL_INTERNAL_SERVER_ERROR
Language=English
The HTTP server encountered an unexpected internal condition which prevented it from fulfilling the request.
.
MessageId=9156
SymbolicName=FDO_156_URL_REQUEST_NOT_IMPLEMENTED
Language=English
The HTTP server does not support the functionality required to fulfill the request.
.
MessageId=9157
SymbolicName=FDO_157_URL_OVERLOADED
Language=English
The HTTP server, while acting as a gateway or proxy, received an invalid response from the upstream server it accessed in attempting to fulfill the request. 
.
MessageId=9158
SymbolicName=FDO_158_URL_SERVICE_UNAVAILABLE
Language=English
The HTTP server is currently unable to handle the request due to a temporary overloading or maintenance of the server. 
.
MessageId=9159
SymbolicName=FDO_159_URL_GATEWAY_TIMEOUT
Language=English
The HTTP server, while acting as a gateway or proxy, did not receive a timely response from the upstream server specified by the URI. 
.
MessageId=9160
SymbolicName=FDO_160_HTTP_UNSUPPORTED_VERION
Language=English
The server does not support, or refuses to support, the HTTP protocol version that was used in the request message.
.
MessageId=9161
SymbolicName=FDO_161_UNABLE_CONNECT_HOST
Language=English
Unable to connect to the host.
.
MessageId=9162
SymbolicName=FDO_162_CONNECT_HOST_TIMEOUT
Language=English
The connection to the host has timed out.
.
MessageId=9163
SymbolicName=FDO_163_UNABLE_RESOLVE_HOST
Language=English
Unable to resolve the host. Host not found.
.
MessageId=9164
SymbolicName=FDO_164_UNSUPPORTED_HTTP_PROTOCOL
Language=English
Unsupported HTTP Protocol.
.
MessageId=9165
SymbolicName=FDO_165_MALFORMED_URL
Language=English
Malformed URL.
.
MessageId=9166
SymbolicName=FDO_166_UNABLE_RESOLVE_PROXY
Language=English
Couldn't resolve proxy name.
.
MessageId=9167
SymbolicName=FDO_167_DOWNLOAD_PARTIAL_FILE
Language=English
Downloaded a partial file.
.
MessageId=9168
SymbolicName=FDO_168_FAILURE_OPEN_FILE
Language=English
Could not open/read from file.
.
MessageId=9169
SymbolicName=FDO_169_FAILURE_POST
Language=English
Failure to HTTP POST data.
.
MessageId=9170
SymbolicName=FDO_170_FAILURE_SSL_CONNECT
Language=English
Server SSL connect error.
.
MessageId=9171
SymbolicName=FDO_171_UNABLE_RESUME_DOWNLOAD
Language=English
Could not resume data download.
.
MessageId=9172
SymbolicName=FDO_172_ABORTED_BY_CALLBACK
Language=English
The operation was aborted by an application callback.
.
MessageId=9173
SymbolicName=FDO_173_ENDLESS_URL_REDIRECT
Language=English
The number of URL redirects hit the maximum amount.
.
MessageId=9174
SymbolicName=FDO_174_SSL_PEER_CERTIFICATE
Language=English
The SSL peer certificate was not ok.
.
MessageId=9175
SymbolicName=FDO_175_SERVER_RETURNED_NOTHING
Language=English
The server returned no data.
.
MessageId=9176
SymbolicName=FDO_176_FAILURE_SENDING_NETWORK_DATA
Language=English
Failure sending Network data.
.
MessageId=9177
SymbolicName=FDO_177_FAILURE_RECIEVING_NETWORK_DATA
Language=English
Failure receiving Network data.
.
MessageId=9178
SymbolicName=FDO_178_SSL_CERTIFICATE_ERROR
Language=English
Error using the the local SSL certificate.
.
MessageId=9179
SymbolicName=FDO_179_SSL_CIPHER_ERROR
Language=English
Couldn't use the specified SSL cipher.
.
MessageId=9180
SymbolicName=FDO_180_CACERT_CIPHER_ERROR
Language=English
Server SSL certificate verification failed. Verify path and access rights.
.
MessageId=9181
SymbolicName=FDO_181_LDAP_INVALID_URL
Language=English
Invalid LDAP URL.
.
MessageId=9182
SymbolicName=FDO_182_AGGREGATE_IN_SELECT
Language=English
Aggregate functions are not supported by the Select command; use the SelectAggregates command instead.
.
MessageId=9183
SymbolicName=FDO_183_INVALID_FUNCTION_ARG
Language=English
One or more arguments for function '%1$ls' did not match the expected argument types.
.
MessageId=9184
SymbolicName=FUNCTION_GENERAL_ARG
Language=English
Argument to be processed
.
MessageId=9185
SymbolicName=FUNCTION_OPERATOR_ARG
Language=English
Operation indicator (ALL or DISTINCT)
.
MessageId=9186
SymbolicName=FUNCTION_OPERATOR_ARG_LIT
Language=English
Operation Indicator
.
MessageId=9187
SymbolicName=FUNCTION_BYTE_ARG_LIT
Language=English
byte
.
MessageId=9188
SymbolicName=FUNCTION_DATE_ARG_LIT
Language=English
date/time
.
MessageId=9189
SymbolicName=FUNCTION_NUMBER_ARG_LIT
Language=English
number
.
MessageId=9190
SymbolicName=FUNCTION_STRING_ARG_LIT
Language=English
string
.
MessageId=9191
SymbolicName=FUNCTION_PARAMETER_NUMBER_ERROR
Language=English
Expression Engine: Invalid number of parameters for function '%1$ls'
.
MessageId=9192
SymbolicName=FUNCTION_PARAMETER_ERROR
Language=English
Expression Engine: Invalid parameters for function '%1$ls'
.
MessageId=9193
SymbolicName=FUNCTION_PARAMETER_DATA_TYPE_ERROR
Language=English
Expression Engine: Invalid parameter data type for function '%1$ls'
.
MessageId=9194
SymbolicName=FUNCTION_RESULT_DATA_TYPE_ERROR
Language=English
Expression Engine: Invalid data type for return value for function '%1$ls'
.
MessageId=9195
SymbolicName=FUNCTION_OPERATOR_ERROR
Language=English
Expression Engine: Invalid first parameter value for function '%1$ls'
.
MessageId=9196
SymbolicName=FUNCTION_BOOL_ARG_LIT
Language=English
boolean
.
MessageId=9197
SymbolicName=FUNCTION_BLOB_ARG_LIT
Language=English
blob
.
MessageId=9198
SymbolicName=FUNCTION_CLOB_ARG_LIT
Language=English
clob
.
MessageId=9199
SymbolicName=FUNCTION_PARAMETER_DISTINCT_ERROR
Language=English
Expression Engine: DISTINCT not allowed for BLOB/CLOB for function '%1$ls'
.
MessageId=9200
SymbolicName=FUNCTION_SIGN
Language=English
Returns -1, 0, 1 depending on whether the value is > 0, == 0 or < 0
.
MessageId=9201
SymbolicName=FUNCTION_CONCAT_STRING1_ARG
Language=English
First string to concatenate
.
MessageId=9202
SymbolicName=FUNCTION_CONCAT_STRING2_ARG
Language=English
Second string to concatenate
.
MessageId=9203
SymbolicName=FUNCTION_UPPER_STRING_ARG
Language=English
String to be converted into uppercase
.
MessageId=9204
SymbolicName=FUNCTION_LOWER_STRING_ARG
Language=English
String to be converted into lowercase
.
MessageId=9205
SymbolicName=FUNCTION_GEOM_ARG_LIT
Language=English
geometry property
.
MessageId=9206
SymbolicName=FUNCTION_ABS
Language=English
Determines the absolute value of a numeric expression
.
MessageId=9207
SymbolicName=FUNCTION_ACOS
Language=English
Determines the arc cosine of a numeric expression
.
MessageId=9208
SymbolicName=FUNCTION_ASIN
Language=English
Determines the arc sine of a numeric expression
.
MessageId=9209
SymbolicName=FUNCTION_ATAN
Language=English
Determines the arc tangent of a numeric expression
.
MessageId=9210
SymbolicName=FUNCTION_COS
Language=English
Determines the cosine of a numeric expression
.
MessageId=9211
SymbolicName=FUNCTION_EXP
Language=English
Determines e raised to the power of a numeric expression value
.
MessageId=9212
SymbolicName=FUNCTION_LN
Language=English
Determines the natural logarithm of a numeric expression
.
MessageId=9213
SymbolicName=FUNCTION_SIN
Language=English
Determines the sine of a numeric expression
.
MessageId=9214
SymbolicName=FUNCTION_SQRT
Language=English
Determines the square root of a numeric expression
.
MessageId=9215
SymbolicName=FUNCTION_TAN
Language=English
Determines the tangent of a numeric expression
.
MessageId=9216
SymbolicName=FUNCTION_ATAN2
Language=English
Determines the arc tangent of two numeric expressions
.
MessageId=9217
SymbolicName=FUNCTION_LOG_BASE_ARG
Language=English
Logarithm base value
.
MessageId=9218
SymbolicName=FUNCTION_BASE_ARG_LIT
Language=English
base
.
MessageId=9219
SymbolicName=FUNCTION_DIV_BY_0_ERROR
Language=English
Expression Engine: Attempt to divide by 0 ('%1$ls')
.
MessageId=9220
SymbolicName=FUNCTION_DIVISOR_ARG
Language=English
The divisor
.
MessageId=9221
SymbolicName=FUNCTION_DIV_ARG_LIT
Language=English
divisor
.
MessageId=9222
SymbolicName=FUNCTION_MOD
Language=English
Determines the remainder of the division of two numeric expressions
.
MessageId=9223
SymbolicName=FUNCTION_LOG
Language=English
Determines the logarithm of two numeric expressions
.
MessageId=9224
SymbolicName=FUNCTION_REMAINDER
Language=English
Determines the remainder of the division of two numeric expressions
.
MessageId=9225
SymbolicName=FUNCTION_POWER_ARG
Language=English
Represents the power value a number should be raised to
.
MessageId=9226
SymbolicName=FUNCTION_POWER
Language=English
Raises a numeric expression to the power of another numeric expression
.
MessageId=9227
SymbolicName=FUNCTION_DATA_VALUE_ERROR
Language=English
Expression Engine: Invalid value for execution of function '%1$ls'
.
MessageId=9228
SymbolicName=FUNCTION_ROUND
Language=English
Rounds a numeric expressions to the specified degree
.
MessageId=9229
SymbolicName=FUNCTION_UNEXPECTED_RESULT_ERROR
Language=English
Expression Engine: Unexpected result for function '%1$ls'
.
