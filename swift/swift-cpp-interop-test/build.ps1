$originalLocation = get-location

try {
    Write-Host -foregroundcolor cyan "making output dir"
    $outputDir = "$PSScriptRoot/output"
    if (Test-Path $outputDir)
    {
        remove-item -force -recurse $outputDir
    }
    new-item -itemtype directory -force -path $outputDir

    Write-Host -foregroundcolor cyan "configuring"
    cd $outputDir
    cmake "$PSScriptRoot"

    Write-Host -foregroundcolor cyan "building"
    cmake --build .
}
finally {
    set-location $originalLocation
}
