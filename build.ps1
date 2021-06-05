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
    $ObjDir = ".\obj\rel\"
    $BinDir = ".\bin\rel\"
}
else
{
    $OptimizationLevel = "0"
    $DebugArg = "-DDBG"
    $ObjDir = ".\obj\dbg\"
    $BinDir = ".\bin\dbg\"
}

$filesToLink = ""
dir .\src\*.c | %{
    $fileBaseName = $_.BaseName
    $objName = "$ObjDir\$fileBaseName.o"
    $cmd = "gcc -c -std=c99 $DebugArg $_ -I .\ext\std_include -Wall -pedantic -O$OptimizationLevel -o $objName"
    Write-Host -ForegroundColor DarkGray $cmd
    invoke-expression -Command $cmd

    if (!$?)
    {
        Write-Warning "Failed to compile $_!"
        return;
    }

    $filesToLink += "$objName "
}

$linkerCmd = "gcc $filesToLink -o $BinDir\hello.elf"
Write-Host -ForegroundColor DarkGray $linkerCmd
invoke-expression $linkerCmd
if (!$?)
{
    Write-Warning "Failed to link!"
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