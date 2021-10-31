param(
    [switch]$Run,
    [string[]]$RunArgs
    )

& "$PSScriptRoot\..\common\Build-TinyC.ps1" `
    -Files "$PSScriptRoot\main.c" `
    -OutputFile "$PSScriptRoot\nato.exe" `
    -Run:$Run `
    -RunArgs:$RunArgs