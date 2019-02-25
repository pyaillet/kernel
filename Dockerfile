FROM haskell:8.6

RUN cabal update && \
  cabal install patat

VOLUME /data
WORKDIR /data

ENTRYPOINT [ "patat" ]
