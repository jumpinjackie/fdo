$solutionDir = [System.IO.Path]::GetDirectoryName($dte.Solution.FullName) + "\"
$path = $installPath.Replace($solutionDir, "`$(SolutionDir)")

$FdoPostBuildCmd = "
copy /y `"`$(SolutionDir)fdo-staging-x86\*`" `"`$(TargetDir)`""