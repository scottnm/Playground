param(
    [switch]$Release,
    [switch]$Run
    )

# cl /TC .\src\*.c /I .\src\ /W4 /WX /Z7 /nologo /Fo:.\obj\ /Fe:.\bin\gba_hello.exe
$compilerFlags = @(
    "-c",
    "-std=c99",
    "-mthumb", # FIXME: what does this mthumb and mthumb-interwork stuff do? need to figure that out
    "-mthumb-interwork",
    "-I .\ext\std_include",
    "-Wall",
    "-pedantic"
)

if ($Release)
{
    $compilerFlags += "-O2" # O2 optimization level
    $ObjDir = ".\obj\rel"
    $BinDir = ".\bin\rel"
}
else
{
    $compilerFlags += "-DDBG"
    $compilerFlags += "-O0"  # no optimization
    $ObjDir = ".\obj\dbg"
    $BinDir = ".\bin\dbg"
}

$elfBinary = "$BinDir\hello.elf"
$gbaBinary = "$BinDir\hello.gba"

$objFilesToLink = @()
dir .\src\*.c | %{
    $fileBaseName = $_.BaseName
    $objName = "$ObjDir\$fileBaseName.o"
    $cmd = "gcc $($compilerFlags -Join " ") $_ -o $objName"
    Write-Host -ForegroundColor DarkGray $cmd
    invoke-expression -Command $cmd

    if (!$?)
    {
        Write-Warning "Failed to compile $_!"
        return;
    }

    $objFilesToLink += $objName
}

$linkerFlags = @(
    "-mthumb",
    "-mthumb-interwork"
)

$linkerCmd = "gcc $($linkerFlags -join " ") $($objFilesToLink -join " ") -o $elfBinary"
Write-Host -ForegroundColor DarkGray $linkerCmd
invoke-expression $linkerCmd
if (!$?)
{
    Write-Warning "Failed to link!"
    return;
}

$objCopyCmd = "objcopy -O binary $elfBinary $gbaBinary"
Write-Host -ForegroundColor DarkGray $objCopyCmd
invoke-expression $objCopyCmd
if (!$?)
{
    Write-Warning "Failed to run objcopy!"
    return;
}

if ($Run)
{
    Write-Host -ForegroundColor Cyan "`nRunning:"
    VisualBoyAdvance.exe $gbaBinary
}