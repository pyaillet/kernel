#!/bin/sh

docker run -it -v $(pwd):/data pyaillet/patat:0.8.2.2 slides.md -w
