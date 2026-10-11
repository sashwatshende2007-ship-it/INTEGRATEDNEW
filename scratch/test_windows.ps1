Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class WinInfo {
    public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
    [DllImport("user32.dll")]
    public static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);
    [DllImport("user32.dll")]
    public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);
    [DllImport("user32.dll")]
    public static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
    [DllImport("user32.dll")]
    public static extern bool IsWindowVisible(IntPtr hWnd);
    [DllImport("user32.dll")]
    public static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);
}
"@

$p = Start-Process -FilePath ".\SmartHospital.exe" -PassThru
Start-Sleep -Milliseconds 2500

[WinInfo]::EnumWindows({
    param($hWnd, $lParam)
    $procId = 0
    [WinInfo]::GetWindowThreadProcessId($hWnd, [ref]$procId) | Out-Null
    if ($procId -eq $p.Id) {
        $sbTitle = New-Object System.Text.StringBuilder 256
        [WinInfo]::GetWindowText($hWnd, $sbTitle, 256) | Out-Null
        $sbClass = New-Object System.Text.StringBuilder 256
        [WinInfo]::GetClassName($hWnd, $sbClass, 256) | Out-Null
        $vis = [WinInfo]::IsWindowVisible($hWnd)
        Write-Host "HWND: $hWnd | Class: $($sbClass.ToString()) | Title: '$($sbTitle.ToString())' | Visible: $vis"
    }
    return $true
}, [IntPtr]::Zero) | Out-Null

Stop-Process -Id $p.Id -Force
