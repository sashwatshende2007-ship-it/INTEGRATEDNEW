$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName = ".\SmartHospital.exe"
$psi.UseShellExecute = $false
$psi.RedirectStandardInput = $true
$psi.RedirectStandardOutput = $true
$psi.RedirectStandardError = $true

$proc = [System.Diagnostics.Process]::Start($psi)

# Give it 3 seconds to run OpenGL window and render multiple frames
Start-Sleep -Seconds 3

Write-Host "Process Still Running after 3s:" (-not $proc.HasExited)
if (-not $proc.HasExited) {
    Write-Host "Sending choice 13 to exit cleanly..."
    $proc.StandardInput.WriteLine("13")
    $proc.WaitForExit(3000)
}

Write-Host "Final ExitCode:" $proc.ExitCode
$stdout = $proc.StandardOutput.ReadToEnd()
$stderr = $proc.StandardError.ReadToEnd()
if ($stderr) {
    Write-Host "Stderr: $stderr"
}
