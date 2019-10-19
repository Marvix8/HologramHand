package com.unlam.hologramhand;

public class DeviceItem {

    private String name;
    private String address;
    private String asdoolean;

    public DeviceItem(String name, String address, String asdoolean){

        this.name = name;
        this.address = address;
        this.asdoolean = asdoolean;
    }


    public String getAddress() {
        return address;
    }

    public String getAsdoolean() {
        return asdoolean;
    }

    public String getName() {
        return name;
    }
}
