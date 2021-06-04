param(
    [switch]$Release,
    [switch]$Run
    )

# cl /TC .\src\*.c /I .\src\ /W4 /WX /Z7 /nologo /Fo:.\obj\ /Fe:.\bin\gba_hello.exe
$DebugArg = if (!$Release) { "-DDBG" } else { "" }
if ($Release)
{
    $DebugArg = ""
    $OptimizationLevel = "2"
    $BinDir = ".\bin\rel\"
}
else
{
    $OptimizationLevel = "0"
    $DebugArg = "-DDBG"
    $BinDir = ".\bin\dbg\"
}

gcc `
    -std=c99 `
    $DebugArg `
    .\src\*.c `
    -I .\ext\std_include `
    -Wall `
    -pedantic `
    "-O$OptimizationLevel" `
    -o "$BinDir\hello.elf" `
    -lm
if (!$?)
{
    Write-Warning "Failed to run gcc!"
    return;
}

objcopy `
    -O binary "$BinDir\hello.elf" `
    "$BinDir\hello.gba"
if (!$?)
{
    Write-Warning "Failed to run objcopy!"
    return;
}

if ($Run)
{
    Write-Host -ForegroundColor Cyan "`nRunning:"
    VisualBoyAdvance.exe "$BinDir\hello.gba"
}