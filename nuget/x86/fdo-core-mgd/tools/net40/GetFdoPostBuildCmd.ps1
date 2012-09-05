$solutionDir = [System.IO.Path]::GetDirectoryName($dte.Solution.FullName) + "\"
$path = $installPath.Replace($solutionDir, "`$(SolutionDir)")

$FdoPostBuildCmd = "
copy /y `"`$(ProjectDir)fdo-libs\*`" `"`$(TargetDir)`""