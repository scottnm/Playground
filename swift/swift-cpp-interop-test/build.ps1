Write-Host -foregroundcolor cyan "making output dir"
$outputDir = "$PSScriptRoot/output"

Write-Host -foregroundcolor cyan "configuring"
cmake -G Ninja -S "$PSScriptRoot" -B $outputDir

Write-Host -foregroundcolor cyan "building"
cmake --build $outputDir