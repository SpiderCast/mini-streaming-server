#!/bin/sh
#--------

## clear variables
## -------------------------------------------------------------------------------------------------

unset PROJECT_NAME

unset PROJECT_ROOT
unset PROJECT_EXPORT
unset PROJECT_MAKE

unset PROJECT_INC
unset PROJECT_DRV
unset PROJECT_STB
unset PROJECT_EXT
unset PROJECT_SVC
unset PROJECT_SYS
unset PROJECT_UTIL

unset MAKEFLAGS
unset HOST_N_CORES
unset PROJECT_DEBUG
unset PROJECT_OPTLEVEL

## project naming
## -----------------------------------------------------------------------------------------------

export PROJECT_NAME=ipbox

## project directory configuration
## -------------------------------------------------------------------------------------------------

export PROJECT_ROOT=$PWD
export PROJECT_EXPORT=$PROJECT_ROOT/export
export PROJECT_MAKE=$PROJECT_ROOT/make

export PROJECT_INC=$PROJECT_ROOT/$PROJECT_NAME/inc
export PROJECT_DRV=$PROJECT_ROOT/$PROJECT_NAME/drv
export PROJECT_STB=$PROJECT_ROOT/$PROJECT_NAME/stb
export PROJECT_EXT=$PROJECT_ROOT/$PROJECT_NAME/ext
export PROJECT_SVC=$PROJECT_ROOT/$PROJECT_NAME/svc
export PROJECT_SYS=$PROJECT_ROOT/$PROJECT_NAME/sys
export PROJECT_UTIL=$PROJECT_ROOT/$PROJECT_NAME/util

## build environment
## -------------------------------------------------------------------------------------------------

export MAKEFLAGS=--no-print-directory
export HOST_N_CORES=4
export PROJECT_DEBUG=1
export PROJECT_OPTLEVEL=0
