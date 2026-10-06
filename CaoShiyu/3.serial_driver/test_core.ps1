$ErrorActionPreference = 'Stop'

# Run from any directory. Compile only the ROS-independent receive core.
$compiler = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $compiler) {
    $compiler = Get-Command clang++ -ErrorAction SilentlyContinue
}
if (-not $compiler) {
    throw 'g++ or clang++ was not found in PATH. Add your existing compiler to PATH first.'
}

$outputDir = Join-Path $PSScriptRoot 'build\core-test'
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
$executable = Join-Path $outputDir 'receive_core_test.exe'
$arguments = @(
    '-std=c++14', '-Wall', '-Wextra', '-Werror', '-pedantic',
    '-I', (Join-Path $PSScriptRoot 'include'),
    (Join-Path $PSScriptRoot 'tests\receive_core_test.cpp'),
    (Join-Path $PSScriptRoot 'src\default_receive_protocol.cpp'),
    (Join-Path $PSScriptRoot 'src\crc.cpp'),
    '-o', $executable
)

& $compiler.Source @arguments
if ($LASTEXITCODE -ne 0) {
    throw 'Compilation failed. The old executable will not be run.'
}
& $executable
if ($LASTEXITCODE -ne 0) {
    throw 'One or more receive-core tests failed.'
}
