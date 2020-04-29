#include <fstream>
#include <thread>
#include <chrono>
#include <gio/gio.h>
#include <filesystem>
#include <iostream>

#include "../Utils/FileUtils.h"
#include "Network.h"
#include "../Main.h"
#include "../Utils/ConfigUtils.h"

namespace Network {

    static std::string output = "";
    static bool showNetworkName;
    static std::string wifiSysfs;
    static std::string ethSysfs;

    void setWifiEthSysfs() {
        for (const auto& i : std::filesystem::directory_iterator("/sys/class/net/")) {
            for (const auto& j : std::filesystem::directory_iterator(i.path().string())) {
                if (j.path().string().find("wireless") != std::string::npos) {
                    wifiSysfs = i.path().string();
                    break;
                }
            }
            if (i.path().string().find("lo") == std::string::npos && wifiSysfs == "") {
                ethSysfs = i.path().string();
            }
        }
    }   

    void setPropertiesFromConfig() {
        if (ConfigUtils::getValue("show-network-name","true") == "true") {
            showNetworkName = true;
        } else {
            showNetworkName = false;
        }
    }

    std::string getNetworkStatus(const char* networkName) {
        std::ifstream wifiOperstate(wifiSysfs + "/operstate");
        std::ifstream ethOperstate(ethSysfs + "/operstate");

        if (FileUtils::doesFileExist(wifiOperstate) && FileUtils::readFirstLine(wifiOperstate) == "up") {
            output =  showNetworkName ? std::string(" ") + networkName : std::string("");
        } else if (FileUtils::doesFileExist(ethOperstate) && FileUtils::readFirstLine(ethOperstate) == "up"){
            output = showNetworkName ? std::string(" ") + networkName : std::string("");
        } else {
            output = std::string("\uf00d");
        }

        return output;
    }

    std::string getOutput() {
        return output;
    }

    char* getNetworkName() {
        GDBusProxy *proxy;
        GError *error = NULL;

        proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                            G_DBUS_PROXY_FLAGS_NONE,
                                            NULL, /* GDBusInterfaceInfo */
                                            "org.freedesktop.NetworkManager",
                                            "/org/freedesktop/NetworkManager",
                                            "org.freedesktop.NetworkManager",
                                            NULL, /* GCancellable */
                                            &error);
        GVariant *ret;
        char **paths;

        /* Call ListConnections D-Bus method */
        ret = g_dbus_proxy_get_cached_property (proxy,
                                    "ActiveConnections");
        g_variant_get (ret, "^a&o", &paths);
        g_variant_unref (ret);

        char* networkName = (char*)"";

        if (paths[0] != NULL) {
            const char* activeConnectionDbusPath = paths[0];

            ret = g_dbus_proxy_get_cached_property (g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                    G_DBUS_PROXY_FLAGS_NONE,
                                                    NULL, /* GDBusInterfaceInfo */
                                                    "org.freedesktop.NetworkManager",
                                                    activeConnectionDbusPath,
                                                    "org.freedesktop.NetworkManager.Connection.Active",
                                                    NULL, /* GCancellable */
                                                    NULL), "Id");
    
            g_variant_get (ret, "s", &networkName);
            g_variant_unref (ret);
            return networkName;
        }
        
        return networkName;
        
    }

    void networkLooper() {
        setWifiEthSysfs();
        std::string newStatus = "";
        while (true) {
            std::string status;
            if (showNetworkName) {
                status = getNetworkStatus(getNetworkName());
            } else {
                status = getNetworkStatus("");
            }

            if (status != newStatus) {
                updateOutput();
                newStatus = status;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    }
}
