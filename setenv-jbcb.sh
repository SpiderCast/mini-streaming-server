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
unset TARGET_ROOTFS

## choose the target board
## -------------------------------------------------------------------------------------------------

echo JBCB configuration selected!

export TARGET_BOARD=jbcb
export TARGET_CHIPSET=arm

## project SDK configuration
## -----------------------------------------------------------------------------------------------

export PROJECT_SDK=$PROJECT_ROOT/sdk/$TARGET_CHIPSET

## environment configuration
## -----------------------------------------------------------------------------------------------

export ARCH=arm
export CROSS_COMPILE_PREFIX=arm-linux-gnueabihf
export CROSS_COMPILE=${CROSS_COMPILE_PREFIX}-
export CROSS_COMPILER_PATH=
export CROSS_KDIR=/home/`whoami`/work/kernel/kernel
export CROSS_UBOOT=/home/`whoami`/work/u-boot
export CROSS_TARGET=$PROJECT_EXPORT/target
export CROSS_ENV_BAK_PATH=$PATH
export TARGET_BOARD_IP=172.16.10.99
export TARGET_DEBUG_PORT=5678
export TARGET_ROOTFS=/export/rootfs

export PATH=$PATH:$CROSS_UBOOT/tools

cd $PROJECT_NAME
