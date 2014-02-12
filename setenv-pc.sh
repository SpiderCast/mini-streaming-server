#!/bin/sh
#--------

. setenv-common.sh

## clear variables
## -------------------------------------------------------------------------------------------------

if [ -n "$CROSS_ENV_BAK_PATH" ] ; then
  export PATH=$CROSS_ENV_BAK_PATH
fi

unset TARGET_BOARD
unset TARGET_CHIPSET

unset PROJECT_SDK

unset ARCH
unset CROSS_COMPILE_PREFIX
unset CROSS_COMPILE
unset CROSS_COMPILER_PATH
unset CROSS_KDIR
unset CROSS_UBOOT
unset CROSS_TARGET
unset CROSS_ENV_BAK_PATH
unset TARGET_BOARD_IP
unset TARGET_DEBUG_PORT

## choose the target board
## -------------------------------------------------------------------------------------------------

echo PC configuration selected!

export TARGET_BOARD=pc
export TARGET_CHIPSET=x86

## project SDK configuration
## -----------------------------------------------------------------------------------------------

export PROJECT_SDK=$PROJECT_ROOT/sdk/$TARGET_CHIPSET

## environment configuration
## -----------------------------------------------------------------------------------------------

export ARCH=arm
export CROSS_COMPILE_PREFIX=
export CROSS_COMPILE=
export CROSS_COMPILER_PATH=
export CROSS_KDIR=
export CROSS_UBOOT=
export CROSS_TARGET=$PROJECT_EXPORT/target
export CROSS_ENV_BAK_PATH=$PATH
export TARGET_BOARD_IP=
export TARGET_DEBUG_PORT=

cd $PROJECT_NAME
