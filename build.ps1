param(
    [switch]$Run
    )

# cl /TC .\src\*.c /I .\src\ /W4 /WX /Z7 /nologo /Fo:.\obj\ /Fe:.\bin\gba_hello.exe
gcc -std=c99 -o .\bin\hello.elf .\src\*.c -lm
if (!$?)
{
    Write-Warning "Failed to run gcc!"
    return;
}

objcopy -O binary .\bin\hello.elf .\bin\hello.gba
if (!$?)
{
    Write-Warning "Failed to run objcopy!"
    return;
}

if ($Run)
{
    Write-Warning "Run not yet supported"
    # Write-Host -ForegroundColor Cyan "`nRunning:"
    # .\bin\slide.exe
}