param(
    [Parameter(Mandatory)][string[]]$Files,
    [Parameter(Mandatory)][string]$OutputFile,
    [switch]$Run,
    [string[]]$RunArgs
    )

Write-Host -ForegroundColor Cyan "Compiling $($OutputFile)..."

$compileCmd = "cl /TC $Files /std:c17 /w44800 /W4 /WX /Z7 /nologo /Fe:$OutputFile; if (!`$?) { throw 'Failed to compile $OutputFile' }"
Write-Host -ForegroundColor DarkGray "    $compileCmd"
Invoke-Expression $compileCmd

if ($Run)
{
    Write-Host -ForegroundColor Cyan "`nRunning $($OutputFile):"
    Invoke-Expression "$OutputFile $RunArgs"
}
