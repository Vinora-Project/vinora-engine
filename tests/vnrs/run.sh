#!/bin/bash
# Compare ./vnrs_parser with tests/vnrs/cases/*.stdout.
# A case may also have a .stderr file. Otherwise stderr must be empty.

cd "$(dirname "$0")/../.."

parser="./vnrs_parser"
cases="tests/vnrs/cases"
fail=0
pass=0

if [[ ! -x "$parser" ]]; then
    echo "missing $parser (make vnrs_parser)" >&2
    exit 1
fi

say_ok() {
    echo "ok   $1"
    pass=$((pass + 1))
}

say_fail() {
    echo "FAIL $1"
    fail=$((fail + 1))
}

run_file() {
    local vnrs="$1"
    local name out err status

    name=$(basename "$vnrs" .vnrs)
    out=$(mktemp)
    err=$(mktemp)
    status=0
    "$parser" "$vnrs" >"$out" 2>"$err" || status=$?

    if [[ "$status" -ne 0 ]]; then
        say_fail "$name (exit $status)"
        rm -f "$out" "$err"
        return
    fi
    if ! diff -u "$cases/$name.stdout" "$out"; then
        say_fail "$name stdout"
        rm -f "$out" "$err"
        return
    fi
    if [[ -f "$cases/$name.stderr" ]]; then
        if ! diff -u "$cases/$name.stderr" "$err"; then
            say_fail "$name stderr"
            rm -f "$out" "$err"
            return
        fi
    elif [[ -s "$err" ]]; then
        echo "unexpected stderr in $name:" >&2
        cat "$err" >&2
        say_fail "$name stderr"
        rm -f "$out" "$err"
        return
    fi

    rm -f "$out" "$err"
    say_ok "$name"
}

run_usage() {
    local out err status

    out=$(mktemp)
    err=$(mktemp)
    status=0
    "$parser" >"$out" 2>"$err" || status=$?
    if [[ "$status" -ne 1 || -s "$out" ]]; then
        say_fail "usage"
    elif ! grep -q "Usage: " "$err"; then
        say_fail "usage"
    else
        say_ok "usage"
    fi
    rm -f "$out" "$err"

    out=$(mktemp)
    err=$(mktemp)
    status=0
    "$parser" a.vnrs b.vnrs >"$out" 2>"$err" || status=$?
    if [[ "$status" -ne 1 || -s "$out" ]]; then
        say_fail "extra-arg"
    elif ! grep -q "Usage: " "$err"; then
        say_fail "extra-arg"
    else
        say_ok "extra-arg"
    fi
    rm -f "$out" "$err"
}

run_missing() {
    local out err status path

    path="tests/vnrs/cases/missing.vnrs"
    out=$(mktemp)
    err=$(mktemp)
    status=0
    "$parser" "$path" >"$out" 2>"$err" || status=$?
    if [[ "$status" -ne 1 || -s "$out" ]]; then
        say_fail "missing-file"
    elif ! grep -q "Cannot open scene: $path" "$err"; then
        say_fail "missing-file"
    else
        say_ok "missing-file"
    fi
    rm -f "$out" "$err"
}

shopt -s nullglob
files=("$cases"/*.vnrs)
if [[ ${#files[@]} -eq 0 ]]; then
    echo "no cases in $cases" >&2
    exit 1
fi

for vnrs in "${files[@]}"; do
    run_file "$vnrs"
done

run_usage
run_missing

echo "$pass passed, $fail failed"
if [[ "$fail" -ne 0 ]]; then
    exit 1
fi
