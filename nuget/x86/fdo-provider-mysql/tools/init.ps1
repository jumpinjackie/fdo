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
    "MySqlOverrides.dll",
    "MySQLProvider.dll"
)

Copy-FDO-Files-To-Staging $package.Id $sourceDir $targetDir $fileList

$fdoVer = Get-Fdo-Version
$fdoVerFull = Get-Fdo-Version-Full
$providerName = "OSGeo.MySQL.$fdoVer"
$providerDispName = "OSGeo FDO Provider for MySQL"
$providerDesc = "FDO Provider for MySql"
$providerLibPath = ".\MySQLProvider.dll"

$providersPath = Join-Path $targetDir "providers.xml"
Write-FDO-Providers-Xml-Entry $providersPath $providerName $providerDispName $providerDesc $fdoVer $providerLibPath