Invoke-Expression 'cls'

Write-Host "Конвертер температуры Windows" -ForegroundColor Red;
Write-Host "примеры: 100, 100K, 100C, 100F"

while (1) {
  $temp = Read-Host "Введите температуру"

  $e = $false;

  if ([string]::IsNullOrWhiteSpace($temp)) { Write-Host "Введена пустая строка." -ForegroundColor Red; continue }

  $norm = $temp.Trim().Replace(',', '.');

  if ($norm -notmatch '^\s*(-?\d+(?:\.\d+)?)\s*([CFKcfk])?\s*$') { Write-Host "Введите корректное значение температуры (напр. 100K, 100C, 100, 100F)." -ForegroundColor Red; continue }


  $value = [double]$matches[1]; $unit = if ($matches[2]) { $matches[2].ToUpper() } else { 'C' }
  if (-not $unit) { $unit = 'C' }

  switch ($unit) {
    'C' {
      $tC = $value; $tK = $tC + 273.15; $tF = ($tC * 9 / 5) + 32
      if (!$e) { Write-Host ("{0:N2} C`n{1:N2} F`n{2:N2} K" -f $tC, $tF, $tK) } else { continue }
    }

    'F' {
      $tF = $value; $tC = ($tF - 32) * 5 / 9; $tK = $tC + 273.15
      if (!$e) { Write-Host ("{0:N2} C`n{1:N2} F`n{2:N2} K" -f $tC, $tF, $tK) } else { continue }
    }

    'K' {
      if ($value -lt 0) { Write-Host "Температура в Кельвинах не может быть отрицательной." -ForegroundColor Red; $e = !$e; }
      $tK = $value; $tC = $tK - 273.15; $tF = ($tC * 9 / 5) + 32
      if (!$e) { Write-Host ("{0:N2} C`n{1:N2} F`n{2:N2} K" -f $tC, $tF, $tK) } else { continue }
    }
  }
}
