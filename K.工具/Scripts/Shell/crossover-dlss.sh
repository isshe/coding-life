#!/bin/bash

cd /Applications/CrossOver.app/Contents/SharedSupport/CrossOver/lib64/apple_gptk
mv external external.old; mv wine wine.old
ditto "/Volumes/Evaluation environment for Windows games 3.0/redist/lib/" .
