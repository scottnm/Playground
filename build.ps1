param(
    [switch]$Run
    )

cl /TC .\src\*.c /I .\src\ /W4 /WX /Z7 /nologo /Fo:.\obj\ /Fe:.\bin\gba_hello.exe

if (!$?)
{
    Write-Warning "Failed to compile!"
    return;
}

if ($Run)
{
    Write-Warning "Run not yet supported"
    # Write-Host -ForegroundColor Cyan "`nRunning:"
    # .\bin\slide.exe
}