<#
.SYNOPSIS
Script for running the ray tracer.
#>
param(
    # The generated ppm file
    [Parameter(Mandatory=$true)]
    [String]$OutputName,

    # The number of anti-aliasing samples to collect per pixel
    # The default value is 8
    [int]$AntiAliasingSamples = 8,

    # The width of the output image
    # The default value is 400
    [int]$Width = 400,

    # The height of the output image
    # The default value is 200
    [int]$Height = 200
    )

$outputFilePath = "data\$OutputName.ppm"
$outputMetaFilePath = "data\$OutputName.txt"
cargo run -- --output $outputFilePath --meta $outputMetaFilePath --antialiasing $AntiAliasingSamples --width $Width --height $Height
if (!$?) { exit }

OpenSeeIt.exe $outputFilePath
echo "Meta:"
type $outputMetaFilePath
