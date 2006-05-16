@echo off
SET FDOPROVIDERTEMPLATE=%cd%
cscript //job:register "%FDO%\..\preparebuilds.wsf"