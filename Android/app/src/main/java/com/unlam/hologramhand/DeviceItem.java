package com.unlam.hologramhand;

public class DeviceItem {

    private String name;
    private String address;

    public DeviceItem(String name, String address) {
        this.name = name;
        this.address = address;
    }

    public String getAddress() {
        return address;
    }

    public String getName() {
        return name;
    }

    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append(this.getName())
                .append(" º ")
                .append(this.getAddress());

        return stringBuilder.toString();
    }
}
