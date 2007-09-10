@ECHO OFF
SET PROJECT=PostGis
SET TARGET=Build
SET TARGETCONFIG=Debug
::Uncomment the line below to build in Release mode
::SET TARGETCONFIG=Debug
SET TARGETPLATFORM=Win32
SET CLP=PerformanceSummary;NoSummary


ECHO ===============================================
ECHO   Build '%PROJECT%' project
ECHO ===============================================

msbuild %PROJECT%.sln /t:%TARGET% /p:Configuration=%TARGETCONFIG% /p:Platform=%TARGETPLATFORM% /clp:%CLP% /v:normal /nologo
