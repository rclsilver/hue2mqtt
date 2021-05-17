docker-shell:
	@docker run --rm -i -t -u openwrt -w /home/openwrt/sdk -v ${PWD}/sdk:/work -v ${PWD}/hue2mqtt:/home/openwrt/sdk/package/hue2mqtt yhnw/openwrt-sdk:15.05.1-ar71xx /work/entrypoint.sh

build:
	@docker run --rm -i -t -u openwrt -w /home/openwrt/sdk -v ${PWD}/sdk:/work -v ${PWD}/hue2mqtt:/home/openwrt/sdk/package/hue2mqtt yhnw/openwrt-sdk:15.05.1-ar71xx /work/entrypoint.sh /work/build.sh

install:
	@scp sdk/hue2mqtt_1_ar71xx.ipk hue-bridge:/root
	@ssh -t hue-bridge opkg remove hue2mqtt
	@ssh -t hue-bridge opkg install --force-reinstall /root/hue2mqtt_1_ar71xx.ipk
