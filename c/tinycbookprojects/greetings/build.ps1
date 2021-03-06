param(
    [switch]$Run,
    [string[]]$RunArgs
    )

cl /TC $PSScriptRoot\main.c /std:c17 /I /W4 /Z7 /nologo /Fe:$PSScriptRoot\greetings_win.exe

if (!$?)
{
    Write-Warning "Failed to compile!"
    return;
}

if ($Run)
{
    Write-Host -ForegroundColor Cyan "`nRunning:"
    Invoke-Expression "$PSScriptRoot\greetings_win.exe $RunArgs"
}