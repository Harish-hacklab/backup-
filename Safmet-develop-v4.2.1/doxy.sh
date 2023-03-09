#!/bin/bash
export PROJECT_NUMBER="$(git rev-parse HEAD ; git diff-index --quiet HEAD || echo '(with uncommitted changes)')"
rm -rf html
doxygen safmet_config
