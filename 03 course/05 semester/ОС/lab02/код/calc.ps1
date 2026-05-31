Invoke-Expression "cls"

Write-Host "Windows Калькулятор" -ForegroundColor Green;
Write-Host @"

  Операции калькулятора:\\n

  + -- сложение
  - -- вычитание
  * -- умножение
  / -- деление (целая часть)
  % -- остаток от деления
  ** -- возведение в степень
  sqrt -- квадратичный корень
  ln -- натуральный логарифм
  lg -- десятичный логарифм
  sin -- синус (в градусах)
  cos -- косинус (в градусах)
  tan -- тангенс (в градусах)
  ctg -- котангенс (в градусах)

"@ -ForegroundColor Red;

$one_ops = @('sqrt', 'ln', 'lg', 'sin', 'cos', 'tan', 'ctg' );
$two_ops = @('+', '-', '*', '/', '**', '%');

while (1) {
  $a = $null; $b = $null; $r = $null
  $op = Read-Host "Выберите операцию";
  if ($op -in $one_ops -or $op -in $two_ops) {
    if ($op -in $one_ops) {
      $a = Read-Host "Введите число";
      if ($a -notmatch '^?\d+(\.\d+)?$') { 
        Write-Host "Введите корректное число." -ForegroundColor Red 
        continue
      };
    }
    else {
      $a = Read-Host "Введите первое число";
      if ($a -notmatch '^-?\d+(\.\d+)?$') { 
        Write-Host "Введите корректное число." -ForegroundColor Red 
        continue
      }; 
      $b = Read-Host "Введите второе число";
      if ($b -notmatch '^-?\d+(\.\d+)?$') { 
        Write-Host "Введите корректное число." -ForegroundColor Red 
        continue
      };
    }
  }
  elseif ($op -eq 'exit') {
    break;
  }
  else {
    Write-Host "Ошибка: неверная операция!" -ForegroundColor Red;
    continue
  };

  if ($a -and -not $b) {
    $a = [double]$a;
    $e = $false; 
    $r = switch ($op) {
      'sqrt' { if ($a -lt 0) { Write-Host "Нельзя извлечь корень отрицательного числа." -ForegroundColor Red; $e = !$e; break }; [Math]::Sqrt($a) }
      'sin' { [Math]::Sin($a * [Math]::PI / 180) }
      'cos' { [Math]::Cos($a * [Math]::PI / 180) }
      'tan' { [Math]::Tan($a * [Math]::PI / 180) }
      'ctg' { 1 / ([Math]::Tan($a * [Math]::PI / 180)) }
      'ln' { if ($a -lt 0) { Write-Host "Число не может быть <=0." -ForegroundColor Red; $e = !$e; break }; [Math]::Log($a) }
      'lg' { if ($a -lt 0) { Write-Host "Число не может быть <=0." -ForegroundColor Red; $e = !$e; break }; [Math]::Log10($a) }
      Default { }
    }

    if (!$e) { Write-Host "Результат: $r" } else { continue }
  }
  elseif ($a -and $b) {
    $a = [double]$a;
    $b = [double]$b;
    $e = $false;
    $r = switch ($op) {
      '+' { $a + $b }
      '-' { $a - $b }
      '*' { $a * $b }
      '/' { if ($b -eq 0) { Write-Host "Делить на ноль нельзя." -ForegroundColor Red; $e = !$e; break }; $a / $b }
      '**' { [Math]::Pow($a, $b) }
      '%' { $a % $b }
      Default { }
    }

    if (!$e) { Write-Host "Результат: $r" } else { continue }
  }
  else { 
    Write-Host "Неверная операция." -ForegroundColor Red; 
    continue 
  }; 
}