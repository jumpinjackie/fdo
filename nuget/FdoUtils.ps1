
# FdoUtils.ps1
#
# Useful utility functions for most FDO nuget packages

# How most fdo packages will work:
#
# We don't store unmanaged dlls under the content directory, as these files are not properly
# re-created when restoring packages.
#
# Instead as part of package preparation, our content files are packaged into a custom directory
# (breaking the convention set by NuGet). On package installation, these files are copied into a
# designated staging area at the solution level.
#
# So a nuget install of fdo-core and fdo-rdbms will produce this type of solution layout
#
# solution/
#   MyFdoApplication/
#   packages/
#       fdo-core/
#       fdo-rdbms/
#   fdo-staging/
#       [fdo-core content files]
#       [fdo-rdbms content files]
#
# The moment the fdo-core-mgd package is installed, it will write a post-build event to copy
# all files in the staging dir to the project's output directory, ensuring all the unmanaged
# dependencies are satisifed when the project executable is launched
#
# This approach gives us the following benefits:
#  - No "pollution" of content files in the target projects
#  - Minimal intrusiveness. Only references and post-builds are applied to the projects in question. Nothing else
#  - Most important. If your project is under revision control (like any good project should), only packages.config
#    needs to be checked in. Everything else (packages and fdo-staging) will be restored by NuGet if required
#
# Now for the caveat:
#
#  We have to do this here instead of install.ps1 because this is a "tools-only" package with "content" stored in
#  a non-standard location to prevent intrusive installation by NuGet. So we have to do this here
#
#  Also because of the tools-only nature, uninstall.ps1 is not called either so the user will have to manually
#  remove the affected files in fdo-staging. It's a small price to pay for maximum non-intrusiveness

function Get-Fdo-Version { "3.7" }

function Get-Fdo-Version-Full { 
    $ver = Get-Fdo-Version
    "$ver.0.0" 
}

function Get-Supported-Fdo-NetFx-Version { "net40" }

function Write-FDO-Providers-Xml-Entry {
    param([string]$providersPath, [string]$providerName, [string]$providerDispName, [string]$providerDesc, [string]$fdoVer, [string]$providerLibPath)

    if (!(Test-Path $providersPath)) {
        Set-Content $providersPath "<?xml version=`"1.0`" encoding=`"utf-8`"?><FeatureProviderRegistry></FeatureProviderRegistry>"
        Write-Host "Wrote empty $providersPath"
    }

    $doc =[xml](Get-Content $providersPath)
    $node = $doc.SelectSingleNode("//FeatureProviderRegistry/FeatureProvider/Name[text() = '$providerName']")
    if (!$node) {
        Write-Host "providers.xml entry for $providerName not found. Creating it"
        $node = $doc.createElement("FeatureProvider")
        $node.InnerXml = "<Name>$providerName</Name><DisplayName>$providerDispName</DisplayName><Description>$providerDesc</Description><IsManaged>False</IsManaged><Version>$fdoVer</Version><FeatureDataObjectsVersion>$fdoVer</FeatureDataObjectsVersion><LibraryPath>$providerLibPath</LibraryPath>"
        $doc.DocumentElement.appendChild($node)
    } else {
        Write-Host "providers.xml entry for $providerName found. Updating it"
        #$node.Name = $providerName
        #$node.DisplayName = $providerDispName
        #$node.Description = $providerDesc
        #$node.IsManaged = "False"
        #$node.Version = $fdoVer
        #$node.FeatureDataObjectsVersion = $fdoVer
        #$node.LibraryPath = $providerLibPath
    }
    $doc.Save($providersPath)
}

# Copy-FDO-Files-To-Staging
#
# Copies the designated list of files from the specific FDO package's source path to the staging
# directory. Files that already exist in the staging directory are not copied
function Copy-FDO-Files-To-Staging {
    param([string]$packageName, [string]$sourcePath, [string]$targetPath, [string[]]$fileList)

    #Write-Host "Target Dir: $targetDir"
    #Write-Host "Custom content path: $sourceDir"

    if (!(Test-Path $targetDir)) {
        New-Item $targetDir -itemtype directory
        Write-Host "Created $targetDir"
    }

    $filesCopied = 0;
    # NOTE: This is called not just on install, but everytime the solution is opened (That's what the docs says!)
    # So only copy the files if they don't already exist
    foreach ($file in $fileList) {
        #Write-Host "$sourceDir\$file => $targetDir\$file"
        if (!(Test-Path "$targetDir\$file")) { 
            Copy-Item "$sourceDir\$file" "$targetDir\$file" -Force
            $filesCopied++
        } else {
            Write-Host "$file already exists"
        }
    }

    Write-Host "$filesCopied files copied"
    if ($filesCopied -gt 0) {
        Write-Host "##########################################################################"
        Write-Host "# This package ($packageName) has added unmanaged dlls to: <$targetDir>"
        Write-Host "#"
        Write-Host "# Installing the fdo-core-mgd package will create a post-build event to"
        Write-Host "# copy all the files in this directory to your project's output directory"
        Write-Host "# with no intervention required on your part"
        Write-Host "#"
        Write-Host "# Please note that uninstalling this package will not delete these files"
        Write-Host "# in <$targetDir> and you will have to remove these files manually"
        Write-Host "##########################################################################"
    }
}