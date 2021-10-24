param(
    [Parameter(Mandatory)][string[]]$Files,
    [Parameter(Mandatory)][string]$OutputFile,
    [switch]$Run,
    [string[]]$RunArgs
    )

Write-Host -ForegroundColor Cyan "Compiling $($OutputFile)..."
Write-Host -ForegroundColor DarkGray "    cl /TC $Files /std:c17 /I /W4 /Z7 /nologo /Fe:$OutputFile"
cl /TC $Files /std:c17 /I /W4 /Z7 /nologo /Fe:$OutputFile

if (!$?)
{
    Write-Warning "Failed to compile $OutputFile!"
    return;
}

if ($Run)
{
    Write-Host -ForegroundColor Cyan "`nRunning $($OutputFile):"
    Invoke-Expression "$OutputFile $RunArgs"
}
