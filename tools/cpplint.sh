#!/bin/sh
cpplint --root=. --counting=detailed $(find dk_pull -name "*.h" -or -name "*.cc")
