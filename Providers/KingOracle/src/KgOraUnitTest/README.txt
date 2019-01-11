About KgOraUnitTest
===================

KgOraUnitTest is a revised unit test suite for the King Oracle provider

The original contributed test suite suffers from the inability to run it due to references to data sources not provided as part of the initial King Oracle provider source contribution

The original test suite also does not build or run or Linux, thus providing no objective baseline for the provider's usability on Linux

Setup/Requirements
==================

KgOraUnitTest requires a running instance of Oracle XE 11g

A Dockerfile is provided to spin up such an environment

KgOraUnitTest can only be built with CMake and requires the main King Oracle provider be built and already installed