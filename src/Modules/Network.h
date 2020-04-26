namespace Network {
    
    std::string getNetworkStatus(const char* networkName);

    std::string getOutput();

    void setActiveConnectionDbusPath();

    char* getNetworkName(const char* ActiveConnectionDbusPath);

    void networkLooper();
}