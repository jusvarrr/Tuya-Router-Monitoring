  1 #
  2 # Copyright (C) 2007-2014 OpenWrt.org
  3 #
  4 # This is free software, licensed under the GNU General Public License v2.
  5 # See /LICENSE for more information.
  6 #
  7 
  8 include $(TOPDIR)/rules.mk
  9 
  10 PKG_NAME:=argp-standalone
  11 PKG_VERSION:=1.3
  12 PKG_RELEASE:=2
  13 
  14 PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
  15 PKG_SOURCE_URL:=http://www.lysator.liu.se/~nisse/misc/
  16 PKG_HASH:=dec79694da1319acd2238ce95df57f3680fea2482096e483323fddf3d818d8be
  17 PKG_MAINTAINER:=Ted Hess <thess@kitschensync.net>
  18 
  19 PKG_LICENSE:=LGPL-2.1
  20 PKG_LICENSE_FILE:=Makefile.am
  21 
  22 PKG_BUILD_FLAGS:=no-lto
  23 
  24 include $(INCLUDE_DIR)/package.mk
  25 include $(INCLUDE_DIR)/host-build.mk
  26 
  27 define Package/argp-standalone
  28   SECTION:=libs
  29   CATEGORY:=Libraries
  30   TITLE:=Hierarchial argument parsing broken out from glibc
  31   URL:=http://www.lysator.liu.se/~nisse/misc/
  32 endef
  33 
  34 define Package/argp-standalone/description
  35   GNU libc hierarchial argument parsing library broken out from glibc.
  36 endef
  37 
  38 MAKE_FLAGS += \
  39         CFLAGS="$(TARGET_CFLAGS) $(FPIC) -std=gnu89"
  40 
  41 HOST_MAKE_FLAGS += \
  42         CFLAGS="$(HOST_CFLAGS) $(FPIC) -std=gnu89"
  43 
  44 define Build/InstallDev
  45         $(INSTALL_DIR) $(1)/usr/include
  46         $(CP)   $(PKG_BUILD_DIR)/argp.h \
  47                 $(1)/usr/include/
  48         $(INSTALL_DIR) $(1)/usr/lib
  49         $(CP)   $(PKG_BUILD_DIR)/libargp.a \
  50                 $(1)/usr/lib/
  51 endef
  52 
  53 $(eval $(call BuildPackage,argp-standalone))

