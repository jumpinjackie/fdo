#!/bin/sh

docker build -t fdo-oracle-xe-11g .
docker run -p 1521:1521 -p 7000:8080 -e ORACLE_ALLOW_REMOTE=true fdo-oracle-xe-11g