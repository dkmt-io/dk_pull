#!/bin/sh

find dk_pull -name "*.h" -or -name "*.cc" -or -name "*.proto" | xargs clang-format -i
