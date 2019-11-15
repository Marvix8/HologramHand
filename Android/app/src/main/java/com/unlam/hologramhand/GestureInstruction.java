package com.unlam.hologramhand;

import java.util.Observable;
import java.util.Observer;

public class GestureInstruction extends Observable {

    public GestureInstruction(Observer observer){
        this.addObserver(observer);
    }

    private String instruction;

    public String getInstruction() {
        return instruction;
    }

    public void setInstruction(String instruction) {
        this.instruction = instruction;
        this.notifyObservers(this.getInstruction());
    }
}
