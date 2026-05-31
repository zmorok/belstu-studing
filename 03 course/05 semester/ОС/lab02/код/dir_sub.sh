BASE="${1:-.}"
[ -d "$BASE" ] || { echo "Каталог не найден: $BASE" >&2; exit 1; }

human() {
  local bytes=$1
  local size unit
  if (( bytes < 1024 )); then size=$bytes; unit="B"
  elif (( bytes < 1024*1024 )); then size=$(awk "BEGIN {printf \"%.2f\", $bytes/1024}"); unit="KB"
  elif (( bytes < 1024*1024*1024 )); then size=$(awk "BEGIN {printf \"%.2f\", $bytes/1024/1024}"); unit="MB"
  else size=$(awk "BEGIN {printf \"%.2f\", $bytes/1024/1024/1024}"); unit="GB"
  fi
  echo "$size $unit"
}

files_in_dir() { find "$1" -type f 2>/dev/null | wc -l }

find "$BASE" -mindepth 1 -print0 | while IFS= read -r -d '' p; do
  rel="${p#"$BASE"/}"
  depth=$(tr -cd '/' <<<"$rel" | wc -c)
  indent="$(printf '%*s' $((depth*2)) '')"

  if [ -d "$p" ]; then echo "${indent}DIR $rel — files: $(files_in_dir "$p")"
  elif [ -f "$p" ]; then size_b=$(stat -c %s "$p"); echo "${indent}FILE $rel — $(human "$size_b")"
  fi
done
