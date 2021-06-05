param(
    [switch]$Release,
    [switch]$Run
    )

$compilerFlags = @(
    "-c",
    "-std=c99",
    "-I .\ext\std_include",
    "-Wall",
    "-pedantic",
    "-Werror"
)

# -mthumb and -mthumb-interwork allow for using thumb instructions in our binary.
# This significantly speeds up program execution
$compilerFlags += "-mthumb";
$compilerFlags += "-mthumb-interwork";

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
    $cmd = "gcc $($compilerFlags -Join " ") $_ -o $objName"+ ';$?'
    Write-Host -ForegroundColor DarkGray $cmd
    $success = invoke-expression -Command $cmd

    if (!$success)
    {
        Write-Warning "Failed to compile $_!"
        exit;
    }

    $objFilesToLink += $objName
}

$linkerFlags = @(
    "-mthumb",
    "-mthumb-interwork"
)

$linkerCmd = "gcc $($linkerFlags -join " ") $($objFilesToLink -join " ") -o $elfBinary" + ';$?'
Write-Host -ForegroundColor DarkGray $linkerCmd
$success = invoke-expression $linkerCmd
if (!$success)
{
    Write-Warning "Failed to link!"
    exit;
}

$objCopyCmd = "objcopy -O binary $elfBinary $gbaBinary" + ';$?'
Write-Host -ForegroundColor DarkGray $objCopyCmd
$success = invoke-expression $objCopyCmd
if (!$success)
{
    Write-Warning "Failed to run objcopy!"
    exit;
}

if ($Run)
{
    Write-Host -ForegroundColor Cyan "`nRunning:"
    VisualBoyAdvance.exe $gbaBinary
}