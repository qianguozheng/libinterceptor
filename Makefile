#
# Copyright (C) 2012-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libinterceptor
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/libinterceptor/Default
  TITLE:=USB multiplexing daemon
  URL:=http://www.libimobiledevice.org/
endef


define Package/libinterceptor
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE+= library
  DEPENDS:=+libstdcpp
endef

define Package/libinterceptor/description
  This package contains the libinterceptor shared library.
endef

TARGET_CFLAGS += $(FPIC)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/libinterceptor/install
	$(INSTALL_DIR) $(1)/lib
	$(CP) $(PKG_BUILD_DIR)/libinterceptor.so $(1)/lib/
	$(CP) $(PKG_BUILD_DIR)/libinterceptor.so $(TOOLCHAIN_DIR)/lib
	#$(INSTALL_DIR) $(PKG_INSTALL_DIR)/lib
	#$(CP) $(PKG_INSTALL_DIR)/lib/libinterceptor.so $(1)/lib/
endef

$(eval $(call BuildPackage,libinterceptor))
