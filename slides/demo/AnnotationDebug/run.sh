#!/bin/sh

javac MyProcessor.java
javac -cp . -processor MyProcessor Main.java

