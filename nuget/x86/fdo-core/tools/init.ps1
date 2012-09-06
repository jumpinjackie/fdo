param($installPath, $toolsPath, $package, $project)

$ScriptDirectory = Split-Path $MyInvocation.MyCommand.Path
. (Join-Path $ScriptDirectory FdoUtils.ps1)

$fxVer = Get-Supported-Fdo-NetFx-Version
$solutionDir = (Get-Item $dte.Solution.FullName).Directory.FullName
$targetDir = Join-Path $solutionDir "fdo-staging-x86"
$sourceDir = (Join-Path ((Get-Item $toolsPath).parent.FullName) "fdo-libs\$fxVer")

# Keep this up to date. boost, gdal, xalan and xerces dlls may change with newer versions
# of FDO
$fileList = @(
    "boost_date_time-vc100-mt-1_42.dll",
    "boost_thread-vc100-mt-1_42.dll",
    "ExpressionEngine.dll",
    "Fdo.dll",
    "FDOCommon.dll",
    "FDOGeometry.dll",
    "FDOMessage.dll",
    "FDOSpatial.dll",
    "gdal17.dll",
    "OWS.dll",
    #"providers.xml",
    "Xalan-C_1_11.dll",
    "XalanMessages_1_11.dll",
    "xerces-c_3_1.dll"
)

Copy-FDO-Files-To-Staging $package.Id $sourceDir $targetDir $fileList