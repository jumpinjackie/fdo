param($installPath, $toolsPath, $package, $project)

. (Join-Path $toolsPath "GetFdoPostBuildCmd.ps1")

# Get the current Post Build Event cmd
$currentPostBuildCmd = $project.Properties.Item("PostBuildEvent").Value

# Append our post build command if it's not already there
if (!$currentPostBuildCmd.Contains($FdoPostBuildCmd)) {
    $project.Properties.Item("PostBuildEvent").Value += $FdoPostBuildCmd
    Write-Host "Updated Post-Build event for project"
}