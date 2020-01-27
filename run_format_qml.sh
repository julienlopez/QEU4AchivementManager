#!/usr/bin/env bash
echo "...running qmlfmt on cpt-ui/..."
./qmlfmt.exe -l ./cpt-ui
./qmlfmt.exe -w ./cpt-ui
echo "done"
