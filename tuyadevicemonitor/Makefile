include $(TOPDIR)/rules.mk

PKG_NAME:=tuyadevicemonitor
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	CATEGORY:=Base system
	TITLE:=tuyadevicemonitor
	DEPENDS:=+libtuyasdk +libubox +libubus +USE_UCLIBC:argp-standalone
endef

define Package/$(PKG_NAME)/description
    Tuya Device Monitor using Tuya SDK
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuyadevicemonitor $(1)/usr/bin
	$(INSTALL_BIN) ./files/tuyadevicemonitor.init $(1)/etc/init.d/tuyadevicemonitor
	$(INSTALL_CONF) ./files/tuyadevicemonitor.config $(1)/etc/config/tuyadevicemonitor
endef

$(eval $(call BuildPackage,tuyadevicemonitor))
