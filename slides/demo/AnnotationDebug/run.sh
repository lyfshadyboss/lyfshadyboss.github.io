#!/bin/sh

javac -proc:none MyProcessor.java
javac -cp . -processor MyProcessor Main.java

