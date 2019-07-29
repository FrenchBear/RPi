#!/bin/bash

pushd ~/Development >/dev/null
cd GitVSTS
gitall.sh $@
cd ../GitHub
gitall.sh $@
popd >/dev/null
