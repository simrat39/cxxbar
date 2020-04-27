namespace Network {
    
    void setPropertiesFromConfig();

    std::string getNetworkStatus(const char* networkName);

    std::string getOutput();

    char* getNetworkName();

    void networkLooper();
}