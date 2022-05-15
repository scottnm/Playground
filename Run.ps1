<#
.SYNOPSIS
Script for running the ray tracer.
#>
param(
    # The generated ppm file
    [Parameter(Mandatory=$true)]
    [String]$OutputName,

    # Determines whether to do a full or quick render pass
    # The default is quick
    [validateset("Quick", "Full", "Both")]
    [String]$RunType,

    # The width of the output image
    # The default value is 400
    [int]$Width = 400,

    # The height of the output image
    # The default value is 200
    [int]$Height = 200
    )

$runSet = [System.Collections.ArrayList]::new()
if ($RunType -eq "Quick" -or $RunType -eq "Both")
{
    $runSet.Add($false) | Out-Null
}
if ($RunType -eq "Full" -or $RunType -eq "Both")
{
    $runSet.Add($true) | Out-Null
}

foreach ($fullRun in $runSet)
{
    $AntiAliasingSamples = if ($fullRun) { 100 } else { 8 };
    $AdjustedOutputName = if ($fullRun) { $OutputName } else { "$($OutputName)_quick" };
    echo "Starting run: $AdjustedOutputName"

    $outputFilePath = "data\$AdjustedOutputName.ppm"
    $outputMetaFilePath = "data\$AdjustedOutputName.txt"
    cargo run -- --output $outputFilePath --meta $outputMetaFilePath --antialiasing $AntiAliasingSamples --width $Width --height $Height
    if (!$?) { exit }

    OpenSeeIt.exe $outputFilePath
    echo "Meta:"
    type $outputMetaFilePath
}
