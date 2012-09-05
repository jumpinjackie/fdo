@echo off
REM This batch file builds the file exclusion list for a seamless xcopy
REM of the desired content to the target directory
REM
REM This file is meant to be called by build.bat

if exist tmp_exclude_list.txt del tmp_exclude_list.txt
for /f %%f in ('dir /b list*') do (
	if not "list_%1.txt" == "%%f" (
		type %%f >> tmp_exclude_list.txt
		echo. >> tmp_exclude_list.txt
	)
)