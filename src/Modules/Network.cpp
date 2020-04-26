#include <fstream>
#include <thread>
#include <chrono>
#include <gio/gio.h>

#include "../Utils/FileUtils.h"
#include "Network.h"
#include "../Main.h"

namespace Network {

    static std::string output = "";

    std::string getNetworkStatus(const char* networkName) {
        std::ifstream wifiOperstate("/sys/class/net/wlp7s0/operstate");
        std::ifstream ethOperstate("/sys/class/net/enp8s0/operstate");

        if (FileUtils::doesFileExist(wifiOperstate) && FileUtils::readFirstLine(wifiOperstate) == "up") {
            output = std::string(" ") + networkName;
        } else if (FileUtils::doesFileExist(ethOperstate) && FileUtils::readFirstLine(ethOperstate) == "up"){
            output = std::string(" ") + networkName;
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
    
            char* networkName;
            g_variant_get (ret, "s", &networkName);
            g_variant_unref (ret);
            return networkName;
        }
        
        return (char*) "";
        
    }

    void networkLooper() {
        std::string newStatus = "";
        while (true) {
            std::string status = getNetworkStatus(getNetworkName());

            if (status != newStatus) {
                updateOutput();
                newStatus = status;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    }

}
