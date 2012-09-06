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
    "com\fdo_sys.sql",
    "com\fdo_sys_idx.sql",
    "com\fdosys_sys.sql",
    "RdbmsMsg.dll",
    "RdbmsOverrides.dll",
    "SmMessage.dll"
)

$comDir = Join-Path $targetDir "com"
if (!(Test-Path $comDir)) {
    New-Item $comDir -itemtype directory
    Write-Host "Created $comDir"
}

Copy-FDO-Files-To-Staging $package.Id $sourceDir $targetDir $fileList