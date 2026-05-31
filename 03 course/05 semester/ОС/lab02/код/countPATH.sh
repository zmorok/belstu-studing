x_count=0
no_x_count=0

IFS=':'

for dir in $PATH; do
  if [ -d "$dir" ]; then
    for file in "$dir"/*; do
      if [ -f "$file" ]; then
        if [ -x "$file" ]; then
          ((x_count++))
        else
          ((no_x_count++))
        fi
      fi
    done
  fi
done

echo "executables:     $x_count"
echo "non_executables: $no_x_count"
