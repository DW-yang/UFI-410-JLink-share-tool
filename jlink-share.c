#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <NetworkManager.h>

gboolean is_wlan0_connected() {
    // 连接NetworkManager
    NMClient *client = nm_client_new(NULL, NULL);
    if (!client) {
        fprintf(stderr, "Error: could not connect to NetworkManager!\n");
        return FALSE;
    }

    // 获取wlan0设备
    NMDevice *device = nm_client_get_device_by_iface(client, "wlan0");
    if (!device) {
        fprintf(stderr, "Error: wlan0 not found!\n");
        g_object_unref(client);
        return FALSE;
    }

    // 验证设备类型为WIFI
    if (nm_device_get_device_type(device) != NM_DEVICE_TYPE_WIFI) {
        fprintf(stderr, "Error: wlan0 is not a wifi device!\n");
        g_object_unref(client);
        return FALSE;
    }

    // 验证NM层面设备已激活
    NMDeviceState dev_state = nm_device_get_state(device);
    if (dev_state != NM_DEVICE_STATE_ACTIVATED) {
        fprintf(stderr, "Error: wlan0 not activated!(status code: %d)\n", dev_state);
        g_object_unref(client);
        return FALSE;
    }

    // 释放资源
    g_object_unref(client);
    return TRUE;
}

int main() {
    int times = 0;
    while(!is_wlan0_connected()){
        if(times > 10) break;
        fprintf(stderr, "Error: wlan0 not connected!\n");
        fprintf(stderr, "Note: Retry after 5s.\n");
        times++;
        sleep(5);
    }
    if (is_wlan0_connected()) {
        printf("Note: wlan0 connected. Switch usb mode to host.\n");
        // 切换USB至host模式
        if (WEXITSTATUS(system("echo host > /sys/devices/platform/soc@0/78d9000.usb/ci_hdrc.0/role")) != 0) {
	        fprintf(stderr, "Error: Failed to switch usb mode to host!\n");
	        exit(EXIT_FAILURE);
        }
        // 等待切换完成
        sleep(1);
        // 运行JLinkRemoteServer
        if (WEXITSTATUS(system("/usr/bin/JLinkRemoteServer")) != 0) {
	        fprintf(stderr, "Error: Failed to run /usr/bin/JLinkRemoteServer !\n");
	        exit(EXIT_FAILURE);
        }
        return 0;
    } else {
        fprintf(stderr, "Error: wlan0 not connected! Exit!\n");
        return 1;
    }
}
